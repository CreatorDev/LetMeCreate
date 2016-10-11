#include <stddef.h>
#include <letmecreate/click/bargraph.h>
#include <letmecreate/core/pwm.h>
#include <letmecreate/core/spi.h>


int bargraph_click_set_value(uint16_t value)
{
    uint8_t buffer[2];
    buffer[0] = (value >> 8) & 0x3;
    buffer[1] = value;
    return spi_transfer(buffer, NULL, sizeof(buffer));
}

int bargraph_click_set_intensity(uint8_t mikrobus_index, float intensity)
{
    if (pwm_init(mikrobus_index) < 0
    ||  pwm_set_duty_cycle(mikrobus_index, intensity) < 0
    ||  pwm_enable(mikrobus_index) < 0)
        return -1;

    return 0;
}
