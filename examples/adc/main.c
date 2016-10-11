/**
 * This example shows how to use the ADC Click wrapper of the LetMeCreate
 * library.
 *
 * It reads ADC values from the four channels. Each value should be in range
 * 0..4095. If no wires are connected to the channels of the ADC Click, then
 * the values should be random.
 *
 * The ADC Click must be inserted in Mikrobus 1 before running the program.
 */

#include <stdio.h>
#include <letmecreate/letmecreate.h>


int main(void)
{
    uint16_t value;
    int i = 0;

    spi_init();
    for (; i < ADC_CLICK_CHANNEL_COUNT; ++i) {
        adc_click_get_raw_value(i, &value);
        printf("channel %d value: %u\n", i, value);
    }
    spi_release();

    return 0;
}
