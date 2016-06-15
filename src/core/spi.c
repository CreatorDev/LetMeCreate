#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "core/spi.h"
#include "core/common.h"

#define MIKROBUS_SPI_PATH_1 "/dev/spidev0.2"
#define MIKROBUS_SPI_PATH_2 "/dev/spidev0.3"

#define SPI_SPEED           (1000000)   /* 1MHz */
#define BITS_PER_WORD       (8)

static int fds[] = { -1, -1 };
static uint8_t current_mikrobus_index = MIKROBUS_1;

static int spi_init_bus(const uint8_t mikrobus_index)
{
    int fd = -1;
    uint8_t bits_per_word = BITS_PER_WORD;
    uint32_t speed = SPI_SPEED;
    uint32_t mode = SPI_MODE_3;
    const char *spi_path = NULL;

    switch (mikrobus_index) {
    case MIKROBUS_1:
        spi_path = MIKROBUS_SPI_PATH_1;
        break;
    case MIKROBUS_2:
        spi_path = MIKROBUS_SPI_PATH_2;
        break;
    default:
        fprintf(stderr, "spi: Invalid mikrobus index.\n");
        return -1;
    }

    if (fds[mikrobus_index] >= 0)
        return 0;

    if ((fd = open(spi_path, O_RDWR)) == -1) {
        fprintf(stderr, "spi: Failed to open device.\n");
        return -1;
    }

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) {
        fprintf(stderr, "spi: Failed to set mode.\n");
        return -1;
    }

    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) == -1) {
        fprintf(stderr, "spi: Failed to set bits per word.\n");
        return -1;
    }

    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        fprintf(stderr, "spi: Failed to set max speed.\n");
        return -1;
    }

    fds[mikrobus_index] = fd;

    return fd;
}

static int spi_release_bus(const uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
    case MIKROBUS_2:
        if (fds[mikrobus_index] >= 0) {
            close(fds[mikrobus_index]);
            fds[mikrobus_index] = -1;
        }
        break;
    default:
        fprintf(stderr, "spi: Invalid mikrobus index.\n");
        return -1;
    }

    return 0;
}

int spi_init(void)
{
    if (spi_init_bus(MIKROBUS_1) < 0) {
        fprintf(stderr, "spi: Failed to initialise spi on mikrobus 1\n");
        return -1;
    }

    if (spi_init_bus(MIKROBUS_2) < 0) {
        fprintf(stderr, "spi: Failed to initialise spi on mikrobus 2\n");
        return -1;
    }

    return 0;
}

int spi_set_mode(const uint8_t mikrobus_index, const uint32_t mode)
{
    if (fds[mikrobus_index] < 0) {
        fprintf(stderr, "spi: Cannot set mode of uninitialised bus.\n");
        return -1;
    }

    if (ioctl(fds[mikrobus_index], SPI_IOC_WR_MODE, &mode) < 0) {
        fprintf(stderr, "spi: Failed to set mode.\n");
        return -1;
    }

    return 0;
}

int spi_set_speed(const uint8_t mikrobus_index, const uint32_t speed)
{
    if (fds[mikrobus_index] < 0) {
        fprintf(stderr, "spi: Cannot set mode of uninitialised bus.\n");
        return -1;
    }

    if (ioctl(fds[mikrobus_index], SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        fprintf(stderr, "spi: Failed to set speed.\n");
        return -1;
    }

    return 0;
}

int spi_select_bus(const uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
    case MIKROBUS_2:
        current_mikrobus_index = mikrobus_index;
        break;
    default:
        fprintf(stderr, "spi: Invalid mikrobus index.\n");
        return -1;
    }

    return 0;
}

uint8_t spi_get_current_bus(void)
{
    return current_mikrobus_index;
}

int spi_transfer(const uint8_t *tx_buffer, uint8_t *rx_buffer, const uint32_t count)
{
    int fd;
    struct spi_ioc_transfer tr;

    fd = fds[current_mikrobus_index];
    if (fd < 0)  {
        fprintf(stderr, "spi: Cannot make transfer with invalid file descriptor.\n");
        return -1;
    }

    if (count == 0)
        return 0;

    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (const unsigned long)tx_buffer;
    tr.rx_buf = (unsigned long)rx_buffer;
    tr.len = count;

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        fprintf(stderr, "spi: Failed to transfer message.\n");
        return -1;
    }

    return 0;
}

int spi_release(void)
{
    if (spi_release_bus(MIKROBUS_1) < 0) {
        fprintf(stderr, "spi: Failed to release spi on mikrobus 1\n");
        return -1;
    }

    if (spi_release_bus(MIKROBUS_2) < 0) {
        fprintf(stderr, "spi: Failed to release spi on mikrobus 2\n");
        return -1;
    }

    return 0;
}

