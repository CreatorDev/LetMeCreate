/**
 * @file motion.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_MOTION_H__
#define __LETMECREATE_CLICK_MOTION_H__

#include <stdint.h>


int motion_click_enable(const uint8_t mikrobus_index);
int motion_click_attach_callback(const uint8_t mikrobus_index, void(*callback)(uint8_t));
int motion_click_disable(const uint8_t mikrobus_index);

#endif
