#include <stdio.h>
#include <letmecreate/letmecreate.h>

/*
 * Example to write the characters on the alphanum clicker.
 */
int main(void)
{
    int ret = 0;
    char str[] = "Hi"; /* String to print */

    /* SPI int */
    if ((ret = spi_init()) != 0) {
        printf("Error spi init failed with %i\n", ret);
        return ret;
    }

    spi_select_bus(MIKROBUS_1);

    /* Initialize the alphanum clicker */
    if (alphanum_click_init(MIKROBUS_1) != 0) {
        printf("Error: Clicker cannot be initiliazed\n");
        return 1;
    }

    printf("Writing: %s\n", str);
    if (alphanum_click_write(str[0], str[1]) != 0) {
        printf("Error: Cannot write to alphanum clicker\n");
        return 1;
    }

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
