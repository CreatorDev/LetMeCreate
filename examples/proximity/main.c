#include <stdio.h>
#include "core/i2c.h"
#include "click/proximity.h"

int main(void)
{
    uint16_t measure;

    i2c_init(0);
    i2c_select_bus(0);

    proximity_click_enable();
    proximity_click_get_measure(&measure);
    printf("measure: %d\n", measure);
    proximity_click_disable();

    i2c_release(0);

    return 0;
}
