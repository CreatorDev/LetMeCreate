#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <letmecreate/core/i2c.h>
#include <letmecreate/core/common.h>

#define MIKROBUS_I2C_PATH_1 "/dev/i2c-0"
#define MIKROBUS_I2C_PATH_2 "/dev/i2c-1"


static int fds[] = { -1, -1 };
static uint8_t current_mikrobus_index = MIKROBUS_1;
static uint32_t timeout_for_bus[MIKROBUS_COUNT] = {NO_TIMEOUT, NO_TIMEOUT};

static int i2c_select_slave(int fd, uint16_t address)
{
    if (ioctl(fd, I2C_SLAVE, address) < 0) {
        fprintf(stderr, "i2c: Failed to select slave address.\n");
        return -1;
    }

    return 0;
}

static int i2c_init_bus(uint8_t mikrobus_index)
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

    timeout_for_bus[mikrobus_index] = NO_TIMEOUT;

    return 0;
}

static int i2c_release_bus(uint8_t mikrobus_index)
{
    int ret = 0;
    switch (mikrobus_index) {
    case MIKROBUS_1:
    case MIKROBUS_2:
        if (fds[mikrobus_index] >= 0) {
            ret = close(fds[mikrobus_index]);
            fds[mikrobus_index] = -1;
        }
        break;
    }

    return ret;
}

int i2c_init(void)
{
    if (i2c_init_bus(MIKROBUS_1) < 0)
        return -1;

    return i2c_init_bus(MIKROBUS_2);
}

void i2c_select_bus(uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
    case MIKROBUS_2:
        current_mikrobus_index = mikrobus_index;
    }
}

uint8_t i2c_get_current_bus(void)
{
    return current_mikrobus_index;
}

int i2c_write(uint16_t slave_address, const uint8_t *buffer, uint32_t count)
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

int i2c_read(uint16_t slave_address, uint8_t *buffer, uint32_t count)
{
    int ret, fd;
    uint32_t nbBytesReceived = 0;
    uint32_t timeout = timeout_for_bus[current_mikrobus_index];

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

    while (nbBytesReceived < count) {
        if (timeout != NO_TIMEOUT) {
            fd_set set;
            struct timeval tmp_timeout;
            tmp_timeout.tv_sec = timeout / 1000;
            tmp_timeout.tv_usec = (timeout % 1000) * 1000;
            FD_ZERO(&set);
            FD_SET(fds[current_mikrobus_index], &set);

            ret = select(fds[current_mikrobus_index] + 1, &set, NULL, NULL, &tmp_timeout);
            if (ret == -1) {
                fprintf(stderr, "i2c: Failed to wait for data.\n");
                return -1;
            } else if (ret == 0) {
                fprintf(stderr, "i2c: Read timeout.\n");
                return nbBytesReceived;
            }
        }

        ret = read(fd, &buffer[nbBytesReceived], count - nbBytesReceived);
        if (ret < 0) {
            fprintf(stderr, "i2c: Failed to read.\n");
            return -1;
        }

        nbBytesReceived += ret;
    }

    return nbBytesReceived;
}

int i2c_write_byte(uint16_t slave_address, uint8_t data)
{
    return i2c_write(slave_address, &data, 1);
}

int i2c_read_byte(uint16_t slave_address, uint8_t *data)
{
    return i2c_read(slave_address, data, 1);
}

int i2c_write_register(uint16_t address, uint8_t reg_address, uint8_t value)
{
    uint8_t buffer[2] = { reg_address, value };
    return i2c_write(address, buffer, sizeof(buffer));
}

int i2c_read_register(uint16_t address, uint8_t reg_address, uint8_t *data)
{
    if (i2c_write_byte(address, reg_address) < 0)
        return -1;

    return i2c_read_byte(address, data);
}

int i2c_read_16b_register(uint16_t address,
                          uint8_t reg_low_address, uint8_t reg_high_address,
                          uint16_t *data)
{
    uint8_t low = 0, high = 0;

    if (data == NULL)
        return -1;

    if (i2c_read_register(address, reg_low_address, &low) < 0)
        return -1;

    if (i2c_read_register(address, reg_high_address, &high) < 0)
        return -1;

    *data = high;
    *data <<= 8;
    *data |= low;

    return 0;
}

uint32_t i2c_get_timeout(void)
{
    return timeout_for_bus[current_mikrobus_index];
}

void i2c_set_timeout(uint32_t timeout)
{
    timeout_for_bus[current_mikrobus_index] = timeout;
}

int i2c_release(void)
{
    int ret = 0;
    ret += i2c_release_bus(MIKROBUS_1);
    ret += i2c_release_bus(MIKROBUS_2);
    return ret ? -1 : 0;
}
