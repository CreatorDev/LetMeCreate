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
    int i = 0;
    spi_init();
    seven_seg_click_set_intensity(MIKROBUS_1, 100.f);

    for (; i < 100; ++i) {
        seven_seg_click_display_decimal_number(i);
        sleep_ms(100);
    }

    spi_release();

    return 0;
}
