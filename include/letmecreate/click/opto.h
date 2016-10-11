/**
 * @file opto.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 */


#ifndef __LETMECREATE_CLICK_OPTO_H__
#define __LETMECREATE_CLICK_OPTO_H__

#include <stdint.h>
#include <letmecreate/click/export.h>

/** Index of channels on Opto Click. Channel 2 is not supported on Ci40. */
enum OPTO_CHANNEL_INDEX {
    OPTO_CHANNEL_1,
    OPTO_CHANNEL_2,
    OPTO_CHANNEL_3,
    OPTO_CHANNEL_4,
};

/**
 * @brief Initialise GPIO's to interact with Opto Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT opto_click_init(uint8_t mikrobus_index);

/**
 * @brief Enable one channel of the Opto Click.
 *
 * Notice that channel 2 is not supported on Ci40.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @param[in] channel_index Index of the channel (see #OPTO_CHANNEL_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT opto_click_enable(uint8_t mikrobus_index, uint8_t channel_index);

/**
 * @brief Disable one channel of the Opto Click.
 *
 * Notice that channel 2 is not supported on Ci40.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @param[in] channel_index Index of the channel (see #OPTO_CHANNEL_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT opto_click_disable(uint8_t mikrobus_index, uint8_t channel_index);

/**
 * @brief Release GPIO's used to interact with the Opto Click.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT opto_click_release(uint8_t mikrobus_index);

#endif
