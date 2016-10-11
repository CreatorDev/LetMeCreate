/**
 * @file ir_distance.h
 * @author Matthew Fennell, Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_IR_DISTANCE_H__
#define __LETMECREATE_CLICK_IR_DISTANCE_H__

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Enable IR distance click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT ir_distance_click_enable(uint8_t mikrobus_index);

/**
 * @brief Get a measurement from IR distance click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @param[out] measure the value provided by the IR distance click in range 0..65535
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT ir_distance_click_get_measure(uint8_t mikrobus_index, uint16_t *measure);

/**
 * @brief Disable IR distance click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT ir_distance_click_disable(uint8_t mikrobus_index);

#endif
