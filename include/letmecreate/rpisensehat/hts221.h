/**
 * @file hts221.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_RPISENSEHAT_HTS221_H__
#define __LETMECREATE_RPISENSEHAT_HTS221_H__

int hts221_enable(void);
int hts221_get_temperature_measure(float *temperature);
int hts221_get_humidity_measure(float *humidity);
int hts221_disable(void);

#endif
