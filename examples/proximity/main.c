#include <stdio.h>
#include <letmecreate/letmecreate.h>

int main(void)
{
    uint16_t measure;

    i2c_init();
    i2c_select_bus(MIKROBUS_1);

    proximity_click_enable();
    proximity_click_get_measure(&measure);
    printf("measure: %d\n", measure);
    proximity_click_disable();

    i2c_release();

    return 0;
}
