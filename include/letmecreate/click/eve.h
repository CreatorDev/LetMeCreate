/**
 * @file eve.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example eve/main.c
 * @example eve/main2.c
 * @example eve/main3.c
 * @example eve/main4.c
 */

#ifndef __LETMECREATE_CLICK_EVE_H__
#define __LETMECREATE_CLICK_EVE_H__

#include <stdbool.h>
#include <stdint.h>
#include <letmecreate/click/ft800_defs.h>
#include <letmecreate/click/export.h>

/**
 * @brief Initialise the EVE click.
 *
 * Initialise the EVE click for controlling a WQVGA scren (480x272 display).
 * It displays a black screen.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_enable(uint8_t mikrobus_index);

/**
 * @brief Shutdown EVE click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_disable(uint8_t mikrobus_index);

/*
 *******************************************************************************
 * Buffering affect the following commands:
 *   - all graphics commands
 *   - all bitmap transformorm matrix commands except eve_click_get_matrix
 *
 * When buffering is disabled, all commands are sent immediately to FT800. This
 * does not mean that the command will appear on screen.
 * All other commands are never buffered and transmitted immediately to ft800.
 * Some commands flush the buffer before executing.
 *******************************************************************************
 */

/**
 * @brief Disable buffering.
 */
void LETMECREATE_CLICK_EXPORT eve_click_disable_buffering(void);

/**
 * @brief Enable buffering.
 */
void LETMECREATE_CLICK_EXPORT eve_click_enable_buffering(void);

/**
 * @return True if buffering is enabled, false otherwise.
 */
bool LETMECREATE_CLICK_EXPORT eve_click_is_buffering_enabled(void);

/*
 *******************************************************************************
 * Drawing operations on FT800 chip
 *
 * To display a new frame on the screen, always respect this order:
 * 1. Clear the screen using eve_click_clear
 * 2. Multiple calls to eve_click_draw
 * 3. Display the frame using eve_click_display
 *******************************************************************************
 */

/**
 * @brief Clear the screen using the specified color.
 *
 * This function must be called before drawing anything on the screen. This function
 * will not clear the screen until eve_click_display is called.
 *
 * @param[in] r Red component of the color to clear the screen with
 * @param[in] g Green component of the color to clear the screen with
 * @param[in] b Blue component of the color to clear the screen with
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT eve_click_clear(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Add a graphics command in the buffer.
 *
 * If buffering is used, then the command is immediately sent to the EVE click.
 * Otherwise, it is stored in a buffer until eve_click_display is called or any
 * other functions that flush the command buffer.
 *
 * Documentation for each command is available at:
 * http://www.ftdichip.com/Support/Documents/ProgramGuides/FT800%20Programmers%20Guide.pdf
 * This function can accept both display list commands and coprocessor commands.
 *
 * @param[in] cmd Opcode of the command.
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_draw(uint32_t cmd, ...);

/**
 * @brief Flush the buffer and display the frame on the screen.
 *
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_display(void);

/*
 *******************************************************************************
 * Loading image/compressed data in FT800 chip memory.
 *
 * To ensure that images are in a format supported by the FT800, use the tool
 * provided by FTDI to  available at:
 * http://www.ftdichip.com/Support/Utilities.htm#EVEImageConverters
 *******************************************************************************
 */

/**
 * @brief Load a JPEG image in FT800 memory.
 *
 * FT800 chip only supports regular JPEG (JFIF) format.
 *
 * @param[in] ptr Starting address to store image in FT800 memory.
 * @param[in] options Can be FT800_OPT_RGB565 or FT800_OPT_MONO
 * @param[in] data Pointer to an array of bytes containing the image (must not be null)
 * @param[in] count Size in bytes of the image
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_load_image(uint32_t ptr, uint32_t options, const uint8_t *data, uint32_t count);

/**
 * @brief Decompress data compressed with the deflate algorithm.
 *
 * @param[in] ptr Starting address to store data in FT800 memory
 * @param[in] data Pointer to an array of bytes containing the data (must not be null)
 * @param[in] count Size in bytes of the compressed data
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_inflate(uint32_t ptr, const uint8_t *data, uint32_t count);

/**
 * @brief Get the end address of the decompressed data
 *
 * This is used with eve_click_inflate to compute the size taken by the compressed
 * data after decompression.
 *
 * @param[out] ptr Last address of decompressed data (must not be null)
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_get_ptr(uint32_t *ptr);

/*
 *******************************************************************************
 * Bitmap transform matrix operations
 *
 * Translating, scaling and rotating operations will not change the matrix,
 * until eve_click_set_matrix is called.
 *******************************************************************************
 */

