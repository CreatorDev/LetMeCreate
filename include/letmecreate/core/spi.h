/**
 * @file spi.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_SPI_H__
#define __LETMECREATE_CORE_SPI_H__

#include <stdint.h>

/** SPI speed */
enum SPI_SPEED {
    SPI_680K  = 680000,
    SPI_1M36  = 1360000,
    SPI_2M73  = 2730000,
    SPI_5M46  = 5460000,
    SPI_10M93 = 10930000,
    SPI_21M87 = 21870000,
    SPI_43M75 = 43750000
};

/**
 * @brief Initialise all SPI bus of the Ci-40.
 *
 * Configure all SPI bus:
 *   - 8 bits per word
 *   - 2.73MHz
 *   - SPI_MODE_3
 *
 * The current SPI bus is set to MIKROBUS_1.
 *
 * @return 0 if successful, -1 otherwise
 */
int spi_init(void);

/**
 * @brief Set the mode of an SPI bus.
 *
 * @param[in] mikrobus_index Index of the bus to initialise (see #MIKROBUS_INDEX)
 * @param[in] mode Mode of the SPI bus (mode 0, 1, 2 or 3)
 * @return 0 if successful, -1 otherwise
 */
int spi_set_mode(uint8_t mikrobus_index, uint32_t mode);

/**
 * @brief Set the speed of an SPI bus.
 *
 * The SPI driver only has seven different speeds available, defined in #SPI_SPEED. If you try to
 * define a speed that is not supported by the driver, it will find the closest speed without
 * exceeding it.
 * For instance, if you try to set the speed to 3MHz, the actual speed will be set to 2.73MHz.
 *
 * @param[in] mikrobus_index Index of the bus to initialise (see #MIKROBUS_INDEX)
 * @param[in] speed Speed in Hz of the SPI bus (see #SPI_SPEED)
 * @return 0 if successful, -1 otherwise
 */
int spi_set_speed(uint8_t mikrobus_index, uint32_t speed);

/**
 * @brief Select the bus to use.
 *
 * If the index given is invalid, the current bus will not change.
 *
 * @param[in] mikrobus_index Index of the bus to select (see #MIKROBUS_INDEX)
 */
void spi_select_bus(uint8_t mikrobus_index);

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
int spi_transfer(const uint8_t *tx_buffer, uint8_t *rx_buffer, uint32_t count);

/**
 * @brief Close all file descriptors.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int spi_release(void);

#endif
