#include <time.h>
#include <letmecreate/core.h>

static void sleep_ms(unsigned int ms)
{
    struct timespec req, rem;

    req.tv_sec = ms / 1000;
    ms -= req.tv_sec * 1000;
    req.tv_nsec = ms * 1000000;

    while (nanosleep(&req, &rem))
        req = rem;
}

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
