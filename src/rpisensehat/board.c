#include <time.h>
#include <letmecreate/core/gpio.h>
#include <letmecreate/rpisensehat/board.h>
#include <letmecreate/rpisensehat/hts221.h>
#include <letmecreate/rpisensehat/lps25h.h>
#include <letmecreate/rpisensehat/lsm9ds1.h>


static void sleep_50ms(void)
{
    struct timespec rem, req = {
        .tv_sec = 0,
        .tv_nsec = 50000000
    };

    while (nanosleep(&req, &rem))
        req = rem;
}

static int reset_atmel_chip(void)
{
    if (gpio_init(GPIO_21) < 0
    ||  gpio_set_direction(GPIO_21, GPIO_OUTPUT) < 0)
        return -1;

    if (gpio_set_value(GPIO_21, 0) < 0)
        return -1;

    sleep_50ms();

    if (gpio_set_value(GPIO_21, 1) < 0)
        return -1;

    sleep_50ms();
    sleep_50ms();

    return 0;
}

int rpisensehat_init(void)
{
    /* Reset Atmel Chip otherwise it hangs the I2C bus. */
    if (reset_atmel_chip() < 0)
        return -1;

    if (hts221_enable() < 0     /* Enable humidity sensor */
    ||  lps25h_enable() < 0     /* Enable pressure sensor */
    ||  lsm9ds1_enable() < 0)   /* Enable accel/gyro/mag sensor */
     return -1;

    return 0;
}

int rpisensehat_get_temperature(float *temperature)
{
    return hts221_get_temperature_measure(temperature);
}

int rpisensehat_get_humidity(float *humidity)
{
    return hts221_get_humidity_measure(humidity);
}

int rpisensehat_get_pressure(float *pressure)
{
    return lps25h_get_pressure_measure(pressure);
}

int rpisensehat_get_accelerometer_measure(float *accelX, float *accelY, float *accelZ)
{
    return lsm9ds1_get_accelerometer_measure(accelX, accelY, accelZ);
}

int rpisensehat_get_gyroscope_measure(float *gyroX, float *gyroY, float *gyroZ)
{
    return lsm9ds1_get_gyroscope_measure(gyroX, gyroY, gyroZ);
}

int rpisensehat_get_magnetometer_measure(float *magX, float *magY, float *magZ)
{
    return lsm9ds1_get_magnetometer_measure(magX, magY, magZ);
}

int rpisensehat_release(void)
{
    if (hts221_disable() < 0    /* Disable humidity sensor */
    ||  lps25h_disable() < 0    /* Disable pressure sensor */
    ||  lsm9ds1_disable() < 0)  /* Disable accel/gyro/mag sensor */
        return -1;

    return 0;
}
