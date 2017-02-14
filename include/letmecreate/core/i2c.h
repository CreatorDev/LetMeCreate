/**
 * @file i2c.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CORE_I2C_H__
#define __LETMECREATE_CORE_I2C_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/core/export.h>

/**
 * @brief Initialise all I²C bus.
 *
 * This function creates a new file descriptor for the device file associated with each I²C bus
 * of the Ci-40. Timeout is set to 0.
 * If the bus is already initialised, nothing is done.
 *
 * @return Returns -1 if it fails, otherwise it returns 0.
 */
int LETMECREATE_CORE_EXPORT i2c_init(void);

/**
 * @brief Select the current bus.
 *
 * All following read/write will use the selected bus. If the index given is invalid, the current
 * bus will not change.
 *
 * @param[in] mikrobus_index index of the bus to initialise (see #MIKROBUS_INDEX)
 */
void LETMECREATE_CORE_EXPORT i2c_select_bus(uint8_t mikrobus_index);

/**
 * @brief Get the current mikrobus index.
 *
 * @return Current mikrobus index used by I2C (see #MIKROBUS_INDEX)
 */
uint8_t LETMECREATE_CORE_EXPORT i2c_get_current_bus(void);

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
int LETMECREATE_CORE_EXPORT i2c_write(uint16_t slave_address, const uint8_t *buffer, uint32_t count);

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
int LETMECREATE_CORE_EXPORT i2c_read(uint16_t slave_address, uint8_t *buffer, uint32_t count);

/**
 * @brief Send one byte to a slave.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave
 * @param data Byte to send to slave
 * @return Returns 1 if successful, otherwise it returns -1.
 */
int LETMECREATE_CORE_EXPORT i2c_write_byte(uint16_t slave_address, uint8_t data);

/**
 * @brief Read one byte from slave.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave to read from
 * @param[out] data Address to store byte read from slave (must not be null)
 * @return Returns 1 if successful, otherwise it returns -1.
 */
int LETMECREATE_CORE_EXPORT i2c_read_byte(uint16_t slave_address, uint8_t *data);

/**
 * @brief Write a 8bit register to a slave.
 *
 * This function writes two bytes to the slave. The first byte is the register
 * address and the second byte is the value of the register.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave to read from
 * @param[in] reg_address Address of the register to write
 * @param[in] data New value of the register
 * @return 1 if successful, otherwise it returns -1.
 */
int LETMECREATE_CORE_EXPORT i2c_write_register(uint16_t slave_address, uint8_t reg_address, uint8_t data);

/**
 * @brief Read a 8bit register from a slave.
 *
 * This function writes the register address and then reads one byte from the
 * slave.
 *
 * @param[in] slave_address Address (7-bit or 10-bit) of the slave to read from
 * @param[in] reg_address Address of the register to read
 * @param[out] data Pointer to store content of register (must not be null)
 * @return 1 if successful, otherwise it returns -1.
 */
int LETMECREATE_CORE_EXPORT i2c_read_register(uint16_t slave_address, uint8_t reg_address, uint8_t *data);

/**
 * @brief Read a 16bit register from a slave over I²C.
 *
 * @param[in] address Address (7 bits or 10 bits) of the slave
 * @param[in] reg_low_address Address of the lower half of the register on the slave
 * @param[in] reg_high_address Address of the upper half of the register on the slave
 * @param[out] data Pointer to a 16-bit variable to store the value of the register read from the slave
 * @return 1 if successful, otherwise it returns -1.
 */
int LETMECREATE_CORE_EXPORT i2c_read_16b_register(uint16_t address, uint8_t reg_low_address, uint8_t reg_high_address, uint16_t *data);

/**
 * @brief Get timeout of currently selected I²C bus.
 *
 * @return timeout Timeout in ms after which uart_receive will return, even if it does not read the request amount of bytes.
 */
uint32_t LETMECREATE_CORE_EXPORT i2c_get_timeout(void);

/**
 * @brief Set timeout of currently selected I²C bus.
 *
 * Pass UART_TIMEOUT_NEVER to disable timeout functionality.
 *
 * @param[in] timeout Timeout in ms after which i2c_receive will return, even if it does not read the request amount of bytes.
 */
void LETMECREATE_CORE_EXPORT i2c_set_timeout(uint32_t timeout);

/**
 * @brief Close all file descriptor.
 *
 * @return Returns -1 if it fails, otherwise it returns 0.
 */
int LETMECREATE_CORE_EXPORT i2c_release(void);

#ifdef __cplusplus
}
#endif

#endif
