#include <stdio.h>
#include <letmecreate/core/i2c.h>
#include <letmecreate/rpisensehat/lsm9ds1.h>

/* I2C address for accelerometer and gyroscope */
#define LSM9DS1_ACCEL_GYRO_ADDRESS  (0x6A)

/* I2C address for magnetometer */
#define LSM9DS1_MAG_ADDRESS         (0x1C)

/* Register addresses */
#define LSM9DS1_DEVICE_ID_REG           (0x0F)
#define LSM9DS1_ACCEL_OUT_X_L_REG       (0x28)
#define LSM9DS1_GYRO_OUT_X_L_REG        (0x18)
#define LSM9DS1_GYRO_CTRL1_REG          (0x10)
#define LSM9DS1_GYRO_CTRL4_REG          (0x1E)
#define LSM9DS1_ACCEL_CTRL5_REG         (0x1F)
#define LSM9DS1_ACCEL_CTRL6_REG         (0x20)
#define LSM9DS1_FIFO_CTRL_REG           (0x2E)
#define LSM9DS1_MAG_OUT_X_L_REG         (0x28)
#define LSM9DS1_MAG_CTRL1_REG           (0x20)
#define LSM9DS1_MAG_CTRL2_REG           (0x21)
#define LSM9DS1_MAG_CTRL3_REG           (0x22)
#define LSM9DS1_MAG_CTRL4_REG           (0x23)

/* Register values for accel/gyro */
#define LSM9DS1_ACCEL_GYRO_DEVICE_ID        (0x68)
#define GYRO_BANDWIDTH                      (0x03)
#define GYRO_500_DPS                        (0x08)  /* scale: 500 degrees/second */
#define MAX_OUTPUT_RATE                     (0xC0)  /* output rate: 952Hz */
#define DPS_PER_LSB                         (0.015258789f)  /* 500/32768 */

#define GYRO_X_EN                           (0x08)
#define GYRO_Y_EN                           (0x10)
#define GYRO_Z_EN                           (0x20)
#define ACCEL_X_EN                          (0x08)
#define ACCEL_Y_EN                          (0x10)
#define ACCEL_Z_EN                          (0x20)
#define ACCEL_RANGE_8G                      (0x18)
#define G_PER_LSB                           (0.000244141f)  /* 8/32768 */

/* Register values for magnetometer */
#define LSM9DS1_MAG_DEVICE_ID               (0x3D)
#define TEMP_COMPENSATION                   (0x80)
#define XY_ULTRA_HIGH_PERF                  (0x60)
#define MAG_MAX_OUTPUT_RATE                 (0x1C)
#define MAG_8GAUSS                          (0x20)
#define Z_ULTRA_HIGH_PERF                   (0xC0)
#define GAUSS_PER_LSB                       (0.000244141f)  /* 8/32768 */

static int check_device_id(void)
{
    uint8_t device_id = 0;

    if (i2c_read_register(LSM9DS1_ACCEL_GYRO_ADDRESS, LSM9DS1_DEVICE_ID_REG, &device_id) < 0)
        return -1;
    if (device_id != LSM9DS1_ACCEL_GYRO_DEVICE_ID)
        return -1;

    if (i2c_read_register(LSM9DS1_MAG_ADDRESS, LSM9DS1_DEVICE_ID_REG, &device_id) < 0)
        return -1;
    if (device_id != LSM9DS1_MAG_DEVICE_ID)
        return -1;

    return 0;
}

int lsm9ds1_enable(void)
{
    if (check_device_id() < 0) {
        fprintf(stderr, "lsm9ds1: Invalid device id.\n");
        return -1;
    }

    /* Configure accelerometer and gyroscope */
    if (i2c_write_register(LSM9DS1_ACCEL_GYRO_ADDRESS, LSM9DS1_GYRO_CTRL4_REG, GYRO_X_EN | GYRO_Y_EN | GYRO_Z_EN) < 0
    ||  i2c_write_register(LSM9DS1_ACCEL_GYRO_ADDRESS, LSM9DS1_GYRO_CTRL1_REG, MAX_OUTPUT_RATE | GYRO_500_DPS | GYRO_BANDWIDTH) < 0
    ||  i2c_write_register(LSM9DS1_ACCEL_GYRO_ADDRESS, LSM9DS1_ACCEL_CTRL5_REG, ACCEL_X_EN | ACCEL_Y_EN | ACCEL_Z_EN) < 0
    ||  i2c_write_register(LSM9DS1_ACCEL_GYRO_ADDRESS, LSM9DS1_ACCEL_CTRL6_REG, ACCEL_RANGE_8G | MAX_OUTPUT_RATE) < 0
    ||  i2c_write_register(LSM9DS1_ACCEL_GYRO_ADDRESS, LSM9DS1_FIFO_CTRL_REG, 0) < 0)           /* Bypass FIFO */
        return -1;

    /* Configure magnetometer */
    if (i2c_write_register(LSM9DS1_MAG_ADDRESS, LSM9DS1_MAG_CTRL1_REG, TEMP_COMPENSATION | XY_ULTRA_HIGH_PERF | MAG_MAX_OUTPUT_RATE) < 0
    ||  i2c_write_register(LSM9DS1_MAG_ADDRESS, LSM9DS1_MAG_CTRL2_REG, MAG_8GAUSS) < 0
    ||  i2c_write_register(LSM9DS1_MAG_ADDRESS, LSM9DS1_MAG_CTRL3_REG, 0) < 0
    ||  i2c_write_register(LSM9DS1_MAG_ADDRESS, LSM9DS1_MAG_CTRL4_REG, Z_ULTRA_HIGH_PERF) < 0)
        return -1;

    return 0;
}

