#include <stddef.h>
#include "click/common.h"
#include "core/i2c.h"
#include "core/spi.h"

int i2c_write_register(uint16_t address, uint8_t reg_address, uint8_t value)
{
    uint8_t buffer[2];
    buffer[0] = reg_address;
    buffer[1] = value;
    return i2c_write(address, buffer, sizeof(buffer));
}

int i2c_read_register(uint16_t address, uint8_t reg_address, uint8_t *data)
{
    if (i2c_write_byte(address, reg_address) < 0)
        return -1;

    return i2c_read_byte(address, data);
}

int i2c_read_16b_register(uint16_t address,
                          uint8_t reg_low_address, uint8_t reg_high_address,
                          uint16_t *data)
{
    uint8_t low = 0, high = 0;

    if (i2c_read_register(address, reg_low_address, &low) < 0)
        return -1;

    if (i2c_read_register(address, reg_high_address, &high) < 0)
        return -1;

    *data = high;
    *data <<= 8;
    *data |= low;

    return 0;
}

int spi_write_register(uint8_t reg_address, uint8_t data)
{
    uint8_t tx_buffer[2];

    tx_buffer[0] = reg_address;
    tx_buffer[1] = data;
    return spi_transfer(tx_buffer, NULL, sizeof(tx_buffer));
}
