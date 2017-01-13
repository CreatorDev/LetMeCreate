/**
 * @file lsm9ds1.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_RPISENSEHAT_LSM9DS1_H__
#define __LETMECREATE_RPISENSEHAT_LSM9DS1_H__

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief Enable sensor.
 *
 * Enable all axis of accelerometer, gyroscope and magnetometer:
 *   - Accelerometer is configured to maximum output rate and the range is set to 8g.
 *   - Gyroscope is configured to maximum output rate and the range is set to 500 degrees per second.
 *   - Magnetometer is configured to maximum output rate and the range is set to 8 gauss. Temperature
 *     compensation is enabled. Ultra high performance mode for all axis is selected.
 *
 * This function must be called before reading measures from device.
 *
 * @return 0 if successful, -1 otherwise
 */
int lsm9ds1_enable(void);

/**
 * @brief Read measure from accelerometer.
 *
 * @param[out] accelX X component in g (must not be null)
 * @param[out] accelY Y component in g (must not be null)
 * @param[out] accelZ Z component in g (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int lsm9ds1_get_accelerometer_measure(float *accelX, float *accelY, float *accelZ);

/**
 * @brief Read measure from gyroscope.
 *
 * @param[out] gyroX X component in degrees per second (must not be null)
 * @param[out] gyroY Y component in degrees per second (must not be null)
 * @param[out] gyroZ Z component in degrees per second (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int lsm9ds1_get_gyroscope_measure(float *gyroX, float *gyroY, float *gyroZ);

/**
 * @brief Read measure from magnetometer.
 *
 * @param[out] magX X component in gauss (must not be null)
 * @param[out] magY Y component in gauss (must not be null)
 * @param[out] magZ Z component in gauss (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int lsm9ds1_get_magnetometer_measure(float *magX, float *magY, float *magZ);

/**
 * @brief Disable sensor
 *
 * Power down accelerometer, gyroscope and magnetometer.
 *
 * @return 0 if successful, -1 otherwise
 */
int lsm9ds1_disable(void);

#ifdef __cplusplus
}
#endif

#endif
