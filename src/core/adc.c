#include <stdio.h>
#include "letmecreate/core/adc.h"
#include "letmecreate/core/common.h"

#define ADC_BASE_PATH       "/sys/bus/iio/devices/iio:device0/"


int adc_get_value(uint8_t mikrobus_index, float *value)
{
    uint32_t tmp = 0;
    char path[MAX_STR_LENGTH];

    if (mikrobus_index != MIKROBUS_1 && mikrobus_index != MIKROBUS_2) {
        fprintf(stderr, "adc: Invalid index.\n");
        return -1;
    }

    if (value == NULL) {
        fprintf(stderr, "adc: Cannot store ADC value to null variable.\n");
        return -1;
    }

    if (snprintf(path, MAX_STR_LENGTH, ADC_BASE_PATH"in_voltage%d_raw", mikrobus_index) < 0) {
        fprintf(stderr, "adc: Failed to create path to access value of ADC %d.\n", mikrobus_index);
        return -1;
    }

    if (read_int_file(path, &tmp) < 0) {
        fprintf(stderr, "adc: Failed to read value from ADC %d.\n", mikrobus_index);
        return -1;
    }

    *value = 5.f * ((float)tmp) / 1023.f;

    return 0;
}
