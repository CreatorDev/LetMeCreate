#ifndef __LETMECREATE_CLICK_THERMO3_H__
#define __LETMECREATE_CLICK_THERMO3_H__

#include <stdint.h>

int thermo3_click_enable(const uint8_t add_bit);
int thermo3_click_get_temperature(float *temperature);
int thermo3_click_disable(void);

#endif
