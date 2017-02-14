/**
 * @file uni_hall.h
 * @author Francois Berder
 * @date 2017
 * @copyright 3-clause BSD
 *
 * @example uni_hall/main.c
 */

#ifndef __LETMECREATE_CLICK_UNI_HALL_H__
#define __LETMECREATE_CLICK_UNI_HALL_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Call function when north pole is detected.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click board (see #MIKROBUS_INDEX)
 * @param[in] callback Function to call when uni hall click detects north pole (must not be null).
 * The meaning of the argument of the callback is:
 *    - 1 when the uni hall stop detecting the north pole.
 *    - 2 when the uni hall starts detecting the north pole.
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT uni_hall_click_attach_callback(uint8_t mikrobus_index, void (*callback)(uint8_t));

#ifdef __cplusplus
}
#endif

#endif
