/*
 * This example shows how to use the Color2 Click wrapper of the LetMeCreate
 * library.
 *
 * The Color2 Click must be plugged in Mikrobus 1 of Ci40.
 */

#include <stdio.h>
#include <letmecreate/letmecreate.h>


int main(void)
{
    uint16_t r, g, b;

    /* Initialise I2C and color2 click */
    i2c_init();
    color2_click_enable();

    /* Read measurement from Colo2 Click */
    color2_click_get_color(&r, &g, &b);
    printf("red: %u\n", r);
    printf("green: %u\n", g);
    printf("blue: %u\n\n", b);

    /* Disable color2 click and I2C */
    color2_click_disable();
    i2c_release();

    return 0;
}
