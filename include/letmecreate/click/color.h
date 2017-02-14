/**
 * @file color.h
 * @author Matthew Fennell
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example color/main.c
 */

#ifndef __LETMECREATE_CLICK_COLOR_H__
#define __LETMECREATE_CLICK_COLOR_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Enables color click.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT color_click_enable(void);

/**
 * @brief Reads color measurement from sensor.
 *
 * @param[out] clear Light intensity
 * @param[out] red Red light intensity
 * @param[out] green Green light intensity
 * @param[out] blue Blue light intensity
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT color_click_get_color(uint16_t *clear, uint16_t *red, uint16_t *green, uint16_t *blue);

/**
 * @brief Disables color click.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT color_click_disable(void);

#ifdef __cplusplus
}
#endif

#endif
