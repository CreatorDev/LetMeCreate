#include <letmecreate/letmecreate.h>
#include "examples/common.h"


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
