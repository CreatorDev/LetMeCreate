/**
 * @brief Implement SPI section of miscellaneous/testing_plan.
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#include <linux/spi/spidev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "letmecreate/core/common.h"
#include "letmecreate/core/spi.h"

#define ADXL345_DEVICE_ID_REG           (0x00)
#define ADXL345_DEVICE_ID               (0xE5)

static bool test_spi_set_mode_before_init(void)
{
    return spi_set_mode(MIKROBUS_1, SPI_MODE_0) == -1
        && spi_set_mode(MIKROBUS_2, SPI_MODE_0) == -1;
}

static bool test_spi_set_speed_before_init(void)
{
    return spi_set_mode(MIKROBUS_1, 1000000) == -1
        && spi_set_mode(MIKROBUS_2, 1000000) == -1;
}

static bool test_spi_transfer_before_init(void)
{
    uint8_t tx_buffer = 0, rx_buffer = 0;

    return spi_transfer(&tx_buffer, &rx_buffer, 1) == -1;
}

static bool test_spi_init(void)
{
    if (spi_init() < 0)
        return false;

    if (spi_init() < 0)
        return false;

    return spi_get_current_bus() == MIKROBUS_1;
}

static bool test_spi_transfer_zero_byte(void)
{
    return spi_transfer(NULL, NULL, 0) == 0;
}

static bool test_spi_transfer_null_buffers(void)
{
    return spi_transfer(NULL, NULL, 1) == -1;
}

static bool read_accel_product_id(uint8_t mikrobus_index)
{
    int ret = -1;
    uint8_t tx_buffer[2], rx_buffer[2];

    spi_select_bus(mikrobus_index);
    if (spi_get_current_bus() != mikrobus_index)
        return false;

    ret = ask_question("Do you have an Accel Click (SPI) ?", 15);
    if (ret < 0)
        return false;
    else if (ret == 2)
        return true;

    printf("Insert your Accel Click in mikrobus %u\n", mikrobus_index+1);
    printf("Press a switch when ready.\n");
    if (wait_for_switch(10) < 0)
        return false;

    tx_buffer[0] = 0x80 | ADXL345_DEVICE_ID_REG;
    tx_buffer[1] = 0;
    if (spi_transfer(tx_buffer, rx_buffer, 2) < 0)
        return false;

    return rx_buffer[1] == ADXL345_DEVICE_ID;
}

static bool test_spi_read_id_mikrobus_1(void)
{
    return read_accel_product_id(MIKROBUS_1);
}

static bool test_spi_read_id_mikrobus_2(void)
{
    return read_accel_product_id(MIKROBUS_2);
}

static bool test_spi_release(void)
{
    return spi_release() == 0
        && spi_release() == 0;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(spi, 9);
    ADD_TEST_CASE(spi, set_mode_before_init);
    ADD_TEST_CASE(spi, set_speed_before_init);
    ADD_TEST_CASE(spi, transfer_before_init);
    ADD_TEST_CASE(spi, init);
    ADD_TEST_CASE(spi, transfer_zero_byte);
    ADD_TEST_CASE(spi, transfer_null_buffers);
    ADD_TEST_CASE(spi, read_id_mikrobus_1);
    ADD_TEST_CASE(spi, read_id_mikrobus_2);
    ADD_TEST_CASE(spi, release);

    ret = run_test(test_spi);
    free(test_spi.cases);

    return ret;
}
