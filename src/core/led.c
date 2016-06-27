#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "core/common.h"
#include "core/led.h"


#define LED_DEVICE_FILE_PATH                    "/sys/class/leds/marduk:red:user%d/%s"
#define LED_HEARTBEAT_DEVICE_FILE_PATH          "/sys/class/leds/marduk:red:heartbeat/%s"


int fds[LED_CNT] = { -1, -1, -1, -1, -1, -1, -1, -1 };

static int build_file_path(char *path, const uint8_t led_index, const char *filename)
{
    if (led_index >= LED_CNT) {
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

    if (led_index > LED_CNT)
        return -1;

    if (value == 0)
        str = "0";
    else
        str = "1";

    return write(fds[led_index], str, 2);
}

static int set_mode(const uint8_t led_index, const char *mode)
{
    char path[MAX_STR_LENGTH];

    if (build_file_path(path, led_index, "trigger") < 0)
        return -1;

    return write_str_file(path, mode);
}

static int set_delay(const uint8_t led_index, const char *filename, const uint32_t value)
{
    char path[MAX_STR_LENGTH];

    if (build_file_path(path, led_index, filename) < 0)
        return -1;

    return write_int_file(path, value);
}

int led_init(void)
{
    int i = 0;

    for (; i < LED_CNT; ++i) {
        char path[MAX_STR_LENGTH];

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

    return led_switch_off(ALL_LEDS);
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

    for (; i < LED_CNT; ++i, tmp <<= 1) {
        uint8_t mode;
        if ((mask & tmp) == 0)
            continue;

        /* Check that led is in ON/OFF mode */
        if (led_get_mode(tmp, &mode) < 0)
            return -1;
        if (mode != ON_OFF_MODE) {
            fprintf(stderr, "led: Invalid mode of led %d\n", i);
            return -1;
        }

        if (set_value(i, value & tmp) < 0) {
            fprintf(stderr, "led: Failed to switch %s led %d\n", (value & tmp) ? "on" : "off", i);
            return -1;
        }
    }

    return 0;
}

int led_configure_on_off_mode(const uint8_t mask)
{
    int i = 0, tmp = 1;

    for (; i < LED_CNT; ++i, tmp <<= 1) {
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

    for (; i < LED_CNT; ++i, tmp <<= 1) {
        if ((mask & tmp) == 0)
            continue;

        if (set_mode(i, "timer") < 0) {
            fprintf(stderr, "led: Failed to configure led %d in timer mode\n", i);
            return -1;
        }
    }

    return led_set_delay(ALL_LEDS, 0, 500);
}

int led_get_mode(const uint8_t led_index, uint8_t *led_mode)
{
    char path[MAX_STR_LENGTH];
    char str[MAX_STR_LENGTH];
    uint8_t index;

    if (led_mode == NULL) {
        fprintf(stderr, "led: Cannot store mode using null pointer.\n");
        return -1;
    }

    switch (led_index) {
    case LED_0:
        index = 0;
        break;
    case LED_1:
        index = 1;
        break;
    case LED_2:
        index = 2;
        break;
    case LED_3:
        index = 3;
        break;
    case LED_4:
        index = 4;
        break;
    case LED_5:
        index = 5;
        break;
    case LED_6:
        index = 6;
        break;
    case LED_HEARTBEAT:
        index = 7;
        break;
    default:
        fprintf(stderr, "led: Invalid led_index\n");
        return -1;
    }

    if (build_file_path(path, index, "trigger") < 0)
        return -1;

    if (read_str_file(path, str, MAX_STR_LENGTH) < 0)
        return -1;

    if (strstr(str, "[none]") != NULL)
        *led_mode = ON_OFF_MODE;
    else if (strstr(str, "[timer]") != NULL)
        *led_mode = TIMER_MODE;
    else {
        fprintf(stderr, "led: Unknown mode.\n");
        return -1;
    }

    return 0;
}

int led_set_delay(const uint8_t mask, const uint32_t delay_on, const uint32_t delay_off)
{
    int i = 0, tmp = 1;

    for (; i < LED_CNT; ++i, tmp <<= 1) {
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

    for (; i < LED_CNT; ++i) {
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
