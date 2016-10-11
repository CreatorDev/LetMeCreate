/**
 * @brief Implement I2C section of miscellaneous/testing_plan.
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include <letmecreate/core/common.h>
#include <letmecreate/core/i2c.h>

#define VCNL4010_ADDRESS            (0x13)
#define VCNL4010_PRODUCT_ID_REG     (0x81)
#define VCNL4010_PRODUCT_ID         (0x02)

static bool test_i2c_write_before_init(void)
{
    uint8_t buffer = 0;
    return i2c_write(0x12, &buffer, 1) == -1
        && i2c_write_byte(0x12, 0) == -1;
}

static bool test_i2c_read_before_init(void)
{
    uint8_t buffer = 0;
    return i2c_read(0x12, &buffer, 1) == -1
        && i2c_read_byte(0x12, &buffer) == -1;
}

static bool test_i2c_init(void)
{
    if (i2c_init() < 0)
        return false;

    if (i2c_init() < 0)
        return false;

    return i2c_get_current_bus() == MIKROBUS_1;
}

static bool test_i2c_select_invalid_bus(void)
{
    uint8_t current_bus = i2c_get_current_bus();
    i2c_select_bus(4);

    return current_bus == i2c_get_current_bus();
}

static bool test_i2c_write_null_buffer(void)
{
    return i2c_write(0x12, NULL, 1) == -1;
}

static bool test_i2c_read_null_buffer(void)
{
    return i2c_read(0x12, NULL, 1) == -1;
}

static bool test_i2c_write_zero_byte(void)
{
    const uint8_t buffer = 0;
    return i2c_write(0x12, &buffer, 0) == 0;
}

static bool test_i2c_read_zero_byte(void)
{
    uint8_t buffer = 0;
    return i2c_read(0x12, &buffer, 0) == 0;
}

static bool read_proximity_product_id(uint8_t mikrobus_index)
{
    int ret = -1;
    uint8_t product_ID = 0;

    i2c_select_bus(mikrobus_index);
    if (i2c_get_current_bus() != mikrobus_index)
        return false;

    ret = ask_question("Do you have a Proximity Click ?", 15);
    if (ret < 0)
        return false;
    else if (ret == 2)
        return true;

    printf("Insert your Proximity Click in mikrobus %u\n", mikrobus_index+1);
    if (wait_for_switch(10) < 0)
        return false;

    if (i2c_write_byte(VCNL4010_ADDRESS, VCNL4010_PRODUCT_ID_REG) < 0)
        return false;

    if (i2c_read_byte(VCNL4010_ADDRESS, &product_ID) < 0)
        return false;

    return (product_ID >> 4) == VCNL4010_PRODUCT_ID;
}

static bool test_i2c_read_id_mikrobus_1(void)
{
    return read_proximity_product_id(MIKROBUS_1);
}

static bool test_i2c_read_id_mikrobus_2(void)
{
    return read_proximity_product_id(MIKROBUS_2);
}

static bool test_i2c_release(void)
{
    return i2c_release() == 0
        && i2c_release() == 0;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(i2c, 11)
    ADD_TEST_CASE(i2c, write_before_init);
    ADD_TEST_CASE(i2c, read_before_init);
    ADD_TEST_CASE(i2c, init);
    ADD_TEST_CASE(i2c, select_invalid_bus);
    ADD_TEST_CASE(i2c, write_null_buffer);
    ADD_TEST_CASE(i2c, read_null_buffer);
    ADD_TEST_CASE(i2c, write_zero_byte);
    ADD_TEST_CASE(i2c, read_zero_byte);
    ADD_TEST_CASE(i2c, read_id_mikrobus_1);
    ADD_TEST_CASE(i2c, read_id_mikrobus_2);
    ADD_TEST_CASE(i2c, release);

    ret = run_test(test_i2c);
    free(test_i2c.cases);

    return ret;
}
