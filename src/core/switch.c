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
#include "letmecreate/core/switch.h"

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

static void process_event(uint8_t switch_event)
{
    struct switch_callback *cur = NULL;

    pthread_mutex_lock(&mutex);
    cur = callback_list_head;
    while (cur) {
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

int switch_add_callback(uint8_t event_mask, void (*callback)(void))
{
    struct switch_callback *entry = NULL;
    if (fd < 0) {
        fprintf(stderr, "switch: Failed to add callback, switch_init must be called before\n");
        return -1;
    }

    if ((event_mask & SWITCH_ALL_EVENTS) == 0) {
        fprintf(stderr, "switch: Invalid event mask.\n");
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


    if (callback_list_head == NULL) {
        pthread_mutex_lock(&mutex);
        callback_list_head = entry;
        pthread_mutex_unlock(&mutex);
    }
    else {
        struct switch_callback *last = callback_list_head;
        while (last->next)
            last = last->next;

        pthread_mutex_lock(&mutex);
        last->next = entry;
        pthread_mutex_unlock(&mutex);
    }

    return entry->ID;
}

int switch_remove_callback(int callback_ID)
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

    entry = callback_list_head;
    while (entry) {
        if (entry->ID == callback_ID)
            break;

        prev = entry;
        entry = entry->next;
    }
    if (entry == NULL)
        return -1;

    pthread_mutex_lock(&mutex);
    if (prev)
        prev->next = entry->next;
    else
        callback_list_head = entry->next;
    pthread_mutex_unlock(&mutex);

    free(entry);

    return 0;
}

int switch_release(void)
{
    if (fd >= 0) {
        running = false;
        if (pthread_join(thread, NULL) < 0) {
            fprintf(stderr, "switch: Failed to terminate monitoring thread.\n");
            return -1;
        }

        if (pthread_mutex_destroy(&mutex) < 0) {
            fprintf(stderr, "switch: Failed to destroy mutex.\n");
            return -1;
        }

        if (close(fd) < 0) {
            fprintf(stderr, "switch: Failed to close file descriptor.\n");
            return -1;
        }

        fd = -1;

        /* Delete any entries in callback list */
        while (callback_list_head) {
            struct switch_callback *tmp = callback_list_head;
            callback_list_head = callback_list_head->next;
            free(tmp);
        }
    }

    return 0;
}
