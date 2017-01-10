#include <stdio.h>
#include <letmecreate/click/joystick.h>
#include <letmecreate/core/i2c.h>

#define JOYSTICK_ADDRESS (0x40 | add_bit)

#define X_REG       (0x10)
#define Y_REG       (0x11)

static uint8_t add_bit = 0;

void joystick_click_set_add_bit(uint8_t lsb)
{
    add_bit = !!lsb;
}

int joystick_click_get_x(int8_t *x)
{
    if (x == NULL) {
        fprintf(stderr, "joystick: Cannot store X coordinate using null pointer.\n");
        return -1;
    }

    if (i2c_read_register(JOYSTICK_ADDRESS, X_REG, (uint8_t*)x) == -1) {
        fprintf(stderr, "joystick: Failed to read X coordinate from device.\n");
        return -1;
    }

    return 0;
}

int joystick_click_get_y(int8_t *y)
{
    if (y == NULL) {
        fprintf(stderr, "joystick: Cannot store Y coordinate using null pointer.\n");
        return -1;
    }

    if (i2c_read_register(JOYSTICK_ADDRESS, Y_REG, (uint8_t*)y) == -1) {
        fprintf(stderr, "joystick: Failed to read Y coordinate from device.\n");
        return -1;
    }

    return 0;
}

int joystick_click_get_position(int8_t *x, int8_t *y)
{
    /* We read to temp variables to make sure we do not write
     * to only one coordinate if read fails halfway through.
     */
    int8_t tempX = 0, tempY = 0;

    if (x == NULL || y == NULL) {
        fprintf(stderr, "joystick: Cannot store coordinates using null pointer.\n");
        return -1;
    }

    if (joystick_click_get_x(&tempX) < 0
    ||  joystick_click_get_y(&tempY) < 0)
        return -1;

    *x = tempX;
    *y = tempY;

    return 0;
}
