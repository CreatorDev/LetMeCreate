#include <stdbool.h>
#include "core/led.h"
#include "click/motion.h"
#include "core/common.h"

void flash(uint8_t null)
{
	int x = 0;
	for(x=0; x<10; x++)
	{
		led_switch_on(ALL_LEDS);
		usleep(100000);
		led_switch_off(ALL_LEDS);
		usleep(100000);
	}
}

int main(void)
{
	motion_click_enable(MIKROBUS_1);
	led_init();
	while(1)
	{
		static bool b = true;
		if(b)
		{
			motion_click_attach_callback(MIKROBUS_1, flash);
			b = false;      
		}  
	}
}

