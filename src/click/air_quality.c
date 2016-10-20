#include <stdio.h>
#include <letmecreate/click/air_quality.h>
#include <letmecreate/core/adc.h>


int air_quality_click_get_measure(uint8_t mikrobus_index, uint16_t *measure)
{
    float tmp = 0.f;

    if (measure == NULL) {
        fprintf(stderr, "air quality: Cannot store measure using null pointer.\n");
        return -1;
    }

    if (adc_get_value(mikrobus_index, &tmp) < 0)
        return -1;

    *measure = (tmp / 5.f) * 65535;

    return 0;
}
