/**
 * @file alphanum.h
 * @author Michael Schnell, Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example alphanum/main.c
 */

#ifndef __LETMECREATE_CLICK_ALPHANUM_H__
#define __LETMECREATE_CLICK_ALPHANUM_H__

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Initialise the Alphanum Click.
 *
 * During initialisation, the left display is selected.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT alphanum_click_init(uint8_t mikrobus_index);

/**
 * @brief Convert a character to a 14bit value.
 *
 * Not all characters can be converted. For the complete list, see the table in
 * alphanum.c. The output of this function can be passed as an argument to
 * alphanum_click_raw_write.
 *
 * @param[in] c Character to convert
 * @param[out] value 14bit value which encodes the character (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT alphanum_click_get_char(char c, uint16_t *value);

/**
 * @brief Latch two characters in the LED drivers.
 *
 * Depending on the selected display, either a or b will be displayed. Not all
 * characters can be displayed. See table in alphanum.c for the complete list.
 *
 * @param[in] a Character on left display
 * @param[in] b Character on right display
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT alphanum_click_write(char a, char b);

/**
 * @brief Latch two values in the LED drivers.
 *
 * @param[in] a Value for left display
 * @param[in] b Value for right display
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT alphanum_click_raw_write(uint16_t a, uint16_t b);

/**
 * @brief Select the left display.
 *
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT alphanum_click_select_left_display(void);

/**
 * @brief Select the right display.
 *
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT alphanum_click_select_right_display(void);

#endif
