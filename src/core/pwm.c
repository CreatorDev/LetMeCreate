#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "core/pwm.h"

#define DEVICE_FILE_BASE_PATH           "/sys/class/pwm/pwmchip0/"
#define PWM_DEVICE_FILE_BASE_PATH       "/sys/class/pwm/pwmchip0/pwm"

#define MAX_STR_LENGTH                  (255)


static bool check_pwm_pin(const uint8_t pwm_pin)
{
    if (pwm_pin == MIKROBUS_1_PWM)
        return true;
    if (pwm_pin == MIKROBUS_2_PWM)
        return true;

    fprintf(stderr, "pwm: Invalid pin.\n");
    return false;
}

static bool is_pwm_pin_exported(const uint8_t pwm_pin)
{
    DIR *dir = NULL;
    char path[MAX_STR_LENGTH];

    if (snprintf(path, MAX_STR_LENGTH, PWM_DEVICE_FILE_BASE_PATH"%d", pwm_pin) < 0)
        return false;

    dir = opendir(path);
    if (!dir)
        return false;

    closedir(dir);

    return true;
}

static int write_str_pwm_file(const uint8_t pwm_pin, const char *file_name, const char *str)
{
    int fd = -1;
    char path[MAX_STR_LENGTH];

    if (snprintf(path, MAX_STR_LENGTH, PWM_DEVICE_FILE_BASE_PATH"%d/%s", pwm_pin, file_name) < 0) {
        fprintf(stderr, "pwm: Could not write to file %s of pwm pin %d.\n", file_name, pwm_pin);
        return -1;
    }

    if ((fd = open(path, O_WRONLY)) < 0) {
        fprintf(stderr, "pwm: Failed to open file %s\n", path);
        return -1;
    }

    if (write(fd, str, strlen(str)+1) < 0) {
        fprintf(stderr, "pwm: Failed to write to file %s\n", path);
        return -1;
    }

    close(fd);

    return 0;
}

static int write_int_pwm_file(const uint8_t pwm_pin, const char *file_name, const uint32_t value)
{
    char str[MAX_STR_LENGTH];

    if (snprintf(str, MAX_STR_LENGTH, "%d", value) < 0) {
        fprintf(stderr, "pwm: Failed to convert %d to string.\n", value);
        return -1;
    }

    return write_str_pwm_file(pwm_pin, file_name, str);
}

static int read_str_pwm_file(const uint8_t pwm_pin, const char *file_name, char *str, const uint32_t count)
{
    int fd = -1;
    char path[MAX_STR_LENGTH];

    if (snprintf(path, MAX_STR_LENGTH, PWM_DEVICE_FILE_BASE_PATH"%d/%s", pwm_pin, file_name) < 0) {
        fprintf(stderr, "pwm: Could not write to file %s of pwm pin %d.\n", file_name, pwm_pin);
        return -1;
    }

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(stderr, "pwm: Failed to open file %s\n", path);
        return -1;
    }

    if (read(fd, str, count) < 0) {
        fprintf(stderr, "pwm: Failed to read from file %s\n", path);
        return -1;
    }

    close(fd);

    return 0;
}

static int read_int_pwm_file(const uint8_t pwm_pin, const char *file_name, uint32_t *value)
{
    char str[MAX_STR_LENGTH];

    if (read_str_pwm_file(pwm_pin, file_name, str, MAX_STR_LENGTH) < 0)
        return -1;

    *value = strtoul(str, NULL, 10);

    return 0;
}

static int export_pin(const uint8_t pwm_pin, const bool export)
{
    int fd = -1;
    char path[MAX_STR_LENGTH];
    char str[MAX_STR_LENGTH];

    if (snprintf(path, MAX_STR_LENGTH, DEVICE_FILE_BASE_PATH"%s", export ? "export" : "unexport") < 0)
        return -1;

    if (snprintf(str, MAX_STR_LENGTH, "%d", pwm_pin) < 0)
        return -1;

    if ((fd = open(path, O_WRONLY)) < 0) {
        fprintf(stderr, "pwm: Failed to open file %s.\n", path);
        return -1;
    }

    if (write(fd, str, strlen(str) + 1) < 0) {
        fprintf(stderr, "pwm: Failed to write to file %s.\n", path);
    }

    close(fd);

    return 0;
}

int pwm_init(const uint8_t pwm_pin)
{
    char str[2];

    if (!check_pwm_pin(pwm_pin))
        return -1;

    if (is_pwm_pin_exported(pwm_pin))
        return 0;

    return export_pin(pwm_pin, true);
}

