#include <stdbool.h>
#include <stdio.h>
#include "letmecreate/click/common.h"
#include "letmecreate/click/led_matrix.h"
#include "letmecreate/core/spi.h"

/* Commands */
#define SHUTDOWN        (0x0C)
#define INTENSITY       (0x0A)
#define SCAN_LIMIT      (0x0B)
#define DECODE          (0x09)
#define COL(I)          (I+0x01)

#define MAX_INTENSITY   (0x0F)
#define ENABLE_ALL_COLS (0x07)
#define NO_DECODE       (0x00)
#define COL_CNT         (0x08)

#define MIN_DISPLAYED   (0x00)
#define MAX_DISPLAYED   (0x63)

static bool enabled = false;

static uint8_t display_numbers[10][COL_CNT/2] = {
    { 0x3C, 0x42, 0x42, 0x3C }, // 0
    { 0x00, 0x7E, 0x20, 0x10 }, // 1
    { 0x32, 0x4A, 0x46, 0x22 }, // 2
    { 0x7E, 0x5A, 0x5A, 0x42 }, // 3
    { 0x08, 0x7E, 0x28, 0x18 }, // 4
    { 0x5E, 0x52, 0x52, 0x72 }, // 5
    { 0x5E, 0x52, 0x52, 0x7E }, // 6
    { 0x7E, 0x40, 0x40, 0x60 }, // 7
    { 0x7E, 0x5A, 0x5A, 0x7E }, // 8
    { 0x7E, 0x52, 0x52, 0x72 }, // 9
};

int led_matrix_click_enable(void)
{
    if (enabled)
        return 0;

    if (spi_write_register(SHUTDOWN, 0x01) < 0) {
        fprintf(stderr, "led_matrix: Failed to leave shutdown mode.\n");
        return -1;
    }

    if (spi_write_register(INTENSITY, MAX_INTENSITY) < 0) {
        fprintf(stderr, "led_matrix: Failed to set intensity.\n");
        return -1;
    }

    if (spi_write_register(SCAN_LIMIT, ENABLE_ALL_COLS) < 0) {
        fprintf(stderr, "led_matrix: Failed to set scan limit.\n");
        return -1;
    }

    if (spi_write_register(DECODE, NO_DECODE) < 0) {
        fprintf(stderr, "led_matrix: Failed to set decode mode.\n");
        return -1;
    }

    enabled = true;

    return 0;
}

int led_matrix_click_set_intensity(uint8_t intensity)
{
    uint8_t i = intensity;
    if (intensity > MAX_INTENSITY)
        i = MAX_INTENSITY;

    if (enabled == false) {
        fprintf(stderr, "led_matrix: Cannot set intensity if disabled.\n");
        return -1;
    }

    if (spi_write_register(INTENSITY, i) < 0) {
        fprintf(stderr, "led_matrix: Failed to set intensity.\n");
        return -1;
    }

    return 0;
}

int led_matrix_click_set_column(uint8_t column_index, uint8_t data)
{
    if (column_index >= COL_CNT) {
        fprintf(stderr, "led_matrix: Invalid column index.\n");
        return -1;
    }

    if (enabled == false) {
        fprintf(stderr, "led_matrix: Cannot switch on/off leds while device is shutdown.\n");
        return -1;
    }

    return spi_write_register(COL(column_index), data);
}

int led_matrix_click_display_number(uint8_t number)
{
    int i;
    int digits[2];

    if(number < MIN_DISPLAYED || number > MAX_DISPLAYED) {
        fprintf(stderr, "led_matrix: Requested an out of bounds number for display");
        return -1;
    }

    digits[0] = number / 10;
    digits[1] = number % 10;

    uint8_t display[COL_CNT];
    for(i = 0; i < COL_CNT; i++)
            display[i] = 0;

    for(i = 0; i < COL_CNT/2; i++)
            display[i] = display_numbers[digits[1]][i];

    for(i = COL_CNT/2; i < COL_CNT; i++)
            display[i] = display_numbers[digits[0]][i - COL_CNT/2];

    if(led_matrix_click_set(display) < 0)
        return -1;

    return 0;

}

int led_matrix_click_set(const uint8_t *columns)
{
    uint8_t i;

    if (columns == NULL) {
        fprintf(stderr, "led_matrix: Cannot switch on/off leds using null pointer.\n");
        return -1;
    }

    if (enabled == false) {
        fprintf(stderr, "led_matrix: Cannot switch on/off leds while device is shutdown.\n");
        return -1;
    }

    for (i = 0; i < COL_CNT; ++i) {
        if (spi_write_register(COL(i), columns[i]) < 0) {
            fprintf(stderr, "led_matrix: Failed to switch on/off leds.\n");
            return -1;
        }
    }

    return 0;
}

int led_matrix_click_disable(void)
{
    if (enabled == false)
        return 0;

    if (spi_write_register(SHUTDOWN, 0x00) < 0) {
        fprintf(stderr, "led_matrix: Failed to shutdown device.\n");
        return -1;
    }

    enabled = false;

    return 0;
}
