#include "click/joystick.h"
#include "click/common.h"


#define JOYSTICK_ADDRESS (0x40)

#define X_REG (0x10)
#define Y_REG (0x11)

int joystick_click_get_x(int8_t * x)
{
    if(i2c_read_register(JOYSTICK_ADDRESS, X_REG, (uint8_t*)x) == -1)
        return -1;

    return 0;
}

int joystick_click_get_y(int8_t * y)
{
    if(i2c_read_register(JOYSTICK_ADDRESS, Y_REG, (uint8_t*)y) == -1)
        return -1;

    return 0;
}

int joystick_click_get_position(int8_t * x, int8_t * y)
{
    // We read to temp variables to make sure we do not write
    // to only one coordinate if read fails halfway through
    int8_t tempX, tempY;
    if(joystick_click_get_x(&tempX) == -1)
        return -1;

    if(joystick_click_get_y(&tempY) == -1)
        return -1;

    *x = tempX;
    *y = tempY;

    return 0;   
}

