/**
 * @file light.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_LIGHT_H__
#define __LETMECREATE_CLICK_LIGHT_H__

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Get light intensity measurement from Light Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click (see #MIKROBUS_INDEX)
 * This parameter is ignored if use_spi is different from 0.
 * @param[out] measure 12 bits value representing light intensity
 * @param[in] use_spi 0: use AN output, otherwise read value from SPI
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT light_click_get_measure(uint8_t mikrobus_index, uint16_t *measure, uint8_t use_spi);

#endif
