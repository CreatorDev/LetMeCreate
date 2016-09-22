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
