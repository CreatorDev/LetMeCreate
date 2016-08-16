#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "letmecreate/core/pwm.h"
#include "letmecreate/core/common.h"

#define DEVICE_FILE_BASE_PATH           "/sys/class/pwm/pwmchip0/"
#define PWM_DEVICE_FILE_BASE_PATH       "/sys/class/pwm/pwmchip0/pwm"

static bool pin_initialised[2] = { false, false };

static bool check_mikrobus_index(uint8_t mikrobus_index)
{
    if (mikrobus_index == MIKROBUS_1)
        return true;
    if (mikrobus_index == MIKROBUS_2)
        return true;

    fprintf(stderr, "pwm: Invalid pin.\n");
    return false;
}

static bool is_pwm_pin_exported(uint8_t mikrobus_index)
{
    DIR *dir = NULL;
    char path[MAX_STR_LENGTH];

    if (snprintf(path, MAX_STR_LENGTH, PWM_DEVICE_FILE_BASE_PATH"%d", mikrobus_index) < 0)
        return false;

    dir = opendir(path);
    if (!dir)
        return false;

    closedir(dir);

    return true;
}

static int write_str_pwm_file(uint8_t mikrobus_index, const char *file_name, const char *str)
{
    char path[MAX_STR_LENGTH];

    if (snprintf(path, MAX_STR_LENGTH, PWM_DEVICE_FILE_BASE_PATH"%d/%s", mikrobus_index, file_name) < 0) {
        fprintf(stderr, "pwm: Could not write to file %s of pwm pin %d.\n", file_name, mikrobus_index);
        return -1;
    }

    return write_str_file(path, str);
}

static int write_int_pwm_file(uint8_t mikrobus_index, const char *file_name, uint32_t value)
{
    char str[MAX_STR_LENGTH];

    if (snprintf(str, MAX_STR_LENGTH, "%d", value) < 0) {
        fprintf(stderr, "pwm: Failed to convert %d to string.\n", value);
        return -1;
    }

    return write_str_pwm_file(mikrobus_index, file_name, str);
}

static int read_int_pwm_file(uint8_t mikrobus_index, const char *file_name, uint32_t *value)
{
    char path[MAX_STR_LENGTH];

    if (snprintf(path, MAX_STR_LENGTH, PWM_DEVICE_FILE_BASE_PATH"%d/%s", mikrobus_index, file_name) < 0) {
        fprintf(stderr, "pwm: Could not read from file %s of pwm pin %d.\n", file_name, mikrobus_index);
        return -1;
    }

    return read_int_file(path, value);
}

int pwm_init(uint8_t mikrobus_index)
{
    if (!check_mikrobus_index(mikrobus_index))
        return -1;

    if (pin_initialised[mikrobus_index])
        return 0;

    if (!is_pwm_pin_exported(mikrobus_index)) {
        if (export_pin(DEVICE_FILE_BASE_PATH, mikrobus_index) < 0)
            return -1;
    }

    pin_initialised[mikrobus_index] = true;

    if (pwm_disable(mikrobus_index) < 0
    ||  write_int_pwm_file(mikrobus_index, "period", 333333) < 0
    ||  write_int_pwm_file(mikrobus_index, "duty_cycle", 166666) < 0) {
        pwm_release(mikrobus_index);
        return -1;
    }

    return 0;
}

int pwm_enable(uint8_t mikrobus_index)
{
    if (!check_mikrobus_index(mikrobus_index))
        return -1;

    if (!pin_initialised[mikrobus_index]) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", mikrobus_index);
        return -1;
    }

    return write_str_pwm_file(mikrobus_index, "enable", "1");
}

int pwm_set_duty_cycle(uint8_t mikrobus_index, float percentage)
{
    uint32_t duty_cycle = 0;
    uint32_t period = 0;

    if (!check_mikrobus_index(mikrobus_index))
        return -1;

    if (!pin_initialised[mikrobus_index]) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", mikrobus_index);
        return -1;
    }

    if (percentage < 0.f || percentage > 100.f) {
        fprintf(stderr, "pwm: Invalid percentage (must be in range 0..100).\n");
        return -1;
    }

    /* Compute duty cycle in nanoseconds from period */
    if (pwm_get_period(mikrobus_index, &period) < 0)
        return -1;

    duty_cycle = period * (percentage / 100.f);
    if (duty_cycle < 45)
        duty_cycle = 45;
    return write_int_pwm_file(mikrobus_index, "duty_cycle", duty_cycle);
}

