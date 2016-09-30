#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "letmecreate/core/common.h"
#include "letmecreate/core/gpio_monitor.h"


#define GPIO_PATH_FORMAT      "/sys/class/gpio/gpio%d/"

static volatile bool thread_running = false;
static pthread_t thread;
static pthread_mutex_t inotify_watch_mutex;
static pthread_mutex_t gpio_watch_mutex;

static int fd = -1;     /* inotify file descriptor */

static uint32_t current_watch_ID = 0;
struct gpio_watch {
    uint32_t ID;
    uint8_t gpio_pin;
    uint8_t event_mask;
    void(*callback)(uint8_t);
    struct gpio_watch *next;
};
static struct gpio_watch *gpio_watch_list_head = NULL;

/* Mapping wd <-> gpio_pin */
struct inotify_watch {
    int wd;
    uint8_t gpio_pin;
    struct inotify_watch *next;
};
static struct inotify_watch *inotify_watch_list_head = NULL;


static uint8_t wd_to_gpio_pin(int wd)
{
    uint8_t gpio_pin = 0;
    struct inotify_watch *cur = NULL;
    pthread_mutex_lock(&inotify_watch_mutex);
    cur = inotify_watch_list_head;
    while (cur) {
        if (cur->wd == wd) {
            gpio_pin = cur->gpio_pin;
            break;
        }
        cur = cur->next;
    }
    pthread_mutex_unlock(&inotify_watch_mutex);

    return gpio_pin;
}

static int find_event_type(uint8_t gpio_pin, uint8_t *event_type)
{
    uint8_t value;

    if (gpio_get_value(gpio_pin, &value) < 0)
        return -1;

    if (value == 0)
        *event_type = GPIO_FALLING;
    else
        *event_type = GPIO_RAISING;

    return 0;
}

static void call_callbacks(uint8_t gpio_pin, uint8_t event_type)
{
    struct gpio_watch *cur = NULL;

    /* Call callbacks */
    pthread_mutex_lock(&gpio_watch_mutex);
    cur = gpio_watch_list_head;
    while (cur) {
        if (cur->gpio_pin == gpio_pin && cur->event_mask & event_type)
            cur->callback(event_type);

        cur = cur->next;
    }
    pthread_mutex_unlock(&gpio_watch_mutex);
}

static void* monitor_gpio(void __attribute__ ((unused))*arg)
{
    thread_running = true;
    while (thread_running) {
        struct timeval timeout = {          /* 20ms timeout */
            .tv_sec = 0,
            .tv_usec = 20000
        };
        int ret;
        uint8_t gpio_pin, event_type;
        fd_set fds;
        struct inotify_event event;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        ret = select(fd+1, &fds, NULL, NULL, &timeout);
        if (ret <= 0)
            continue;

        ret = read(fd, &event, sizeof(event));
        if (ret <= 0)
            continue;

        gpio_pin = wd_to_gpio_pin(event.wd);

        /* Ignore event if we cannot find the gpio */
        if (gpio_pin == 0)
            continue;
        if (find_event_type(gpio_pin, &event_type) < 0)
            continue;

        call_callbacks(gpio_pin, event_type);
    }

    return NULL;
}

static int enable_gpio_interrupt(uint8_t gpio_pin)
{
    char path[MAX_STR_LENGTH];

    if (snprintf(path, MAX_STR_LENGTH, GPIO_PATH_FORMAT"edge", gpio_pin) < 0)
        return -1;

    return write_str_file(path, "both");
}

static bool is_gpio_monitored(uint8_t gpio_pin)
{
    struct gpio_watch *cur = gpio_watch_list_head;
    while (cur) {
        if (cur->gpio_pin == gpio_pin)
            return true;
        cur = cur->next;
    }

    return false;
}

static bool has_inotify_watch(uint8_t gpio_pin)
{
    struct inotify_watch *cur = inotify_watch_list_head;
    while (cur) {
        if (cur->gpio_pin == gpio_pin)
            return true;

        cur = cur->next;
    }

    return false;
}

