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

static int build_file_path(char *path, const uint8_t led_index, const char *filename)
{
    if (led_index >= NB_LEDS) {
        fprintf(stderr, "led: Cannot build device file path for invalid led %d\n", led_index);
        return -1;
    }

    if (path == NULL) {
        fprintf(stderr, "led: Cannot store device file path for led %d to null variable\n", led_index);
        return -1;
    }

    if (filename == NULL) {
        fprintf(stderr, "led: Cannot build device file path for led %d with null filename\n", led_index);
        return -1;
    }

    if(led_index == 7) {
        if (sprintf(path, LED_HEARTBEAT_DEVICE_FILE_PATH, filename) < 0) {
            fprintf(stderr, "led: Failed to build %s device file path for led heartbeat\n", filename);
            return -1;
        }
    } else {
        if (sprintf(path, LED_DEVICE_FILE_PATH, led_index+1, filename) < 0) {
            fprintf(stderr, "led: Failed to build %s device file path for led %d\n", filename, led_index);
            return -1;
        }
    }

    return 0;
}

static int set_value(const uint8_t led_index, const uint8_t value)
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

static int set_mode(const uint8_t led_index, const char *mode)
{
    int fd = -1;
    char path[255];

    if (build_file_path(path, led_index, "trigger") < 0)
        return -1;

    if ((fd = open(path, O_WRONLY)) < 0) {
        fprintf(stderr, "led: Failed to open trigger device file for led %d\n", led_index);
        return -1;
    }
    if (write(fd, mode, strlen(mode)+1) < 0) {
        close(fd);
        fprintf(stderr, "led: Failed to set mode to %s for led %d\n", mode, led_index);
        return -1;
    }
    close(fd);

    return 0;
}

static int set_delay(const uint8_t led_index, const char *filename, const uint32_t value)
{
    int fd = -1;
    char path[255];
    char str[255];

    if (build_file_path(path, led_index, filename) < 0)
        return -1;

    if (snprintf(str, sizeof(str), "%d", value) < 0) {
        fprintf(stderr, "led: Failed to convert unsigned integer into string\n");
        return -1;
    }

    if ((fd = open(path, O_WRONLY)) < 0) {
        fprintf(stderr, "led: Failed to open %s device file for led %d\n", filename, led_index);
        return -1;
    }

    if (write(fd, str, strlen(str)+1) < 0) {
        fprintf(stderr, "led: Failed to set value for %s for led %d\n", filename, led_index);
        close(fd);
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

        if (set_mode(i, "none") < 0)
            return -1;

        if (fds[i] >= 0)
            continue;

        if (build_file_path(path, i, "brightness") < 0)
            return -1;

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

        if (set_value(i, value & mask) < 0) {
            fprintf(stderr, "Failed to switch %s led %d\n", (value & mask) ? "on" : "off", i);
            return -1;
        }
    }

    return 0;
}

int led_configure_on_off_mode(const uint8_t mask)
{
    int i = 0, tmp = 1;

    for (; i < NB_LEDS; ++i, tmp <<= 1) {
        if ((mask & tmp) == 0)
            continue;

        if (set_mode(i, "none") < 0) {
            fprintf(stderr, "led: Failed to configure led %d in on/off mode\n", i);
            return -1;
        } 
    }

    return 0;
}

int led_configure_timer_mode(const uint8_t mask)
{
    int i = 0, tmp = 1;

    for (; i < NB_LEDS; ++i, tmp <<= 1) {
        if ((mask & tmp) == 0)
            continue;

        if (set_mode(i, "timer") < 0) {
            fprintf(stderr, "led: Failed to configure led %d in timer mode\n", i);
            return -1;
        }
    }

    return 0;
}

int led_set_delay(const uint8_t mask, const uint32_t delay_on, const uint32_t delay_off)
{
    int i = 0, tmp = 1;

    for (; i < NB_LEDS; ++i, tmp <<= 1) {
        if ((mask & tmp) == 0)
            continue;

        if (set_delay(i, "delay_on", delay_on) < 0
        ||  set_delay(i, "delay_off", delay_off) < 0)
            return -1;
    }

    return 0;
}

int led_release(void)
{
    int i = 0;

    for (; i < NB_LEDS; ++i) {
        if (fds[i] < 0)
            continue;

        if (led_switch_off(1 << i) < 0)
            return -1;

        if (close(fds[i]) < 0) {
            fprintf(stderr, "led: Failed to close file descriptor for led %d\n", i);
            return -1;
        }
        fds[i] = -1;
    }

    return 0;
}
