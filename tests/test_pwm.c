/**
 * @brief Implement PWM section of miscellaneous/testing_plan.
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "core/common.h"
#include "core/pwm.h"


static bool test_pwm_get_set_duty_cycle_before_init(void)
{
    float duty_cycle = 50.f;

    return pwm_set_duty_cycle(MIKROBUS_1, duty_cycle) == -1
        && pwm_get_duty_cycle(MIKROBUS_1, &duty_cycle) == -1;
}

static bool test_pwm_get_set_period_before_init(void)
{
    uint32_t period = 100;

    return pwm_get_period(MIKROBUS_1, &period) == -1
        && pwm_set_period(MIKROBUS_1, period) == -1;
}

static bool test_pwm_get_set_frequency_before_init(void)
{
    uint32_t frequency = 5000;

    return pwm_set_frequency(MIKROBUS_1, frequency) == -1
        && pwm_get_frequency(MIKROBUS_1, &frequency) == -1;
}

static bool test_pwm_init_invalid_index(void)
{
    return pwm_init(3) == -1;
}

static bool test_pwm_release_invalid_index(void)
{
    return pwm_release(3) == -1;
}

static bool test_pwm_init(void)
{
    float duty_cycle_1, duty_cycle_2;
    uint32_t freq_1, freq_2;

    if (pwm_init(MIKROBUS_1) == -1
    ||  pwm_init(MIKROBUS_1) == -1
    ||  pwm_init(MIKROBUS_2) == -1
    ||  pwm_init(MIKROBUS_1) == -1)
        return false;

    if (pwm_get_duty_cycle(MIKROBUS_1, &duty_cycle_1) == -1
    ||  pwm_get_duty_cycle(MIKROBUS_2, &duty_cycle_2) == -1)
        return false;

    if (fabs(duty_cycle_1 - 50.f) > 0.1f) {
        printf("duty_cycle on mikrobus 1 is %f%% instead of 50%%\n", duty_cycle_1);
        return false;
    }
    if (fabs(duty_cycle_2 - 50.f) > 0.1f) {
        printf("duty_cycle on mikrobus 2 is %f%% instead of 50%%\n", duty_cycle_2);
        return false;
    }

    if (pwm_get_frequency(MIKROBUS_1, &freq_1) == -1
    ||  pwm_get_frequency(MIKROBUS_2, &freq_2) == -1)
        return false;

    if (abs(freq_1 - 3000) > 1) {
        printf("frequency on mikrobus 1 is %uHz instead of 3kHz\n", freq_1);
        return false;
    }
    if (abs(freq_2 - 3000) > 1) {
        printf("frequency on mikrobus 2 is %uHz instead of 3kHz\n", freq_1);
        return false;
    }

    return true;
}

static bool test_pwm_get_duty_cycle_null_var(void)
{
    return pwm_get_duty_cycle(MIKROBUS_1, NULL) == -1;
}

static bool test_pwm_get_period_null_var(void)
{
    return pwm_get_period(MIKROBUS_1, NULL) == -1;
}

static bool test_pwm_get_frequency_null_var(void)
{
    return pwm_get_frequency(MIKROBUS_1, NULL) == -1;
}

static bool test_pwm_get_set_duty_cycle_invalid_index(void)
{
    float duty_cycle = 5000.f;

    return pwm_set_duty_cycle(3, duty_cycle) == -1
        && pwm_get_duty_cycle(3, &duty_cycle) == -1;
}

static bool test_pwm_get_set_period_invalid_index(void)
{
    uint32_t period = 100;
    return pwm_set_period(3, period) == -1
        && pwm_get_period(3, &period) == -1;
}

static bool test_pwm_get_set_frequency_invalid_index(void)
{
    uint32_t frequency = 5000;

    return pwm_set_frequency(3, frequency) == -1
        && pwm_get_frequency(3, &frequency) == -1;
}

static bool test_pwm_set_frequency(void)
{
    uint32_t frequency = 5000;
    uint32_t tmp;
    uint32_t period;

    if (pwm_set_frequency(MIKROBUS_1, frequency) == -1)
        return false;

    if (pwm_get_frequency(MIKROBUS_1, &tmp) == -1)
        return false;

    if (abs(tmp - frequency) > 1)
        return false;

    if (pwm_get_period(MIKROBUS_1, &period) == -1)
        return false;

    tmp = 1000000000/period;
    return fabs(tmp - frequency) < 1.f;
}

static bool test_pwm_release(void)
{
    return pwm_release(MIKROBUS_1) == 0
        && pwm_release(MIKROBUS_1) == 0
        && pwm_release(MIKROBUS_2) == 0
        && pwm_release(MIKROBUS_2) == 0;
}

static bool test_pwm_manual_check(void)
{
    int ret = ask_question("Do you have a frequency meter ?", 15);
    if (ret == 2)
        return true;
    if (ret == -1)
        return false;

    if (pwm_init(MIKROBUS_1) < 0)
        return false;

    if (pwm_enable(MIKROBUS_1) < 0)
        return false;

    printf("The PWM pin on mikrobus 1 has been configured.\n");
    printf("frequency: 3kHz\n");
    printf("duty cycle: 50%%\n");
    if (ask_question("Does it match what you measure ?", 60) != 1)
        return false;

    if (pwm_set_frequency(MIKROBUS_1, 10000) < 0)
        return false;

    printf("Frequency has been changed to 10kHz.\n");
    if (ask_question("Does it match what you measure ?", 60) != 1)
        return false;

    if (pwm_disable(MIKROBUS_1) < 0)
        return false;

    printf("PWM pin has been disabled.\n");
    if (ask_question("Is there still some output?", 60) != 2)
        return false;

    return pwm_release(MIKROBUS_1) == 0;
}

int main(void)
{
    int ret = -1;

    CREATE_TEST(pwm, 15)
    ADD_TEST_CASE(pwm, get_set_duty_cycle_before_init);
    ADD_TEST_CASE(pwm, get_set_period_before_init);
    ADD_TEST_CASE(pwm, get_set_frequency_before_init);
    ADD_TEST_CASE(pwm, init_invalid_index);
    ADD_TEST_CASE(pwm, release_invalid_index);
    ADD_TEST_CASE(pwm, init);
    ADD_TEST_CASE(pwm, get_duty_cycle_null_var);
    ADD_TEST_CASE(pwm, get_period_null_var);
    ADD_TEST_CASE(pwm, get_frequency_null_var);
    ADD_TEST_CASE(pwm, get_set_duty_cycle_invalid_index);
    ADD_TEST_CASE(pwm, get_set_period_invalid_index);
    ADD_TEST_CASE(pwm, get_set_frequency_invalid_index);
    ADD_TEST_CASE(pwm, set_frequency);
    ADD_TEST_CASE(pwm, release);
    ADD_TEST_CASE(pwm, manual_check);

    ret = run_test(test_pwm);
    free(test_pwm.cases);

    return ret;
}
