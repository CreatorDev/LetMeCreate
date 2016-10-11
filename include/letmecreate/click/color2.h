/**
 * @file color2.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example color2/main.c
 */

#ifndef __LETMECREATE_CLICK_COLOR2_H__
#define __LETMECREATE_CLICK_COLOR2_H__

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Enables Color2 Click.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT color2_click_enable(void);

/**
 * @brief Reads color measurement from sensor.
 *
 * @param[out] red Red light intensity (must not be null)
 * @param[out] green Green light intensity (must not be null)
 * @param[out] blue Blue light intensity (must not be null)
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT color2_click_get_color(uint16_t *red, uint16_t *green, uint16_t *blue);

/**
 * @brief Disables Color2 Click.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT color2_click_disable(void);

#endif
