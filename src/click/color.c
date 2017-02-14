#include <stdio.h>
#include <letmecreate/click/common.h>
#include <letmecreate/click/color.h>
#include <letmecreate/core/i2c.h>

#define ENABLE_REG_ADDRESS      (0x00) // register for enabling/disabling the color click and settings
#define TCS3471_ADDRESS         (0x29) // address of the TCS3471 color sensor
#define COLOR_REG_ADDRESS       (0x14) // colour values stored in successive registers
#define STATUS_REG_ADDRESS      (0X13) // used to determine when the click has been powered on and is ready to read values

#define PON                     (0x01) // power on
#define AEN                     (0x02) // ADC enable
#define SHUTDOWN_MODE           (0x00) // power off
#define REPEATED_TRANSACTION    (0x80) // successive reads read from the same register
#define INCREMENT_TRANSACTION   (0xA0) // successive reads read from successive registers
#define RGBC_VALID_BIT          (0x01) // data is available

int color_click_enable(void)
{
    if (i2c_write_register(TCS3471_ADDRESS,
                           REPEATED_TRANSACTION | ENABLE_REG_ADDRESS,
                           PON | AEN) < 0) {
        fprintf(stderr, "color: Failed to configure sensor.\n");
        return -1;
    }

    /* Wait 5ms to ensure that device has at least 2.4ms before the first measurement request. */
    sleep_ms(5);

    return 0;
}

int color_click_get_color(uint16_t *clear, uint16_t *red, uint16_t *green, uint16_t *blue)
{
    uint8_t buffer[8];
    const uint8_t check_status_cmd = REPEATED_TRANSACTION | STATUS_REG_ADDRESS;
    const uint8_t get_measure_cmd = INCREMENT_TRANSACTION | COLOR_REG_ADDRESS;
    uint8_t status;

    if (clear == NULL || red == NULL || green == NULL || blue == NULL) {
        fprintf(stderr, "color: Cannot store data using null pointers.\n");
        return -1;
    }

    if (i2c_write(TCS3471_ADDRESS, &check_status_cmd, sizeof(check_status_cmd)) < 0) {
        fprintf(stderr, "color: Failed to send status check command.");
        return -1;
    }

    do {
        if (i2c_read(TCS3471_ADDRESS, &status, sizeof(status)) < 0) {
            fprintf(stderr, "color: Failed to read status register.\n");
            return -1;
        }
    } while(!(RGBC_VALID_BIT & status));


    if (i2c_write(TCS3471_ADDRESS, &get_measure_cmd, sizeof(get_measure_cmd)) < 0) {
        fprintf(stderr, "color: Failed to send measurement request.\n");
        return -1;
    }

    if (i2c_read(TCS3471_ADDRESS, buffer, sizeof(buffer)) < 0) {
        fprintf(stderr, "color: Failed to read measurement.\n");
        return -1;
    }

    *clear = (buffer[1] << 8) + buffer[0];
    *red = (buffer[3] << 8) + buffer[2];
    *green = (buffer[5] << 8) + buffer[4];
    *blue = (buffer[7] << 8) + buffer[6];

    return 0;
}

int color_click_disable(void)
{
    return i2c_write_register(TCS3471_ADDRESS,
                              REPEATED_TRANSACTION | ENABLE_REG_ADDRESS,
                              SHUTDOWN_MODE);
}
