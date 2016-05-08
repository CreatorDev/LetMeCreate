/**
 * @file common.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_COMMON_H__
#define __LETMECREATE_CLICK_COMMON_H__

#include <stdint.h>

/**
 * @brief Write one byte value to a register over I²C.
 *
 * @param[in] address Address (7 bits or 10 bits) of the slave
 * @param[in] reg_address Address of the register on the slave
 * @param[in] value New value of the register
 * @return Returns 0 if succesfull, otherwise it returns -1.
 */
int i2c_write_register(const uint16_t address, const uint8_t reg_address, const uint8_t value);

/**
 * @brief Read a one-byte register from a slave over I²C.
 *
 * @param[in] address Address (7 bits or 10 bits) of the slave
 * @param[in] reg_address Address of the register to read on the slave.
 * @param[out] data Pointer to a 8-bit variable to store the value of the register read from the slave
 * @return Returns 0 if successfull, otherwise it returns -1.
 */
int i2c_read_register(const uint16_t address, const uint8_t reg_address, uint8_t *data);

/**
 * @brief Read a 16bit register from a slave over I²C.
 *
 * @param[in] address Address (7 bits or 10 bits) of the slave
 * @param[in] reg_low_address Address of the lower half of the register on the slave
 * @param[in] reg_high_address Address of the upper half of the register on the slave
 * @param[out] data Pointer to a 16-bit variable to store the value of the register read from the slave
 * @return Returns 0 if successfull, otherwise it returns -1.
 */
int i2c_read_16b_register(const uint16_t address, const uint8_t reg_low_address, const uint8_t reg_high_address, uint16_t *data);

#endif
