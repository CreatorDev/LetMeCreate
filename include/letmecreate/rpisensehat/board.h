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

int LETMECREATE_RPISENSEHAT_EXPORT rpisensehat_release(void);

#endif
