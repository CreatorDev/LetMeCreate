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
 * The Light click's jumper must be configured to AN.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click (see #MIKROBUS_INDEX)
 * @param[out] measure Light intensity in range 0-65535
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT light_click_get_measure(uint8_t mikrobus_index, uint16_t *measure);

#endif
