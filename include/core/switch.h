/**
 * @file switch.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_SWITCH_H__
#define __LETMECREATE_CORE_SWITCH_H__

#include <stdint.h>

/** Event that can be triggered by the two switches */
enum SWITCH_EVENT {
    SWITCH_1_PRESSED    = 0x01,
    SWITCH_1_RELEASED   = 0x02,
    SWITCH_2_PRESSED    = 0x04,
    SWITCH_2_RELEASED   = 0x08,
    SWITCH_ALL_EVENTS   = 0x0F
};

/**
 * @brief Start a thread monitoring switch events
 *
 * @return 0 if successful, -1 otherwise.
 */
int switch_init(void);

/**
 * @brief Attach a callback to switch events
 *
 * @param event_mask Events which will trigger a call to callback function
 * @param callback Pointer to a function (must not be null)
 * @return ID of the callback, -1 if it fails.
 */
int switch_add_callback(const uint8_t event_mask, void (*callback)(void));

/**
 * @brief Remove a callback
 *
 * @param callback_ID ID of the callback to remove
 * @return 0 if successful, -1 otherwise
 */
int switch_remove_callback(const int callback_ID);

/**
 * @brief Stop monitoring switch events
 *
 * @return 0 if successful, -1 otherwise
 */
int switch_release(void);

#endif
