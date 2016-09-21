#include <letmecreate/core.h>
#include "examples/common.h"


int main(void)
{
    int loops;
    led_init();

    for (loops = 3; loops > 0; --loops) {
       int value;
       for (value = 1; value < 0x100; value <<= 1, value |= 1) {
           led_set(ALL_LEDS, value);
           sleep_ms(80);
       }
    }

    led_release();
    return 0;
}
