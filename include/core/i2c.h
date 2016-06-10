/**
 * @file i2c.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CORE_I2C_H__
#define __LETMECREATE_CORE_I2C_H__

#include <stdint.h>

/**
 * @brief Initialise a I²C bus.
 *
 * This function creates a new file descriptor for the device file associated with a I²C bus of the
 * Ci-40. If the bus is already initialised, nothing is done.
 *
 * @param[in] mikrobus_index index of the bus to initialise (see #MIKROBUS_INDEX)
 * @return Returns -1 if it fails, otherwise it returns 0.
 */
int i2c_init(const uint8_t mikrobus_index);

/**
 * @brief Select the current bus.
 *
 * All following read/write will use the selected bus. The bus selected must be initialised before
 * calling this function.
 *
 * @param[in] mikrobus_index index of the bus to initialise (see #MIKROBUS_INDEX)
 * @return Returns -1 if it fails, otherwise it returns 0.
 */
int i2c_select_bus(const uint8_t mikrobus_index);

/**
 * @brief Get the current mikrobus index.
 *
 * @return Current mikrobus index used by I2C (see #MIKROBUS_INDEX)
 */
uint8_t i2c_get_current_bus(void);

/**
 * @brief Send some data to a slave.
 *
 * This sends some data to the slave. The buffer must be non-null and the current bus selected must
 * be initialised before calling this function.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave
 * @param[in] buffer Array of bytes to send
 * @param[in] count Number of bytes to send
 * @return Returns @p count if successful, otherwise it returns -1.
 */
int i2c_write(const uint16_t slave_address, const uint8_t *buffer, const uint32_t count);

/**
 * @brief Read data from a slave.
 *
 * This function sends some data to the slave. The buffer must be non-null and the current bus
 * selected must be initialised before calling this function.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave to read from
 * @param[out] buffer Allocated memory where data is stored (must be non-null)
 * @param[in] count Number of bytes to read from slave
 * @return Returns @p count if successful, otherwise it returns -1.
 */
int i2c_read(const uint16_t slave_address, uint8_t *buffer, const uint32_t count);

/**
 * @brief Send one byte to a slave.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave
 * @param data Byte to send to slave
 * @return Returns 1 if successful, otherwise it returns -1.
 */
int i2c_write_byte(const uint16_t slave_address, const uint8_t data);

/**
 * @brief Read one byte from slave.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave to read from
 * @param[out] data Address to store byte read from slave (must not be null)
 * @return Returns 1 if successful, otherwise it returns -1.
 */
int i2c_read_byte(const uint16_t slave_address, uint8_t *data);

/**
 * @brief Close the file descriptor associated with this bus.
 *
 * @param[in] mikrobus_index Index of the bus to release (see #MIKROBUS_INDEX)
 * @return Returns -1 if the bus index is invalid (greater than 1), otherwise it returns 0.
 */
int i2c_release(const uint8_t mikrobus_index);

#endif
