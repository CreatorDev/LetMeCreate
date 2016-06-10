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

static int check_mikrobus_index(const uint8_t mikrobus_index)
{
    if (mikrobus_index > MIKROBUS_2) {
        fprintf(stderr, "spi: Invalid bus index.\n");
        return -1;
    }

    return 0;
}

int spi_init(const uint8_t mikrobus_index, const uint32_t mode)
{
    int fd = -1;
    uint8_t bits_per_word = BITS_PER_WORD;
    uint32_t speed = SPI_SPEED;
    const char *spi_path = NULL;

    if (check_mikrobus_index(mikrobus_index) < 0)
        return -1;

    if (fds[mikrobus_index] >= 0)
        return 0;

    switch (mikrobus_index) {
    case MIKROBUS_1:
        spi_path = MIKROBUS_SPI_PATH_1;
        break;
    case MIKROBUS_2:
        spi_path = MIKROBUS_SPI_PATH_2;
        break;
    }

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

int spi_select_bus(const uint8_t mikrobus_index)
{
    if (check_mikrobus_index(mikrobus_index) < 0)
        return -1;

    current_mikrobus_index = mikrobus_index;
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

int spi_release(const uint8_t mikrobus_index)
{
    if (check_mikrobus_index(mikrobus_index) < 0)
        return -1;

    if (fds[mikrobus_index] >= 0) {
        close(fds[mikrobus_index]);
        fds[mikrobus_index] = -1;
    }

    return 0;
}

