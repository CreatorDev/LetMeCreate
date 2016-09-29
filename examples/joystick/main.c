/**
 * This example shows how to use the Joystick Click wrapper of the LetMeCreate.
 *
 * It continuously reads the position of the joystick, prints it in the terminal
 * and displays a pattern on the LED's based on the x coordinate.
 *
 * The Joystick Click must be inserted in Mikrobus 1 before running this program.
 */


#include <stdio.h>
#include <letmecreate/letmecreate.h>

/*   Roughly the variable that will be reached when the joystick is farthest
     left or right */
#define OFFSET      (98)
#define MAXIMUM     ((OFFSET) * 2)


static uint8_t get_led_mask(float perc)
{
    uint8_t mask = 0;
    int div = (1.f - perc) * LED_CNT;
    int i;

    if (div > LED_CNT)
        div = LED_CNT;

    for (i = 0; i < div; i++)
        mask |= (1 << i);

    return mask;
}

int main(void)
{
    i2c_init();
    led_init();

    /* Use Ctrl-C to break the loop */
    while (1) {
        int8_t x, y;
        int mask;

        if (joystick_click_get_position(&x, &y) == -1) {
            fprintf(stderr, "Failed to get joystick position.\n");
            break;
        }

        printf("%i %i\n", (int)x, (int)y);

        mask = get_led_mask((float)(x + OFFSET)/(float)MAXIMUM);
        led_switch_on(mask);
        led_switch_off(~mask);
    }

    led_release();
    i2c_release();

    return 0;
}
