#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "core/adc.h"

#define TEST_ADC_CASE_CNT       (8)

static bool check_adc_value(const uint8_t mikrobus_index, const float min, const float max)
{
    float value;
    if (adc_get_value(mikrobus_index, &value))
        return false;

    if (value < min || value > max)
        return false;

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

    return check_adc_value(MIKROBUS_1, 4.97f, 5.03f);
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

    return check_adc_value(MIKROBUS_2, 4.97f, 5.03f);
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

    struct test test_adc = {
        .name = "adc",
        .case_cnt = TEST_ADC_CASE_CNT,
        .cases = malloc(TEST_ADC_CASE_CNT * sizeof(struct test_case))
    };

    /* 2.1.1 */
    struct test_case mikrobus_1_gnd = { "Mikrobus 1 ADC - GND", test_adc_mikrobus_1_gnd };
    test_adc.cases[0] = mikrobus_1_gnd;

    /* 2.1.2 */
    struct test_case mikrobus_1_3v3 = { "Mikrobus 1 ADC - 3V3", test_adc_mikrobus_1_3v3 };
    test_adc.cases[1] = mikrobus_1_3v3;

    /* 2.1.3 */
    struct test_case mikrobus_1_5v = { "Mikrobus 1 ADC - 5V", test_adc_mikrobus_1_5v };
    test_adc.cases[2] = mikrobus_1_5v;

    /* 2.2.1 */
    struct test_case mikrobus_2_gnd = { "Mikrobus 2 ADC - GND", test_adc_mikrobus_2_gnd };
    test_adc.cases[3] = mikrobus_2_gnd;

    /* 2.2.2 */
    struct test_case mikrobus_2_3v3 = { "Mikrobus 2 ADC - 3V3", test_adc_mikrobus_2_3v3 };
    test_adc.cases[4] = mikrobus_2_3v3;

    /* 2.2.3 */
    struct test_case mikrobus_2_5v = { "Mikrobus 2 ADC - 5V", test_adc_mikrobus_2_5v };
    test_adc.cases[5] = mikrobus_2_5v;

    /* 2.3 */
    struct test_case invalid_mikrobus_index = { "invalid mikrobus index", test_adc_invalid_mikrobus_index };
    test_adc.cases[6] = invalid_mikrobus_index;

    /* 2.4 */
    struct test_case null_value = { "null value", test_adc_null_value };
    test_adc.cases[7] = null_value;

    ret = run_test(test_adc);
    free(test_adc.cases);

    return ret;
}
