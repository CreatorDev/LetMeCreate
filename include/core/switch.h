/**
 * @file switch.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_SWITCH_H__
#define __LETMECREATE_CORE_SWITCH_H__

#include <stdint.h>

enum SWITCH_EVENT {
    SWITCH_1_PRESSED    = 0x01,
    SWITCH_1_RELEASED   = 0x02,
    SWITCH_2_PRESSED    = 0x04,
    SWITCH_2_RELEASED   = 0x08
};

int switch_init(void);
int switch_add_callback(const uint8_t event_mask, void (*callback)(void));
int switch_remove_callback(const int callback_ID);
void switch_release(void);

#endif
