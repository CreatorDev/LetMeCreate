/**
 * @file board.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_RPISENSEHAT_BOARD_H__
#define __LETMECREATE_RPISENSEHAT_BOARD_H__

#include <letmecreate/rpisensehat/export.h>

int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_init(void);

int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_temperature(float *temperature);

int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_humidity(float *humidity);

int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_pressure(float *pressure);

int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_accelerometer_measure(float *accelX, float *accelY, float *accelZ);

int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_gyroscope_measure(float *gyroX, float *gyroY, float *gyroZ);

int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_get_magnetometer_measure(float *magX, float *magY, float *magZ);

int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_release(void);

#endif
