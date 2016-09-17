#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "letmecreate/click/accel.h"
#include "letmecreate/click/common.h"
#include "letmecreate/core/i2c.h"
#include "letmecreate/core/spi.h"


/* I2C ADXL345 addresses */
#define ADXL345_ADDRESS_0           (0x53)
#define ADXL345_ADDRESS_1           (0x1D)

/* Control registers */
#define BW_RATE_REG         (0x2C)
#define POWER_CTRL_REG      (0x2D)
#define DATA_FORMAT_REG     (0x31)
#define FIFO_CTRL_REG       (0x38)

/* Control values for */
#define DATA_RATE           (0x0F)  /* 3.2kHz */
#define MEASURE_EN          (0x08)
#define SLEEP_EN            (0x04)
#define WAKEUP_DATA_RATE    (0x03)  /* 8 readings/sec in sleep mode */
#define FULL_RES_EN         (0x08)
#define RANGE               (0x03)  /* max range +- 16g */

/* Output registers */
#define DATAX0_REG          (0x32)
#define DATAX1_REG          (0x33)
#define DATAY0_REG          (0x34)
#define DATAY1_REG          (0x35)
#define DATAZ0_REG          (0x36)
#define DATAZ1_REG          (0x37)


/* SPI specific */
#define SPI_READ_BIT            (0x80)
#define SPI_MULTIPLE_BYTE_BIT   (0x40)

#define G_PER_LSB           (0.004f)   /* 4mg/LSB */

static bool enabled = false;
static bool use_spi = true;
static uint8_t lsb_address;
static uint16_t slave_address = ADXL345_ADDRESS_0;

static int write_register(uint8_t reg, uint8_t value)
{
    if (use_spi)
        return spi_write_register(reg, value);
    else
        return i2c_write_register(slave_address, reg, value);
}

void accel_click_use_spi(void)
{
    use_spi = true;
}

void accel_click_use_i2c(uint8_t add_bit)
{
    use_spi = false;
    if (add_bit)
        slave_address = ADXL345_ADDRESS_1;
    else
        slave_address = ADXL345_ADDRESS_0;
}

int accel_click_enable(void)
{
    if (enabled)
        return 0;

    if (write_register(POWER_CTRL_REG, MEASURE_EN | WAKEUP_DATA_RATE) < 0) {
        fprintf(stderr, "accel: Failed to enable device.\n");
        return -1;
    }

    if (write_register(FIFO_CTRL_REG, 0) < 0) {     /* bypass FIFO */
        fprintf(stderr, "accel: Failed to set fifo settings.\n");
        return -1;
    }

    if (write_register(BW_RATE_REG, DATA_RATE) < 0) {
        fprintf(stderr, "accel: Failed to set data rate.\n");
        return -1;
    }

    if (write_register(DATA_FORMAT_REG, FULL_RES_EN | RANGE) < 0) {
        fprintf(stderr, "accel: Failed to set data format.\n");
        return -1;
    }

    enabled = true;

    return 0;
}

int accel_click_get_measure(float *accelX, float *accelY, float *accelZ)
{
    uint8_t rx_buffer[7];
    int16_t x, y, z;

    if (enabled == false) {
        fprintf(stderr, "accel: Cannot get measure while device is shutdown.\n");
        return -1;
    }

    if (accelX == NULL || accelY == NULL || accelZ == NULL) {
        fprintf(stderr, "accel: Cannot store acceleration using null pointers.\n");
        return -1;
    }

    if (use_spi) {
        uint8_t tx_buffer[7] = { SPI_READ_BIT | SPI_MULTIPLE_BYTE_BIT | DATAX0_REG };
        if (spi_transfer(tx_buffer, rx_buffer, sizeof(tx_buffer)) < 0) {
            fprintf(stderr, "accel: Failed to get measure from device.\n");
            return -1;
        }
    } else {
        if (i2c_write_byte(slave_address, DATAX0_REG) < 0
        ||  i2c_read(slave_address, &rx_buffer[1], 6) < 0) {
            fprintf(stderr, "accel: Failed to get measure from device.\n");
            return -1;
        }
    }

    memcpy(&x, &rx_buffer[1], 2);
    memcpy(&y, &rx_buffer[3], 2);
    memcpy(&z, &rx_buffer[5], 2);

    *accelX = ((float)x) * G_PER_LSB;
    *accelY = ((float)y) * G_PER_LSB;
    *accelZ = ((float)z) * G_PER_LSB;

    return 0;
}

int accel_click_disable(void)
{
    if (enabled == false)
        return 0;

    if (write_register(POWER_CTRL_REG, SLEEP_EN) < 0) {
        fprintf(stderr, "accel: Failed to shutdown device.\n");
        return -1;
    }

    enabled = false;

    return 0;
}
