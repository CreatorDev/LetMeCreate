#include <stdio.h>
#include "letmecreate/click/7seg.h"
#include "letmecreate/core/pwm.h"
#include "letmecreate/core/spi.h"


static const uint8_t characters[] = {
0x7E,   /* 0 */
0x0A,   /* 1 */
0xB6,   /* 2 */
0x9E,   /* 3 */
0xCA,   /* 4 */
0xDC,   /* 5 */
0xFC,   /* 6 */
0x0E,   /* 7 */
0xFE,   /* 8 */
0xDE,   /* 9 */
0xEE,   /* A */
0xF8,   /* B */
0x74,   /* C */
0xBA,   /* D */
0xF4,   /* E */
0xE4    /* F */
};

int seven_seg_click_display_decimal_number(uint8_t number)
{
    uint8_t buffer[2];

    if (number >= 100) {
        fprintf(stderr, "7seg: Out of range number.\n");
        return -1;
    }

    uint8_t dec = number / 10;
    buffer[1] = characters[dec];
    buffer[0] = characters[number - (dec * 10)];

    /* Turn off first 7seg display if number is a single digit number */
    if (dec == 0)
        buffer[1] = 0;

    return spi_transfer(buffer, NULL, sizeof(buffer));
}

int seven_seg_click_display_hex_number(uint8_t hex)
{
    uint8_t buffer[2];

    buffer[1] = characters[hex >> 4];
    buffer[0] = characters[hex & 0xF];
    return spi_transfer(buffer, NULL, sizeof(buffer));
}

int seven_seg_click_set_intensity(uint8_t mikrobus_index, float intensity)
{
    if (pwm_init(mikrobus_index) < 0
    ||  pwm_set_duty_cycle(mikrobus_index, intensity) < 0
    ||  pwm_enable(mikrobus_index) < 0)
        return -1;

    return 0;
}
