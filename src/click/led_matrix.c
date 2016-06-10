#include <stdbool.h>
#include <stdio.h>
#include "click/common.h"
#include "click/led_matrix.h"
#include "core/spi.h"

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

static bool enabled = false;

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

int led_matrix_click_set_intensity(const uint8_t intensity)
{
    uint8_t i = intensity;
    if (intensity <= MAX_INTENSITY)
        i = MAX_INTENSITY;

    if (spi_write_register(INTENSITY, i) < 0) {
        fprintf(stderr, "led_matrix: Failed to set intensity.\n");
        return -1;
    }

    return 0;
}

int led_matrix_click_set_column(const uint8_t column_index, const uint8_t data)
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