static int add_inotify_watch(uint8_t gpio_pin)
{
    struct inotify_watch *watch = NULL;
    char path[MAX_STR_LENGTH];
    int wd = -1;

    if (snprintf(path, MAX_STR_LENGTH, GPIO_PATH_FORMAT"value", gpio_pin) < 0)
        return -1;

    watch = malloc(sizeof(struct inotify_watch));
    if (watch == NULL) {
        fprintf(stderr, "gpio_monitor: Failed to allocate memory for a inotify watch.\n");
        return -1;
    }

    if ((wd = inotify_add_watch(fd, path, IN_MODIFY)) < 0) {
        free(watch);
        return -1;
    }

    watch->wd = wd;
    watch->gpio_pin = gpio_pin;
    watch->next = NULL;

    if (inotify_watch_list_head == NULL) {
        pthread_mutex_lock(&inotify_watch_mutex);
        inotify_watch_list_head = watch;
        pthread_mutex_unlock(&inotify_watch_mutex);
    } else {
        struct inotify_watch *last = inotify_watch_list_head;
        while (last->next)
            last = last->next;
        pthread_mutex_lock(&inotify_watch_mutex);
        last->next = watch;
        pthread_mutex_unlock(&inotify_watch_mutex);
    }

    return 0;
}

static int remove_inotify_watch(uint8_t gpio_pin)
{
    struct inotify_watch *prev = NULL;
    struct inotify_watch *cur = inotify_watch_list_head;
    while (cur) {
        if (cur->gpio_pin == gpio_pin)
            break;
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL)
        return -1;

    if (inotify_rm_watch(fd, cur->wd) < 0) {
        fprintf(stderr, "gpio_monitor: Failed to remove inotify watch.\n");
        return -1;
    }

    pthread_mutex_lock(&inotify_watch_mutex);
    if (prev)
        prev->next = cur->next;
    else
        inotify_watch_list_head = cur->next;
    pthread_mutex_unlock(&inotify_watch_mutex);

    free(cur);

    return 0;
}

static void remove_gpio_watch(uint32_t ID)
{
    struct gpio_watch *cur = gpio_watch_list_head;
    struct gpio_watch *prev = NULL;

    while (cur) {
        if (cur->ID == ID)
            break;
        prev = cur;
        cur = cur->next;
    }
    if (cur != NULL) {
        pthread_mutex_lock(&gpio_watch_mutex);
        if (prev)
            prev->next = cur->next;
        else
            gpio_watch_list_head = cur->next;
        pthread_mutex_unlock(&gpio_watch_mutex);

        free(cur);
    }
}

int gpio_monitor_init(void)
{
    if (fd == -1) {
        if ((fd = inotify_init()) < 0)
            return -1;
    }

    if (thread_running == true)
        return 0;

    if (pthread_mutex_init(&inotify_watch_mutex, NULL) != 0) {
        fprintf(stderr, "switch: Error while initialising inotify_watch_mutex\n");
        return -1;
    }

    if (pthread_mutex_init(&gpio_watch_mutex, NULL) != 0) {
        fprintf(stderr, "switch: Error while initialising gpio_watch_mutex\n");
        pthread_mutex_destroy(&inotify_watch_mutex);
        return -1;
    }

    if (pthread_create(&thread, NULL, monitor_gpio, NULL) < 0) {
        pthread_mutex_destroy(&gpio_watch_mutex);
        pthread_mutex_destroy(&inotify_watch_mutex);
        return -1;
    }

    return 0;
}

