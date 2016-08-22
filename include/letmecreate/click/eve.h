/**
 * @file eve.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_EVE_H__
#define __LETMECREATE_CLICK_EVE_H__

#include <stdbool.h>
#include <stdint.h>
#include "letmecreate/click/ft800_defs.h"

int eve_click_enable(uint8_t mikrobus_index);
int eve_click_disable(uint8_t mikrobus_index);

/*
 * Enable/Disable buffering
 *   * all graphics commands:
 *   * load_identity
 *
 *All other commands are never buffered and transmitted immediately to ft800.
 */
void eve_click_disable_buffering(void);
void eve_click_enable_buffering(void);
bool eve_click_is_buffering_enabled(void);

/* Draw operations on FT800 chip */
int eve_click_clear(uint8_t r, uint8_t g, uint8_t b);
int eve_click_draw(uint32_t cmd, ...);
int eve_click_display(void);

/* Loading image data in FT800 chip memory */
int eve_click_load_image(uint32_t ptr, uint32_t options, const uint8_t *data, uint32_t count);

/* Memory operations on FT800 chip */
int eve_click_memcrc(uint32_t ptr, uint32_t byte_count, uint32_t *crc);
int eve_click_memset(uint32_t ptr, uint32_t value, uint32_t byte_count);
int eve_click_memcpy(uint32_t dest, uint32_t src, uint32_t byte_count);
int eve_click_memzero(uint32_t ptr, uint32_t byte_count);

/* Other commands */
int eve_click_ftdi_logo(void);
int eve_click_snapshot(uint32_t ptr, uint8_t *data);
int eve_click_spinner(int16_t x, int16_t y, uint16_t style, uint16_t scale);
int eve_click_stop(void);

#endif
