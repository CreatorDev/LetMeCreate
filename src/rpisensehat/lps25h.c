#include <stdio.h>
#include <letmecreate/core/i2c.h>
#include <letmecreate/rpisensehat/lps25h.h>

/* I2C address of LPS25H device */
#define LPS25H_ADDRESS          (0x5C)

/* Register addresses */
#define LPS25H_DEVICE_ID_REG    (0x0F)
#define LPS25H_CTRL1_REG        (0x20)
#define LPS25H_CTRL2_REG        (0x21)
#define LPS25H_CTRL3_REG        (0x22)
#define LPS25H_INT_CFG_REG      (0x24)
#define LPS25H_PRESS_OUT_XL_REG (0x28)
#define LPS25H_PRESS_OUT_L_REG  (0x29)
#define LPS25H_PRESS_OUT_H_REG  (0x2A)

/* Register values */
#define LPS25H_DEVICE_ID        (0xBD)
#define ACTIVE_MODE             (0x80)
#define START_CONVERSION        (0x01)


static int check_device_id(void)
{
    uint8_t device_id = 0;

    if (i2c_read_register(LPS25H_ADDRESS, LPS25H_DEVICE_ID_REG, &device_id) < 0)
        return -1;

    if (device_id != LPS25H_DEVICE_ID)
        return -1;

    return 0;
}

int lps25h_enable(void)
{
    if (check_device_id() < 0) {
        fprintf(stderr, "lps25h: Invalid device id.\n");
        return -1;
    }

    if (i2c_write_register(LPS25H_ADDRESS, LPS25H_CTRL1_REG, ACTIVE_MODE) < 0
    ||  i2c_write_register(LPS25H_ADDRESS, LPS25H_CTRL2_REG, 0) < 0             /* Disable FIFO */
    ||  i2c_write_register(LPS25H_ADDRESS, LPS25H_INT_CFG_REG, 0) < 0)          /* Disable interrupts */
        return -1;

    return 0;
}

int lps25h_get_pressure_measure(float *pressure)
{
    uint8_t ctrl2_reg = START_CONVERSION;
    uint32_t raw_pressure = 0;
    uint8_t tmp = 0;

    if (pressure == NULL) {
        fprintf(stderr, "lps25h: Cannot store pressure using null pointer.\n");
        return -1;
    }

    /* Ask device to perform conversion */
    if (i2c_write_register(LPS25H_ADDRESS, LPS25H_CTRL2_REG, ctrl2_reg) < 0)
        return -1;
    while (ctrl2_reg & START_CONVERSION) {
        if (i2c_read_register(LPS25H_ADDRESS, LPS25H_CTRL2_REG, &ctrl2_reg) < 0)
            return -1;
    }

    /* Read output registers */
    if (i2c_read_register(LPS25H_ADDRESS, LPS25H_PRESS_OUT_H_REG, &tmp) < 0)
        return -1;
    raw_pressure = tmp;
    if (i2c_read_register(LPS25H_ADDRESS, LPS25H_PRESS_OUT_L_REG, &tmp) < 0)
        return -1;
    raw_pressure <<= 8;
    raw_pressure |= tmp;
    if (i2c_read_register(LPS25H_ADDRESS, LPS25H_PRESS_OUT_XL_REG, &tmp) < 0)
        return -1;
    raw_pressure <<= 8;
    raw_pressure |= tmp;

    *pressure = ((float)raw_pressure) / 4096.f;

    return 0;
}

int lps25h_disable(void)
{
    /* Set device in power down mode */
    if (i2c_write_register(LPS25H_ADDRESS, LPS25H_CTRL1_REG, 0) < 0)
        return -1;

    return 0;
}
