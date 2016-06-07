/**
 * @file relay2.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_RELAY2_H__
#define __LETMECREATE_CLICK_RELAY2_H__

#include <stdint.h>

int relay2_click_enable_relay_1(const uint8_t mikrobus_index);
int relay2_click_disable_relay_1(const uint8_t mikrobus_index);
int relay2_click_enable_relay_2(const uint8_t mikrobus_index);
int relay2_click_disable_relay_2(const uint8_t mikrobus_index);

#endif
