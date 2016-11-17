/**
 * @file lsm9ds1.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_RPISENSEHAT_LSM9DS1_H__
#define __LETMECREATE_RPISENSEHAT_LSM9DS1_H__

int lsm9ds1_enable(void);
int lsm9ds1_get_accelerometer_measure(float *accelX, float *accelY, float *accelZ);
int lsm9ds1_get_gyroscope_measure(float *gyroX, float *gyroY, float *gyroZ);
int lsm9ds1_get_magnetometer_measure(float *magX, float *magY, float *magZ);
int lsm9ds1_disable(void);

#endif
