/**
 * @file oled.h
 * @author Michael Schnell, Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example oled/main.c
 */

#ifndef __LETMECREATE_CLICK_OLED_H__
#define __LETMECREATE_CLICK_OLED_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/**
 * @brief Use SPI bus to communicate with OLED Click.
 *
 * By default, it uses I2C to communicate with OLED Click. This function should
 * be called before using the device.
 */
void LETMECREATE_CLICK_EXPORT oled_click_use_spi(void);

/**
 * @brief Use I2C bus to communicate with OLED Click.
 *
 * This function should be called before using the device.
 */
void LETMECREATE_CLICK_EXPORT oled_click_use_i2c(void);

/**
 * @brief Enable the OLED Click.
 *
 * Upon initialisation, the screen is blank and the constrast is set to 127.
 *
 * @param[in] mikrobus_index Index of the mikrobus used by the click board (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT oled_click_enable(uint8_t mikrobus_index);

/**
 * @brief Set the constrast
 *
 * @param[in] contrast 0: no contrast, 255: maximum contrast
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT oled_click_set_contrast(uint8_t contrast);

/**
 * @brief Display pixel buffer
 *
 * Each bit of the array represents the state of a pixel. The first 96 bytes represent
 * the first page (96x8 pixels), the following 96 bytes represent the second page...
 *
 * @param[in] data Array of bytes of length 384 (96x4).
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT oled_click_raw_write(uint8_t *data);

/**
 * @brief Write some text
 *
 * @param[in] str A string of length less or equal than 16.
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT oled_click_write_text(char *str);

/**
 * @brief Convert a character into an array of bytes
 *
 * @param[in] c character to convert (must be between '!' and '~' in the ASCII table)
 * @param[out] out An array of 22 bytes representing the character.
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT oled_click_get_char(char c, const uint8_t **out);

/**
 * @brief Disable the OLED Click.
 *
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT oled_click_disable(void);

#ifdef __cplusplus
}
#endif

#endif
