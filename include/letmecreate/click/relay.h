/**
 * @file relay.h
 * @author Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 *
 */

#ifndef __LETMECREATE_CLICK_RELAY_H__
#define __LETMECREATE_CLICK_RELAY_H__

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Enable relay 1 on Relay Click.
 *
 * This is the only relay that can be switched on the CI40. The second relay
 * uses a CS pin which is reserved by SPI.
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT relay_click_enable_relay_1(uint8_t mikrobus_index);

/**
 * @brief Disable relay 1 on Relay Click.
 *
 * This is the only relay that can be switched on the CI40. The second relay
 * uses a CS pin which is reserved by SPI.
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT relay_click_disable_relay_1(uint8_t mikrobus_index);

#endif
