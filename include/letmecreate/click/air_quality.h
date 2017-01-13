/**
 * @file air_quality.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CLICK_AIR_QUALITY_H__
#define __LETMECREATE_CLICK_AIR_QUALITY_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Reads measure from Air Quality Click.
 *
 * @param mikrobus_index Index of the mikrobus used by the click (see #MIKROBUS_INDEX)
 * @param measure Indicator of air pollution in range 0..65535
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT air_quality_click_get_measure(uint8_t mikrobus_index, uint16_t *measure);

#ifdef __cplusplus
}
#endif

#endif
