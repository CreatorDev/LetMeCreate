#ifndef LORA_H
#define LORA_H

#include <stdbool.h>
#include <stdint.h>
#include <letmecreate/click/export.h>

/** Automatic frequency corrections supported by RN2483 */
enum LORA_CLICK_AUTO_FREQ_BAND {
    LORA_CLICK_AUTO_FREQ_BAND_250KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_125KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_62_5KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_31_3KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_15_6KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_7_8KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_3_9KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_200KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_100KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_50KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_25KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_12_5KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_6_3KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_3_1KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_166_7KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_83_3KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_41_7KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_20_8KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_10_4KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_5_2KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_2_6KHZ,
    LORA_CLICK_AUTO_FREQ_BAND_COUNT
};

/** Coding rates supported by RN2483 */
enum LORA_CLICK_CODING_RATE {
    LORA_CLICK_CODING_RATE_4_5,
    LORA_CLICK_CODING_RATE_4_6,
    LORA_CLICK_CODING_RATE_4_7,
    LORA_CLICK_CODING_RATE_4_8,
    LORA_CLICK_CODING_RATE_COUNT
};

/** Bandwiths supported by RN2483 */
enum LORA_CLICK_BANDWIDTH {
    LORA_CLICK_BANDWIDTH_125KHZ,
    LORA_CLICK_BANDWIDTH_250KHZ,
    LORA_CLICK_BANDWIDTH_500KHZ,
};

struct lora_click_config {
    uint32_t frequency;             /**< In Hz. Must be in range 433000000..434800000 or in range 863000000..870000000 */
    uint8_t spreading_factor;       /**< Must be in range 7..12 */
    enum LORA_CLICK_AUTO_FREQ_BAND auto_freq_band;      /**< See #LORA_CLICK_AUTO_FREQ_BAND */
    enum LORA_CLICK_CODING_RATE coding_rate;            /**< See #LORA_CLICK_CODING_RATE */
    enum LORA_CLICK_BANDWIDTH bandwidth;                /**< See #LORA_CLICK_BANDWIDTH */
    int8_t power;                   /**< Must be in range -3..15 */
    uint16_t bitrate;               /**< In b/s. Must be in range 0..65535 */
    uint16_t freq_deviation;        /**< In Hz. Must be in range 0..65535 */
    uint16_t preamble_length;       /**< In bytes. Must be in range 0..65535 */
    bool enable_crc_header;
};

struct lora_click_config LETMECREATE_CLICK_EXPORT lora_click_get_default_configuration(void);
int LETMECREATE_CLICK_EXPORT lora_click_enable(struct lora_click_config config);
int LETMECREATE_CLICK_EXPORT lora_click_send(uint8_t *data, uint32_t count);
int LETMECREATE_CLICK_EXPORT lora_click_receive(uint8_t *data, uint32_t count);
int LETMECREATE_CLICK_EXPORT lora_click_disable(void);

#endif
