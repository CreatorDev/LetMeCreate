#include <stdio.h>
#include <letmecreate/click/adc.h>
#include <letmecreate/core/spi.h>

#define START_BIT       (0x04)
#define SINGLED_ENDED   (0x02)


int adc_click_get_raw_value(uint8_t channel, uint16_t *value)
{
    uint8_t tx_buffer[3], rx_buffer[3] = {};

    if (channel >= ADC_CLICK_CHANNEL_COUNT) {
        fprintf(stderr, "adc click: Invalid channel.\n");
        return -1;
    }

    if (value == NULL) {
        fprintf(stderr, "adc click: Cannot store value using null pointer.\n");
        return -1;
    }

    tx_buffer[0] = START_BIT | SINGLED_ENDED;
    tx_buffer[1] = channel << 6;
    tx_buffer[2] = 0;
    if (spi_transfer(tx_buffer, rx_buffer, 3) < 0)
        return -1;

    *value = (rx_buffer[1] << 8) | rx_buffer[2];
    *value &= 0xFFF;

    return 0;
}
