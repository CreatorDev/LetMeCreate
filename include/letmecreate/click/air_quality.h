/**
 * @file air_quality.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CLICK_AIR_QUALITY_H__
#define __LETMECREATE_CLICK_AIR_QUALITY_H__

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Attach callback triggered if air quality is bad.
 *
 * Multiple callbacks can be attached to the air click output. Use #gpio_monitor_remove_callback
 * to remove them.
 *
 * @param mikrobus_index Index of the mikrobus used by the click (see #MIKROBUS_INDEX)
 * @param callback Function to call if air quality is bad (must not be null)
 * @return callback ID (positive integer) if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT air_quality_click_set_callback(uint8_t mikrobus_index, void(*callback)(uint8_t));

#endif