int pwm_get_duty_cycle(uint8_t mikrobus_index, float *percentage)
{
    uint32_t duty_cycle = 0;
    uint32_t period = 0;

    if (!check_mikrobus_index(mikrobus_index))
        return -1;

    if (!pin_initialised[mikrobus_index]) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", mikrobus_index);
        return -1;
    }

    if (percentage == NULL) {
        fprintf(stderr, "pwm: Cannot store duty cycle in null variable.\n");
        return -1;
    }

    if (read_int_pwm_file(mikrobus_index, "period", &period) < 0)
        return -1;

    if (read_int_pwm_file(mikrobus_index, "duty_cycle", &duty_cycle) < 0)
        return -1;

    if (period == 0)
        *percentage = 0.f;
    else
        *percentage = ((float)duty_cycle) / ((float)period) * 100.f;

    return 0;
}

int pwm_set_frequency(uint8_t mikrobus_index, uint32_t frequency)
{
    return pwm_set_period(mikrobus_index, 1000000000/frequency);
}

int pwm_set_period(uint8_t mikrobus_index, uint32_t period)
{
    float percentage = 0.f;
    uint32_t old_period = 0, old_duty_cycle = 0, duty_cycle = 0;

    if (!check_mikrobus_index(mikrobus_index))
        return -1;

    if (!pin_initialised[mikrobus_index]) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", mikrobus_index);
        return -1;
    }

    if (period < 45) {
        fprintf(stderr, "pwm: Period is out of range, needs to be greater than 44.\n");
        return -1;
    }

    if (period > 373028) {
        fprintf(stderr, "pwm: Period is out of range, needs to be less than 373029ns.\n");
        return -1;
    }

    if (pwm_get_period(mikrobus_index, &old_period) < 0)
        return -1;
    if (pwm_get_duty_cycle(mikrobus_index, &percentage) < 0)
        return -1;

    old_duty_cycle = (percentage/100.f) * old_period;
    duty_cycle = (percentage/100.f) * period;
    if (duty_cycle < 45)
        duty_cycle = 45;

    if (old_duty_cycle > period) {
        if (write_int_pwm_file(mikrobus_index, "duty_cycle", duty_cycle) < 0)
            return -1;

        return write_int_pwm_file(mikrobus_index, "period", period);
    } else {
        if (write_int_pwm_file(mikrobus_index, "period", period) < 0)
            return -1;

        return write_int_pwm_file(mikrobus_index, "duty_cycle", duty_cycle);
    }
}

int pwm_get_period(uint8_t mikrobus_index, uint32_t *period)
{
    if (!check_mikrobus_index(mikrobus_index))
        return -1;

    if (!pin_initialised[mikrobus_index]) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", mikrobus_index);
        return -1;
    }

    if (period == NULL) {
        fprintf(stderr, "pwm: Cannot store period in null variable.\n");
        return -1;
    }

    return read_int_pwm_file(mikrobus_index, "period", period);
}

int pwm_get_frequency(uint8_t mikrobus_index, uint32_t *frequency)
{
    uint32_t period = 0;

    if (frequency == NULL) {
        fprintf(stderr, "pwm: Cannot store frequency to null variable.\n");
        return -1;
    }

    if (pwm_get_period(mikrobus_index, &period) < 0)
        return -1;

    *frequency = 1000000000/period;

    return 0;
}

int pwm_disable(uint8_t mikrobus_index)
{
    if (!check_mikrobus_index(mikrobus_index))
        return -1;

    if (!pin_initialised[mikrobus_index]) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", mikrobus_index);
        return -1;
    }

    return write_str_pwm_file(mikrobus_index, "enable", "0");
}

int pwm_release(uint8_t mikrobus_index)
{
    if (!check_mikrobus_index(mikrobus_index))
        return -1;

    if (!pin_initialised[mikrobus_index])
        return 0;

    if (is_pwm_pin_exported(mikrobus_index)) {
        if (unexport_pin(DEVICE_FILE_BASE_PATH, mikrobus_index) < 0)
            return -1;
    }

    pin_initialised[mikrobus_index] = false;

    return 0;
}
