/**
 * @file thermo3.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example thermo3/main.c
 */


#ifndef __LETMECREATE_CLICK_THERMO3_H__
#define __LETMECREATE_CLICK_THERMO3_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Enable Thermo3 click.
 *
 * @param add_bit Thermo3 click has a jumper on its board to change its address (must be 0 or 1)
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT thermo3_click_enable(uint8_t add_bit);

/**
 * @brief Get a temperature measure (in degrees Celsius) from Thermo3 click.
 *
 * @param[out] temperature Pointer to a 16-bit variable for writing temperature read from Thermo3 click
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT thermo3_click_get_temperature(float *temperature);

/**
 * @brief Configure Thermo3 click to trigger an interrupt if temperature is too high.
 *
 * Notice that once the temperature is higher than threshold, callback will be called up to four
 * times a second. Use #gpio_monitor_remove_callback to remove callback.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click (see #MIKROBUS_INDEX)
 * @param[in] threshold in degrees Celsius
 * @param[in] callback Function to call if temperature is higher than threshold (must not be null)
 * @return callback ID (positive integer if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT thermo3_click_set_alarm(uint8_t mikrobus_index, float threshold, void(*callback)(uint8_t));

/**
 * @brief Disable Thermo3 click.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT thermo3_click_disable(void);

#ifdef __cplusplus
}
#endif

#endif
