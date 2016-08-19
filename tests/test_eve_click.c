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

static bool test_eve_click_button(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_BUTTON, 100, 100, 100, 40, 26, 0, "Press") < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see a button ?", 15) == 1;
}

static bool test_eve_click_clock(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_CLOCK, 80, 60, 50, 0, 8, 15, 0, 0) < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see a clock ?", 15) == 1;
}

static bool test_eve_click_dial(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_DIAL, 80, 60, 24, 0, 0x5555) < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see a dial ?", 15) == 1;
}

static bool test_eve_click_gauge(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_GAUGE, 80, 60, 50, 0, 5, 4, 30, 100) < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see a gauge ?", 15) == 1;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(eve_click, 7)
    ADD_TEST_CASE(eve_click, enable_disable);
    ADD_TEST_CASE(eve_click, black_screen_on_enable);
    ADD_TEST_CASE(eve_click, clear);
    ADD_TEST_CASE(eve_click, button);
    ADD_TEST_CASE(eve_click, clock);
    ADD_TEST_CASE(eve_click, dial);
    ADD_TEST_CASE(eve_click, gauge);

    if (spi_init() < 0
    ||  spi_set_mode(MIKROBUS_1, SPI_MODE_0) < 0)
        return -1;

    ret = run_test(test_eve_click);
    free(test_eve_click.cases);

    eve_click_disable(MIKROBUS_1);
    spi_release();

    return ret;
}

