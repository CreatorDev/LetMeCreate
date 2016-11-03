#include <stdbool.h>
#include <stdio.h>
#include "letmecreate/click/common.h"
#include "letmecreate/click/fan.h"
#include "letmecreate/core/common.h"
#include "letmecreate/core/gpio.h"
#include "letmecreate/core/gpio_monitor.h"


/* I2C address of EMC2301 */
#define EMC2301_ADDRESS     (0x2F)

/* Register addresses */
#define EMC2301_PRODUCT_ID_REG              (0xFD)
#define EMC2301_MANUFACTURER_ID_REG         (0xFE)
#define EMC2301_FAN_CONFIG_1_REG            (0x32)
#define EMC2301_TACH_TARGET_LOW             (0x3C)
#define EMC2301_TACH_TARGET_HIGH            (0x3D)
#define EMC2301_INT_REG                     (0x29)
#define EMC2301_FAN_STATUS_REG              (0x24)

/* Register values */
#define EMC2301_PRODUCT_ID              (0x37)
#define EMC2301_MANUFACTURER_ID         (0x5D)
#define EMC2301_ENABLE_FSC_ALGO         (0x80)
#define EMC2301_MIN_RPM                 (0)     /* min 500 rpm */
#define EMC2301_MIN_EDGE                (0x08)  /* min 5 edges */
#define EMC2301_UPDATE                  (0)     /* 400ms */

#define FAN_CLICK_MIN_RPM               (600)
#define FAN_CLICK_MAX_RPM               (2500)

static bool check_device(void)
{
    uint8_t product_id = 0, manufacturer_id = 0;

    if (i2c_read_register(EMC2301_ADDRESS, EMC2301_PRODUCT_ID_REG, &product_id) < 0)
        return false;

    if (product_id != EMC2301_PRODUCT_ID)
        return false;

    if (i2c_read_register(EMC2301_ADDRESS, EMC2301_MANUFACTURER_ID_REG, &manufacturer_id) < 0)
        return false;

    return manufacturer_id == EMC2301_MANUFACTURER_ID;
}

int fan_click_init(void)
{
    if (check_device() == false) {
        fprintf(stderr, "fan: Failed to find device emc2301.\n");
        return -1;
    }

    if (i2c_write_register(EMC2301_ADDRESS, EMC2301_FAN_CONFIG_1_REG,
                              EMC2301_ENABLE_FSC_ALGO | EMC2301_MIN_RPM |
                              EMC2301_MIN_EDGE | EMC2301_UPDATE) < 0) {
        fprintf(stderr, "fan: Failed to configure device.\n");
        return -1;
    }

    return 0;
}

int fan_click_set_speed(uint16_t rpm)
{
    uint16_t tach = 0;

    if (rpm < FAN_CLICK_MIN_RPM || rpm > FAN_CLICK_MAX_RPM) {
        fprintf(stderr, "fan: rpm out of range 600-2500.\n");
        return -1;
    }

    /* Using equation 3 from emc2301 5.18 section of the datasheet.
     * Datasheet available at http://ww1.microchip.com/downloads/en/DeviceDoc/2301.pdf
     */
    tach = 3932160 / rpm;
    tach &= 0x1FFF;

    if (i2c_write_register(EMC2301_ADDRESS, EMC2301_TACH_TARGET_LOW, tach << 3) < 0
    ||  i2c_write_register(EMC2301_ADDRESS, EMC2301_TACH_TARGET_HIGH, tach >> 5) < 0) {
        fprintf(stderr, "fan: Failed to update tach register.\n");
        return -1;
    }

    return 0;
}
