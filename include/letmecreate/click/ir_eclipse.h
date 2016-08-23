#ifndef __LETMECREATE_CLICK_IR_ECLIPSE_H__
#define __LETMECREATE_CLICK_IR_ECLIPSE_H__

#include <stdint.h>

/**
 * @brief Attach callback triggered if an object is detected.
 *
 * To remove this callback, call gpio_monitor_remove_callback function with
 * the callback ID returned by this function.
 *
 * @param[in] mikrobus_index Index of the mikrobus
 * @param[in] callback Must not be null.
 * @return A callback ID (positive integer) if successful, -1 otherwise
 */
int ir_eclipse_click_add_callback(uint8_t mikrobus_index, void (*callback)(uint8_t));

#endif
