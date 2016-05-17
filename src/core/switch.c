#include <fcntl.h>
#include <linux/input.h>
#include <poll.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "core/switch.h"

#define DEVICE_FILE         "/dev/input/event1"
#define TIMEOUT             (20)        /* 20 ms timeout while polling */
#define SWITCH_1_CODE       (257)
#define SWITCH_2_CODE       (258)

static int fd = -1;
static pthread_t thread;
static pthread_mutex_t mutex;
static bool running = false;
static int switch_callback_ID = 0;

struct switch_callback
{
    int ID;
    uint8_t event_mask;
    void (*f)(void);
    struct switch_callback *next;
};
static struct switch_callback *callback_list_head = NULL;

static void process_event(const uint8_t switch_event)
{
    struct switch_callback *cur = NULL;

    pthread_mutex_lock(&mutex);
    cur = callback_list_head;
    while (cur != NULL) {
        if (cur->event_mask & switch_event)
            cur->f();

        cur = cur->next;
    }
    pthread_mutex_unlock(&mutex);
}

static void* switch_update(void *arg)
{
    int ret;
    struct input_event event[2];
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;

    running = true;
    while (running) {
        ret = poll(&pfd, 1, TIMEOUT);

        if (ret < 0) {
            fprintf(stderr, "switch: Error while polling file descriptor\n");
            continue;
        } else if (ret == 0) {
            continue; 
        } else {
            if (read(fd, &event, sizeof(event)) != sizeof(event)) {
                fprintf(stderr, "switch: Error while reading event from file descriptor\n");
                continue;
            }

            switch (event[0].code) {
            case SWITCH_1_CODE:
                if (event[0].value == 0)
                    process_event(SWITCH_1_RELEASED);
                else if (event[0].value == 1)
                    process_event(SWITCH_1_PRESSED);
                break;
            case SWITCH_2_CODE:
                if (event[0].value == 0)
                    process_event(SWITCH_2_RELEASED);
                else if (event[0].value == 1)
                    process_event(SWITCH_2_PRESSED);
                break;
            default:
                fprintf(stderr, "switch: Unrecognized event code\n");
                break;
            }
        }
    }

    return NULL;
}

int switch_init(void)
{
    if (fd >= 0)
        return 0;

    if ((fd = open(DEVICE_FILE, O_RDONLY)) < 0) {
        fprintf(stderr, "switch: Error while opening device file\n");
        return -1;
    }

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        fprintf(stderr, "switch: Error while initialising mutex\n");
        return -1;
    }

    if (pthread_create(&thread, NULL, switch_update, NULL) != 0) {
        pthread_mutex_destroy(&mutex);
        close(fd);
        fd = -1;
        return -1;
    }

    return 0;
}

int switch_add_callback(const uint8_t event_mask, void (*callback)(void))
{
    struct switch_callback *entry = NULL;
    if (fd < 0) {
        fprintf(stderr, "switch: Failed to add callback, switch_init must be called before\n");
        return -1;
    }

    if (callback == NULL) {
        fprintf(stderr, "switch: Cannot add a null callback\n");
        return -1;
    }

    entry = malloc(sizeof(struct switch_callback));
    if (entry == NULL) {
        fprintf(stderr, "switch: Failed to allocate memory for switch_callback entry\n");
        return -1;
    }
    entry->ID = switch_callback_ID;
    ++switch_callback_ID;
    entry->event_mask = event_mask;
    entry->f = callback;
    entry->next = NULL;

    pthread_mutex_lock(&mutex);
    if (callback_list_head == NULL) {
        callback_list_head = entry;
    }
    else {
        struct switch_callback *last = callback_list_head;
        while (last->next != NULL)
            last = last->next;
        last->next = entry;
    }
    pthread_mutex_unlock(&mutex);

    return entry->ID;
}

int switch_remove_callback(const int callback_ID)
{
    struct switch_callback *entry = NULL, *prev = NULL;

    if (fd <= 0) {
        fprintf(stderr, "switch: Failed to add callback, switch_init must be called before\n");
        return -1;
    }

    if (callback_ID < 0) {
        fprintf(stderr, "switch: Cannot remove switch callback with invalid ID\n");
        return -1;
    }

    if (callback_list_head == NULL)
        return -1;

    pthread_mutex_lock(&mutex);
    entry = callback_list_head;
    while (entry != NULL) {
        if (entry->ID == callback_ID) {
            prev->next = entry->next;
            free(entry);
            pthread_mutex_unlock(&mutex);
            return 0;
        }

        prev = entry;
        entry = entry->next;
    }
    pthread_mutex_unlock(&mutex);

    return -1;
}

void switch_release(void)
{
    if (fd >= 0) {
        struct switch_callback *entry = NULL, *tmp = NULL;

        running = false;
        pthread_join(thread, NULL);
        close(fd);
        fd = -1;
        pthread_mutex_destroy(&mutex);

        /* Delete any entries in callback list */
        entry = callback_list_head;
        while (entry != NULL) {
            tmp = entry->next;
            free(entry);
            entry = tmp;
        }
        callback_list_head = NULL;
    }
}
