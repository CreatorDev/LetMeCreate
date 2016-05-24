/**
 * @file uart.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_UART_H__
#define __LETMECREATE_CORE_UART_H__

#include <stdint.h>

/** Index of UART devices */
enum UART_DEVICE {
    MIKROBUS_1_UART, /**< UART available on Mikrobus 1 */
    MIKROBUS_2_UART /**< UART available on Mikrobus 2 and RPI interface */
};

/** UART baudrates  */
enum UART_BAUDRATE {
    UART_BD_9600    = 9600,
    UART_BD_19200   = 19200,
    UART_BD_38400   = 38400,
    UART_BD_57600   = 57600
};

/**
 * @brief Initialise a UART device.
 *
 * Open a device file and configure it.
 *
 * @param uart_device Index of the device (see #UART_DEVICE for valid indices)
 * @param baudrate Baudrate of the UART (only values from #UART_BAUDRATE are valid)
 * @return 0 if succesful, -1 otherwise
 */
int uart_init(const uint8_t uart_device, const uint32_t baudrate);

/**
 * @brief Select the current UART device.
 *
 * @param uart_device Index of the device (see #UART_DEVICE for valid indices)
 * @return 0 if succesful, -1 otherwise
 */
int uart_select(const uint8_t uart_device);

/**
 * @brief Send some data using current UART device.
 *
 * @param buffer Array of bytes
 * @param count Number of bytes to send
 * @return 0 if succesful, -1 otherwise
 */
int uart_send(const uint8_t *buffer, const uint32_t count);

/**
 * @brief Receive some data using current UART device.
 *
 * @param buffer Array of bytes
 * @param count Number of bytes to receive
 * @return 0 if succesful, -1 otherwise
 */
int uart_receive(uint8_t *buffer, const uint32_t count);

/**
 * @brief Release a UART device.
 *
 * Close device file and restore old parameters.
 *
 * @param uart_device Index of the device (see #UART_DEVICE for valid indices)
 * @return 0 if succesful, -1 otherwise
 */
int uart_release(const uint8_t uart_device);

#endif
