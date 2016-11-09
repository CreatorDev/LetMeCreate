#include <stdio.h>
#include <letmecreate/letmecreate.h>


int main(void)
{
    spi_init();
    spi_select_bus(MIKROBUS_1);

    if (alphanum_click_init(MIKROBUS_1) < 0)
        return 1;

    if (alphanum_click_write('C', 'i') < 0)
        return 1;

    /* This alternately switches on the output of either the two shift registers
     * to appear to print two characters at the same time.
     */
    while (1) {
        alphanum_click_select_left_display();
        sleep_ms(5);
        alphanum_click_select_right_display();
        sleep_ms(5);
    }


    return 0;
}
