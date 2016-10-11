/************************************************************************************************************************
 Copyright (c) 2016, Imagination Technologies Limited and/or its affiliated group companies.
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 following conditions are met:
     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the
        following disclaimer.
     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
        following disclaimer in the documentation and/or other materials provided with the distribution.
     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
        products derived from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <letmecreate/click/relay4.h>
#include <letmecreate/core/gpio.h>

#define RELAY4_PIN_COUNT        (4)

static uint8_t relay4_pins[RELAY4_PIN_COUNT] = {};

static bool check_valid_pin_index(uint8_t pin_index)
{
    return pin_index < RELAY4_PIN_COUNT && relay4_pins[pin_index] != 0;
}

int relay4_click_enable(uint8_t relay_0_pin, uint8_t relay_1_pin, uint8_t relay_2_pin, uint8_t relay_3_pin)
{
    uint8_t i;

    relay4_pins[0] = relay_0_pin;
    relay4_pins[1] = relay_1_pin;
    relay4_pins[2] = relay_2_pin;
    relay4_pins[3] = relay_3_pin;

    for (i = 0; i < RELAY4_PIN_COUNT; ++i) {
        if (gpio_init(relay4_pins[i]) < 0
        ||  gpio_set_direction(relay4_pins[i], GPIO_OUTPUT) < 0) {
            fprintf(stderr, "relay4: Failed to configure relay %u pin.\n", i);
            return -1;
        }
    }

    return 0;
}

int relay4_click_disable(void)
{
    int ret = 0;
    uint8_t i;

    for(i = 0; i < RELAY4_PIN_COUNT; ++i) {
        if (relay4_pins[i] > 0) {
            if (gpio_release(relay4_pins[i]) < 0) {
                fprintf(stderr, "relay4: Failed to release relay %u pin.\n", i);
                ret = -1;
            }
            relay4_pins[i] = 0;
        }
    }

    return ret;
}

int relay4_click_set_state(uint8_t pin_index, uint8_t state)
{
    if (!check_valid_pin_index(pin_index)) {
        fprintf(stderr, "relay4: Wrong pin index, or pin is not initialized through call to relay4_enable.\n");
        return -1;
    }

    return gpio_set_value(relay4_pins[pin_index], state);
}

int relay4_click_get_state(uint8_t pin_index, uint8_t* state)
{
    if (!check_valid_pin_index(pin_index)) {
        fprintf(stderr, "relay4: Wrong pin index, or pin is not initialized through call to relay4_enable.\n");
        return -1;
    }

    if (state == NULL) {
        fprintf(stderr, "relay4: Cannot store state using null pointer.\n");
        return -1;
    }

    return gpio_get_value(relay4_pins[pin_index], state);
}

int relay4_click_toggle(uint8_t pin_index)
{
    uint8_t state = 0;

    if (!check_valid_pin_index(pin_index)) {
        fprintf(stderr, "relay4: Wrong pin index, or pin is not initialized through call to relay4_enable.\n");
        return -1;
    }

    if (gpio_get_value(relay4_pins[pin_index], &state) < 0) {
        fprintf(stderr, "relay4: Failed to get gpio value.\n");
        return -1;
    }

    return gpio_set_value(relay4_pins[pin_index], !state);
}
