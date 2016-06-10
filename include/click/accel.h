/**
 * @file accel.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_ACCEL_H__
#define __LETMECREATE_CLICK_ACCEL_H__

/**
 * @brief Enable the Accel Click (Use SPI bus).
 *
 * @return 0 if successful, -1 otherwise
 */
int accel_click_enable(void);

/**
 * @brief Get current measure from Accel Click.
 *
 * @param[out] accelX X component of acceleration in g (must not be null)
 * @param[out] accelY Y component of acceleration in g (must not be null)
 * @param[out] accelZ Z component of acceleration in g (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int accel_click_get_measure(float *accelX, float *accelY, float *accelZ);

/**
 * @brief Disable the Accel Click (set it in sleep mode).
 *
 * @return 0 if successful, -1 otherwise
 */
int accel_click_disable(void);

#endif
