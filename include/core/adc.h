/**
 * @file adc.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_GPIO_H__
#define __LETMECREATE_CORE_GPIO_H__

#include <stdint.h>

/** Index of ADC */
enum ADC_INDEX {
    MIKROBUS_1_ADC, /**< ADC available on Mikrobus 1 and Raspberry pi interface */
    MIKROBUS_2_ADC  /**< ADC available on Mikrobus 2 and Raspberry pi interface */
};

/**
 * @brief Get the reading of an ADC in Volt, in range 0-5V.
 *
 * @param[in] index Index of the ADC (see #ADC_INDEX)
 * @param[out] value Pointer to a floating point variable (must be non-null)
 * @return 0 if succesful, -1 otherwise
 */
int adc_get_value(const uint8_t index, float *value);

#endif
