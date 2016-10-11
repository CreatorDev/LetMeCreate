/**
 * This example shows how to use the Proximity Click wrapper of the LetMeCreate library.
 *
 * It reads one proximity measurement from the click and exits.
 *
 * The Proximity Click must be inserted in Mikrobus 1 before running this program.
 */

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
