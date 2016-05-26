/**
 * @file pwm.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_PWM_H__
#define __LETMECREATE_CORE_PWM_H__

#include <stdint.h>

enum PWM_PINS {
    MIKROBUS_1_PWM,
    MIKROBUS_2_PWM
};

int pwm_init(const uint8_t pwm_pin);
int pwm_enable(const uint8_t pwm_pin);
int pwm_set_duty_cycle(const uint8_t pwm_pin, const float percentage);
int pwm_get_duty_cycle(const uint8_t pwm_pin, float *percentage);
int pwm_set_frequency(const uint8_t pwm_pin, const uint32_t frequency);
int pwm_set_period(const uint8_t pwm_pin, const uint32_t period);
int pwm_get_period(const uint8_t pwm_pin, uint32_t *period);
int pwm_get_frequency(const uint8_t pwm_pin, float *frequency);
int pwm_disable(const uint8_t pwm_pin);
int pwm_release(const uint8_t pwm_pin);

#endif
