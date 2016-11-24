/**
 * @file uni_hall.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_UNI_HALL_H__
#define __LETMECREATE_CLICK_UNI_HALL_H__

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Call function when north pole is detected.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click board (see #MIKROBUS_INDEX)
 * @param[in] callback Function to call when uni hall click detects north pole (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT uni_hall_click_attach_callback(uint8_t mikrobus_index, void (*callback)(uint8_t));

#endif
