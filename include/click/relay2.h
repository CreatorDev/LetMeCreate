/**
 * @file relay2.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_RELAY2_H__
#define __LETMECREATE_CLICK_RELAY2_H__

#include <stdint.h>

/**
 * @brief Enable relay 1 on Relay2 Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int relay2_click_enable_relay_1(uint8_t mikrobus_index);

/**
 * @brief Disable relay 1 on Relay2 Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int relay2_click_disable_relay_1(uint8_t mikrobus_index);

/**
 * @brief Enable relay 2 on Relay2 Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int relay2_click_enable_relay_2(uint8_t mikrobus_index);

/**
 * @brief Disable relay 2 on Relay2 Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int relay2_click_disable_relay_2(uint8_t mikrobus_index);

#endif
