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

int main(void)
{
    uint16_t value = 1;
    spi_init();

    /* Set LED's to max intensity */
    bargraph_click_set_intensity(MIKROBUS_1, 100.f);

    /* Gradually turn on each LED. */
    for (; value < 0x400; value <<= 1, value |= 1) {
        bargraph_click_set_value(value);
        sleep_ms(200);
    }

    spi_release();

    return 0;
}
