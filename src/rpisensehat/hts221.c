#include <stdio.h>
#include <stdint.h>
#include <letmecreate/core/i2c.h>
#include <letmecreate/rpisensehat/hts221.h>

/* I2C address of device */
#define HTS221_ADDRESS              (0x5F)

/* Register addresses */
#define HTS221_DEVICE_ID_REG        (0x0F)
#define HTS221_AV_CONF_REG          (0x10)
#define HTS221_CTRL1_REG            (0x20)
#define HTS221_CTRL2_REG            (0x21)
#define HTS221_CTRL3_REG            (0x22)
#define HTS221_HUMIDITY_OUT_L_REG   (0x28)
#define HTS221_HUMIDITY_OUT_H_REG   (0x29)
#define HTS221_TEMP_OUT_L_REG       (0x2A)
#define HTS221_TEMP_OUT_H_REG       (0x2B)
#define HTS221_H0_RH_X2_REG         (0x30)
#define HTS221_H1_RH_X2_REG         (0x31)
#define HTS221_T0_DEG_LOW_REG       (0x32)
#define HTS221_T1_DEG_LOW_REG       (0x33)
#define HTS221_T0_T1_DEG_HIGH_REG   (0x35)
#define HTS221_H0_T0_OUT_L_REG      (0x36)
#define HTS221_H0_T0_OUT_H_REG      (0x37)
#define HTS221_H1_T0_OUT_L_REG      (0x3A)
#define HTS221_H1_T0_OUT_H_REG      (0x3B)
#define HTS221_T0_OUT_L_REG         (0x3C)
#define HTS221_T0_OUT_H_REG         (0x3D)
#define HTS221_T1_OUT_L_REG         (0x3E)
#define HTS221_T1_OUT_H_REG         (0x3F)

/* Register values */
#define HTS221_DEVICE_ID    (0xBC)
#define ACTIVE_MODE         (0x80)
#define ONE_SHOT            (0x00)
#define START_CONVERSION    (0x01)


/* HTS221 device contains calibration data for temperature and humidity.
 * Using linear interpolation, it is then possible to convert a 16bit signed
 * integer to a floating point variable in the right unit (celsius degrees
 * for temperature, percentage for relative humidity).
 *
 * The following formulas are used:
 *
 * temperature_a * raw_temperature_measure + temperature_b = temperature in deg celsius
 *
 * humidity_a * raw_humidity_measure + humidity_b = relative humidity in %
 *
 * This structure contains all calibration coefficients.
 */
static struct {
    float temperature_a;
    float temperature_b;
    float humidity_a;
    float humidity_b;
} calibration_data;

static int check_device_id(void)
{
    uint8_t device_id = 0;

    if (i2c_read_register(HTS221_ADDRESS, HTS221_DEVICE_ID_REG, &device_id) < 0)
        return -1;

    if (device_id != HTS221_DEVICE_ID) {
        fprintf(stderr, "hts221: Invalid device id.\n");
        return -1;
    }

    return 0;
}

static int read_calibration_data(void)
{
    uint8_t tmp;
    int16_t raw_t0, raw_t1, raw_h0, raw_h1, t0_deg_8x, t1_deg_8x;
    float t0_deg, t1_deg;
    float rh0, rh1;

    /* Reading raw_t0 and raw_t1 */
    if (i2c_read_16b_register(HTS221_ADDRESS, HTS221_T0_OUT_L_REG, HTS221_T0_OUT_H_REG, (uint16_t*)&raw_t0) < 0
    ||  i2c_read_16b_register(HTS221_ADDRESS, HTS221_T1_OUT_L_REG, HTS221_T1_OUT_H_REG, (uint16_t*)&raw_t1) < 0)
        return -1;

    /* Since we divide by (raw_t1 - raw_t0) for computing temperature_a,
     * we need to make sure we are not going to divide by 0.
     */
    if (raw_t1 == raw_t0) {
        fprintf(stderr, "hts221: Invalid calibration data for temperature.\n");
        return -1;
    }

    /* Reading t0_deg and t1_deg */
    if (i2c_read_register(HTS221_ADDRESS, HTS221_T0_T1_DEG_HIGH_REG, &tmp) < 0)
        return -1;
    t0_deg_8x = (tmp & 0x03) << 8;
    t1_deg_8x = (tmp & 0x0C) << 6;

    if (i2c_read_register(HTS221_ADDRESS, HTS221_T0_DEG_LOW_REG, &tmp) < 0)
        return -1;
    t0_deg_8x |= tmp;
    t0_deg = ((float)t0_deg_8x)/ 8.f;

    if (i2c_read_register(HTS221_ADDRESS, HTS221_T1_DEG_LOW_REG, &tmp) < 0)
        return -1;
    t1_deg_8x |= tmp;
    t1_deg = ((float)t1_deg_8x)/ 8.f;

    /* Computing temperature calibration coefficients */
    calibration_data.temperature_a = (t1_deg - t0_deg) / ((float)(raw_t1 - raw_t0));
    calibration_data.temperature_b = t0_deg - calibration_data.temperature_a * (float)raw_t0;


    /* Reading raw_h0 and raw_h1 */
    if (i2c_read_16b_register(HTS221_ADDRESS, HTS221_H0_T0_OUT_L_REG, HTS221_H0_T0_OUT_H_REG, (uint16_t*)&raw_h0) < 0)
        return -1;
    if (i2c_read_16b_register(HTS221_ADDRESS, HTS221_H1_T0_OUT_L_REG, HTS221_H1_T0_OUT_H_REG, (uint16_t*)&raw_h1) < 0)
        return -1;

    /* Since we divide by (raw_h1 - raw_h0) for computing humidity_a,
     * we need to make sure we are not going to divide by 0.
     */
    if (raw_h1 == raw_h0) {
        fprintf(stderr, "hts221: Invalid calibration data for humidity.\n");
        return -1;
    }

    /* Reading rh0 and rh1 */
    if (i2c_read_register(HTS221_ADDRESS, HTS221_H0_RH_X2_REG, &tmp) < 0)
        return -1;
    rh0 = tmp;
    rh0 /= 2.f;

    if (i2c_read_register(HTS221_ADDRESS, HTS221_H1_RH_X2_REG, &tmp) < 0)
        return -1;
    rh1 = tmp;
    rh1 /= 2.f;

    /* Computing humidity calibration coefficients */
    calibration_data.humidity_a = (rh1 - rh0) / ((float)(raw_h1 - raw_h0));
    calibration_data.humidity_b = rh0 - calibration_data.humidity_a * (float)raw_h0;

    return 0;
}

