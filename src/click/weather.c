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

#include <letmecreate/click/common.h>
#include <letmecreate/letmecreate.h>
#include <letmecreate/bosch/bme280.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

static struct bme280_t bme280;
static uint8_t readResult = 0;

static s8 weather_click_i2c_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
    int iError = BME280_INIT_VALUE;
    uint8_t t;

    for(t = 0; t < cnt; t++) {
        uint8_t tmp = reg_addr + t;
        i2c_write(dev_addr, &tmp, 1);
        iError = i2c_read(dev_addr, &tmp, 1) >= 0 ? 0 : -1;
        *(reg_data + t) = tmp;
        if (iError < 0) {
            readResult = -1;
            break;
        }
    }

    return (s8)iError;
}

static s8 weather_click_i2c_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
    int iError = BME280_INIT_VALUE;
    uint8_t array[2];
    uint8_t t;

    for(t = 0; t < cnt; t++) {
        array[0] = reg_addr;
        array[1] = *(reg_data + t);
        iError = i2c_write(dev_addr, &array[0], 2) >= 0 ? 0 : -1;
        if (iError < 0) {
            readResult = -1;
            break;
        }
    }

    return (s8)iError;
}

int weather_click_read_measurements(double* temperature, double* pressure, double* humidity)
{
    readResult = 0;
    int32_t uncompTemp = BME280_INIT_VALUE;
    int32_t uncompPress = BME280_INIT_VALUE;
    int32_t uncompHumidity = BME280_INIT_VALUE;

    uint8_t operationResult = bme280_read_uncomp_pressure_temperature_humidity(&uncompPress, &uncompTemp, &uncompHumidity);

    if (temperature != NULL)
        *temperature = bme280_compensate_temperature_double(uncompTemp);

    if (pressure != NULL)
        *pressure = bme280_compensate_pressure_double(uncompPress);

    if (humidity != NULL)
        *humidity = bme280_compensate_humidity_double(uncompHumidity);

    return operationResult != 0 ? -1 : readResult;
}

int weather_click_enable(void)
{
    readResult = 0;

    bme280.bus_write = weather_click_i2c_bus_write;
    bme280.bus_read = weather_click_i2c_bus_read;
    bme280.dev_addr = BME280_I2C_ADDRESS1;
    bme280.delay_msec = sleep_ms;

    if (bme280_init(&bme280) != 0
        || bme280_set_power_mode(BME280_SLEEP_MODE) != 0
        || bme280_get_calib_param() != 0
        || bme280_set_standby_durn(BME280_STANDBY_TIME_1_MS)  != 0
        || bme280_set_oversamp_humidity(BME280_OVERSAMP_1X) != 0
        || bme280_set_oversamp_pressure(BME280_OVERSAMP_16X) != 0
        || bme280_set_oversamp_temperature(BME280_OVERSAMP_2X) != 0
        || bme280_set_filter(BME280_FILTER_COEFF_16) != 0
        || bme280_set_power_mode(BME280_NORMAL_MODE) != 0)
        return -1;

    return readResult;
}

int weather_click_disable(void)
{
    readResult = 0;

    if (bme280_set_power_mode(BME280_SLEEP_MODE) != 0)
        return -1;

    return readResult;
}
