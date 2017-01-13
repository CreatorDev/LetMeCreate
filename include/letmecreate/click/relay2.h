/**
 * @file relay2.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example relay2/main.c
 */

#ifndef __LETMECREATE_CLICK_RELAY2_H__
#define __LETMECREATE_CLICK_RELAY2_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/** Relay Click channel index */
enum RELAY2_CLICK_RELAY {
    RELAY2_CLICK_RELAY_1,
    RELAY2_CLICK_RELAY_2,
    RELAY2_CLICK_RELAY_COUNT
};

/**
 * @brief Enable a relay of Relay2 Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @param[in] relay Index of the relay to enable (see #RELAY2_CLICK_RELAY)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT relay2_click_enable_relay(uint8_t mikrobus_index, uint8_t relay);

/**
 * @brief Disable a relay of Relay2 Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @param[in] relay Index of the realy to disable (see #RELAY2_CLICK_RELAY)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT relay2_click_disable_relay(uint8_t mikrobus_index, uint8_t relay);

/**
 * @brief Set state a relay of Relay2 Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @param[in] relay Index of the realy to disable (see #RELAY2_CLICK_RELAY)
 * @param[in] state 0 for disabling a relay, any other value for enabling a relay
 * @return 0 if successful, -1 otherwise
 */
int relay2_click_set_relay_state(uint8_t mikrobus_index, uint8_t relay, uint8_t state);

/**
 * @brief Get current state (on/off) of a relay.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @param[in] relay Index of the realy to disable (see #RELAY2_CLICK_RELAY)
 * @param[out] state 0 if off, 1 if on
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT relay2_click_get_relay_state(uint8_t mikrobus_index, uint8_t relay, uint8_t *state);

#ifdef __cplusplus
}
#endif

#endif
