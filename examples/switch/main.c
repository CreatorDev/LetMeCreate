/**
 * This example shows how to use the Switch wrapper of the LetMeCreate library.
 *
 * It attaches a callback to all switch events for 15 seconds. Each time an
 * event happens, it prints hello on the screen.
 */

#include <stdio.h>
#include <unistd.h>
#include <letmecreate/letmecreate.h>

static void print_hello(void)
{
    printf("hello\n");
}

int main(void)
{
    switch_init();
    switch_add_callback(SWITCH_ALL_EVENTS, print_hello);
    printf("Switch callback are now active for 15 seconds.\n");
    printf("Press a switch to print \"hello\" on screen.\n");
    sleep(15);
    switch_release();

    return 0;
}
