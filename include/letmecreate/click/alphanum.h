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

/* Init the alphanum clicker */
int LETMECREATE_CLICK_EXPORT alphanum_click_init(uint8_t mikrobus_index);

/* Converts a char into a 14 segment display value */
int LETMECREATE_CLICK_EXPORT alphanum_click_get_char(char c, uint16_t *value);

/* Write the two chars to */
int LETMECREATE_CLICK_EXPORT alphanum_click_write(char a, char b);

/* Raw write  */
int LETMECREATE_CLICK_EXPORT alphanum_click_raw_write(uint16_t a, uint16_t b);

int LETMECREATE_CLICK_EXPORT alphanum_click_select_left_display(void);

int LETMECREATE_CLICK_EXPORT alphanum_click_select_right_display(void);

#endif
