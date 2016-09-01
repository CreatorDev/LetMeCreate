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

#ifndef __LETMECREATE_CLICK_RELAY4_H__
#define __LETMECREATE_CLICK_RELAY4_H__

#include <stdbool.h>

/**
 * @brief Configure relay4 to be controlled over given pins on cI40 board (those pins should be connected to pins P0-P3
 * on relay4 board). Each pin will be set as GPIO_OUTPUT. Due to Ci40 pins layout it's proposed to use GPIO_83, GPIO_81, 
 * GPIO_80, GPIO_72 as a default values.
 *
 * @param relay0Pin pin which is connected to relay4 P0 pin
 * @param relay1Pin pin which is connected to relay4 P1 pin
 * @param relay2Pin pin which is connected to relay4 P2 pin
 * @param relay3Pin pin which is connected to relay4 P3 pin
 * @return 0 if success otherwise -1
 */
uint8_t relay4_enable(uint8_t relay0Pin, uint8_t relay1Pin, uint8_t relay2Pin, uint8_t relay3Pin);

/**
 * @brief Releases usage of GPIO pins reserved through call to relay4_enable or relay4_enable_defaults.
 *
 * @return 0 if success otherwise -1
 */
uint8_t relay4_disable();

/**
 * @brief Changes state of one of 4 relay switch. Input parameter is a index number not a GPIO pin ID.
 *
 * @param pinIndex Index of the relay which state will be set.
 * @param on Set to true if relay should be turned on.
 * @return 0 if success otherwise -1
 */
uint8_t relay4_set_state(uint8_t pinIndex, bool on);

/**
 * @brief Returns state of one of 4 relay switch. Input parameter is a index number not a GPIO pin ID.
 *
 * @param pinIndex Index of the relay which state will be returned.
 * @param[out] state Current state of switch.
 * @return 0 if success otherwise -1
 */
uint8_t relay4_get_state(uint8_t pinIndex, bool* state);

/**
 * @brief Changes state of one of 4 relay switch to opposite.
 *
 * @param pinIndex Index of the relay which state will be changed.
 * @param[out] newState State of switch after toggle operation.
 * @return 0 if success otherwise -1
 */
uint8_t relay4_toggle(uint8_t pinIndex, bool* newState);

#endif /* __LETMECREATE_CLICK_RELAY4_H__ */
