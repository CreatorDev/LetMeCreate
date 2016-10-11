/**
 * @file click/adc.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example adc/main.c
 */

#ifndef __LETMECREATE_CLICK_ADC_H__
#define __LETMECREATE_CLICK_ADC_H__


#include <stdint.h>
#include <letmecreate/click/export.h>

#define ADC_CLICK_CHANNEL_COUNT (4)

/** Channel index of the ADC Click */
enum ADC_CLICK_CHANNEL_INDEX {
    ADC_CLICK_CHANNEL_1,
    ADC_CLICK_CHANNEL_2,
    ADC_CLICK_CHANNEL_3,
    ADC_CLICK_CHANNEL_4,
};


/**
 * @brief Read raw value from a channel of the ADC Click.
 *
 * @param[in] channel Index of the channel, must be in range 0-3 (see #ADC_CLICK_CHANNEL_INDEX)
 * @param[out] value Pointer to a 16-bit integer, must not be null. The value
 * returned is a 12-bit integer.
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT adc_click_get_raw_value(uint8_t channel, uint16_t *value);

#endif
