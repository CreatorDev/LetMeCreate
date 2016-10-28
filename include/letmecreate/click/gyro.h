/**
 * @file gyro.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_GYRO_H__
#define __LETMECREATE_CLICK_GYRO_H__

#include <letmecreate/click/export.h>

/**
 * @brief Enable and initialise the GYRO Click.
 *
 * Note that the I2C bus must be initialised before attempting to enable the
 * device.
 *
 * @param[in] add_bit GYRO click has a jumper on its board to change its I2C address (must be 0 or 1)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT gyro_click_enable(uint8_t add_bit);

/**
 * @brief Read measurement from GYRO Click.
 *
 * @param[out] x Measurement on X axis in degrees per second
 * @param[out] y Measurement on Y axis in degrees per second
 * @param[out] z Measurement on Z axis in degrees per second
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT gyro_click_get_measure(float *x, float *y, float *z);

/**
 * @brief Disable GYRO Click.
 *
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT gyro_click_disable(void);

#endif
