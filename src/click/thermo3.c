#include <stdbool.h>
#include <stddef.h>
#include "click/thermo3.h"
#include "core/i2c.h"

#define TMP102_ADDRESS                  (0x48 | (last_address_bit & 0x01))
#define TEMPERATURE_REG_ADDRESS         (0x00)
#define CONFIGURATION_REG_ADDRESS       (0x01)

#define SHUTDOWN_MODE           (0x01)
#define CONVERSION_RATE         (0xC0)

static bool enabled = false;
static uint8_t last_address_bit = 0;

int thermo3_click_enable(const uint8_t add_bit)
{
    uint8_t buffer[3];
    int ret;

    last_address_bit = add_bit;

    buffer[0] = CONFIGURATION_REG_ADDRESS;
    buffer[1] = 0;
    buffer[2] = CONVERSION_RATE;
    ret = i2c_write(TMP102_ADDRESS, buffer, sizeof(buffer));
    if (ret >= 0) {
        enabled = true;
    }

    return ret;
}

int thermo3_click_get_temperature(float *temperature)
{
    uint8_t buffer[2];
    int ret;

    if (temperature == NULL) {
        return -1;
    }

    ret = i2c_write_byte(TMP102_ADDRESS, TEMPERATURE_REG_ADDRESS);
    if (ret < 0) {
        return -1;
    }

    ret = i2c_read(TMP102_ADDRESS, buffer, sizeof(buffer));
    if (ret < 0) {
        return -1;
    }

    *temperature = (float)(buffer[0]);
    *temperature += ((float)(buffer[1] >> 4)) * 0.0625f;

    return 0;
}

int thermo3_click_disable(void)
{
    uint8_t buffer[3];
    int ret;

    buffer[0] = CONFIGURATION_REG_ADDRESS;
    buffer[1] = SHUTDOWN_MODE;
    buffer[2] = 0;
    ret = i2c_write(TMP102_ADDRESS, buffer, sizeof(buffer));
    if (ret >= 0) {
        enabled = false;
    }

    return ret;
}
