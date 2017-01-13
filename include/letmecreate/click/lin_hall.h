/**
 * @file lin_hall.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_LIN_HALL_H__
#define __LETMECREATE_CLICK_LIN_HALL_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Read measurement from LIN Hall Click
 *
 * SPI must be initialised and the correct bus must be selected before calling
 * this function.
 *
 * @param[out] measure 12 bits value
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT lin_hall_click_get_measure(uint16_t *measure);

#ifdef __cplusplus
}
#endif

#endif
