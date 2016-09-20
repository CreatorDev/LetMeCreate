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

static bool test_eve_click_memcpy(void)
{
    uint32_t crc;

    if (eve_click_memset(0, 0xFF, 8) < 0
    ||  eve_click_memset(0x100, 0xAA, 8) < 0
    ||  eve_click_memcpy(0, 0x100, 8) < 0
    ||  eve_click_memcrc(0, 8, &crc) < 0)
        return false;

    return crc == 0xABB622F0;
}

static bool test_eve_click_memzero(void)
{
    uint32_t crc;

    if (eve_click_memset(0, 0xFF, 8) < 0
    ||  eve_click_memzero(0, 8) < 0
    ||  eve_click_memcrc(0, 8, &crc) < 0)
        return false;

    return crc == 0x6522DF69;
}

static bool test_eve_click_ftdi_logo(void)
{
    if (eve_click_ftdi_logo() < 0)
        return false;

    return ask_question("Do you see the ftdi logo ?", 15) == 1;
}

static bool test_eve_click_snapshot(void)
{
    bool ret = true;
    uint8_t *data = NULL;
    FILE *file = NULL;

    /* Display a red screen */
    if (eve_click_clear(255, 0, 0) < 0
    ||  eve_click_display() < 0)
        return false;

    if (ask_question("Do you see a red screen ?", 15) != 1)
        return false;

    data = malloc(480*272*2);
    if (data == NULL)
        return false;
    if (eve_click_snapshot(0, data) < 0)
        ret = false;
    free(data);
    if (ret == false)
        return ret;

    /* Display a blue screen */
    if (eve_click_clear(0, 0, 255) < 0
    ||  eve_click_display() < 0)
        return false;
    if (ask_question("Do you see a blue screen ?", 15) != 1)
        return false;

    /* Display snapshot (should be a red screen) */
    if (eve_click_clear(255, 0, 0) < 0
    ||  eve_click_draw(FT800_BITMAP_SOURCE, 0) < 0
    ||  eve_click_draw(FT800_BITMAP_LAYOUT, FT800_ARGB4, 2*480, 272) < 0
    ||  eve_click_draw(FT800_BITMAP_SIZE, FT800_NEAREST, FT800_BORDER, FT800_BORDER, 480, 272) < 0
    ||  eve_click_draw(FT800_BEGIN, FT800_BITMAPS) < 0
    ||  eve_click_draw(FT800_VERTEX2II, 0, 0, 0, 0) < 0
    ||  eve_click_draw(FT800_END) < 0
    ||  eve_click_display() < 0)
        ret = false;
    return ask_question("Do you see a red screen ?", 15) == 1;
}

static bool test_eve_click_spinner(void)
{
    /* Display a black screen */
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_spinner(240, 136, 0, 0) < 0
    ||  eve_click_display() < 0)
        return false;

    if (ask_question("Do you see a animating spinner ?", 15) != 1)
        return false;

    if(eve_click_stop() < 0)
        return false;

    return ask_question("Did the spinner stop ?", 15) == 1;
}

static bool test_eve_click_load_image(void)
{
    FILE *image = NULL;
    uint8_t *data = NULL;
    uint32_t length = 0;

    image = fopen("data/image.jpg", "rb");
    if (image == NULL)
        return false;
    fseek(image, 0, SEEK_END);
    length = ftell(image);
    rewind(image);

    data = malloc(length);
    if (data == NULL) {
        fclose(image);
        return false;
    }

    if (fread(data, 1, length, image) != length) {
        fclose(image);
        return false;
    }
    fclose(image);

    if (eve_click_load_image(0, FT800_OPT_NODL, data, length) < 0)
        return false;

    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_BITMAP_SOURCE, 0) < 0
    ||  eve_click_draw(FT800_BITMAP_LAYOUT, FT800_RGB565, 128*2, 100) < 0
    ||  eve_click_draw(FT800_BITMAP_SIZE, FT800_NEAREST, FT800_BORDER, FT800_BORDER, 128, 100) < 0
    ||  eve_click_draw(FT800_BEGIN, FT800_BITMAPS) < 0
    ||  eve_click_draw(FT800_VERTEX2II, 100, 100, 0, 0) < 0
    ||  eve_click_display() < 0)
        return false;

    return ask_question("Do you see an image ?", 15) == 1;
}

