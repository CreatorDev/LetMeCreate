/**
 * @file motion.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_MOTION_H__
#define __LETMECREATE_CLICK_MOTION_H__

#include <stdint.h>


/**
 * @brief Enable the Motion Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click board (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int motion_click_enable(uint8_t mikrobus_index);

/**
 * @brief Attach a callback if movement is detected.
 *
 * Notice that you can attach multiple callbacks.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click board (see #MIKROBUS_INDEX)
 * @param[in] callback Function to call if movement is detected
 * @return ID of the callback. Use #gpio_monitor_remove_callback to detach your callback.
 */
int motion_click_attach_callback(uint8_t mikrobus_index, void(*callback)(uint8_t));

/**
 * @brief Disable the Motion Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click board (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int motion_click_disable(uint8_t mikrobus_index);

#endif
