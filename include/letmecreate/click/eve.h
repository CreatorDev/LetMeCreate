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

#endif
