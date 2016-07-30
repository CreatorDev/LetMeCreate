#include <linux/spi/spidev.h>
#include <time.h>
#include <letmecreate/letmecreate.h>

static void sleep_40ms(void)
{
    struct timespec rem, req = {
        .tv_sec = 0,
        .tv_nsec = 40000000
    };

    while (nanosleep(&req, &rem))
        req = rem;
}

int main(void)
{
    uint8_t columns[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t cols;

    spi_init();
    spi_select_bus(MIKROBUS_1);
    led_matrix_click_enable();
    led_matrix_click_set_intensity(3);
    for (cols = 0; cols < 8; ++cols) {
        uint8_t lines;
        for (lines = 0; lines < 8; ++lines) {
            columns[cols] |= (1 << lines);
            led_matrix_click_set(columns);
            sleep_40ms();
        }
    }

    led_matrix_click_disable();
    spi_release();

    return 0;
}
