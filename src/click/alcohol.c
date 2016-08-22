#include <stdio.h>
#include "letmecreate/click/alcohol.h"
#include "letmecreate/core/common.h"
#include "letmecreate/core/adc.h"


int alcohol_click_get_measure(uint8_t mikrobus_index, uint16_t *measure)
{
    if (measure == NULL) {
        fprintf(stderr, "alcohol: Cannot store measurement in null variable.\n");
        return -1;
    }
    if (mikrobus_index != MIKROBUS_1
    &&  mikrobus_index != MIKROBUS_2) {
        fprintf(stderr, "alcohol: Invalid mikrobus index.\n");
        return -1;
    }

    return adc_get_value(mikrobus_index, measure);
}
