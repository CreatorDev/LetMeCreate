/**
 * @file adc.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_GPIO_H__
#define __LETMECREATE_CORE_GPIO_H__

#include <stdint.h>

enum ADC_INDEX {
    MIKROBUS_1_ADC,
    MIKROBUS_2_ADC
};

int adc_get_value(const uint8_t index, float *value);

#endif
