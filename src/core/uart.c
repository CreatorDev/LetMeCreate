#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include "core/common.h"
#include "core/uart.h"

#define UART_1_DEVICE_FILE      "/dev/ttySC0"
#define UART_2_DEVICE_FILE      "/dev/ttySC1"

static int fds[2] = { -1, -1 };
static struct termios old_pts[2];
static uint8_t current_mikrobus_index = MIKROBUS_1;

static bool check_mikrobus_index(const uint8_t mikrobus_index)
{
    if (mikrobus_index == MIKROBUS_1)
        return true;
    if (mikrobus_index == MIKROBUS_2)
        return true;

    fprintf(stderr, "uart: Invalid mikrobus_index\n.");
    return false;
}

static int uart_init_bus(const uint8_t mikrobus_index)
{
    char *device_file = NULL;
    struct termios pts;

    if (!check_mikrobus_index(mikrobus_index))
        return -1;

    if (fds[mikrobus_index] >= 0)
        return 0;

    if (mikrobus_index == MIKROBUS_1)
        device_file = UART_1_DEVICE_FILE;
    else
        device_file = UART_2_DEVICE_FILE;

    if ((fds[mikrobus_index] = open(device_file, O_RDWR)) < 0) {
        fprintf(stderr, "uart: Failed to open file descriptor.\n");
        return -1;
    }

    if (tcgetattr(fds[mikrobus_index], &old_pts[mikrobus_index]) < 0) {
        fprintf(stderr, "uart: Failed to get current parameters.\n");
        close(fds[mikrobus_index]);
        fds[mikrobus_index] = -1;
        return -1;
    }

    memcpy(&pts, &old_pts[mikrobus_index], sizeof(pts));

    pts.c_cflag = (pts.c_cflag & ~CSIZE) | CS8;
    pts.c_cflag &= ~CSTOPB;
    pts.c_cflag &= ~CRTSCTS;
    pts.c_cflag |= (CLOCAL | CREAD);

    pts.c_lflag = 0;
    pts.c_oflag = 0;
    pts.c_iflag &= ~(IXON | IXOFF | IXANY);
    pts.c_cc[VMIN] = 1;
    pts.c_cc[VTIME] = 0;

    if (tcflush(fds[mikrobus_index], TCIOFLUSH) < 0) {
        fprintf(stderr, "uart: Failed to flush buffer.\n");
        close(fds[mikrobus_index]);
        fds[mikrobus_index] = -1;
        return -1;
    }

    if (tcsetattr(fds[mikrobus_index], TCSANOW, &pts) < 0) {
        fprintf(stderr, "uart: Failed to set parameters.\n");
        close(fds[mikrobus_index]);
        fds[mikrobus_index] = -1;
        return -1;
    }

    return 0;
}

static int uart_release_bus(const uint8_t mikrobus_index)
{
    if (!check_mikrobus_index(mikrobus_index))
        return -1;

    if (fds[mikrobus_index] < 0)
        return 0;

    /* Flush buffers */
    if (tcflush(fds[mikrobus_index], TCIOFLUSH) < 0) {
        fprintf(stderr, "uart: Failed to flush buffers.\n");
        return -1;
    }

    /* Restore old parameters */
    if (tcsetattr(fds[mikrobus_index], TCSANOW, &old_pts[mikrobus_index]) < 0) {
        fprintf(stderr, "uart: Failed to restore old parameters.\n");
        return -1;
    }

    close(fds[mikrobus_index]);
    fds[mikrobus_index] = -1;
    return 0;
}

int uart_init(void)
{
    if (uart_init_bus(MIKROBUS_1) < 0)
        return -1;
    uart_select_bus(MIKROBUS_1);
    if (uart_set_baudrate(UART_BD_9600) < 0)
        return -1;

    if (uart_init_bus(MIKROBUS_2) < 0)
        return -1;
    uart_select_bus(MIKROBUS_2);
    if (uart_set_baudrate(UART_BD_9600) < 0)
        return -1;
    uart_select_bus(MIKROBUS_1);

    return 0;
}

void uart_select_bus(const uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
    case MIKROBUS_2:
        current_mikrobus_index = mikrobus_index;
        break;
    }
}

int uart_set_baudrate(const uint32_t baudrate)
{
    struct termios pts;
    speed_t speed;

    if (fds[current_mikrobus_index] < 0) {
        fprintf(stderr, "uart: device %d must be initialised before sending data.\n", current_mikrobus_index);
        return -1;
    }

    if (tcgetattr(fds[current_mikrobus_index], &pts) < 0) {
        fprintf(stderr, "uart: Failed to get current parameters.\n");
        return -1;
    }

    switch(baudrate) {
    case UART_BD_1200:
        speed = B1200;
        break;
    case UART_BD_2400:
        speed = B2400;
        break;
    case UART_BD_4800:
        speed = B4800;
        break;
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
        fprintf(stderr, "uart: Invalid baudrate.\n");
        return -1;
    }

    cfsetospeed(&pts, speed);
    cfsetispeed(&pts, speed);

    if (tcsetattr(fds[current_mikrobus_index], TCSANOW, &pts) < 0) {
        fprintf(stderr, "uart: Failed to set baudrate.\n");
        return -1;
    }

    return 0;
}

int uart_get_baudrate(uint32_t *baudrate)
{
    struct termios pts;

    if (baudrate == NULL) {
        fprintf(stderr, "uart: Cannot set baudrate using null pointer.\n");
        return -1;
    }

    if (fds[current_mikrobus_index] < 0) {
        fprintf(stderr, "uart: device %d must be initialised before sending data.\n", current_mikrobus_index);
        return -1;
    }

    if (tcgetattr(fds[current_mikrobus_index], &pts) < 0) {
        fprintf(stderr, "uart: Failed to get current parameters.\n");
        return -1;
    }

    switch (cfgetispeed(&pts)) {
    case B1200:
        *baudrate = UART_BD_1200;
        break;
    case B2400:
        *baudrate = UART_BD_2400;
        break;
    case B4800:
        *baudrate = UART_BD_4800;
        break;
    case B9600:
        *baudrate = UART_BD_9600;
        break;
    case B19200:
        *baudrate = UART_BD_19200;
        break;
    case B38400:
        *baudrate = UART_BD_38400;
        break;
    case B57600:
        *baudrate = UART_BD_57600;
        break;
    default:
        fprintf(stderr, "uart: UART device use unknown baudrate.\n");
        return -1;
    }

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

    if (fds[current_mikrobus_index] < 0) {
        fprintf(stderr, "uart: device %d must be initialised before sending data.\n", current_mikrobus_index);
        return -1;
    }

    while (sent_cnt < count) {
        int ret = write(fds[current_mikrobus_index], &buffer[sent_cnt], count - sent_cnt);
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

    if (fds[current_mikrobus_index] < 0) {
        fprintf(stderr, "uart: device %d must be initialised before receiving data.\n", current_mikrobus_index);
        return -1;
    }

    while (received_cnt < count) {
        int ret = read(fds[current_mikrobus_index], &buffer[received_cnt], count - received_cnt);
        if (ret < 0) {
            fprintf(stderr, "uart: Failed to read\n");
            return -1;
        }
        received_cnt += ret;
    }

    return received_cnt;
}

int uart_release(void)
{
    if (uart_release_bus(MIKROBUS_1) < 0)
        return -1;

    return uart_release_bus(MIKROBUS_2);
}
