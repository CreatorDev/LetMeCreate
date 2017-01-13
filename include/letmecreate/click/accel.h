/**
 * @file accel.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example accel/main.c
 */

#ifndef __LETMECREATE_CLICK_ACCEL_H__
#define __LETMECREATE_CLICK_ACCEL_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Use SPI bus to communicate with Accel Click.
 *
 * By default, it uses SPI to communicate with Accel Click. This function should
 * be called before using the device.
 */
void LETMECREATE_CLICK_EXPORT accel_click_use_spi(void);

/**
 * @brief Use I2C bus to communicate with Accel Click.
 *
 * This function should be called before using the device.
 *
 * @param add_bit Jumper on the click board to change the address of the device.
 * Should be 0 or 1.
 */
void LETMECREATE_CLICK_EXPORT accel_click_use_i2c(uint8_t add_bit);

/**
 * @brief Enable the Accel Click (Use SPI bus).
 *
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT accel_click_enable(void);

/**
 * @brief Get current measure from Accel Click.
 *
 * @param[out] accelX X component of acceleration in g (must not be null)
 * @param[out] accelY Y component of acceleration in g (must not be null)
 * @param[out] accelZ Z component of acceleration in g (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT accel_click_get_measure(float *accelX, float *accelY, float *accelZ);

/**
 * @brief Disable the Accel Click (set it in sleep mode).
 *
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT accel_click_disable(void);

#ifdef __cplusplus
}
#endif

#endif
