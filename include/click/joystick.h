/**
 * @file proximity.h
 * @author Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_JOYSTICK_H__
#define __LETMECREATE_CLICK_JOYSTICK_H__

#include <stdint.h>

/**
 * @brief Get the x coordinate of the joystick
 *
 * @param[out] x Pointer to an 8-bit variable to retrieve the x
 * coordinate of the click
 * @return 0 if successful, otherwise it returns -1.
 */
int joystick_click_get_x(int8_t * x);

/**
 * @brief Get the y coordinate of the joystick
 *
 * @param[out] y Pointer to an 8-bit variable to retrieve the y
 * coordinate of the click
 * @return 0 if successful, otherwise it returns -1.
 */
int joystick_click_get_y(int8_t * y);

/**
 * @brief Get the x and y coordinates of the joystick
 *
 * @param[out] x Pointer to an 8-bit variable to retrieve the x
 * coordinate of the click
 * @param[out] y Pointer to an 8-bit variable to retrieve the y
 * coordinate of the click
 * @return 0 if successful, otherwise it returns -1.
 */
int joystick_click_get_position(int8_t * x, int8_t * y);

#endif

