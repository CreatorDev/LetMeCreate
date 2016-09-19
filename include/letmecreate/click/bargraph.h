/**
 * @file bargraph.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_BARGRAPH_H__
#define __LETMECREATE_CLICK_BARGRAPH_H__

#include <stdint.h>

/**
 * @brief Switch on/off LED's of the Bargraph Click.
 *
 * Notice that the intensity of the LED's might be 0. In that case, calling this
 * function will not turn on the LED's.
 *
 * @param[in] value Bit string to switch on/off LED's. Only the 10 least
 * significant bits are used.
 * @return 0 if successful, otherwise it returns -1.
 */
int bargraph_click_set_value(uint16_t value);

/**
 * @brief Set the intensity of the LED's of the Bargraph Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @param[in] intensity Must be in range 0..100
 * @return 0 if successful, otherwise it returns -1.
 */
int bargraph_click_set_intensity(uint8_t mikrobus_index, float intensity);

#endif