int lsm9ds1_get_accelerometer_measure(float *accelX, float *accelY, float *accelZ)
{
    int16_t buffer[3];

    if (accelX == NULL || accelY == NULL || accelZ == NULL) {
        fprintf(stderr, "lsm9ds1: Cannot store accelerometer measure using null pointers.\n");
        return -1;
    }

    if (i2c_write_byte(LSM9DS1_ACCEL_GYRO_ADDRESS, LSM9DS1_ACCEL_OUT_X_L_REG) < 0
    ||  i2c_read(LSM9DS1_ACCEL_GYRO_ADDRESS, (uint8_t*)buffer, sizeof(buffer)) < 0)
        return -1;

    *accelX = (float)(buffer[0]);
    *accelY = (float)(buffer[1]);
    *accelZ = (float)(buffer[2]);

    *accelX *= G_PER_LSB;
    *accelY *= G_PER_LSB;
    *accelZ *= G_PER_LSB;

    return 0;
}

int lsm9ds1_get_gyroscope_measure(float *gyroX, float *gyroY, float *gyroZ)
{
    int16_t buffer[3];

    if (gyroX == NULL || gyroY == NULL || gyroZ == NULL) {
        fprintf(stderr, "lsm9ds1: Cannot store gyroscope measure using null pointers.\n");
        return -1;
    }

    if (i2c_write_byte(LSM9DS1_ACCEL_GYRO_ADDRESS, LSM9DS1_GYRO_OUT_X_L_REG) < 0
    ||  i2c_read(LSM9DS1_ACCEL_GYRO_ADDRESS, (uint8_t*)buffer, sizeof(buffer)) < 0)
        return -1;

    *gyroX = (float)(buffer[0]);
    *gyroY = (float)(buffer[1]);
    *gyroZ = (float)(buffer[2]);

    *gyroX *= DPS_PER_LSB;
    *gyroY *= DPS_PER_LSB;
    *gyroZ *= DPS_PER_LSB;

    return 0;
}

int lsm9ds1_get_magnetometer_measure(float *magX, float *magY, float *magZ)
{
    int16_t buffer[3];

    if (magX == NULL || magY == NULL || magZ == NULL) {
        fprintf(stderr, "lsm9ds1: Cannot store magnetometer measure using null pointers.\n");
        return -1;
    }

    if (i2c_write_byte(LSM9DS1_MAG_ADDRESS, LSM9DS1_MAG_OUT_X_L_REG) < 0
    ||  i2c_read(LSM9DS1_MAG_ADDRESS, (uint8_t*)buffer, sizeof(buffer)) < 0)
        return -1;

    *magX = (float)(buffer[0]);
    *magY = (float)(buffer[1]);
    *magZ = (float)(buffer[2]);

    *magX *= GAUSS_PER_LSB;
    *magY *= GAUSS_PER_LSB;
    *magZ *= GAUSS_PER_LSB;

    return 0;
}

int lsm9ds1_disable(void)
{
    if (i2c_write_register(LSM9DS1_ACCEL_GYRO_ADDRESS, LSM9DS1_GYRO_CTRL1_REG, 0) < 0
    ||  i2c_write_register(LSM9DS1_ACCEL_GYRO_ADDRESS, LSM9DS1_ACCEL_CTRL6_REG, 0) < 0
    ||  i2c_write_register(LSM9DS1_MAG_ADDRESS, LSM9DS1_MAG_CTRL3_REG, 0x3) < 0
    ||  i2c_write_register(LSM9DS1_MAG_ADDRESS, LSM9DS1_MAG_CTRL4_REG, 0) < 0)
        return -1;

    return 0;
}