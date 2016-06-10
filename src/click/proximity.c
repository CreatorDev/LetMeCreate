#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "click/proximity.h"
#include "click/common.h"

#define COMMAND_REG             (0x80)
#define PRIDREV_REG             (0x81)
#define PROXRATE_REG            (0x82)
#define LED_REG                 (0x83)
#define PROX_RESULT_HIGH_REG    (0x87)
#define PROX_RESULT_LOW_REG     (0x88)

#define VCNL4010_ADDRESS        (0x13)

#define COMMAND_SELFTIMED_EN    (0x01)
#define COMMAND_PROX_EN         (0x02)
#define COMMAND_PROX_DATA_RDY   (0x20)

/* Configuration */
#define PROXIMITY_RATE          (1)         /* 3.90625 measurements per second */
#define LED_CURRENT             (4)         /* 40mA */

static bool enabled = false;

int proximity_click_enable(void)
{
    if (enabled)
        return 0;

    if (i2c_write_register(VCNL4010_ADDRESS, PROXRATE_REG, PROXIMITY_RATE) < 0) {
        fprintf(stderr, "proximity: Failed to set measurement rate.\n");
        return -1;
    }

    if (i2c_write_register(VCNL4010_ADDRESS, LED_REG, LED_CURRENT) < 0) {
        fprintf(stderr, "proximity: Failed to configure led current.\n");
        return -1;
    }

    if (i2c_write_register(VCNL4010_ADDRESS, COMMAND_REG,
                             COMMAND_SELFTIMED_EN | COMMAND_PROX_EN) < 0) {
        fprintf(stderr, "proximity: Failed to enable sensor.\n");
        return -1;
    }

    enabled = true;

    return 0;
}

int proximity_click_get_measure(uint16_t *measure)
{
    bool measure_available = false;

    if (measure == NULL) {
        fprintf(stderr, "proximity: Cannot store measure using null pointer.\n");
        return -1;
    }

    if (enabled == false) {
        fprintf(stderr, "proximity: Cannot get measure from disabled sensor.\n");
        return -1;
    }

    while (measure_available == false) {
        uint8_t value;

        if (i2c_read_register(VCNL4010_ADDRESS, COMMAND_REG, &value) < 0) {
            fprintf(stderr, "proximity: Failed to read command register.\n");
            return -1;
        }

        if (value & COMMAND_PROX_DATA_RDY)
            measure_available = true;
    }

    return i2c_read_16b_register(VCNL4010_ADDRESS, PROX_RESULT_LOW_REG, PROX_RESULT_HIGH_REG, measure);
}

int proximity_click_disable(void)
{
    if (enabled == false)
        return 0;

    if (i2c_write_register(VCNL4010_ADDRESS, COMMAND_REG, 0) < 0) {
        fprintf(stderr, "proximity: Failed to disable sensor.\n");
        return -1;
    }

    enabled = false;

    return 0;
}