/**
 * @brief Set transform matrix as identity.
 *
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_load_identity(void);

/**
 * @brief Translate
 *
 * eve_click_set_matrix must be called to update the bitmap transform matrix.
 *
 * @param[in] tx signed 16.16 fixed-point x coordinate
 * @param[in] ty signed 16.16 fixed-point y coordinate
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_translate(int32_t tx, int32_t ty);

/**
 * @brief Scale
 *
 * eve_click_set_matrix must be called to update the bitmap transform matrix.
 *
 * @param[in] sx signed 16.16 fixed-point x factor
 * @param[in] sy signed 16.16 fixed-point x factor
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_scale(int32_t sx, int32_t sy);

/**
 * @brief Rotate
 *
 * eve_click_set_matrix must be called to update the bitmap transform matrix.
 *
 * @param[in] angle Clockwise rotation angle in units of 1/65536 of a circle
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_rotate(int32_t angle);

/**
 * @brief Retrieve bitmap transformation matrix from FT800.
 *
 * The format of the matrix is as followed:
 * | a  b  c |
 * | d  e  f |
 *
 * @param[out] a
 * @param[out] b
 * @param[out] c
 * @param[out] d
 * @param[out] e
 * @param[out] f
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_get_matrix(int32_t *a, int32_t *b, int32_t *c,
                         int32_t *d, int32_t *e, int32_t *f);

/**
 * @brief Set matrix
 *
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_set_matrix(void);

/*
 *******************************************************************************
 * Memory operations on FT800 chip
 *******************************************************************************
 */

/**
 * @brief Compute the CRC32 of a memory region
 *
 * @param[in] ptr Starting address of the memory region
 * @param[in] byte_count Length in bytes of the memory region
 * @param[out] crc CRC of the memory region specified by ptr and byte_count (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT eve_click_memcrc(uint32_t ptr, uint32_t byte_count, uint32_t *crc);

/**
 * @brief Set memory region to a specific value.
 *
 * @param[in] ptr Starting address of the memory region
 * @param[in] value 8bit value to set
 * @param[in] byte_count Number of bytes to set
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT eve_click_memset(uint32_t ptr, uint32_t value, uint32_t byte_count);

/**
 * @brief Copy one region of memory to another one.
 *
 * Notice that this function only operates on memory of the FT800 chip. It cannot
 * be used to copy data from the Ci40 to the EVE click.
 *
 * @param[in] dest Starting address of the destination region
 * @param[in] src Starting address of the source region
 * @param[in] byte_count Number of byte to copy from src to dest
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT eve_click_memcpy(uint32_t dest, uint32_t src, uint32_t byte_count);

/**
 * @brief Set to zero a memory region.
 *
 * @param[in] ptr Starting address
 * @param[in] byte_count Number of byte to zeroed
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT eve_click_memzero(uint32_t ptr, uint32_t byte_count);

/*
 *******************************************************************************
 * Other commands
 *******************************************************************************
 */

/**
 * @brief Display the FTDI logo on the screen.
 *
 * This function takes about 2.5 seconds to complete.
 *
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_ftdi_logo(void);

/**
 * @brief Take a snapshot of the screen.
 *
 * data array must be big enough to store at least 261120 (2*480*272) bytes. The
 * format is RGB565.
 * The coprocessor takes a snapshot of the screen, then this wrapper reads 261K
 * of memory from the FT800 chip. Hence, this function might take several seconds
 * to complete depending on the speed of the SPI bus.
 *
 * @param[in] ptr Starting address to store snapshot in FT800 memory
 * @param[out] data Pointer to an array of bytes
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_snapshot(uint32_t ptr, uint8_t *data);

/**
 * @brief Display a spinner.
 *
 * The spinner is stopped by calling eve_click_stop.
 *
 * @param[in] x X screen coordinate of top left of spinner
 * @param[in] y Y screen coordinate of top right of spinner
 * @param[in] style Must be in range 0 to 3
 * @param[in] scale Scaling coefficient. 0 for no scaling, 1: half screen, 2: full screen
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_spinner(int16_t x, int16_t y, uint16_t style, uint16_t scale);

/**
 * @brief Stop a spinner or a screensaver.
 *
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_stop(void);

/**
 * @brief Reset the coprocessor of the EVE click.
 *
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_coldstart(void);

/**
 * @brief Display a screensaver.
 *
 * The screensaver animation is stopped by calling eve_click_stop.
 *
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_screensaver(void);

/**
 * @brief Control the intensity of the backlight.
 *
 * By default, the intensity is set to maximum.
 *
 * @param[in] intensity Must be in range 0..100
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_set_backlight_intensity(uint8_t intensity);

/*
 *******************************************************************************
 * Touch screen
 *
 * The screen should be first calibrated, then a callback should be specified to
 * receive touch events. The touch screen is not very accurate so you might get
 * incorrect coordinates.
 *******************************************************************************
 */

/**
 * @brief Attach a callback
 *
 * If you want to remove the callback, call this function again with NULL argument.
 *
 * @param[in] callback Arguments to this callback are the screen coordinates of
 * the detected touch point.
 */
void LETMECREATE_CLICK_EXPORT eve_click_attach_touch_callback(void (*callback)(uint16_t, uint16_t));

/**
 * @brief Calibrate touch screen.
 *
 * This function flushes the command buffer.
 * It asks the user to tap on a dot to calibrate the touch screen. Upon completion,
 * the command buffer is empty.
 *
 * @return 0 if successful, -1 otherwise.
 */
int LETMECREATE_CLICK_EXPORT eve_click_calibrate(void);

#endif
