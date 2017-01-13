/**
 * @file led_matrix.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example led_matrix/main.c
 */

#ifndef __LETMECREATE_CLICK_LED_MATRIX_H__
#define __LETMECREATE_CLICK_LED_MATRIX_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Enable 8x8 Click.
 *
 * By default, it enables all lines and sets intensity to maximum. All LED's are switched off.
 *
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT led_matrix_click_enable(void);

/**
 * @brief Set the intensity of the LED matrix.
 *
 * @param[in] intensity in range 0-15
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT led_matrix_click_set_intensity(uint8_t intensity);

/**
 * @brief Switch on/off all LED's of one column.
 *
 * @param[in] column_index Index of the column (must be in range 0-7)
 * @param[in] data bit string to switch on/off LED's in this column
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT led_matrix_click_set_column(uint8_t column_index, uint8_t data);


/**
 * @brief Displays a two digit number.
 *
 * @param[in] number a number between 0 and 99 which will be displayed
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT led_matrix_click_display_number(uint8_t number);

/**
 * @brief Switch on/off all LED's.
 *
 * Notice that the origin is on the bottom-right corner of the matrix.
 *
 * @param[in] columns State of all LED's (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT led_matrix_click_set(const uint8_t *columns);

/**
 * @brief Disable 8x8 Click.
 *
 * Switch off all LED's and shutdown device.
 *
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT led_matrix_click_disable(void);

#ifdef __cplusplus
}
#endif

#endif
