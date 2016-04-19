#ifndef __LETMECREATE_CLICK_COMMON_H__
#define __LETMECREATE_CLICK_COMMON_H__

#include <stdint.h>

int i2c_write_register(const uint16_t address, const uint8_t reg_address, const uint8_t value);
int i2c_read_register(const uint16_t address, const uint8_t reg_address, uint8_t *data);
int i2c_read_16b_register(const uint16_t address, const uint8_t reg_low_address, const uint8_t reg_high_address, uint16_t *data);

#endif
