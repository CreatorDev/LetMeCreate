#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "core/led.h"


#define LED_DEVICE_FILE_PATH        "/sys/class/leds/marduk:red:user%d/brightness"
#define NB_LEDS                     (7)

int fds[NB_LEDS] = { -1, -1, -1, -1, -1, -1, -1};

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

int led_initialize(void)
{
    int i = 0;
    char path[255];

    for (; i < NB_LEDS; ++i) {
        if (fds[i] >= 0)
            continue;

        if (snprintf(path, sizeof(path), LED_DEVICE_FILE_PATH, i+1) < 0) {
            fprintf(stderr, "led: Failed to build device file path for led %d\n", i);
            return -1;
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
                 | LED_6);

    return 0;
}

int led_switch_on(const uint8_t led_mask)
{
    int i = 0, tmp = 1;

    for (; i < NB_LEDS; ++i, tmp <<= 1) {
        if (led_mask & tmp) {
            if (led_set_value(i, 1) < 0) {
                fprintf(stderr, "Failed to switch on led %d\n", i);
                return -1;
            }
        }
    }

    return 0;
}

int led_switch_off(const uint8_t led_mask)
{
    int i = 0, tmp = 1;

    for (; i < NB_LEDS; ++i, tmp <<= 1) {
        if (led_mask & tmp) {
            if (led_set_value(i, 0) < 0) {
                fprintf(stderr, "Failed to switch off led %d\n", i);
                return -1;
            }
        }
    }

    return 0;
}

int led_set(const uint8_t led_mask, const uint8_t led_value)
{
    int i = 0, tmp = 1;

    for (; i < NB_LEDS; ++i, tmp <<= 1) {
        if ((led_mask & tmp) == 0)
            continue;

        if (led_set_value(i, led_value & led_mask) < 0) {
            fprintf(stderr, "Failed to switch %s led %d\n", (led_value & led_mask) ? "on" : "off", i);
            return -1;
        }
    }

    return 0;
}

void led_release(void)
{
    int i = 0;

    for (; i < NB_LEDS; ++i) {
        if (fds[i] < 0)
            continue;

        close(fds[i]);
        fds[i] = -1;
    }
}
