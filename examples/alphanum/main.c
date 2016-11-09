#include <stdio.h>
#include <letmecreate/letmecreate.h>

/*
 * Example to write the characters on the alphanum clicker.
 */
int
main(int argc, char *argv[])
{
    int ret = 0;
    char str[] = "Hi"; /* String to print */

    /* SPI int */
    if ((ret = spi_init()) != 0) {
        printf("Error spi init failed with %i\n", ret);
        return ret;
    }

    /* Initialize the alphanum clicker */
    if (alphanum_init(MIKROBUS_2) != 0) {
        printf("Error: Clicker cannot be initiliazed\n");
        return 1;
    }

    printf("Writing: %s\n", str);
    if (alphanum_write(str[0], str[1]) != 0) {
        printf("Error: Cannot write to alphanum clicker\n");
        return 1;
    }

    /* This alternately switches on the output of either the two shift registers
     * to appear to print two characters at the same time.
     */
    alphanum_switch_cycles(0);

    return 0;
}
