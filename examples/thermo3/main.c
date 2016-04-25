#include <stdio.h>
#include "core/i2c.h"
#include "click/thermo3.h"

int main(void)
{
    float temperature = 0.f;

    i2c_init(MIKROBUS_I2C_BUS_1);
    i2c_select_bus(MIKROBUS_I2C_BUS_1);

    thermo3_click_enable(0);
    thermo3_click_get_temperature(&temperature);
    printf("temperature: %.3f°C\n", temperature);
    thermo3_click_disable();

    i2c_release(MIKROBUS_I2C_BUS_1);

    return 0;
}
