/**
 * @file proximity.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_PROXIMITY_H__
#define __LETMECREATE_CLICK_PROXIMITY_H__

#include <stdint.h>

/**
 * @brief Enable the proximity click.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int proximity_click_enable(void);

/**
 * @brief Get a measure from proximity click.
 *
 * The proximity click must be enabled before calling this function.
 *
 * @param[out] measure Pointer to a 16-bit variable to retrieve the output from the click (must not be null)
 * @return 0 if successful, otherwise it returns -1.
 */
int proximity_click_get_measure(uint16_t *measure);

/**
 * @brief Shutdown the proximity click.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int proximity_click_disable(void);

#endif
