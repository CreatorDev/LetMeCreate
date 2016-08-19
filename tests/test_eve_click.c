/**
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#include <linux/spi/spidev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "letmecreate/core/spi.h"
#include "letmecreate/click/eve.h"


static bool test_eve_click_enable_disable(void)
{
    if (eve_click_enable(MIKROBUS_1) < 0
    ||  eve_click_disable(MIKROBUS_1) < 0)
        return false;

    if (eve_click_enable(MIKROBUS_1) < 0
    ||  eve_click_disable(MIKROBUS_1) < 0)
        return false;

    return true;
}

static bool test_eve_click_black_screen_on_enable(void)
{
    if (eve_click_enable(MIKROBUS_1) < 0)
        return false;

    return ask_question("Do you see a black screen ?", 15) == 1;
}

static bool test_eve_click_clear(void)
{
    if (eve_click_clear(0, 255, 0) < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see a green screen ?", 15) == 1;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(eve_click, 3)
    ADD_TEST_CASE(eve_click, enable_disable);
    ADD_TEST_CASE(eve_click, black_screen_on_enable);
    ADD_TEST_CASE(eve_click, clear);

    if (spi_init() < 0
    ||  spi_set_mode(MIKROBUS_1, SPI_MODE_0) < 0)
        return -1;

    ret = run_test(test_eve_click);
    free(test_eve_click.cases);

    spi_release();

    return ret;
}

