/**
 * This example shows how to use the UNI Hall Click wrapper of the LetMeCreate
 * library.
 *
 * It attaches a callback that prints hello everytime it starts/stops detecting
 * the north pole of a magnet during 10 seconds.
 *
 * The UNI Hall Click must be inserted in Mikrobus 1 before running the program.
 */


#include <letmecreate/letmecreate.h>
#include <stdio.h>
#include <unistd.h>

void print_hello(uint8_t arg)
{
    if (arg == GPIO_FALLING)
        printf("Hello, starts dectecting north pole\n");
    else if (arg == GPIO_RAISING)
        printf("Hello, stops dectecting north pole\n");
}

int main(void)
{
    uni_hall_click_attach_callback(MIKROBUS_1, print_hello);
    printf("Callback is now active for 15 seconds.\n");
    printf("Move the north pole of a magnet over the sensor to print \"hello\".\n");
    sleep(15);

    return 0;
}
