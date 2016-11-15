#include <time.h>
#include <letmecreate/core/gpio.h>
#include <letmecreate/rpisensehat/board.h>


static void sleep_50ms(void)
{
    struct timespec rem, req = {
        .tv_sec = 0,
        .tv_nsec = 50000000
    };

    while (nanosleep(&req, &rem))
        req = rem;
}

static int reset_atmel_chip(void)
{
    if (gpio_init(GPIO_21) < 0
    ||  gpio_set_direction(GPIO_21, GPIO_OUTPUT) < 0)
        return -1;

    if (gpio_set_value(GPIO_21, 0) < 0)
        return -1;

    sleep_50ms();

    if (gpio_set_value(GPIO_21, 1) < 0)
        return -1;

    return 0;
}

int rpisensehat_init(void)
{
    /* Reset Atmel Chip otherwise it hangs the I2C bus. */
    if (reset_atmel_chip() < 0)
        return -1;

    return 0;
}
