#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "click/thermo3.h"
#include "core/common.h"
#include "core/gpio.h"
#include "core/gpio_monitor.h"
#include "core/i2c.h"

#define TMP102_ADDRESS                  (0x48 | (last_address_bit & 0x01))
#define TEMPERATURE_REG_ADDRESS         (0x00)
#define CONFIGURATION_REG_ADDRESS       (0x01)
#define TEMPERATURE_HIGH_REG_ADDRESS    (0x03)

#define DEGREES_CELCIUS_PER_LSB         (0.0625f)
#define SHUTDOWN_MODE           (0x01)
#define CONVERSION_RATE         (0xC0)

static bool enabled = false;
static uint8_t last_address_bit = 0;
static int alarm_callback_ID = -1;

int thermo3_click_enable(const uint8_t add_bit)
{
    uint8_t buffer[3];

    last_address_bit = add_bit;

    buffer[0] = CONFIGURATION_REG_ADDRESS;
    buffer[1] = 0;
    buffer[2] = CONVERSION_RATE;
    if (i2c_write(TMP102_ADDRESS, buffer, sizeof(buffer)) < 0) {
        fprintf(stderr, "thermo3: Failed to configure and enable sensor.\n");
        return -1;
    }

    enabled = true;

    return 0;
}

int thermo3_click_get_temperature(float *temperature)
{
    uint8_t buffer[2];

    if (temperature == NULL) {
        fprintf(stderr, "thermo3: Cannot store temperature using null pointer.\n");
        return -1;
    }

    if (enabled == false) {
        fprintf(stderr, "thermo3: Cannot get temperature from disabled sensor.\n");
        return -1;
    }

    if (i2c_write_byte(TMP102_ADDRESS, TEMPERATURE_REG_ADDRESS) < 0) {
        fprintf(stderr, "thermo3: Failed to request temperature from sensor.\n");
        return -1;
    }

    if (i2c_read(TMP102_ADDRESS, buffer, sizeof(buffer)) < 0) {
        fprintf(stderr, "thermo3: Failed to read temperature from sensor.\n");
        return -1;
    }

    *temperature = (float)(buffer[0]);
    *temperature += ((float)(buffer[1] >> 4)) * DEGREES_CELCIUS_PER_LSB;

    return 0;
}

int thermo3_click_set_alarm(const uint8_t mikrobus_index, const float threshold, void(*callback)(uint8_t))
{
    uint8_t alarm_pin = 0;
    uint8_t buffer[3];

    if (callback == NULL) {
        fprintf(stderr, "thermo3: Cannot set alarm using null callback.\n");
        return -1;
    }

    switch (mikrobus_index) {
    case MIKROBUS_1:
        alarm_pin = MIKROBUS_1_INT;
        break;
    case MIKROBUS_2:
        alarm_pin = MIKROBUS_2_INT;
        break;
    default:
        fprintf(stderr, "thermo3: Invalid mikrobus index.\n");
        return -1;
    }

    if (alarm_callback_ID >= 0) {
        if (gpio_monitor_remove_callback(alarm_callback_ID) < 0) {
            fprintf(stderr, "thermo3: Failed to remove old alarm callback.\n");
            return -1;
        }
        alarm_callback_ID = -1;
    }

    if (gpio_init(alarm_pin) < 0
    ||  gpio_set_direction(alarm_pin, GPIO_INPUT) < 0) {
        fprintf(stderr, "thermo3: Failed to configure alert pin as an input.\n");
        return -1;
    }

    buffer[0] = TEMPERATURE_HIGH_REG_ADDRESS;
    buffer[1] = (uint8_t)(threshold);
    buffer[2] = (threshold - (float)(buffer[1])) / DEGREES_CELCIUS_PER_LSB;
    buffer[2] <<= 4;
    if (i2c_write(TMP102_ADDRESS, buffer, sizeof(buffer)) < 0) {
        fprintf(stderr, "thermo3: Failed to set threshold on sensor.\n");
        return -1;
    }

    if (gpio_monitor_init() < 0)
        return -1;

    if ((alarm_callback_ID = gpio_monitor_add_callback(alarm_pin, GPIO_FALLING, callback)) < 0)
        fprintf(stderr, "thermo3: Failed to attach callback to alarm pin.\n");

    return alarm_callback_ID;
}

int thermo3_click_disable(void)
{
    uint8_t buffer[3];

    buffer[0] = CONFIGURATION_REG_ADDRESS;
    buffer[1] = SHUTDOWN_MODE;
    buffer[2] = 0;
    if (i2c_write(TMP102_ADDRESS, buffer, sizeof(buffer)) < 0) {
        fprintf(stderr, "thermo3: Failed to shutdown sensor.\n");
        return -1;
    }

    enabled = false;

    if (alarm_callback_ID >= 0) {
        if (gpio_monitor_remove_callback(alarm_callback_ID) < 0) {
            fprintf(stderr, "thermo3: Failed to remove old alarm callback.\n");
            return -1;
        }
        alarm_callback_ID = -1;
    }

    return 0;
}