static bool test_eve_click_get_matrix(void)
{
    int32_t a, b, c, d, e, f;

    if (eve_click_get_matrix(&a, &b, &c, &d, &e, &f) < 0)
        return false;

    return a == 65536 && b == 0 && c == 0
        && d == 0 && e == 65536 && f == 0;
}

static bool test_eve_click_translate_and_set_matrix(void)
{
    int32_t a, b, c, d, e, f;

    eve_click_disable_buffering();

    if (eve_click_translate(-100*65536, 100 *65536) < 0
    ||  eve_click_set_matrix() < 0
    ||  eve_click_get_matrix(&a, &b, &c, &d, &e, &f) < 0) {
        eve_click_enable_buffering();
        return false;
    }

    eve_click_enable_buffering();

    return a == 65536 && b == 0 && c == 6553600
        && d == 0 && e == 65536 && f == -6553600;
}

static bool test_eve_click_scale(void)
{
    int32_t a, b, c, d, e, f;

    eve_click_disable_buffering();

    if (eve_click_scale(2*65536, 2*65536) < 0
    ||  eve_click_set_matrix() < 0
    ||  eve_click_get_matrix(&a, &b, &c, &d, &e, &f) < 0) {
        eve_click_enable_buffering();
        return false;
    }

    eve_click_enable_buffering();

    return a == 32768 && b == 0 && c == 3276800
        && d == 0 && e == 32768 && f == -3276800;
}

static bool test_eve_click_rotate(void)
{
    int32_t a, b, c, d, e, f;

    eve_click_disable_buffering();

    if (eve_click_rotate(90 * 65536 / 360) < 0
    ||  eve_click_set_matrix() < 0
    ||  eve_click_get_matrix(&a, &b, &c, &d, &e, &f) < 0) {
        eve_click_enable_buffering();
        return false;
    }

    eve_click_enable_buffering();

    return a == 0 && b == 32767 && c == -3276700
        && d == -32767 && e == 0 && f == -3276700;
}

static bool test_eve_click_load_identity(void)
{
    int32_t a, b, c, d, e, f;

    eve_click_disable_buffering();

    if (eve_click_load_identity() < 0
    ||  eve_click_get_matrix(&a, &b, &c, &d, &e, &f) < 0) {
        eve_click_enable_buffering();
        return false;
    }

    eve_click_enable_buffering();

    return a == 65536 && b == 0 && c == 0
        && d == 0 && e == 65536 && f == 0;
}

static bool test_eve_click_inflate(void)
{
    uint32_t uncompressed_length = 0;
    FILE *image = NULL;
    uint8_t *data = NULL;
    uint32_t length = 0;

    image = fopen("data/image.bin", "rb");
    if (image == NULL)
        return false;
    fseek(image, 0, SEEK_END);
    length = ftell(image);
    rewind(image);

    data = malloc(length);
    if (data == NULL) {
        fclose(image);
        return false;
    }

    if (fread(data, 1, length, image) != length) {
        fclose(image);
        return false;
    }
    fclose(image);

    if (eve_click_inflate(0, data, length) < 0)
        return false;

    if (eve_click_get_ptr(&uncompressed_length) < 0)
        return false;

    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_BITMAP_SOURCE, 0) < 0
    ||  eve_click_draw(FT800_BITMAP_LAYOUT, FT800_RGB565, 128*2, 100) < 0
    ||  eve_click_draw(FT800_BITMAP_SIZE, FT800_NEAREST, FT800_BORDER, FT800_BORDER, 128, 100) < 0
    ||  eve_click_draw(FT800_BEGIN, FT800_BITMAPS) < 0
    ||  eve_click_draw(FT800_VERTEX2II, 100, 100, 0, 0) < 0
    ||  eve_click_display() < 0)
        return false;

    return ask_question("Do you see an image ?", 15) == 1;
}

static bool test_eve_click_coldstart(void)
{
    eve_click_disable_buffering();
    if (eve_click_clear(0, 0, 0) < 0
    ||  eve_click_draw(FT800_FGCOLOR, 0x00c040) < 0
    ||  eve_click_draw(FT800_GRADCOLOR, 0) < 0
    ||  eve_click_draw(FT800_BUTTON, 2, 32, 76, 56, 26, 0, "custom") < 0
    ||  eve_click_coldstart() < 0
    ||  eve_click_draw(FT800_BUTTON, 82, 32, 76, 56, 26, 0, "default") < 0
    ||  eve_click_display() < 0) {
        eve_click_enable_buffering();
        return false;
    }

    eve_click_enable_buffering();

    return ask_question("Do you see a green \"custom\" button and a blue \"default\" button ?", 15) == 1;
}

