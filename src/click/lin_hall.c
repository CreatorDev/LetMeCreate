#include <stdio.h>
#include <letmecreate/core/spi.h>
#include <letmecreate/click/lin_hall.h>


int lin_hall_click_get_measure(uint16_t *measure)
{
    uint8_t buffer[2];

    if (measure == NULL) {
        fprintf(stderr, "lin_hall: Cannot store measure using null pointer.\n");
        return -1;
    }

    if (spi_transfer(NULL, buffer, sizeof(buffer)) < 0)
        return -1;

    *measure = buffer[0] & 0x1F;        /* buffer[0] contains the most 5 significant bits */
    *measure <<= 7;
    *measure |= (buffer[1] >> 1);       /* buffer[1] contains the least 7 significant bits */

    return 0;
}
