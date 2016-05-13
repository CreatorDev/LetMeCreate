#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "core/led.h"


#define LED_DEVICE_FILE_PATH                    "/sys/class/leds/marduk:red:user%d/%s"
#define LED_HEARTBEAT_DEVICE_FILE_PATH          "/sys/class/leds/marduk:red:heartbeat/%s"
#define NB_LEDS                     (8)

int fds[NB_LEDS] = { -1, -1, -1, -1, -1, -1, -1, -1 };

static int led_set_value(const uint8_t led_index, const uint8_t value)
{
    char *str = NULL;

    if (led_index > NB_LEDS)
        return -1;

    if (value == 0)
        str = "0";
    else
        str = "1";

    return write(fds[led_index], str, 2);
}

static int led_set_mode(const uint8_t led_index, const char *mode)
{
    int fd = -1;
    char path[255];

    if(led_index == 7) {
        if (snprintf(path, sizeof(path), LED_HEARTBEAT_DEVICE_FILE_PATH, "trigger") < 0) {
            fprintf(stderr, "led: Failed to build device file path for led heartbeat\n");
            return -1;
        }
    } else {
        if (snprintf(path, sizeof(path), LED_DEVICE_FILE_PATH, led_index+1, "trigger") < 0) {
            fprintf(stderr, "led: Failed to build device file path for led %d\n", led_index);
            return -1;
        }
    }

    if ((fd = open(path, O_WRONLY)) < 0) {
        fprintf(stderr, "led: Failed to open trigger device file for led %d\n", led_index);
        return -1;
    }
    if (write(fd, mode, strlen(mode)+1) < 0) {
        fprintf(stderr, "led: Failed to set mode to %s for led %d\n", mode, led_index);
        return -1;
    }
    close(fd);

    return 0;
}

int led_init(void)
{
    int i = 0;

    for (; i < NB_LEDS; ++i) {
        char path[255];

        if (led_set_mode(i, "none") < 0)
            return -1;

        if (fds[i] >= 0)
            continue;

        if(i == 7) {
            if (snprintf(path, sizeof(path), LED_HEARTBEAT_DEVICE_FILE_PATH, "brightness") < 0) {
                fprintf(stderr, "led: Failed to build device file path for led heartbeat\n");
                return -1;
            }
        } else {
            if (snprintf(path, sizeof(path), LED_DEVICE_FILE_PATH, i+1, "brightness") < 0) {
                fprintf(stderr, "led: Failed to build device file path for led %d\n", i);
                return -1;
            }
        }

        fds[i] = open(path, O_WRONLY);
        if (fds[i] < 0) {
            fprintf(stderr, "led: Failed to open device file %s\n", path);
            return -1;
        }
    }

    /* Switch off all LEDS */
    led_switch_off(LED_0
                 | LED_1
                 | LED_2
                 | LED_3
                 | LED_4
                 | LED_5
                 | LED_6
                 | LED_HEARTBEAT);
    return 0;
}

int led_switch_on(const uint8_t mask)
{
    return led_set(mask, 0xFF);
}

int led_switch_off(const uint8_t mask)
{
    return led_set(mask, 0);
}

int led_set(const uint8_t mask, const uint8_t value)
{
    int i = 0, tmp = 1;

    for (; i < NB_LEDS; ++i, tmp <<= 1) {
        if ((mask & tmp) == 0)
            continue;

        if (led_set_value(i, value & mask) < 0) {
            fprintf(stderr, "Failed to switch %s led %d\n", (value & mask) ? "on" : "off", i);
            return -1;
        }
    }

    return 0;
}

void led_release(void)
{
    int i = 0;

    /* Switch off all LEDS */
    led_switch_off(LED_0
                 | LED_1
                 | LED_2
                 | LED_3
                 | LED_4
                 | LED_5
                 | LED_6
                 | LED_HEARTBEAT);

    for (; i < NB_LEDS; ++i) {
        if (fds[i] < 0)
            continue;

        close(fds[i]);
        fds[i] = -1;
    }
}
