#include "click/common.h"
#include "core/i2c.h"

int i2c_write_register(const uint16_t address, const uint8_t reg_address, const uint8_t value)
{
    uint8_t buffer[2];
    buffer[0] = reg_address;
    buffer[1] = value;
    return i2c_write(address, buffer, sizeof(buffer));
}

int i2c_read_register(const uint16_t address, const uint8_t reg_address, uint8_t *data)
{
    int ret;
    ret = i2c_write_byte(address, reg_address);
    if (ret < 0) {
        return ret;
    }

    return i2c_read_byte(address, data);
}

int i2c_read_16b_register(const uint16_t address, 
                          const uint8_t reg_low_address, const uint8_t reg_high_address, 
                          uint16_t *data)
{
    int ret;
    uint8_t low, high;

    ret = i2c_read_register(address, reg_low_address, &low);
    if (ret < 0) {
        return ret;
    }

    ret = i2c_read_register(address, reg_high_address, &high);
    if (ret < 0) {
        return ret;
    }

    *data = high;
    *data <<= 8;
    *data |= low;

    return *data;
}
