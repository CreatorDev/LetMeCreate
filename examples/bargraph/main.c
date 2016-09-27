/**
 * This example shows how to use the BarGraph Click wrapper of the LetMeCreate
 * library.
 *
 * It turns on gradually all the LED's from left to right in 2 seconds.
 *
 * The BarGraph Click must be inserted in Mikrobus 1 before running this program.
 */

#include <letmecreate/letmecreate.h>
#include "examples/common.h"


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
