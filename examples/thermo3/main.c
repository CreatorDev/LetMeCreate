/**
 * This example shows how to use the Thermo3 Click wrapper of the LetMeCreate
 * library.
 *
 * It reads the temperature from the sensor and exits.
 *
 * The Thermo3 Click must be inserted in Mikrobus 1 before running this program.
 */

#include <stdio.h>
#include <letmecreate/letmecreate.h>


int main(void)
{
    float temperature = 0.f;

    i2c_init();
    i2c_select_bus(MIKROBUS_1);

    thermo3_click_enable(0);
    thermo3_click_get_temperature(&temperature);
    printf("temperature: %.3f°C\n", temperature);
    thermo3_click_disable();

    i2c_release();

    return 0;
}