int gpio_monitor_add_callback(uint8_t gpio_pin, uint8_t event_mask, void(*callback)(uint8_t))
{
    struct gpio_watch *watch = NULL;

    if (event_mask == 0) {
        fprintf(stderr, "gpio_monitor: event_mask is invalid (must not be zero).\n");
        return -1;
    }

    if (callback == NULL) {
        fprintf(stderr, "gpio_monitor: callback must not be null.\n");
        return -1;
    }

    /* Ensure that GPIO raise interrupt when its value change */
    if (enable_gpio_interrupt(gpio_pin) < 0) {
        fprintf(stderr, "gpio_monitor: Failed to enable gpio interrupt.\n");
        return -1;
    }

    watch = malloc(sizeof(struct gpio_watch));
    if (watch == NULL) {
        fprintf(stderr, "gpio_monitor: Failed to allocate memory for watch.\n");
        return -1;
    }

    /* Start monitoring file */
    if (has_inotify_watch(gpio_pin) == false) {
        if (add_inotify_watch(gpio_pin) < 0) {
            fprintf(stderr, "gpio_monitor: Failed to add inotify watch.\n");
            free(watch);
            return -1;
        }
    }

    /* Create new watch ID. Because users use signed integers as ID, and the
     * wrapper use unsigned integers, there is a limit of about 2 billions
     * callbacks that can be created. Past this point, it will be unable for the
     * 2 billions to create callbacks. Then, it can create callbacks again...
     */
    watch->ID = current_watch_ID;
    ++current_watch_ID;

    watch->gpio_pin = gpio_pin;
    watch->event_mask = event_mask;
    watch->callback = callback;
    watch->next = NULL;

    /* Add to end of gpio watch list */
    if (gpio_watch_list_head == NULL) {
        pthread_mutex_lock(&gpio_watch_mutex);
        gpio_watch_list_head = watch;
        pthread_mutex_unlock(&gpio_watch_mutex);
    } else {
        struct gpio_watch *last = gpio_watch_list_head;
        while (last->next)
            last = last->next;
        pthread_mutex_lock(&gpio_watch_mutex);
        last->next = watch;
        pthread_mutex_unlock(&gpio_watch_mutex);
    }

    return watch->ID;
}

int gpio_monitor_remove_callback(int callbackID)
{
    struct gpio_watch *cur = gpio_watch_list_head;
    uint8_t gpio_pin = 0;
    uint32_t ID = 0;

    if (callbackID < 0) {
        fprintf(stderr, "gpio_monitor: Invalid callback ID.\n");
        return -1;
    }

    ID = (uint32_t)callbackID;
    if (ID >= current_watch_ID) {
        fprintf(stderr, "gpio_monitor: Invalid callback ID.\n");
        return -1;
    }

    /* Find gpio from callback ID */
    while (cur) {
        if (cur->ID == ID) {
            gpio_pin = cur->gpio_pin;
            break;
        }
        cur = cur->next;
    }
    if (cur == NULL) {
        fprintf(stderr, "gpio_monitor: Failed to convert callback ID to gpio pin.\n");
        return -1;
    }

    remove_gpio_watch(ID);

    /* No more callback associated with gpio, remove inotify watch */
    if (is_gpio_monitored(gpio_pin) == false) {
        if (remove_inotify_watch(gpio_pin) < 0) {
            fprintf(stderr, "gpio_monitor: Failed to remove inotify watch.\n");
            return -1;
        }
    }

    return 0;
}

int gpio_monitor_release(void)
{
    int ret = 0;

    if (thread_running == false)
        return 0;

    /* Stop monitoring thread */
    thread_running = false;
    if (pthread_join(thread, NULL))
        ret = -1;
    if (pthread_mutex_destroy(&inotify_watch_mutex) < 0)
        ret = -1;
    if (pthread_mutex_destroy(&gpio_watch_mutex) < 0)
        ret = -1;

    /* Delete all inotify watches */
    while (inotify_watch_list_head) {
        struct inotify_watch *tmp = inotify_watch_list_head;
        inotify_watch_list_head = inotify_watch_list_head->next;

        inotify_rm_watch(fd, tmp->wd);
        free(tmp);
    }

    /* Delete all gpio watches */
    while (gpio_watch_list_head) {
        struct gpio_watch *tmp = gpio_watch_list_head;
        gpio_watch_list_head = gpio_watch_list_head->next;
        free(tmp);
    }

    /* Release file descriptor */
    if (close(fd) < 0)
        ret = -1;
    fd = -1;

    return ret;
}
