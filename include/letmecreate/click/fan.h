/**
 * @file fan.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_FAN_H__
#define __LETMECREATE_CLICK_FAN_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Initialise Fan Click.
 *
 * The right I2C bus must be selected before calling this function.
 *
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT fan_click_init(void);

/**
 * @brief Set speed of fan.
 *
 * @param[in] rpm Target speed of fan in rpm, must be in range 600, 2500
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT fan_click_set_speed(uint16_t rpm);

#ifdef __cplusplus
}
#endif

#endif
