#include <stdio.h>
#include <letmecreate/click/dc_motor.h>
#include <letmecreate/core/common.h>
#include <letmecreate/core/gpio.h>
#include <letmecreate/core/pwm.h>


int dc_motor_click_init(uint8_t mikrobus_index)
{
    uint8_t select1_pin, not_sleep_pin;

    /* Ensure that nSLEEP pin is high */
    if (gpio_get_pin(mikrobus_index, TYPE_RST, &not_sleep_pin) < 0
    ||  gpio_init(not_sleep_pin) < 0
    ||  gpio_set_direction(not_sleep_pin, GPIO_OUTPUT) < 0
    ||  gpio_set_value(not_sleep_pin, 1) < 0)
        return -1;

    if (gpio_get_pin(mikrobus_index, TYPE_RST, &select1_pin) < 0
    ||  gpio_init(select1_pin) < 0
    ||  gpio_set_direction(select1_pin, GPIO_OUTPUT) < 0
    ||  gpio_set_value(select1_pin, 1) < 0
    ||  pwm_init(mikrobus_index) < 0
    ||  pwm_set_frequency(mikrobus_index, 20000) < 0
    ||  pwm_set_duty_cycle(mikrobus_index, 0.f) < 0
    ||  pwm_enable(mikrobus_index) < 0)
        return -1;

    return 0;
}

int dc_motor_click_set_direction(uint8_t mikrobus_index, uint8_t direction)
{
    uint8_t select1_pin, old_direction;

    if (direction > 1) {
        fprintf(stderr, "dc_motor: Invalid direction value.\n");
        return -1;
    }

    if (dc_motor_click_get_direction(mikrobus_index, &old_direction) < 0)
        return -1;

    if (gpio_get_pin(mikrobus_index, TYPE_RST, &select1_pin) < 0
    ||  gpio_set_value(select1_pin, direction) < 0)
        return -1;

    if (old_direction != direction) {
        float speed = 0.f;
        if (pwm_get_duty_cycle(mikrobus_index, &speed) < 0)
            return -1;
        speed = 100.f - speed;
        return pwm_set_duty_cycle(mikrobus_index, speed);
    }

    return 0;
}

int dc_motor_click_get_direction(uint8_t mikrobus_index, uint8_t *direction)
{
    uint8_t select1_pin;

    if (gpio_get_pin(mikrobus_index, TYPE_RST, &select1_pin) < 0
    ||  gpio_get_value(select1_pin, direction) < 0)
        return -1;

    return 0;
}

int dc_motor_click_set_speed(uint8_t mikrobus_index, float speed)
{
    uint8_t direction;

    if (speed < 0.f || speed > 100.f) {
        fprintf(stderr, "dc_motor: Invalid speed value, must be in range 0-100.\n");
        return -1;
    }

    if (dc_motor_click_get_direction(mikrobus_index, &direction) < 0)
        return -1;

    if (direction == 1)
        speed = 100.f - speed;

    return pwm_set_duty_cycle(mikrobus_index, speed);
}

int dc_motor_click_get_speed(uint8_t mikrobus_index, float *speed)
{
    uint8_t direction;

    if (speed == NULL) {
        fprintf(stderr, "dc_motor: Cannot store speed in null variable\n");
        return -1;
    }

    if (pwm_get_duty_cycle(mikrobus_index, speed) < 0)
        return -1;

    if (dc_motor_click_get_direction(mikrobus_index, &direction) < 0)
        return -1;

    if (direction == 1)
        *speed = 100.f - *speed;

    return 0;
}

int dc_motor_click_release(uint8_t mikrobus_index)
{
    uint8_t select1_pin, not_sleep_pin;

    if (gpio_get_pin(mikrobus_index, TYPE_RST, &select1_pin) < 0
    ||  gpio_get_pin(mikrobus_index, TYPE_RST, &not_sleep_pin) < 0)
        return -1;

    if (gpio_release(select1_pin) < 0
    ||  pwm_release(mikrobus_index) < 0)
        return -1;

    /* Put device to sleep */
    return gpio_set_value(not_sleep_pin, 0);
}
