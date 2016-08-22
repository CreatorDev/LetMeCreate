#include <stdio.h>
#include "letmecreate/core/adc.h"
#include "letmecreate/core/common.h"

#define ADC_BASE_PATH       "/sys/bus/iio/devices/iio:device0/"

/*
 * The Ci40 contains several 10-bit ADC which can be accessed by reading
 * /sys/bus/iio/devices/iio:device0/in_voltage0_raw (for mikrobus 1)
 * or /sys/bus/iio/devices/iio:device0/in_voltage1_raw (for mikrobus 2).
 *
 * These two files are updated regularly and contain the value measured by the
 * ADC in range 0-1023. The pins can accept voltage up to 5V and so to convert
 * a measurement to an actual voltage, one needs to apply a linear scale.
 * 0 - 0V
 * 1023 - 5V
 *
 */
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
