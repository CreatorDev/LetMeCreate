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
 * @brief Configure wrapper to use SPI to communicate with the GYRO Click.
 */
void LETMECREATE_CLICK_EXPORT gyro_click_use_spi(void);

/**
 * @brief Configure wrapper to use I2C to communicate with the GYRO Click.
 *
 * @param[in] add_bit GYRO click has a jumper on its board to change its I2C address.
 * (must be 0 if the jumper is on the 0 position, any positive number otherwise)
 */
void LETMECREATE_CLICK_EXPORT gyro_click_use_i2c(uint8_t add_bit);

/**
 * @brief Enable and initialise the GYRO Click.
 *
 * Note that the I2C or SPI bus must be initialised before attempting to enable the
 * device. Also, if you are using I2C, you might need to call gyro_click_use_i2c to
 * use the correct I2C address because of the add_bit jumper.
 *
 * Upon startup, the wrapper is configured to use I2C and the jumper is assumed to
 * be on the 0 position.
 *
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT gyro_click_enable(void);

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
