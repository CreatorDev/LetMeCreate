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

#include <stdlib.h>
#include <stdbool.h>
#include <letmecreate/letmecreate.h>
#include <stdio.h>

static uint8_t relay4_pins[4] = {0, 0, 0, 0};


uint8_t relay4_enable(uint8_t relay0Pin, uint8_t relay1Pin, uint8_t relay2Pin, uint8_t relay3Pin) {
    relay4_pins[0] = relay0Pin;
    relay4_pins[1] = relay1Pin;
    relay4_pins[2] = relay2Pin;
    relay4_pins[3] = relay3Pin;

    if (gpio_init(relay0Pin) < 0
    ||  gpio_init(relay1Pin) < 0
    ||  gpio_init(relay2Pin) < 0
    ||  gpio_init(relay3Pin) < 0
    ||  gpio_set_direction(relay0Pin, GPIO_OUTPUT) < 0
    ||  gpio_set_direction(relay1Pin, GPIO_OUTPUT) < 0
    ||  gpio_set_direction(relay2Pin, GPIO_OUTPUT) < 0
    ||  gpio_set_direction(relay3Pin, GPIO_OUTPUT) < 0) {
        fprintf(stderr, "relay4: Failed to configure pins\n");
        return -1;
    }

    return 0;
}

uint8_t relay4_disable() {
    uint8_t result = 0;
    int t;
    for(t = 0; t < 4; t++) {
        if (relay4_pins[t] > 0) {
            if (gpio_release(relay4_pins[t]) < 0) {
                result = -1;
            }
            relay4_pins[t] = 0;
        }
    }
    return result;
}

uint8_t relay4_set_state(uint8_t pinIndex, bool on) {
    if ((pinIndex > 3) || (relay4_pins[pinIndex] == 0)) {
        fprintf(stderr, "relay4: Wrong pin Index, or pin is not initialized through call to relay4_enable.\n");
        return -1;

    }
    return gpio_set_value(relay4_pins[pinIndex], on);
}

uint8_t relay4_get_state(uint8_t pinIndex, bool* state) {
    if ((pinIndex > 3) || (relay4_pins[pinIndex] == 0)) {
        fprintf(stderr, "relay4: Wrong pin Index, or pin is not initialized through call to relay4_enable.\n");
        return -1;

    }
    uint8_t intState;
    uint8_t result = gpio_get_value(relay4_pins[pinIndex], &intState);
    if (state != NULL) {
        *state = (bool)intState;
    }
    return result;
}

uint8_t relay4_toggle(uint8_t pinIndex, bool* newState) {
    if ((pinIndex > 3) || (relay4_pins[pinIndex] == 0)) {
        fprintf(stderr, "relay4: Wrong pin Index, or pin is not initialized through call to relay4_enable.\n");
        return -1;

    }
    uint8_t intState;
    if (gpio_get_value(relay4_pins[pinIndex], &intState) < 0) {
        fprintf(stderr, "relay4: Failed to get gpio value\n");
        return -1;
    }
    //note: new state is negation of current state
    bool state = intState == 0 ? true : false;
    if (newState != NULL) {
        *newState = state;
    }
    return gpio_set_value(relay4_pins[pinIndex], state);
}
