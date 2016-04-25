#ifndef __LETMECREATE_CORE_I2C_H__
#define __LETMECREATE_CORE_I2C_H__

#include <stdint.h>

typedef enum MIKROBUS_I2C_BUS {
    MIKROBUS_I2C_BUS_1,
    MIKROBUS_I2C_BUS_2
} MIKROBUS_I2C_BUS;

int i2c_init(const uint8_t bus_index);
int i2c_select_bus(const uint8_t bus_index);
int i2c_write(const uint16_t slave_address, const uint8_t *buffer, const uint32_t count);
int i2c_read(const uint16_t slave_address, uint8_t *buffer, const uint32_t count);
int i2c_write_byte(const uint16_t slave_address, const uint8_t data);
int i2c_read_byte(const uint16_t slave_address, uint8_t *data);
int i2c_release(const uint8_t bus_index);

#endif
