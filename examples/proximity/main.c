#include <stdio.h>
#include "core/i2c.h"
#include "click/proximity.h"

int main(void)
{
    uint16_t measure;

    i2c_init(MIKROBUS_I2C_BUS_1);
    i2c_select_bus(MIKROBUS_I2C_BUS_1);

    proximity_click_enable();
    proximity_click_get_measure(&measure);
    printf("measure: %d\n", measure);
    proximity_click_disable();

    i2c_release(MIKROBUS_I2C_BUS_1);

    return 0;
}
