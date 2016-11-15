#include <stdio.h>
#include <letmecreate/click/light.h>
#include <letmecreate/core/adc.h>

#define VREF            (2.048f)

int light_click_get_measure(uint8_t mikrobus_index, uint16_t *measure)
{
    float raw = 0.f;

    if (measure == NULL) {
        fprintf(stderr, "light: Cannot store measure using null pointer.\n");
        return -1;
    }

    if (adc_get_value(mikrobus_index, &raw) < 0)
        return -1;

    *measure = raw * (((float)UINT16_MAX) / VREF);

    return 0;
}
