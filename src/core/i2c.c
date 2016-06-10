#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include "core/i2c.h"
#include "core/common.h"

#define MIKROBUS_I2C_PATH_1 "/dev/i2c-0"
#define MIKROBUS_I2C_PATH_2 "/dev/i2c-1"


static int fds[] = { -1, -1 };
static uint8_t current_mikrobus_index = MIKROBUS_1;

static int i2c_select_slave(const int fd, const uint16_t address)
{
    if (ioctl(fd, I2C_SLAVE, address) < 0) {
        fprintf(stderr, "i2c: Failed to select slave address.\n");
        return -1;
    }

    return 0;
}

static int i2c_init_bus(const uint8_t mikrobus_index)
{
    const char *i2c_path = NULL;

    switch (mikrobus_index) {
    case MIKROBUS_1:
        i2c_path = MIKROBUS_I2C_PATH_1;
        break;
    case MIKROBUS_2:
        i2c_path = MIKROBUS_I2C_PATH_2;
        break;
    }

    if (fds[mikrobus_index] >= 0)
        return 0;

    if ((fds[mikrobus_index] = open(i2c_path, O_RDWR)) < 0) {
        fprintf(stderr, "i2c: Cannot open device for bus %d.\n", mikrobus_index);
        return -1;
    }

    return 0;
}

static void i2c_release_bus(const uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
    case MIKROBUS_2:
        if (fds[mikrobus_index] >= 0) {
            close(fds[mikrobus_index]);
            fds[mikrobus_index] = -1;
        }
        break;
    }
}

int i2c_init(void)
{
    if (i2c_init_bus(MIKROBUS_1) < 0)
        return -1;

    return i2c_init_bus(MIKROBUS_2);
}

int i2c_select_bus(const uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
    case MIKROBUS_2:
        current_mikrobus_index = mikrobus_index;
        break;
    default:
        fprintf(stderr, "i2c: Invalid bus index.\n");
        return -1;
    }

    return 0;
}

uint8_t i2c_get_current_bus(void)
{
    return current_mikrobus_index;
}

int i2c_write(const uint16_t slave_address, const uint8_t *buffer, const uint32_t count)
{
    int ret, fd;
    uint32_t nbBytesSent;

    fd = fds[current_mikrobus_index];
    if (fd < 0) {
        fprintf(stderr, "i2c: Cannot write to unitialized bus.\n");
        return -1;
    }

    if (buffer == NULL) {
        fprintf(stderr, "i2c: Cannot write using invalid buffer.\n");
        return -1;
    }

    if (count == 0)
        return 0;

    if ((ret = i2c_select_slave(fd, slave_address)) < 0)
        return ret;

    nbBytesSent = 0;
    while (nbBytesSent < count) {
        ret = write(fd, &buffer[nbBytesSent], count - nbBytesSent);
        if (ret < 0) {
            fprintf(stderr, "i2c: Failed to write.\n");
            return -1;
        }

        nbBytesSent += ret;
    }

    return nbBytesSent;
}

int i2c_read(const uint16_t slave_address, uint8_t *buffer, const uint32_t count)
{
    int ret, fd;
    uint32_t nbBytesReceived;

    fd = fds[current_mikrobus_index];
    if (fd < 0) {
        fprintf(stderr, "i2c: Cannot read using unitialized bus.\n");
        return -1;
    }

    if (buffer == NULL) {
        fprintf(stderr, "i2c: Cannot read from invalid buffer.\n");
        return -1;
    }

    if (count == 0)
        return 0;

    if ((ret = i2c_select_slave(fd, slave_address)) < 0)
        return ret;

    nbBytesReceived = 0;
    while (nbBytesReceived < count) {
        ret = read(fd, &buffer[nbBytesReceived], count - nbBytesReceived);
        if (ret < 0) {
            fprintf(stderr, "i2c: Failed to read.\n");
            return -1;
        }

        nbBytesReceived += ret;
    }

    return nbBytesReceived;
}

int i2c_write_byte(const uint16_t slave_address, const uint8_t data)
{
    return i2c_write(slave_address, &data, 1);
}

int i2c_read_byte(const uint16_t slave_address, uint8_t *data)
{
    return i2c_read(slave_address, data, 1);
}

void i2c_release(void)
{
    i2c_release_bus(MIKROBUS_1);
    i2c_release_bus(MIKROBUS_2);
}
