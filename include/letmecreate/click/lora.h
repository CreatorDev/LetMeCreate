/**
 * @file lora.h
 * @author Francois Berder
 * @date 2017
 * @copyright 3-clause BSD
 */

#ifndef __LETMECREATE_CLICK_LORA_H__
#define __LETMECREATE_CLICK_LORA_H__

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

/**
 * @return Default Configuration with following values:
 *      frequency = 868000000
 *      spreading_factor = 12
 *      auto_freq_band = LORA_CLICK_AUTO_FREQ_BAND_125KHZ
 *      coding_rate = LORA_CLICK_CODING_RATE_4_8
 *      bandwidth = LORA_CLICK_BANDWIDTH_250KHZ
 *      power = 14
 *      bitrate = 5000
 *      freq_deviation = 5000
 *      preamble_length = 8
 *      enable_crc_header = true
 */
struct lora_click_config LETMECREATE_CLICK_EXPORT lora_click_get_default_configuration(void);

/**
 * @brief Initialize the Lora Click and configure it.
 *
 * @param[in] mikrobus_index
 * @param[in] config Configuration of the Lora Click
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT lora_click_init(uint8_t mikrobus_index, struct lora_click_config config);

/**
 * @brief Configure the Lora Click
 *
 * @param[in] config Configuration of the Lora Click
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT lora_click_configure(struct lora_click_config config);

/**
 * @brief Send bytes
 *
 * This call is blocking, it will not return until all bytes have been sent.
 *
 * @param[in] data Array of bytes to send (must not be null)
 * @param[in] count Number of bytes to send
 * @return number of bytes sent, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT lora_click_send(const uint8_t *data, uint32_t count);

/**
 * @brief Receive bytes
 *
 * This call is blocking, it will not return until @p count bytes have been received.
 *
 * @param[out] data
 * @param[in] count Number of bytes to receive
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT lora_click_receive(uint8_t *data, uint32_t count);

/**
 * @brief Write bytes to EEPROM
 *
 * The RN2483 chip has 256 bytes of EEPROM located in 0x300-0x3FF range.
 *
 * @param[in] start_address (must be in range 0x300, 0x3FF)
 * @param[in] data Array of bytes (must not be null)
 * @param[in] length Number of bytes to write (start_address + length must not exceed 0x3FF)
 * @return Number of bytes written to the EEPROM, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT lora_click_write_eeprom(uint32_t start_address, const uint8_t *data, uint32_t length);

/**
 * @brief Read bytes from EEPROM
 *
 * The RN2483 chip has 256 bytes of EEPROM located in 0x300-0x3FF range.
 *
 * @param[in] start_address (must be in range 0x300, 0x3FF)
 * @param[out] data Array of bytes (must not be null)
 * @param[in] length Number of bytes to read (start_address + length must not exceed 0x3FF)
 * @return Number of bytes read from the EEPROM, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT lora_click_read_eeprom(uint32_t start_address, uint8_t *data, uint32_t length);

/**
 * @brief Read EUI of the Lora Click
 *
 * Each RN2483 has a unique read-only EUI.
 *
 * @param[out] eui Array of bytes of length 8 (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT lora_click_get_eui(uint8_t *eui);

#endif
