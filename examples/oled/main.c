#include <stdio.h>
#include <letmecreate/letmecreate.h>

/*
 * Example how to write a text to the OLED display.
 */
int main(void)
{
    i2c_init();
    oled_click_enable(MIKROBUS_1);
    oled_click_write_text("Hello   Creator!");

    oled_click_set_contrast(0xFF);

    return 0;
}
