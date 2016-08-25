#include <stdio.h>
#include "letmecreate/click/motion.h"
#include "letmecreate/core/common.h"
#include "letmecreate/core/gpio_monitor.h"

int motion_click_enable(uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
        if (!gpio_init(MIKROBUS_1_RST)
        &&  !gpio_set_direction(MIKROBUS_1_RST, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_1_RST, 1))
            return 0;
    case MIKROBUS_2:
        if (!gpio_init(MIKROBUS_2_RST)
        &&  !gpio_set_direction(MIKROBUS_2_RST, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_2_RST, 1))
            return 0;
    default:
        fprintf(stderr, "motion: Invalid mikrobus index.\n");
        return -1;
    }

    return -1;
}

int motion_click_attach_callback(uint8_t mikrobus_index, void(*callback)(uint8_t))
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
        if (!gpio_init(MIKROBUS_1_INT)
         && !gpio_set_direction(MIKROBUS_1_INT, GPIO_INPUT)
         && !gpio_monitor_init())
            return gpio_monitor_add_callback(MIKROBUS_1_INT, GPIO_EDGE, callback);
    case MIKROBUS_2:
        if (!gpio_init(MIKROBUS_2_INT)
         && !gpio_set_direction(MIKROBUS_2_INT, GPIO_INPUT)
         && !gpio_monitor_init())
            return gpio_monitor_add_callback(MIKROBUS_2_INT, GPIO_EDGE, callback);
    default:
        fprintf(stderr, "motion: Invalid mikrobus index.\n");
        return -1;
    }

    return -1;
}

int motion_click_disable(uint8_t mikrobus_index)
{
    switch (mikrobus_index) {
    case MIKROBUS_1:
        if (!gpio_init(MIKROBUS_1_RST)
        &&  !gpio_set_direction(MIKROBUS_1_RST, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_1_RST, 0))
            return 0;
    case MIKROBUS_2:
        if (!gpio_init(MIKROBUS_2_RST)
        &&  !gpio_set_direction(MIKROBUS_2_RST, GPIO_OUTPUT)
        &&  !gpio_set_value(MIKROBUS_2_RST, 0))
            return 0;
    default:
        fprintf(stderr, "motion: Invalid mikrobus index.\n");
        return -1;
    }

    return -1;
}

