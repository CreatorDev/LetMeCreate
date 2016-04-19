#ifndef __LETMECREATE_CLICK_PROXIMITY_H__
#define __LETMECREATE_CLICK_PROXIMITY_H__

#include <stdint.h>

int proximity_click_enable(void);
int proximity_click_get_measure(uint16_t *measure);
int proximity_click_disable(void);

#endif
