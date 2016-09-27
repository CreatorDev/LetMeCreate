/**
 * This example shows how to use the Motion Click wrapper of the LetMeCreate
 * library.
 *
 * Whenever the motion click detects an event, it flashes all LED's ten times.
 * The user must press Ctrl+C to terminate the program.
 *
 * The Motion Click must be inserted in Mikrobus 1 before running this program.
 */

#include <stdio.h>
#include <letmecreate/letmecreate.h>
#include "examples/common.h"


void flash_leds(uint8_t null)
{
    int x = 0;
    for(; x < 10; ++x) {
        led_switch_on(ALL_LEDS);
        sleep_ms(100);
        led_switch_off(ALL_LEDS);
        sleep_ms(100);
    }
}

int main(void)
{
    led_init();
    motion_click_enable(MIKROBUS_1);
    motion_click_attach_callback(MIKROBUS_1, flash_leds);

    printf("LED's will flash when Motion Click detects a movement.\n");
    printf("Press Ctrl+C to quit.\n");

    while (1)
        ;

    return 0;
}
