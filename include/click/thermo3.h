/**
 * @file thermo3.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CLICK_THERMO3_H__
#define __LETMECREATE_CLICK_THERMO3_H__

#include <stdint.h>

/**
 * @brief Enable Thermo3 click.
 *
 * @param add_bit Thermo3 click has a jumper on its board to change its address (must be 0 or 1)
 * @return 0 if successful, otherwise it returns -1.
 */
int thermo3_click_enable(const uint8_t add_bit);

/**
 * @brief Get a temperature measure (in degrees Celsius) from Thermo3 click.
 *
 * @param[out] temperature Pointer to a 16-bit variable for writing temperature read from Thermo3 click
 * @return 0 if successful, otherwise it returns -1.
 */
int thermo3_click_get_temperature(float *temperature);

/**
 * @brief Disable Thermo3 click.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int thermo3_click_disable(void);

#endif
