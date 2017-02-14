/**
 * @file motion.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_MOTION_H__
#define __LETMECREATE_CLICK_MOTION_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Enable the Motion Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click board (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT motion_click_enable(uint8_t mikrobus_index);

/**
 * @brief Attach a callback if movement is detected or if no movement is detected anymore.
 *
 * Use #gpio_monitor_remove_callback to detach your callback.
 * Notice that you can attach multiple callbacks.
 * The argument of the callback function has value 1 or 2. If it has value 1, it means
 * that a movement has been detected, and if it has value 2 no movement is detected.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click board (see #MIKROBUS_INDEX)
 * @param[in] callback Function to call if movement is detected
 * @return ID of the callback (positive integer) if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT motion_click_attach_callback(uint8_t mikrobus_index, void(*callback)(uint8_t));

/**
 * @brief Disable the Motion Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click board (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT motion_click_disable(uint8_t mikrobus_index);

#ifdef __cplusplus
}
#endif

#endif
