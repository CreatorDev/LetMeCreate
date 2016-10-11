/**
 * @brief Implement UART section of miscellaneous/testing_plan.
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include <letmecreate/core/common.h>
#include <letmecreate/core/uart.h>

static bool test_uart_send_receive_without_init(void)
{
    uint8_t buffer = 0;

    return uart_send(&buffer, 1) == -1
        && uart_receive(&buffer, 1) == -1;
}

static bool test_uart_init(void)
{
    uint32_t baudrate;

    if (uart_init() < 0)
        return false;
    if (uart_init() < 0)
        return false;
    if (uart_get_current_bus() != MIKROBUS_1)
        return false;
    if (uart_get_baudrate(&baudrate) < 0)
        return false;

    return baudrate == UART_BD_9600;
}

static bool test_uart_send_null_buffer(void)
{
    return uart_send(NULL, 1) == -1;
}

static bool test_uart_send_zero_byte(void)
{
    uint8_t buffer = 0;
    return uart_send(&buffer, 0) == 0;
}

static bool test_uart_receive_null_buffer(void)
{
    return uart_receive(NULL, 1) == -1;
}

static bool test_uart_receive_zero_byte(void)
{
    uint8_t buffer = 0;
    return uart_receive(&buffer, 0) == 0;
}

static bool test_uart_set_invalid_baudrate(void)
{
    return uart_set_baudrate(0) == -1
        && uart_set_baudrate(115200) == -1;
}

static bool test_uart_release(void)
{
    return uart_release() == 0
        && uart_release() == 0;
}

static bool test_uart_select_invalid_bus(void)
{
    uint8_t current_bus = uart_get_current_bus();
    uart_select_bus(4);
    return uart_get_current_bus() == current_bus;
}

static bool test_uart_send_receive(void)
{
    uint8_t i;
    uint32_t baudrates[] = {
        UART_BD_1200,
        UART_BD_2400,
        UART_BD_4800,
        UART_BD_9600,
        UART_BD_19200,
        UART_BD_38400,
        UART_BD_57600
    };

    printf("Wire TX/RX of Mikrobus 1 to Mikrobus 2. Press a switch when ready.\n");
    if (wait_for_switch(60) < 0)
        return false;

    for (i = 0; i < 7; ++i) {
        uint32_t bd;
        const uint8_t tx_buffer = 'A';
        uint8_t rx_buffer = 0;

        if (uart_init() < 0)
            return false;

        /* Set baudrate of UART on MIKROBUS_1 */
        if (uart_set_baudrate(baudrates[i]) < 0)
            return false;
        if (uart_get_baudrate(&bd) < 0)
            return false;
        if (bd != baudrates[i])
            return false;

        /* Set baudrate of UART on MIKROBUS_2 */
        uart_select_bus(MIKROBUS_2);
        if (uart_set_baudrate(baudrates[i]) < 0)
            return false;
        if (uart_get_baudrate(&bd) < 0)
            return false;
        if (bd != baudrates[i])
            return false;

        uart_select_bus(MIKROBUS_1);
        if (uart_send(&tx_buffer, 1) < 0)
            return false;

        uart_select_bus(MIKROBUS_2);
        if (uart_get_current_bus() != MIKROBUS_2)
            return false;

        if (uart_receive(&rx_buffer, 1) < 0)
            return false;

        if (tx_buffer != rx_buffer)
            return false;

        rx_buffer = 0;
        if (uart_send(&tx_buffer, 1) < 0)
            return false;

        uart_select_bus(MIKROBUS_1);
        if (uart_get_current_bus() != MIKROBUS_1)
            return false;

        if (uart_receive(&rx_buffer, 1) < 0)
            return false;

        if (tx_buffer != rx_buffer)
            return false;

        if (uart_release() < 0)
            return false;
    }

    return true;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(uart, 10)
    ADD_TEST_CASE(uart, send_receive_without_init);
    ADD_TEST_CASE(uart, init);
    ADD_TEST_CASE(uart, send_null_buffer);
    ADD_TEST_CASE(uart, send_zero_byte);
    ADD_TEST_CASE(uart, receive_null_buffer);
    ADD_TEST_CASE(uart, receive_zero_byte);
    ADD_TEST_CASE(uart, set_invalid_baudrate);
    ADD_TEST_CASE(uart, release);
    ADD_TEST_CASE(uart, select_invalid_bus);
    ADD_TEST_CASE(uart, send_receive);

    ret = run_test(test_uart);
    free(test_uart.cases);

    return ret;
}
