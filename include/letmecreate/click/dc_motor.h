/**
 * @file dc_motor.h
 * @author Francois Berder
 * @date 2017
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_DC_MOTOR_H__
#define __LETMECREATE_CLICK_DC_MOTOR_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/*
 * @brief Initialise the DC motor click
 *
 * The wrapper assumes that the CS pin of the mikrobus index is held high
 * because of the SPI driver.
 *
 * @param[in] mikrobus_index
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT dc_motor_click_init(uint8_t mikrobus_index);

/*
 * @brief Change the direction of the motor (forward or reverse)
 *
 * @param[in] mikrobus_index
 * @param direction 0: reverse, 1: forward. No other values are accepted
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT dc_motor_click_set_direction(uint8_t mikrobus_index, uint8_t direction);

/*
 * @brief Get the direction of the motor (forward or reverse)
 *
 * @param[in] mikrobus_index
 * @param[out] direction 0: reverse, 1: forward
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT dc_motor_click_get_direction(uint8_t mikrobus_index, uint8_t *direction);

/*
 * @brief Set the speed of the motor
 *
 * @param[in] mikrobus_index 
 * @param[in] speed Must be a percentage 
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT dc_motor_click_set_speed(uint8_t mikrobus_index, float speed);

/*
 * @brief Get the speed of the motor
 *
 * @param[in] mikrobus_index
 * @param[out] speed In percentage from 0 to 100 
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT dc_motor_click_get_speed(uint8_t mikrobus_index, float *speed);

/*
 * @brief Stop the motor and set the device in sleep
 *
 * @param[in] mikrobus_index
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT dc_motor_click_release(uint8_t mikrobus_index);

#ifdef __cplusplus
}
#endif

#endif
