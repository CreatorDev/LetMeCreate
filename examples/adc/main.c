#include <stdio.h>
#include <letmecreate/letmecreate.h>


int main(void)
{
    uint16_t value;
    int i = 0;

    spi_init();
    for (; i < 4; ++i) {
        adc_click_get_raw_value(i, &value);
        printf("channel %d value: %u\n", i, value);
    }
    spi_release();

    return 0;
}
