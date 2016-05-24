#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include "core/uart.h"

#define UART_1_DEVICE_FILE      "/dev/ttySC0"
#define UART_2_DEVICE_FILE      "/dev/ttySC1"

static int fds[2] = { -1, -1 };
static struct termios old_pts[2];
static uint8_t current_uart_device = MIKROBUS_1_UART;

static bool check_uart_device(const uint8_t uart_device)
{
    if (uart_device == MIKROBUS_1_UART)
        return true;
    if (uart_device == MIKROBUS_2_UART)
        return true;

    fprintf(stderr, "uart: Invalid uart device\n.");
    return false;
}

int uart_init(const uint8_t uart_device, const uint32_t baudrate)
{
    char *device_file = NULL;
    speed_t speed;
    struct termios pts;

    if (!check_uart_device(uart_device))
        return -1;

    switch(baudrate) {
    case UART_BD_9600:
        speed = B9600;
        break;
    case UART_BD_19200:
        speed = B19200;
        break;
    case UART_BD_38400:
        speed = B38400;
        break;
    case UART_BD_57600:
        speed = B57600;
        break;
    default:
        fprintf(stderr, "uart: Invalid baudrate specified\n.");
        return -1;
    }

    if (fds[uart_device] >= 0)
        return 0;

    if (uart_device == MIKROBUS_1_UART)
        device_file = UART_1_DEVICE_FILE;
    else
        device_file = UART_2_DEVICE_FILE;

    if ((fds[uart_device] = open(device_file, O_RDWR)) < 0) {
        fprintf(stderr, "uart: Failed to open file descriptor.\n");
        return -1;
    }

    if (tcgetattr(fds[uart_device], &old_pts[uart_device]) < 0) {
        fprintf(stderr, "uart: Failed to get current parameters.\n");
        close(fds[uart_device]);
        fds[uart_device] = -1;
        return -1;
    }

    memcpy(&pts, &old_pts[uart_device], sizeof(pts));

    cfsetospeed (&pts, speed);
    cfsetispeed (&pts, speed);

    pts.c_cflag = (pts.c_cflag & ~CSIZE) | CS8;
    pts.c_cflag &= ~CSTOPB;
    pts.c_cflag &= ~CRTSCTS;
    pts.c_cflag |= (CLOCAL | CREAD);

    pts.c_lflag = 0;
    pts.c_oflag = 0;
    pts.c_iflag &= ~(IXON | IXOFF | IXANY);
    pts.c_cc[VMIN] = 1;
    pts.c_cc[VTIME] = 0;

    if (tcflush(fds[uart_device], TCIOFLUSH) < 0) {
        fprintf(stderr, "uart: Failed to flush buffer.\n");
        close(fds[uart_device]);
        fds[uart_device] = -1;
        return -1;
    }

    if (tcsetattr(fds[uart_device], TCSANOW, &pts) < 0) {
        fprintf(stderr, "uart: Failed to set parameters.\n");
        close(fds[uart_device]);
        fds[uart_device] = -1;
        return -1;
    }

    return 0;
}

int uart_select(const uint8_t uart_device)
{
    if (!check_uart_device(uart_device))
        return -1;

    current_uart_device = uart_device;

    return 0;
}

int uart_send(const uint8_t *buffer, const uint32_t count)
{
    uint32_t sent_cnt = 0;

    if (buffer == NULL) {
        fprintf(stderr, "uart: Cannot send data from null buffer.\n");
        return -1;
    }

    if (count == 0)
        return 0;

    if (fds[current_uart_device] < 0) {
        fprintf(stderr, "uart: device %d must be initialised before sending data.\n", current_uart_device);
        return -1;
    }

    while (sent_cnt < count) {
        int ret = write(fds[current_uart_device], &buffer[sent_cnt], count - sent_cnt);
        if (ret < 0) {
            fprintf(stderr, "uart: Failed to write.\n");
            return -1;
        }
        sent_cnt += ret;
    }

    return sent_cnt;
}

int uart_receive(uint8_t *buffer, const uint32_t count)
{
    uint32_t received_cnt = 0;

    if (buffer == NULL) {
        fprintf(stderr, "uart: Cannot store data to null buffer.\n");
        return -1;
    }

    if (count == 0)
        return 0;

    if (fds[current_uart_device] < 0) {
        fprintf(stderr, "uart: device %d must be initialised before receiving data.\n", current_uart_device);
        return -1;
    }

    while (received_cnt < count) {
        int ret = read(fds[current_uart_device], &buffer[received_cnt], count - received_cnt);
        if (ret < 0) {
            fprintf(stderr, "uart: Failed to read\n");
            return -1;
        }
        received_cnt += ret;
    }

    return received_cnt;
}

int uart_release(const uint8_t uart_device)
{
    if (!check_uart_device(uart_device))
        return -1;

    if (fds[uart_device] < 0)
        return 0;

    /* Flush buffers */
    if (tcflush(fds[uart_device], TCIOFLUSH) < 0) {
        fprintf(stderr, "uart: Failed to flush buffers.\n");
        return -1;
    }

    /* Restore old parameters */
    if (tcsetattr(fds[uart_device], TCSANOW, &old_pts[uart_device]) < 0) {
        fprintf(stderr, "uart: Failed to restore old parameters.\n");
        return -1;
    }

    close(fds[uart_device]);
    fds[uart_device] = -1;
    return 0;
}
