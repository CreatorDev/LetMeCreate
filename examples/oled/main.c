/**
 * This example shows how to use the OLED Click wrapper of the LetMeCreate library.
 *
 * It displays some text for three seconds and then, it exits.
 *
 * The OLED Click must be inserted in Mikrobus 1 before running this program.
 * In addition, the OLED Click must be configured to use I2C.
 */

#include <stdio.h>
#include <unistd.h>
#include <letmecreate/letmecreate.h>

int main(void)
{
    i2c_init();
    oled_click_enable(MIKROBUS_1);
    oled_click_write_text("Hello   Creator!");
    sleep(3);
    oled_click_disable();
    i2c_release();

    return 0;
}
