/**
 * @file click/adc.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_ADC_H__
#define __LETMECREATE_CLICK_ADC_H__


#include <stdint.h>

/**
 * @brief Read raw value from a channel of the ADC Click.
 *
 * @param[in] channel Index of the channel, must be in range 0-3
 * @param[out] value Pointer to a 16-bit integer, must not be null. The value
 * returned is a 12-bit integer.
 * @return 0 if successful, otherwise it returns -1.
 */
int adc_click_get_raw_value(uint8_t channel, uint16_t *value);

#endif
