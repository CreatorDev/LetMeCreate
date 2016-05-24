/**
 * @file uart.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_UART_H__
#define __LETMECREATE_CORE_UART_H__

#include <stdint.h>

enum UART_DEVICE {
    MIKROBUS_1_UART,
    MIKROBUS_2_UART
};

enum UART_BAUDRATE {
    UART_BD_9600    = 9600,
    UART_BD_19200   = 19200,
    UART_BD_38400   = 38400,
    UART_BD_57600   = 57600
};

int uart_init(const uint8_t uart_device, const uint32_t baudrate);
int uart_select(const uint8_t uart_device);
int uart_send(const uint8_t *buffer, const uint32_t count);
int uart_receive(uint8_t *buffer, const uint32_t count);
int uart_release(const uint8_t uart_device);

#endif