int pwm_enable(const uint8_t pwm_pin)
{
    if (!check_pwm_pin(pwm_pin))
        return -1;

    if (!is_pwm_pin_exported(pwm_pin)) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", pwm_pin);
        return -1;
    }

    return write_str_pwm_file(pwm_pin, "enable", "1");
}

int pwm_set_duty_cycle(const uint8_t pwm_pin, const float percentage)
{
    int fd;
    uint32_t duty_cycle = 0;
    uint32_t period = 0;
    char path[MAX_STR_LENGTH];

    if (!check_pwm_pin(pwm_pin))
        return -1;

    if (!is_pwm_pin_exported(pwm_pin)) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", pwm_pin);
        return -1;
    }

    if (percentage < 0.f || percentage > 100.f) {
        fprintf(stderr, "pwm: Invalid percentage (must be in range 0..100).\n");
        return -1;
    }

    /* Compute duty cycle in nanoseconds from period */
    if (pwm_get_period(pwm_pin, &period) < 0) {
        return -1;
    }

    duty_cycle = period * (percentage / 100.f);

    return write_int_pwm_file(pwm_pin, "duty_cycle", duty_cycle);
}

int pwm_get_duty_cycle(const uint8_t pwm_pin, float *percentage)
{
    uint32_t duty_cycle = 0;
    uint32_t period = 0;

    if (!check_pwm_pin(pwm_pin))
        return -1;

    if (!is_pwm_pin_exported(pwm_pin)) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", pwm_pin);
        return -1;
    }

    if (percentage == NULL) {
        fprintf(stderr, "pwm: Cannot store duty cycle in null variable.\n");
        return -1;
    }

    if (read_int_pwm_file(pwm_pin, "period", &period) < 0)
        return -1;

    if (read_int_pwm_file(pwm_pin, "duty_cycle", &duty_cycle) < 0)
        return -1;

    *percentage = ((float)duty_cycle) / ((float)period) * 100.f;

    return 0;
}

int pwm_set_frequency(const uint8_t pwm_pin, const uint32_t frequency)
{
    return pwm_set_period(pwm_pin, 1000000000/frequency);
}

int pwm_set_period(const uint8_t pwm_pin, const uint32_t period)
{
    float percentage = 0.f;
    uint32_t duty_cycle = 0;

    if (!check_pwm_pin(pwm_pin))
        return -1;

    if (!is_pwm_pin_exported(pwm_pin)) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", pwm_pin);
        return -1;
    }

    if (pwm_get_duty_cycle(pwm_pin, &percentage) < 0)
        return -1;

    if (write_int_pwm_file(pwm_pin, "period", period) < 0)
        return -1;

    duty_cycle = period * (percentage / 100.f);

    return write_int_pwm_file(pwm_pin, "duty_cycle", duty_cycle);
}

int pwm_get_period(const uint8_t pwm_pin, uint32_t *period)
{
    if (!check_pwm_pin(pwm_pin))
        return -1;

    if (!is_pwm_pin_exported(pwm_pin)) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", pwm_pin);
        return -1;
    }

    if (period == NULL) {
        fprintf(stderr, "pwm: Cannot store period in null variable.\n");
        return -1;
    }

    if (read_int_pwm_file(pwm_pin, "period", period) < 0) {
        return -1;
    }

    return 0;
}

int pwm_get_frequency(const uint8_t pwm_pin, float *frequency)
{
    uint32_t period = 0;

    if (frequency == NULL) {
        fprintf(stderr, "pwm: Cannot store frequency to null variable.\n");
        return -1;
    }

    if (pwm_get_period(pwm_pin, &period) < 0)
        return -1;

    *frequency = 1000000000.f / ((float)period);

    return 0;
}

int pwm_disable(const uint8_t pwm_pin)
{
    if (!check_pwm_pin(pwm_pin))
        return -1;

    if (!is_pwm_pin_exported(pwm_pin)) {
        fprintf(stderr, "pwm: Invalid operation, pin %d must be initialised first.\n", pwm_pin);
        return -1;
    }

    return write_str_pwm_file(pwm_pin, "disable", "0");
}

int pwm_release(const uint8_t pwm_pin)
{
    char str[2];

    if (!check_pwm_pin(pwm_pin))
        return -1;

    if (!is_pwm_pin_exported(pwm_pin))
        return 0;

    return export_pin(pwm_pin, false);
}
