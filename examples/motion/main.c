#include <stdio.h>
#include <time.h>
#include <letmecreate/letmecreate.h>


static void sleep_ms(unsigned int ms)
{
    struct timespec req, rem;

    req.tv_sec = ms / 1000;
    ms -= req.tv_sec * 1000;
    req.tv_nsec = ms * 1000000;

    while (nanosleep(&req, &rem))
        req = rem;
}

void flash_leds(uint8_t null)
{
    int x = 0;
    for(; x < 10; ++x) {
        led_switch_on(ALL_LEDS);
        sleep_ms(100);
        led_switch_off(ALL_LEDS);
        sleep_ms(100);
    }
}

int main(void)
{
    led_init();
    motion_click_enable(MIKROBUS_1);
    motion_click_attach_callback(MIKROBUS_1, flash_leds);

    printf("LED's will flash when Motion Click detects a movement.\n");
    printf("Press Ctrl+C to quit.\n");

    while (1)
        ;

    return 0;
}

