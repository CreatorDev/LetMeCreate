/**
 * @file joystick.h
 * @author Michal Tusnio
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example joystick/main.c
 */

#ifndef __LETMECREATE_CLICK_JOYSTICK_H__
#define __LETMECREATE_CLICK_JOYSTICK_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>


/**
 * @brief Set the LSB of the sensor I2C address.
 *
 * There is a jumper underneath the joystick click which controls the least
 * significant bit of the I2C slave address of the sensor. This means that the
 * the sensor has address 0x40 or 0x41.
 * By default, add_bit is set to 0.
 *
 * @param[in] add_bit
 */
void LETMECREATE_CLICK_EXPORT joystick_click_set_add_bit(uint8_t add_bit);

/**
 * @brief Get the x coordinate of the joystick
 *
 * @param[out] x Pointer to an 8-bit variable to retrieve the x
 * coordinate of the click (must not be null)
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT joystick_click_get_x(int8_t * x);

/**
 * @brief Get the y coordinate of the joystick
 *
 * @param[out] y Pointer to an 8-bit variable to retrieve the y
 * coordinate of the click (must not be null)
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT joystick_click_get_y(int8_t * y);

/**
 * @brief Get the x and y coordinates of the joystick
 *
 * @param[out] x Pointer to an 8-bit variable to retrieve the x
 * coordinate of the click (must not be null)
 * @param[out] y Pointer to an 8-bit variable to retrieve the y
 * coordinate of the click (must not be null)
 * @return 0 if successful, otherwise it returns -1.
 */
int LETMECREATE_CLICK_EXPORT joystick_click_get_position(int8_t * x, int8_t * y);

#ifdef __cplusplus
}
#endif

#endif

