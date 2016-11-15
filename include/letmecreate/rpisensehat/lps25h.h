/**
 * @file lps25h.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_RPISENSEHAT_LPS25H_H__
#define __LETMECREATE_RPISENSEHAT_LPS25H_H__

int lps25h_enable(void);
int lps25h_get_pressure_measure(float *pressure);
int lps25h_disable(void);

#endif
