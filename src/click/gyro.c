#include <stdio.h>
#include <letmecreate/click/common.h>
#include <letmecreate/click/gyro.h>

/* I2C address of device */
#define L3GD20_ADDRESS          (0x6B)

/* Register addresses */
#define L3GD20_DEVICE_ID_REG    (0x0F)
#define L3GD20_CTRL1_REG        (0x20)
#define L3GD20_CTRL4_REG        (0x23)
#define OUT_X_L_REG             (0x28)
#define OUT_X_H_REG             (0x29)
#define OUT_Y_L_REG             (0x2A)
#define OUT_Y_H_REG             (0x2B)
#define OUT_Z_L_REG             (0x2C)
#define OUT_Z_H_REG             (0x2D)

/* Register values */
#define L3GD20_DEVICE_ID        (0xD4)
#define X_AXIS_EN               (0x01)
#define Y_AXIS_EN               (0x02)
#define Z_AXIS_EN               (0x04)
#define L3GD20_PD_N             (0x08)  /* Disable power-down mode */
#define MAX_BANDWIDTH           (0x30)
#define MAX_DATA_RATE           (0xC0)  /* 760Hz */
#define OUTPUT_RANGE            (0x00)  /* 250 degrees per second */

#define DPS_PER_LSB             (0.007629395f)  /* 250/32768 */


int gyro_click_enable(void)
{
    uint8_t device_id = 0;

    /* Check device ID */
    if (i2c_read_register(L3GD20_ADDRESS, L3GD20_DEVICE_ID_REG, &device_id) < 0)
        return -1;
    if (device_id != L3GD20_DEVICE_ID) {
        fprintf(stderr, "gyro: Invalid device ID.\n");
        return -1;
    }

    /* Enable measurements on all axis */
    if (i2c_write_register(L3GD20_ADDRESS, L3GD20_CTRL1_REG, X_AXIS_EN
                                                           | Y_AXIS_EN
                                                           | Z_AXIS_EN
                                                           | L3GD20_PD_N
                                                           | MAX_BANDWIDTH
                                                           | MAX_DATA_RATE) < 0
    ||  i2c_write_register(L3GD20_ADDRESS, L3GD20_CTRL4_REG, OUTPUT_RANGE) < 0)
        return -1;

    return 0;
}

int gyro_click_get_measure(float *x, float *y, float *z)
{
    int16_t measureX, measureY, measureZ;

    if (x == NULL || y == NULL || z == NULL)
        return -1;

    if (i2c_read_16b_register(L3GD20_ADDRESS, OUT_X_L_REG, OUT_X_H_REG, (uint16_t*)&measureX) < 0
    ||  i2c_read_16b_register(L3GD20_ADDRESS, OUT_Y_L_REG, OUT_Y_H_REG, (uint16_t*)&measureY) < 0
    ||  i2c_read_16b_register(L3GD20_ADDRESS, OUT_Z_L_REG, OUT_Z_H_REG, (uint16_t*)&measureZ) < 0)
        return -1;

    *x = ((float)measureX) * DPS_PER_LSB;
    *y = ((float)measureY) * DPS_PER_LSB;
    *z = ((float)measureZ) * DPS_PER_LSB;

    return 0;
}

int gyro_click_disable(void)
{
    /* Disable measurements on all axis and set device in power-down mode. */
    return i2c_write_register(L3GD20_ADDRESS, L3GD20_CTRL1_REG, 0);
}
