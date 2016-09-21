#include <letmecreate/letmecreate.h>
#include "examples/common.h"


int main(void)
{
    int x = 0;

    pwm_init(MIKROBUS_1);

    /* determines pitch + volume(slightly). Stay below 370000. Lower number is higher pitch.
     * period: 340000ns (freq: 2.9kHz)
     */
    pwm_set_period(MIKROBUS_1, 340000);

    /* determines volume (0->100, 100 is not max! Read PWM docs for info). */
    pwm_set_duty_cycle(MIKROBUS_1, 50);


    /* 3 quick beeps */
    for (x = 0; x < 3; ++x) {
        pwm_enable(MIKROBUS_1);
        sleep_ms(100);
        pwm_disable(MIKROBUS_1);
        sleep_ms(100);
    }

    pwm_release(MIKROBUS_1);

    return 0;
}