int hts221_enable(void)
{
    if (check_device_id() < 0)
        return -1;

    if (i2c_write_register(HTS221_ADDRESS, HTS221_CTRL1_REG, ACTIVE_MODE | ONE_SHOT) < 0
    ||  i2c_write_register(HTS221_ADDRESS, HTS221_CTRL2_REG, 0) < 0     /* Disable heater */
    ||  i2c_write_register(HTS221_ADDRESS, HTS221_CTRL3_REG, 0) < 0) {  /* Disable DRDY signal */
        fprintf(stderr, "hts221: Failed to configure device.\n");
        return -1;
    }

    if (read_calibration_data() < 0) {
        fprintf(stderr, "hts221: Failed to read calibration data.\n");
        return -1;
    }

    return 0;
}

int hts221_get_temperature_measure(float *temperature)
{
    uint8_t ctrl2_reg = START_CONVERSION;
    int16_t raw_measure = 0;

    if (temperature == NULL) {
        fprintf(stderr, "hts221: Cannot store temperature using null pointer.\n");
        return -1;
    }

    /* Ask device to perform conversion */
    if (i2c_write_register(HTS221_ADDRESS, HTS221_CTRL2_REG, ctrl2_reg) < 0)
        return -1;
    while (ctrl2_reg & START_CONVERSION) {
        if (i2c_read_register(HTS221_ADDRESS, HTS221_CTRL2_REG, &ctrl2_reg) < 0)
            return -1;
    }

    if (i2c_read_16b_register(HTS221_ADDRESS, HTS221_TEMP_OUT_L_REG, HTS221_TEMP_OUT_H_REG, (uint16_t*)&raw_measure) < 0)
        return -1;

    *temperature = calibration_data.temperature_a * ((float)raw_measure) + calibration_data.temperature_b;

    return 0;
}

int hts221_get_humidity_measure(float *humidity)
{
    uint8_t ctrl2_reg = START_CONVERSION;
    int16_t raw_measure = 0;

    if (humidity == NULL) {
        fprintf(stderr, "hts221: Cannot store humidity using null pointer.\n");
        return -1;
    }

    /* Ask device to perform conversion */
    if (i2c_write_register(HTS221_ADDRESS, HTS221_CTRL2_REG, ctrl2_reg) < 0)
        return -1;
    while (ctrl2_reg & START_CONVERSION) {
        if (i2c_read_register(HTS221_ADDRESS, HTS221_CTRL2_REG, &ctrl2_reg) < 0)
            return -1;
    }

    if (i2c_read_16b_register(HTS221_ADDRESS, HTS221_HUMIDITY_OUT_L_REG, HTS221_HUMIDITY_OUT_H_REG, (uint16_t*)&raw_measure) < 0)
        return -1;

    *humidity = calibration_data.humidity_a * ((float)raw_measure) + calibration_data.humidity_b;

    return 0;
}

int hts221_disable(void)
{
    if (i2c_write_register(HTS221_ADDRESS, HTS221_CTRL2_REG, 0) < 0     /* Disable heater */
    ||  i2c_write_register(HTS221_ADDRESS, HTS221_CTRL3_REG, 0) < 0     /* Disable DRDY signal */
    ||  i2c_write_register(HTS221_ADDRESS, HTS221_CTRL1_REG, 0) < 0)    /* Set device in power-down mode */
        return -1;

    return 0;
}
