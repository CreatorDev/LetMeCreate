/**
 * @brief Implement ADC section of miscellaneous/testing_plan.
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include <letmecreate/core/adc.h>

static bool check_adc_value(const uint8_t mikrobus_index, const float min, const float max)
{
    float value;
    if (adc_get_value(mikrobus_index, &value))
        return false;

    if (value < min || value > max) {
        printf("Measure %fV out of range [%f,%f].\n", value, min, max);
        return false;
    }

    return true;
}

static bool test_adc_mikrobus_1_gnd(void)
{
    printf("Wire ADC from Mikrobus 1 to GND. Press a switch when ready.\n");

    if (wait_for_switch(10) < 0)
        return false;

    return check_adc_value(MIKROBUS_1, 0.f, 0.002f);
}

static bool test_adc_mikrobus_1_3v3(void)
{
    printf("Wire ADC from Mikrobus 1 to 3V3. Press a switch when ready.\n");

    if (wait_for_switch(10) < 0)
        return false;

    return check_adc_value(MIKROBUS_1, 3.27f, 3.33f);
}

static bool test_adc_mikrobus_1_5v(void)
{
    printf("Wire ADC from Mikrobus 1 to 5V. Press a switch when ready.\n");

    if (wait_for_switch(10) < 0)
        return false;

    return check_adc_value(MIKROBUS_1, 4.9f, 5.1f);
}

static bool test_adc_mikrobus_2_gnd(void)
{
    printf("Wire ADC from Mikrobus 2 to GND. Press a switch when ready.\n");

    if (wait_for_switch(10) < 0)
        return false;

    return check_adc_value(MIKROBUS_2, 0.f, 0.002f);
}

static bool test_adc_mikrobus_2_3v3(void)
{
    printf("Wire ADC from Mikrobus 2 to 3V3. Press a switch when ready.\n");

    if (wait_for_switch(10) < 0)
        return false;

    return check_adc_value(MIKROBUS_2, 3.27f, 3.33f);
}

static bool test_adc_mikrobus_2_5v(void)
{
    printf("Wire ADC from Mikrobus 2 to 5V. Press a switch when ready.\n");

    if (wait_for_switch(10) < 0)
        return false;

    return check_adc_value(MIKROBUS_2, 4.9f, 5.1f);
}

static bool test_adc_invalid_mikrobus_index(void)
{
    float value;
    return adc_get_value(2, &value) == -1;
}

static bool test_adc_null_value(void)
{
    return adc_get_value(MIKROBUS_1, NULL) == -1;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(adc, 8)
    ADD_TEST_CASE(adc, mikrobus_1_gnd);
    ADD_TEST_CASE(adc, mikrobus_1_3v3);
    ADD_TEST_CASE(adc, mikrobus_1_5v);
    ADD_TEST_CASE(adc, mikrobus_2_gnd);
    ADD_TEST_CASE(adc, mikrobus_2_3v3);
    ADD_TEST_CASE(adc, mikrobus_2_5v);
    ADD_TEST_CASE(adc, invalid_mikrobus_index);
    ADD_TEST_CASE(adc, null_value);

    ret = run_test(test_adc);
    free(test_adc.cases);

    return ret;
}
