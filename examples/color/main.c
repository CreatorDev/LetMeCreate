#include <stdio.h>
#include "core/i2c.h"
#include "click/color.h"
#include "core/common.h"

int main(void)
{
    uint16_t clear = 0, red = 0, green = 0, blue = 0;

    i2c_init();
    i2c_select_bus(MIKROBUS_1);

    color_click_enable();
    color_click_get_color(&clear, &red, &green, &blue);
    color_click_disable();

    i2c_release();

    printf("clear\t:%d\nred\t:%d\ngreen\t:%d\nblue\t:%d\n", clear, red, green, blue);

    return 0;
}
