/**
 * @file pwm.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_PWM_H__
#define __LETMECREATE_CORE_PWM_H__

#include <stdint.h>

/**
 * List all indices of PWM pins.
 */
enum PWM_PINS {
    MIKROBUS_1_PWM, /**< Index of PWM pin on Mikrobus 1 and Raspberry Pi interface */
    MIKROBUS_2_PWM  /**< Index of PWM pin on Mikrobus 2 and Raspberry Pi interface */
};

/**
 * @brief Initialise a PWM pin.
 *
 * @param[in] pwm_pin Index of the pin to initialise (see #PWM_PINS)
 * @return 0 if successful, -1 otherwise
 */
int pwm_init(const uint8_t pwm_pin);

/**
 * @brief Enable PWM on a pin.
 *
 * @param[in] pwm_pin Index of the pin (see #PWM_PINS)
 * @return 0 if successful, -1 otherwise
 */
int pwm_enable(const uint8_t pwm_pin);

/**
 * @brief Set the duty cycle.
 *
 * @param[in] pwm_pin Index of the pin (see #PWM_PINS)
 * @param[in] percentage Percentage of the period when pin is high (must be between 0 and 100)
 * @return 0 if successful, -1 otherwise
 */
int pwm_set_duty_cycle(const uint8_t pwm_pin, const float percentage);

/**
 * @brief Get the duty cycle.
 *
 * @param[in] pwm_pin Index of the pin (see #PWM_PINS)
 * @param[out] percentage Pointer to a floating point variable (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int pwm_get_duty_cycle(const uint8_t pwm_pin, float *percentage);

/**
 * @brief Set the frequency.
 *
 * @param[in] pwm_pin Index of the pin (see #PWM_PINS)
 * @param[in] frequency Frequency of the PWM output in Hertz
 * @return 0 if successful, -1 otherwise
 */
int pwm_set_frequency(const uint8_t pwm_pin, const uint32_t frequency);

/**
 * @brief Get the frequency.
 *
 * @param[in] pwm_pin Index of the pin (see #PWM_PINS)
 * @param[out] frequency Pointer to a floating point variable (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int pwm_get_frequency(const uint8_t pwm_pin, float *frequency);

/**
 * @brief Set the period.
 *
 * @param[in] pwm_pin Index of the pin (see #PWM_PINS)
 * @param[in] period Period of PWM output in nanoseconds
 * @return 0 if successful, -1 otherwise
 */
int pwm_set_period(const uint8_t pwm_pin, const uint32_t period);

/**
 * @brief Get the frequency.
 *
 * @param[in] pwm_pin Index of the pin (see #PWM_PINS)
 * @param[out] period Pointer to an integer (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int pwm_get_period(const uint8_t pwm_pin, uint32_t *period);

/**
 * @brief Disable PWM output on a pin.
 *
 * @param[in] pwm_pin Index of the pin (see #PWM_PINS)
 * @return 0 if successful, -1 otherwise
 */
int pwm_disable(const uint8_t pwm_pin);

/**
 * @brief Release a PWM pin.
 *
 * @param[in] pwm_pin Index of the pin to release (see #PWM_PINS)
 * @return 0 if successful, -1 otherwise
 */
int pwm_release(const uint8_t pwm_pin);

#endif