static bool test_eve_click_screensaver(void)
{
    bool ret = true;
    eve_click_disable_buffering();

    if (eve_click_screensaver() < 0
    ||  eve_click_clear(0,0,0) < 0
    ||  eve_click_draw(FT800_COLOR_RGB, 0, 255, 0) < 0
    ||  eve_click_draw(FT800_POINT_SIZE, 10 * 16) < 0
    ||  eve_click_draw(FT800_BEGIN, FT800_POINTS) < 0
    ||  eve_click_draw(FT800_MACRO, 0) < 0
    ||  eve_click_display() < 0)
        ret = false;

    if (ret)
        ret = ask_question("Do you see a green point moving on the sreen ?", 15) == 1;

    if (ret)
        ret = eve_click_stop() == 0;

    eve_click_enable_buffering();

    return ret;
}

static bool test_eve_click_backlight_intensity(void)
{
    bool ret = true;

    if (eve_click_set_backlight_intensity(128) < 0)
        ret = false;

    if (ret && (eve_click_clear(255, 255, 255) < 0
                ||  eve_click_display() < 0))
        ret = false;

    if (ret && ask_question("Do you see a white screen ?", 15) != 1)
        ret = false;

    if (ret && eve_click_set_backlight_intensity(64) < 0)
        return false;

    if (ret && ask_question("Did the backlight intensity decreased ?", 15) != 1)
        return false;

    if (ret && eve_click_set_backlight_intensity(1) < 0)
        return false;

    if (ret && ask_question("Did the backlight intensity decreased ?", 15) != 1)
        ret = false;

    /* Restore default intensity */
    eve_click_set_backlight_intensity(128);

    return ret;
}


static uint16_t detected_x, detected_y;
static void touch_callback(uint16_t x, uint16_t y)
{
    detected_x = x;
    detected_y = y;
}

static bool test_eve_click_touch(void)
{
    int timeout = 0;

    if (eve_click_calibrate() < 0)
        return false;

    eve_click_attach_touch_callback(touch_callback);

    printf("Please touch the top left corner of the screen.\n");

    detected_x = 0;
    detected_y = 0;

    while (detected_x == 0 && detected_y == 0 && timeout < 15000) {
        sleep_ms(50);
        timeout += 50;
    }
    if (timeout >= 15000)
        return false;

    eve_click_attach_touch_callback(NULL);

    return detected_x < 100 && detected_y < 100;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(eve_click, 35)
    ADD_TEST_CASE(eve_click, enable_disable);
    ADD_TEST_CASE(eve_click, black_screen_on_enable);
    ADD_TEST_CASE(eve_click, backlight_intensity);
    ADD_TEST_CASE(eve_click, touch);
    ADD_TEST_CASE(eve_click, inflate);
    ADD_TEST_CASE(eve_click, get_matrix);
    ADD_TEST_CASE(eve_click, translate_and_set_matrix);
    ADD_TEST_CASE(eve_click, scale);
    ADD_TEST_CASE(eve_click, rotate);
    ADD_TEST_CASE(eve_click, load_identity);
    ADD_TEST_CASE(eve_click, spinner);
    ADD_TEST_CASE(eve_click, load_image);
    ADD_TEST_CASE(eve_click, memset_and_memcrc);
    ADD_TEST_CASE(eve_click, memcpy);
    ADD_TEST_CASE(eve_click, memzero);
    ADD_TEST_CASE(eve_click, clear);
    ADD_TEST_CASE(eve_click, snapshot);
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
    ADD_TEST_CASE(eve_click, coldstart);
    ADD_TEST_CASE(eve_click, screensaver);
    ADD_TEST_CASE(eve_click, ftdi_logo);


    if (spi_init() < 0
    ||  spi_set_mode(MIKROBUS_1, SPI_MODE_0) < 0)
        return -1;

    ret = run_test(test_eve_click);
    free(test_eve_click.cases);

    eve_click_disable(MIKROBUS_1);
    spi_release();

    return ret;
}

