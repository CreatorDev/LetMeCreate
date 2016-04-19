#include <stddef.h>
#include <stdbool.h>
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
#define PROXIMITY_RATE 1        /* 3.90625 measurements per second */
#define LED_CURRENT 4           /* 40mA */

static bool enabled = false;

int proximity_click_enable(void)
{
    int ret;

    ret = i2c_write_register(VCNL4010_ADDRESS, PROXRATE_REG, PROXIMITY_RATE);
    if (ret < 0) {
        return ret;
    }

    /* Set LED to 40mA */
    ret = i2c_write_register(VCNL4010_ADDRESS, LED_REG, LED_CURRENT);
    if (ret < 0) {
        return ret;
    }

    /* Enable periodic proximity measurements */
    ret = i2c_write_register(VCNL4010_ADDRESS, COMMAND_REG,
                             COMMAND_SELFTIMED_EN | COMMAND_PROX_EN);
    if (ret < 0) {
        return ret;
    }

    enabled = true;

    return 0;
}

int proximity_click_get_measure(uint16_t *measure)
{
    int ret;
    bool measure_available = false;
    uint8_t value;

    if (measure == NULL) {
        return -1;
    }

    if (enabled == false) {
        return -1;
    }

    /* Wait until measure is available */
    while (measure_available == false) {
        ret = i2c_read_register(VCNL4010_ADDRESS, COMMAND_REG, &value);
        if (ret < 0) {
            return -1;
        }

        if (value & COMMAND_PROX_DATA_RDY) {
            measure_available = true;
        }
    }

    return i2c_read_16b_register(VCNL4010_ADDRESS, PROX_RESULT_LOW_REG, PROX_RESULT_HIGH_REG, measure);
}

int proximity_click_disable(void)
{
    int ret;

    ret = i2c_write_register(VCNL4010_ADDRESS, COMMAND_REG, 0);
    if (ret >= 0) {
        enabled = false;
    }

    return ret;
}
