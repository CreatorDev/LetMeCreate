/**
 * @file opto.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 */


#ifndef __LETMECREATE_CLICK_OPTO_H__
#define __LETMECREATE_CLICK_OPTO_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/** Index of channels on Opto Click. Channel 2 is not supported on Ci40. */
enum OPTO_CLICK_CHANNEL_INDEX {
    OPTO_CLICK_CHANNEL_1,
    OPTO_CLICK_CHANNEL_2,
    OPTO_CLICK_CHANNEL_3,
    OPTO_CLICK_CHANNEL_4,
    OPTO_CLICK_CHANNEL_COUNT
};

/**
 * @brief Attach a callback to a channel of the Opto Click.
 *
 * Channel 2 is not supported on Ci40.
 * The callback is called on each edge, the parameter of the callback indicates
 * whether it is a raising or falling edge.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @param[in] channel_index Index of the channel (see #OPTO_CLICK_CHANNEL_INDEX)
 * @param[in] callback Function to call
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT opto_click_attach_callback(uint8_t mikrobus_index, uint8_t channel_index, void (*callback)(uint8_t));

/**
 * @brief Read channel state.
 *
 * Channel 2 is not supported on Ci40.
 *
 * @param[in] mikrobus_index Index of the mikrobus (see #MIKROBUS_INDEX)
 * @param[in] channel_index Index of the channel (see #OPTO_CLICK_CHANNEL_INDEX)
 * @param[out] state 0 if low, 1 if high
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT opto_click_read_channel(uint8_t mikrobus_index, uint8_t channel_index, uint8_t *state);

#ifdef __cplusplus
}
#endif

#endif
