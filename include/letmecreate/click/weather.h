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

/**
 * @file weather.h
 * @author Rafal Baranowski, Bartlomiej Zarnowski
 * @date 2016
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_WEATHER_H__
#define __LETMECREATE_CLICK_WEATHER_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <letmecreate/click/export.h>

/**
 * @brief Read current values of temperature, pressure, humidity from sensor.
 *
 * @param[out] temperature measured by sensor, or NULL if not interested in this value.
 * @param[out] pressure measured by sensor, or NULL if not interested in this value.
 * @param[out] humidity measured by sensor, or NULL if not interested in this value.
 * @return 0 if success otherwise -1
 */
int LETMECREATE_CLICK_EXPORT weather_click_read_measurements(double* temperature, double* pressure, double* humidity);

/**
 * @brief Setup sensor configuration as recommended for indoor measurement in Bosh BME 280 data sheet.
 * @return 0 if success otherwise -1
 */
int LETMECREATE_CLICK_EXPORT weather_click_enable(void);

/**
 * @brief Power off sensor.
 * @return 0 if success otherwise -1
 */
int LETMECREATE_CLICK_EXPORT weather_click_disable(void);

#ifdef __cplusplus
}
#endif

#endif /* __LETMECREATE_CLICK_WEATHER_H__ */
