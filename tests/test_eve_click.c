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

static bool test_eve_click_keys(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_KEYS, 10, 10, 140, 30, 26, 0, "12345") < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see 12345 keys ?", 15) == 1;
}

static bool test_eve_click_number(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_NUMBER, 20, 60, 31, 0, 42) < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see the number 42 ?", 15) == 1;
}

static bool test_eve_click_progress(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_PROGRESS, 20, 50, 120, 12, 0, 50,
100) < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see a progress bar ?", 15) == 1;
}

static bool test_eve_click_scrollbar(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_SCROLLBAR, 20, 50, 120, 8, 0, 10, 40,
100) < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see a scrollbar ?", 15) == 1;
}

static bool test_eve_click_slider(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_SLIDER, 20, 50, 120, 8, 0, 50, 100) < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see a slider ?", 15) == 1;
}

static bool test_eve_click_text(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_TEXT, 0, 0, 31, 0, "Text!") < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see some text ?", 15) == 1;
}

static bool test_eve_click_toggle(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_TOGGLE, 60, 20, 33, 27, 0, 0, "no"
"\xff" "yes") < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see a toggle ?", 15) == 1;
}

static bool test_eve_click_bgcolor_and_fgcolor(void)
{
    bool ret = true;

    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_BGCOLOR, 0x402000) < 0
    ||  eve_click_draw(FT800_FGCOLOR, 0x387000) < 0
    ||  eve_click_draw(FT800_TOGGLE, 60, 20, 33, 27, 0, 0, "no"
"\xff" "yes") < 0
    ||  eve_click_display() < 0)
        ret = false;

    if (ret)
        ret = ask_question("Do you see a toggle with brown background color and green foreground color ?", 15) == 1;

    /* Restore default values for FGCOLOR and BGCOLOR */
    if (ret) {
        if (eve_click_clear(0, 0, 0) < 0
        ||  eve_click_draw(FT800_BGCOLOR, 0x002040) < 0
        ||  eve_click_draw(FT800_FGCOLOR, 0x003870) < 0
        ||  eve_click_display() < 0)
            ret = false;
    }

    return ret;
}

static bool test_eve_click_gradcolor(void)
{
    bool ret = true;

    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_FGCOLOR, 0x101010) < 0
    ||  eve_click_draw(FT800_GRADCOLOR, 0xFF0000) < 0
    ||  eve_click_draw(FT800_BUTTON, 82, 2, 76, 56, 31, 0, "R") < 0
    ||  eve_click_display() < 0)
        ret = false;

    if (ret)
        ret = ask_question("Do you a button with some red ?", 15) == 1;


    /* Restore default values for FGCOLOR and GRADCOLOR */
    if (ret) {
        if (eve_click_clear(0, 0, 0) < 0
        ||  eve_click_draw(FT800_FGCOLOR, 0x003870) < 0
        ||  eve_click_draw(FT800_GRADCOLOR, 0xFFFFFF) < 0
        ||  eve_click_display() < 0)
            ret = false;
    }

    return ret;
}

static bool test_eve_click_gradient(void)
{
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_GRADIENT, 0, 0, 0x0000ff, 160, 0, 0xff0000) < 0
    ||  eve_click_display() < 0)
        return  false;

    return ask_question("Do you see a blue to red gradient ?", 15) == 1;
}

static bool test_eve_click_disable_buffering(void)
{
    bool ret = true;
    eve_click_disable_buffering();
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_BUTTON, 50, 50, 100, 40, 26, 0, "Press") < 0
    ||  eve_click_draw(FT800_BUTTON, 100, 100, 100, 40, 26, 0, "Press") < 0
    ||  eve_click_draw(FT800_BUTTON, 200, 200, 100, 40, 26, 0, "Press") < 0
    ||  eve_click_display() < 0)
        ret = false;

    eve_click_enable_buffering();

    if (ret)
        ret = ask_question("Do you three buttons ?", 15) == 1;

    return ret;
}

static bool test_eve_click_memset_and_memcrc(void)
{
    uint32_t crc;

    if (eve_click_memset(0, 0xAA, 8) < 0)
        return false;

    if (eve_click_memcrc(0, 8, &crc) < 0)
        return false;

    return crc == 0xABB622F0;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(eve_click, 19)
    ADD_TEST_CASE(eve_click, enable_disable);
    ADD_TEST_CASE(eve_click, black_screen_on_enable);
    ADD_TEST_CASE(eve_click, memset_and_memcrc);
    ADD_TEST_CASE(eve_click, clear);
    ADD_TEST_CASE(eve_click, button);
    ADD_TEST_CASE(eve_click, clock);
    ADD_TEST_CASE(eve_click, dial);
    ADD_TEST_CASE(eve_click, gauge);
    ADD_TEST_CASE(eve_click, keys);
    ADD_TEST_CASE(eve_click, number);
    ADD_TEST_CASE(eve_click, progress);
    ADD_TEST_CASE(eve_click, scrollbar);
    ADD_TEST_CASE(eve_click, slider);
    ADD_TEST_CASE(eve_click, text);
    ADD_TEST_CASE(eve_click, toggle);
    ADD_TEST_CASE(eve_click, bgcolor_and_fgcolor);
    ADD_TEST_CASE(eve_click, gradcolor);
    ADD_TEST_CASE(eve_click, gradient);
    ADD_TEST_CASE(eve_click, disable_buffering);

    if (spi_init() < 0
    ||  spi_set_mode(MIKROBUS_1, SPI_MODE_0) < 0)
        return -1;

    ret = run_test(test_eve_click);
    free(test_eve_click.cases);

    eve_click_disable(MIKROBUS_1);
    spi_release();

    return ret;
}

