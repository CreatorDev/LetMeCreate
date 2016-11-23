/**
 * @file hts221.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_RPISENSEHAT_HTS221_H__
#define __LETMECREATE_RPISENSEHAT_HTS221_H__

/**
 * @brief Enable and configure sensor.
 *
 * Heater is disabled, interrupt signel is disabled.
 * Calibration data for temperature and humidity are read.
 *
 * This function must be called before attempting to get temperature or humidity
 * from the sensor.
 *
 * @return 0 if successful, -1 otherwise
 */
int hts221_enable(void);

/**
 * @brief Read temperature measure.
 *
 * Calibration data are used to compute the temperature.
 *
 * @param[out] temperature in degrees celsius (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int hts221_get_temperature_measure(float *temperature);

/**
 * @brief Read humidity measure.
 *
 * Calibration data are used to compute the humidity.
 *
 * @param[out] humidity Percentage of humidity in range 0..100 (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int hts221_get_humidity_measure(float *humidity);

/**
 * @brief Disable sensor.
 *
 * Power down sensor, disable heater.
 *
 * @return 0 if successful, -1 otherwise
 */
int hts221_disable(void);

#endif
