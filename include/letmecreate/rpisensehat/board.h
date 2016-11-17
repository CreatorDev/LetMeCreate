/**
 * @file board.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_RPISENSEHAT_BOARD_H__
#define __LETMECREATE_RPISENSEHAT_BOARD_H__

#include <stdint.h>
#include <letmecreate/rpisensehat/export.h>


/**
 * @brief Initialise all sensors on board.
 *
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_init(void);

/**
 * @brief Read the temperature measurement from humidity sensor.
 *
 * @param[out] temperature Temperature in degrees celsius
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_temperature(float *temperature);

/**
 * @brief Read humidity measurement from humidity sensor.
 *
 * @param[out] humidity Percentage of air humidity
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_humidity(float *humidity);

/**
 * @brief Read pressure measurement from pressure sensor.
 *
 * @param[out] pressure Pressure in hPa
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_pressure(float *pressure);

/**
 * @brief Read acccelerometer measurement from accel/mag/gyro sensor.
 *
 * @param[out] accelX X component of acceleration in g
 * @param[out] accelY Y component of acceleration in g
 * @param[out] accelZ Z component of acceleration in g
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_accelerometer_measure(float *accelX, float *accelY, float *accelZ);

/**
 * @brief Read gyroscope measurement from accel/mag/gyro sensor.
 *
 * @param[out] gyroX X component in degrees per second
 * @param[out] gyroY Y component in degrees per second
 * @param[out] gyroZ Z component in degrees per second
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_gyroscope_measure(float *gyroX, float *gyroY, float *gyroZ);

/**
 * @brief Read magnetometer measurement from accel/mag/gyro sensor.
 *
 * @param[out] magX X component in gauss
 * @param[out] magY Y component in gauss
 * @param[out] magZ Z component in gauss
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_magnetometer_measure(float *magX, float *magY, float *magZ);

/**
 * @brief Read joystick position.
 *
 * @param[out] state (see #RPISENSEHAT_JOYSTICK_STATE for meaning of each value)
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_joystick_input(uint8_t *state);

/**
 * @brief Set LED's
 *
 * Layout of the array:
 *
 *  RGB RGB RGB RGB RGB RGB RGB RGB
 *  RGB RGB RGB RGB RGB RGB RGB RGB
 *  RGB RGB RGB RGB RGB RGB RGB RGB
 *  RGB RGB RGB RGB RGB RGB RGB RGB
 *  RGB RGB RGB RGB RGB RGB RGB RGB
 *  RGB RGB RGB RGB RGB RGB RGB RGB
 *  RGB RGB RGB RGB RGB RGB RGB RGB
 *  RGB RGB RGB RGB RGB RGB RGB RGB
 *
 * The first 24 bytes of the array stores the first line of the LED's, the
 * following 24 bytes stores the second line, etc.
 * Each value of the array should not be greater than 0x1F.
 *
 * @param[in] pixels Array of 8 bits pixels.
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_set_leds_state(uint8_t *pixels);

/**
 * @brief Display a rainbow on the LED matrix.
 *
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_display_rainbow(void);

/**
 * @brief Power-down all sensors on board.
 *
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_release(void);

#endif
