#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "core/common.h"
#include "core/uart.h"

#define TEST_UART_CASE_CNT       (12)

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
    return uart_release() == 0;
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

    struct test test_uart = {
        .name = "uart",
        .case_cnt = TEST_UART_CASE_CNT,
        .cases = malloc(TEST_UART_CASE_CNT * sizeof(struct test_case))
    };

    /* 3.1 */
    struct test_case send_receive_without_init = { "send/receive without init", test_uart_send_receive_without_init };
    test_uart.cases[0] = send_receive_without_init;

    /* 3.2 */
    struct test_case init = { "init", test_uart_init };
    test_uart.cases[1] = init;

    /* 3.3 */
    struct test_case init_twice = { "init twice", test_uart_init };
    test_uart.cases[2] = init_twice;

    /* 3.4 */
    struct test_case send_null_buffer = { "send null buffer", test_uart_send_null_buffer };
    test_uart.cases[3] = send_null_buffer;

    /* 3.5 */
    struct test_case send_zero_byte = { "send zero byte", test_uart_send_zero_byte };
    test_uart.cases[4] = send_zero_byte;

    /* 3.6 */
    struct test_case receive_null_buffer = { "receive null buffer", test_uart_receive_null_buffer };
    test_uart.cases[5] = receive_null_buffer;

    /* 3.7 */
    struct test_case receive_zero_byte = { "receive zero byte", test_uart_receive_zero_byte };
    test_uart.cases[6] = receive_zero_byte;

    /* 3.8 */
    struct test_case set_invalid_baudrate = { "set invalid baudrate", test_uart_set_invalid_baudrate };
    test_uart.cases[7] = set_invalid_baudrate;

    /* 3.9 */
    struct test_case release = { "release", test_uart_release };
    test_uart.cases[8] = release;

    /* 3.10 */
    struct test_case release_twice = { "release twice", test_uart_release };
    test_uart.cases[9] = release_twice;

    /* 3.11 */
    struct test_case select_invalid_bus = { "select invalid bus", test_uart_select_invalid_bus };
    test_uart.cases[10] = select_invalid_bus;

    /* 3.12 */
    struct test_case send_receive = { "send/receive", test_uart_send_receive };
    test_uart.cases[11] = send_receive;

    ret = run_test(test_uart);
    free(test_uart.cases);

    return ret;
}
