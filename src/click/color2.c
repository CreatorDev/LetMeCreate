#include <stdio.h>
#include <time.h>
#include <letmecreate/click/color2.h>
#include <letmecreate/core/i2c.h>

#define ISL29125_ADDRESS        (0x44)

/* Register addresses */
#define DEVICE_REG              (0x00)
#define CONFIG1_REG             (0x01)
#define CONFIG2_REG             (0x02)
#define CONFIG3_REG             (0x03)
#define GREEN_LOW_BYTE          (0x09)
#define GREEN_HIGH_BYTE         (0x0A)
#define RED_LOW_BYTE            (0x0B)
#define RED_HIGH_BYTE           (0x0C)
#define BLUE_LOW_BYTE           (0x0D)
#define BLUE_HIGH_BYTE          (0x0E)


#define ISL29125_DEVICE_ID      (0x7D)
#define RGB_SENSOR_EN           (0x05)
#define FULL_RANGE              (0x08)

static void sleep_ms(unsigned int ms)
{
    struct timespec req, rem;

    req.tv_sec = ms / 1000;
    ms -= req.tv_sec * 1000;
    req.tv_nsec = ms * 1000000;

    while (nanosleep(&req, &rem))
        req = rem;
}

int color2_click_enable(void)
{
    uint8_t device_id;

    /* Check device ID */
    if (i2c_read_register(ISL29125_ADDRESS, DEVICE_REG, &device_id) < 0)
        return -1;

    if (device_id != ISL29125_DEVICE_ID) {
        fprintf(stderr, "color2: Invalid device ID.\n");
        return -1;
    }

    /* Reset device */
    if (i2c_write_register(ISL29125_ADDRESS, DEVICE_REG, 0x46) < 0)
        return -1;

    /* Enable RGB, Full range, 16bit resolution, ADC starts at I2C write 0x01 */
    if (i2c_write_register(ISL29125_ADDRESS, CONFIG1_REG, RGB_SENSOR_EN | FULL_RANGE) < 0)
        return -1;

    /*
     * ADC is integrating measurements over a period of 100ms for each channel.
     * By waiting 350ms, we ensure that the first call to color2_click_get_color
     * will return a correct measurement.
     */
    sleep_ms(350);

    return 0;
}

int color2_click_get_color(uint16_t *red, uint16_t *green, uint16_t *blue)
{
    if (green == NULL || red == NULL || blue == NULL) {
        fprintf(stderr, "color2: Cannot store measure using null pointers.\n");
        return -1;
    }

    if (i2c_read_16b_register(ISL29125_ADDRESS, RED_LOW_BYTE, RED_HIGH_BYTE, red) < 0
    ||  i2c_read_16b_register(ISL29125_ADDRESS, GREEN_LOW_BYTE, GREEN_HIGH_BYTE, green) < 0
    ||  i2c_read_16b_register(ISL29125_ADDRESS, BLUE_LOW_BYTE, BLUE_HIGH_BYTE, blue) < 0)
        return -1;

    return 0;
}

int color2_click_disable(void)
{
    /* Disable RGB */
    return i2c_write_register(ISL29125_ADDRESS, CONFIG1_REG, 0);
}
