/**
 * @file 7seg.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_7SEG_H__
#define __LETMECREATE_CLICK_7SEG_H__

#include <stdint.h>

/**
 * @brief Display a decimal number on the 7Seg Click.
 *
 * Notice that the intensity of the LED's might be 0. In that case, calling this
 * function will not turn on the LED's.
 *
 * @param[in] number Must be in range 0-99
 * significant bits are used.
 * @return 0 if successful, otherwise it returns -1.
 */
int seven_seg_click_display_decimal_number(uint8_t number);

/**
 * @brief Display a hexidecimal number on the 7Seg Click.
 *
 * Notice that the intensity of the LED's might be 0. In that case, calling this
 * function will not turn on the LED's.

 * @param[in] hex Hexadecimal number
 * @return 0 if successful, otherwise it returns -1.
 */
int seven_seg_click_display_hex_number(uint8_t hex);

/**
 * @brief Set the intensity of the LED's of the 7seg Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @param[in] intensity Must be in range 0..100
 * @return 0 if successful, otherwise it returns -1.
 */
int seven_seg_click_set_intensity(uint8_t mikrobus_index, float intensity);

#endif
