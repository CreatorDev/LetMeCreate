#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <letmecreate/core/spi.h>
#include <letmecreate/core/i2c.h>
#include <letmecreate/click/gyro.h>

/* I2C address of device */
#define L3GD20_ADDRESS_0          (0x6A)
#define L3GD20_ADDRESS_1          (0x6B)

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

/* SPI protocol */
#define READ_BIT                (0x80)
#define MULTIPLE_BIT            (0x40)

static bool use_spi = false;
static uint8_t slave_address = L3GD20_ADDRESS_0;


static int read_register(uint8_t reg, uint8_t *value)
{
    if (use_spi) {
        uint8_t tx_buffer[2], rx_buffer[2];
        tx_buffer[0] = READ_BIT | (reg & 0x3F);
        tx_buffer[1] = 0;
        if (spi_transfer(tx_buffer, rx_buffer, sizeof(rx_buffer)) < 0)
            return -1;
        *value = rx_buffer[1];
        return 0;
    } else
        return i2c_read_register(slave_address, reg, value);
}

static int write_register(uint8_t reg, uint8_t value)
{
    if (use_spi) {
        uint8_t tx_buffer[2];
        tx_buffer[0] = reg & 0x3F;
        tx_buffer[1] = value;
        return spi_transfer(tx_buffer, NULL, sizeof(tx_buffer));
    } else
        return i2c_write_register(slave_address, reg, value);
}

void gyro_click_use_spi(void)
{
    use_spi = true;
}

void gyro_click_use_i2c(uint8_t add_bit)
{
    use_spi = false;
    if (add_bit)
        slave_address = L3GD20_ADDRESS_1;
    else
        slave_address = L3GD20_ADDRESS_0;
}

int gyro_click_enable(void)
{
    uint8_t device_id = 0;

    /* Check device ID */
    if (read_register(L3GD20_DEVICE_ID_REG, &device_id) < 0)
        return -1;
    if (device_id != L3GD20_DEVICE_ID) {
        fprintf(stderr, "gyro: Invalid device ID.\n");
        return -1;
    }

    /* Enable measurements on all axis */
    if (write_register(L3GD20_CTRL1_REG, X_AXIS_EN
                                       | Y_AXIS_EN
                                       | Z_AXIS_EN
                                       | L3GD20_PD_N
                                       | MAX_BANDWIDTH
                                       | MAX_DATA_RATE) < 0
    ||  write_register(L3GD20_CTRL4_REG, OUTPUT_RANGE) < 0)
        return -1;

    return 0;
}

int gyro_click_get_measure(float *x, float *y, float *z)
{
    int16_t measureX = 0, measureY = 0, measureZ = 0;

    if (x == NULL || y == NULL || z == NULL) {
        fprintf(stderr, "gyro: Cannot store measure using null pointers.\n");
        return -1;
    }

    if (use_spi) {
        uint8_t tx_buffer[7], rx_buffer[7];
        memset(&tx_buffer[1], 0, 6);
        tx_buffer[0] = READ_BIT | MULTIPLE_BIT | OUT_X_L_REG;
        if (spi_transfer(tx_buffer, rx_buffer, sizeof(tx_buffer)) < 0)
            return -1;

        measureX = rx_buffer[1] | (rx_buffer[2] << 8);
        measureY = rx_buffer[3] | (rx_buffer[4] << 8);
        measureZ = rx_buffer[5] | (rx_buffer[6] << 8);
    } else {
        if (i2c_read_16b_register(slave_address, OUT_X_L_REG, OUT_X_H_REG, (uint16_t*)&measureX) < 0
        ||  i2c_read_16b_register(slave_address, OUT_Y_L_REG, OUT_Y_H_REG, (uint16_t*)&measureY) < 0
        ||  i2c_read_16b_register(slave_address, OUT_Z_L_REG, OUT_Z_H_REG, (uint16_t*)&measureZ) < 0)
            return -1;
    }

    *x = ((float)measureX) * DPS_PER_LSB;
    *y = ((float)measureY) * DPS_PER_LSB;
    *z = ((float)measureZ) * DPS_PER_LSB;

    return 0;
}

int gyro_click_disable(void)
{
    /* Disable measurements on all axis and set device in power-down mode. */
    return write_register(L3GD20_CTRL1_REG, 0);
}
