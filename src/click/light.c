#include <stdio.h>
#include <letmecreate/click/light.h>
#include <letmecreate/core/adc.h>
#include <letmecreate/core/spi.h>

#define VREF            (2.048f)

int light_click_get_measure(uint8_t mikrobus_index, uint16_t *measure, uint8_t use_spi)
{
    if (measure == NULL) {
        fprintf(stderr, "light: Cannot store measure using null pointer.\n");
        return -1;
    }

    if (use_spi) {
        uint8_t buffer[2];
        if (spi_transfer(NULL, buffer, sizeof(buffer)) < 0)
            return -1;

        *measure = buffer[0] & 0x1F;        /* buffer[0] contains the most 5 significant bits */
        *measure <<= 7;
        *measure |= (buffer[1] >> 1);       /* buffer[1] contains the least 7 significant bits */
    } else {
        float raw = 0.f;
        if (adc_get_value(mikrobus_index, &raw) < 0)
            return -1;
        *measure = raw * (4096.f / VREF);
    }

    return 0;
}
