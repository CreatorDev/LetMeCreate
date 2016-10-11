/**
 * This example shows how to use the Color Click wrapper of the LetMeCreate.
 *
 * It takes a measurement from the sensor, prints it in the terminal and exits.
 *
 * The Color Click must be inserted in Mikrobus 1 before running this program.
 */


#include <stdio.h>
#include <letmecreate/letmecreate.h>

int main(void)
{
    uint16_t clear = 0, red = 0, green = 0, blue = 0;

    i2c_init();

    color_click_enable();
    color_click_get_color(&clear, &red, &green, &blue);
    color_click_disable();

    i2c_release();

    printf("clear\t:%d\nred\t:%d\ngreen\t:%d\nblue\t:%d\n", clear, red, green, blue);

    return 0;
}
