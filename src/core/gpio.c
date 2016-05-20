#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "core/gpio.h"

#define GPIO_BASE_PATH          "/sys/class/gpio/%s"
#define GPIO_PIN_PATH_FORMAT    "/sys/class/gpio/gpio%d/%s"

#define STR_MAX_LENGTH          (255)

static int gpio_pin_no[GPIO_CNT] = {
22,    /* MIKROBUS_1_AN */
23,    /* MIKROBUS_1_RST */
21,    /* MIKROBUS_1_INT */
25,    /* MIKROBUS_2_AN */
27,    /* MIKROBUS_2_RST */
24     /* MIKROBUS_2_INT */
};


static bool check_pin(const uint8_t pin)
{
    if (pin >= GPIO_CNT) {
        fprintf(stderr, "Invalid gpio pin.\n");
        return false;
    }

    return true;
}

static bool create_gpio_path(char *path, const uint8_t gpio_pin, const char *file_name)
{
    if (snprintf(path, STR_MAX_LENGTH, GPIO_PIN_PATH_FORMAT, gpio_pin_no[gpio_pin], file_name) < 0) {
        fprintf(stderr, "gpio: Could not create path for accessing %s of gpio %d.\n", file_name, gpio_pin_no[gpio_pin]);
        return false;
    }

    return true;
}

static bool check_gpio_initialised(const uint8_t gpio_pin)
{
    DIR *dir = NULL;
    char path[STR_MAX_LENGTH];

    if (!create_gpio_path(path, gpio_pin, ""))
        return false;

    dir = opendir(path);
    if (!dir)
        return false;

    closedir(dir);

    return true;
}

static bool gpio_process_operation(const uint8_t gpio_pin, const char *operation)
{
    int fd = -1;
    char path[STR_MAX_LENGTH];
    char str[STR_MAX_LENGTH];

    if (snprintf(path, STR_MAX_LENGTH, GPIO_BASE_PATH, operation) < 0)
        return false;

    if (snprintf(str, STR_MAX_LENGTH, "%d", gpio_pin_no[gpio_pin]) < 0) {
        fprintf(stderr, "gpio: Could not convert integer %d to string\n", gpio_pin_no[gpio_pin]);
        return false;
    }

    if ((fd = open(path, O_WRONLY)) < 0) {
        fprintf(stderr, "gpio: Could not open file %s", path);
        return false;
    }

    if (write(fd, str, strlen(str) + 1) < 0) {
        fprintf(stderr, "gpio: Failed to write value %s while %sing gpio %d\n.", str, operation, gpio_pin);
        close(fd);
        return false;
    }

    close(fd);

    return true;
}

static bool read_gpio_file(const uint8_t gpio_pin, const char *file_name, char *value)
{
    int fd = -1;
    char path[STR_MAX_LENGTH];

    if (!check_pin(gpio_pin))
        return false;

    if (!check_gpio_initialised(gpio_pin)) {
        fprintf(stderr, "gpio: Cannot get %s of uninitialised gpio %d\n", file_name, gpio_pin_no[gpio_pin]);
        return false;
    }

    if (!create_gpio_path(path, gpio_pin, file_name))
        return false;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(stderr, "gpio: Could not open file %s", path);
        return false;
    }

    if (read(fd, value, STR_MAX_LENGTH) < 0) {
        fprintf(stderr, "gpio: Could not read from file %s\n", path);
        close(fd);
        return false;
    }

    close(fd);

    return true;
}

static bool write_gpio_file(const uint8_t gpio_pin, const char *file_name, const char *value)
{
    int fd = -1;
    char path[STR_MAX_LENGTH];

    if (!check_pin(gpio_pin))
        return false;

    if (!check_gpio_initialised(gpio_pin)) {
        fprintf(stderr, "gpio: Cannot set %s of uninitialised gpio %d\n", file_name, gpio_pin_no[gpio_pin]);
        return false;
    }

    if (!create_gpio_path(path, gpio_pin, file_name))
        return false;

    if ((fd = open(path, O_WRONLY)) < 0) {
        fprintf(stderr, "gpio: Could not open file %s.\n", path);
        return false;
    }

    if (write(fd, value, strlen(value)+1) < 0) {
        fprintf(stderr, "gpio: Could not write %s of gpio %d.\n", file_name, gpio_pin_no[gpio_pin]);
        close(fd);
        return false;
    }

    close(fd);

    return true;
}

int gpio_init(const uint8_t gpio_pin)
{
    if (!check_pin(gpio_pin))
        return -1;

    if (check_gpio_initialised(gpio_pin))
        return 0;

    if (!gpio_process_operation(gpio_pin, "export"))
        return -1;

    return 0;
}

int gpio_set_direction(const uint8_t gpio_pin, const uint8_t dir)
{
    char str[4];

    if (dir == GPIO_OUTPUT) {
        strcpy(str, "out");
    } else if (dir == GPIO_INPUT) {
        strcpy(str, "in");
    } else {
        fprintf(stderr, "gpio: Cannot set gpio %d to invalid direction.\n", gpio_pin_no[gpio_pin]);
        return -1;
    }

    if (!write_gpio_file(gpio_pin, "direction", str))
        return false;

    return true;
}

int gpio_get_direction(const uint8_t gpio_pin, uint8_t *dir)
{
    char value[STR_MAX_LENGTH];

    if (dir == NULL) {
        fprintf(stderr, "gpio: Cannot store direction of pin %d to null variable.\n", gpio_pin_no[gpio_pin]);
        return -1;
    }

    if (!read_gpio_file(gpio_pin, "direction", value))
        return -1;

    if (strncmp(value, "out", 3) == 0) {
        *dir = GPIO_OUTPUT;
    } else if (strncmp(value, "in", 2) == 0) {
        *dir = GPIO_INPUT;
    } else {
        fprintf(stderr, "gpio: Invalid direction read from gpio %d.\n", gpio_pin_no[gpio_pin]);
        return -1;
    }

    return 0;
}

int gpio_set_value(const uint8_t gpio_pin, const uint8_t value)
{
    uint8_t dir;
    char str[2];

    if (!gpio_get_direction(gpio_pin, &dir))
        return -1;

    if (dir == GPIO_INPUT) {
        fprintf(stderr, "gpio: Cannot set value to an input.\n");
        return -1;
    }

    if (value == 0)
        strcpy(str, "0");
    else
        strcpy(str, "1");

    if (!write_gpio_file(gpio_pin, "value", str))
        return -1;

    return 0;
}

int gpio_get_value(const uint8_t gpio_pin, uint8_t *value)
{
    char str[STR_MAX_LENGTH];

    if (value == NULL) {
        fprintf(stderr, "gpio: Cannot store value of pin %d to null variable.\n", gpio_pin_no[gpio_pin]);
        return -1;
    }

    if (!read_gpio_file(gpio_pin, "value", str))
        return -1;

    if (str[0] == '0') {
        *value = 0;
    } else if (str[0] == '1') {
        *value = 1;
    } else {
        fprintf(stderr, "gpio: Invalid value read from gpio %d.\n", gpio_pin_no[gpio_pin]);
        return -1;
    }

    return 0;
}

int gpio_release(const uint8_t gpio_pin)
{
    if (!check_pin(gpio_pin))
        return -1;

    if (!check_gpio_initialised(gpio_pin))
        return 0;

    if (!gpio_process_operation(gpio_pin, "unexport"))
        return -1;

    return 0;
}

