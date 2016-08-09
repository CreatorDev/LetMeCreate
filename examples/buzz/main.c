#include <stdio.h>
#include "core/pwm.h"
#include "core/common.h"

int main(void)
{
  pwm_init(MIKROBUS_1);

  pwm_set_period(MIKROBUS_1, 340000); //determines pitch + volume(slightly). Stay below 370000. Lower number is higher pitch.
  pwm_set_duty_cycle(MIKROBUS_1, 50); //determines volume (0->100, 100 is not max! Read PWM docs for info).

  int x=0;
  for(x=0; x<3; x++)
  {
    pwm_enable(MIKROBUS_1);
    sleep(1);
    pwm_disable(MIKROBUS_1);
    sleep(1);
  }


  pwm_release(MIKROBUS_1);

  return 0;

}

