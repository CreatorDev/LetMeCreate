/**
 * @file lps25h.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_RPISENSEHAT_LPS25H_H__
#define __LETMECREATE_RPISENSEHAT_LPS25H_H__

/**
 * @brief Enable sensor.
 *
 * @return 0 if successful, -1 otherwise
 */
int lps25h_enable(void);

/**
 * @brief Read pressure measure.
 *
 * @param[out] pressure in hPa (must not be null).
 * @return 0 if successful, -1 otherwise
 */
int lps25h_get_pressure_measure(float *pressure);

/**
 * @brief Enable sensor.
 *
 * Set device in power-down mode.
 *
 * @return 0 if successful, -1 otherwise
 */
int lps25h_disable(void);

#endif
