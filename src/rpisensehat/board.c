#include <time.h>
#include <letmecreate/rpisensehat/attiny88.h>
#include <letmecreate/rpisensehat/board.h>
#include <letmecreate/rpisensehat/hts221.h>
#include <letmecreate/rpisensehat/lps25h.h>
#include <letmecreate/rpisensehat/lsm9ds1.h>


int rpisensehat_init(void)
{
    if (attiny88_enable() < 0   /* Enable attiny88 (led and joystick) */
    ||  hts221_enable() < 0     /* Enable humidity sensor */
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

int rpisensehat_get_joystick_input(uint8_t *state)
{
    return attiny88_get_joystick_input(state);
}

int rpisensehat_set_leds_state(uint8_t *pixels)
{
    return attiny88_set_leds_state(pixels);
}

int rpisensehat_display_rainbow(void)
{
    return attiny88_display_rainbow();
}

int rpisensehat_release(void)
{
    if (attiny88_disable() < 0  /* Switch off all LED's */
    ||  hts221_disable() < 0    /* Disable humidity sensor */
    ||  lps25h_disable() < 0    /* Disable pressure sensor */
    ||  lsm9ds1_disable() < 0)  /* Disable accel/gyro/mag sensor */
        return -1;

    return 0;
}
