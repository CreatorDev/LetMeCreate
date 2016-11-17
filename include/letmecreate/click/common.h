/**
 * @file include/letmecreate/click/common.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_COMMON_H__
#define __LETMECREATE_CLICK_COMMON_H__

#include <stdint.h>


/**
 * @brief Write one byte to a register over SPI.
 *
 * @param[in] reg_address Address of the register
 * @param[in] data New value of the register
 * @return 0 if successful, -1 otherwise
 */
int spi_write_register(uint8_t reg_address, uint8_t data);

#endif
