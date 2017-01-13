/**
 * @file core/adc.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_ADC_H__
#define __LETMECREATE_CORE_ADC_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/core/export.h>

/**
 * @brief Get the reading of an ADC in Volt, in range 0-5V.
 *
 * @param[in] mikrobus_index Index of the ADC (see #MIKROBUS_INDEX)
 * @param[out] value Pointer to a floating point variable (must be non-null)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CORE_EXPORT adc_get_value(uint8_t mikrobus_index, float *value);

#ifdef __cplusplus
}
#endif

#endif
