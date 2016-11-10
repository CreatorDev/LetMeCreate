/**
 * @file oled.h
 * @author Michael Schnell, Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example oled/main.c
 */

#ifndef __LETMECREATE_CLICK_OLED_H__
#define __LETMECREATE_CLICK_OLED_H__

#include <stdint.h>
#include <letmecreate/click/export.h>

/* Initialize the oled clicker */
int LETMECREATE_CLICK_EXPORT oled_click_init(uint8_t mikrobus_index);

int LETMECREATE_CLICK_EXPORT oled_click_set_contrast(uint8_t contrast);

/* Write a raw picture */
int LETMECREATE_CLICK_EXPORT oled_click_raw_write(uint8_t *data);

/* Write a text with the default monospace font */
int LETMECREATE_CLICK_EXPORT oled_click_write_text(char *str);

/* Translate character into raster font graphics */
int LETMECREATE_CLICK_EXPORT oled_click_get_char(char c, const uint8_t **out);

int LETMECREATE_CLICK_EXPORT oled_click_disable(void);

#endif
