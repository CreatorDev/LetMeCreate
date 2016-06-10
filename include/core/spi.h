/**
 * @file spi.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_SPI_H__
#define __LETMECREATE_CORE_SPI_H__

#include <stdint.h>

/**
 * @brief Initialise a SPI bus of the Ci-40.
 *
 * It creates a new file descriptor.
 * If the bus is already initialised, nothing is done and it returns 0.
 *
 * @param[in] mikrobus_index Index of the bus to initialise (see #MIKROBUS_INDEX)
 * @param[in] mode Mode of the SPI bus (mode 0, 1, 2 or 3)
 * @return -1 if it failed, otherwise a positive number if successful.
 */
int spi_init(const uint8_t mikrobus_index, const uint32_t mode);

/**
 * @brief Select the bus to use.
 *
 * @param[in] mikrobus_index Index of the bus to select (see #MIKROBUS_INDEX)
 * @return -1 if @p mikrobus_index is invalid, otherwise it returns 0.
 */
int spi_select_bus(const uint8_t mikrobus_index);

/**
 * @brief Get the current mikrobus index.
 *
 * @return Current mikrobus index used by SPI (see #MIKROBUS_INDEX)
 */
uint8_t spi_get_current_bus(void);

/**
 * @brief Make a transfer of bytes over SPI.
 *
 * Make a transfer using the currently selected bus. @p tx_buffer and @p rx_buffer can be set to
 * NULL if no data has to be sent/received. The bus must be initialised before calling this
 * function.
 *
 * @param[in] tx_buffer Address of the array of bytes to send
 * @param[out] rx_buffer Address of the array of bytes to receive from the bus
 * @param[in] count Number of bytes to read or write from the current bus.
 * @return @p count if successful, otherwise it returns -1.
 */
int spi_transfer(const uint8_t *tx_buffer, uint8_t *rx_buffer, const uint32_t count);

/**
 * @brief Close the file descriptor associated with this bus.
 *
 * @param[in] mikrobus_index Index of the bus to release (see #MIKROBUS_INDEX)
 * @return 0 if successful, otherwise it returns -1.
 */
int spi_release(const uint8_t mikrobus_index);

#endif
