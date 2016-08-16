#include <stdio.h>
#include <letmecreate/letmecreate.h>

/*   Roughly the variable that will be reached when the joystick is farthest
     left or right */
#define OFFSET  98
#define MAXIMUM (OFFSET * 2)


int get_led_mask(float perc)
{
    int mask = 0;
    int div = perc * LED_CNT;
    int i;
    if (div > LED_CNT)
        div = LED_CNT;

    for (i = 0; i < div; i++)
        mask |= (1 << i);

    return mask;
}

int main()
{
    i2c_init();
    i2c_select_bus(MIKROBUS_1);
    led_init();

    /* Use Ctrl-C to break the loop */
    while (1) {
        int8_t x, y;
        int mask;

        if (joystick_click_get_position(&x, &y) == -1)
            break;

        printf("%i %i\n", (int)x, (int)y);

        mask = get_led_mask((float)(x + OFFSET)/(float)MAXIMUM);
        led_switch_on(mask);
        led_switch_off(~mask);
    }

    led_release();
    i2c_release();

    return 0;
}
