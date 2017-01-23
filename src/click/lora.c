#include <stdio.h>
#include <string.h>
#include <letmecreate/click/lora.h>
#include <letmecreate/core/uart.h>

#define MAX_CHUNK_LENGTH        (255)
#define BUFFER_LENGTH           (MAX_CHUNK_LENGTH + 12)
#define MAX_LINE_LENGTH         (64)

#define MIN_LOW_BAND            (433000000)
#define MAX_LOW_BAND            (434800000)
#define MIN_HIGH_BAND           (863000000)
#define MAX_HIGH_BAND           (870000000)
#define MIN_SPREADING_FACTOR    (7)
#define MAX_SPREADING_FACTOR    (12)
#define MIN_POWER               (-3)
#define MAX_POWER               (15)

#define LOG_DEBUG(FMT,...)                  \
    do {                                    \
        printf("[%s:%d]: "FMT,              \
            __FILE__,                       \
            __LINE__,                       \
            ##__VA_ARGS__);                 \
    }while(0)

static const char* auto_freq_str[LORA_CLICK_AUTO_FREQ_BAND_COUNT] = {
    "250",
    "125",
    "62.5",
    "31.3",
    "15.6",
    "7.8",
    "3.9",
    "200",
    "100",
    "50",
    "25",
    "12.5",
    "6.3",
    "3.1",
    "166.7",
    "83.3",
    "41.7",
    "20.8",
    "10.4",
    "5.2",
    "2.6"
};

static const char* coding_rate_str[LORA_CLICK_CODING_RATE_COUNT] = {
    "4/5",
    "4/6",
    "4/7",
    "4/8"
};

static bool isValidFrequency(uint32_t freq)
{
    return (freq >= MIN_LOW_BAND && freq <= MAX_LOW_BAND)
        || (freq >= MIN_HIGH_BAND && freq <= MAX_HIGH_BAND);
}

static bool isValidSpreadingFactor(uint8_t spreading_factor)
{
    return spreading_factor >= MIN_SPREADING_FACTOR
        && spreading_factor <= MAX_SPREADING_FACTOR;
}

static bool isValidPower(int8_t power)
{
    return power >= MIN_POWER && power <= MAX_POWER;
}

/**
 * @brief Receive a string terminated by \r\n
 *
 * @param[in,out] line (not null and at least 65 bytes long)
 * @return 0 if successfull -1 otherwise
 */
static int receive_line(char *line)
{
    char c = 0;
    uint8_t state = 0;
    uint32_t byte_received_count = 0;

    /* Small state machine:
     * 0:
     *      \r -> go to state 1:
     *      anything else -> go to state 0
     * 1:
     *      \n -> go to state 2
     *      anything else -> go to state 0
     * 2: exit
     */
    if (line == NULL) {
        fprintf(stderr, "lora: Cannot receive data using null buffer.\n");
    }

    while (state != 2) {
        if (uart_receive((uint8_t*)&c, 1) < 0) {
            fprintf(stderr, "lora: Error while receiving data.\n");
            return -1;
        }

        if (state == 0 && c == '\r')
            state = 1;
        else if (state == 1 && c == '\n')
            state = 2;
        else
            state = 0;

        if (byte_received_count > MAX_LINE_LENGTH) {
            fprintf(stderr, "lora: Line too long.\n");
            return -1;
        }

        line[byte_received_count++] = c;
    }

    line[byte_received_count] = '\0';

    return 0;
}

/**
 * @brief Send a command to the device
 *
 * @param[in] cmd ASCII commands
 * @param[in] check_response Checks that the device returns ok\n\n if it is set
 * to true.
 * @return 0 if successful, -1 otherwise
 */
static int send_cmd(char *cmd, bool check_response)
{
    char rx_buffer[65];
    int cmd_length = strlen(cmd);

    if (uart_send((uint8_t*)cmd, cmd_length) != cmd_length)
        return -1;
    LOG_DEBUG("cmd: %s", cmd);

    if (receive_line(rx_buffer) < 0)
        return -1;
    LOG_DEBUG("response: %s", rx_buffer);

    if (check_response) {
        if (strcmp(rx_buffer, "ok\r\n") != 0)
            return -1;
    }

    return 0;
}

static int wait_for_answer(char *expected)
{
    char line[65];

    if (receive_line(line) < 0)
        return -1;

    if (strcmp(line, expected) != 0)
        return -1;

    return 0;
}

struct lora_click_config lora_click_get_default_configuration(void)
{
    static struct lora_click_config config = {
        .frequency = 868000000,
        .spreading_factor = 12,
        .auto_freq_band = LORA_CLICK_AUTO_FREQ_BAND_125KHZ,
        .coding_rate = LORA_CLICK_CODING_RATE_4_8,
        .bandwidth = LORA_CLICK_BANDWIDTH_250KHZ,
        .power = 14,
        .bitrate = 5000,
        .freq_deviation = 5000,
        .preamble_length = 8,
        .enable_crc_header = true
    };

    return config;
}

int lora_click_enable(struct lora_click_config config)
{
    char buffer[65];
    /* Reset device */
    if (send_cmd("sys reset\r\n", false) < 0)
        return -1;

    if (send_cmd("mac pause\r\n", false) < 0
    ||  send_cmd("radio set mod lora\r\n", true) < 0
    ||  send_cmd("radio set wdt 0\r\n", true) < 0         /* Disable watchdog */
    ||  send_cmd("radio set sync 12\r\n", true) < 0)
        return -1;

    /* Set frequency */
    if (!isValidFrequency(config.frequency)) {
        fprintf(stderr, "lora: Invalid frequency.\n");
        return -1;
    }
    sprintf(buffer, "radio set freq %u\r\n", config.frequency);
    if (send_cmd(buffer, true) < 0) {
        fprintf(stderr, "lora: Failed to set frequency.\n");
        return -1;
    }

    /* Set spreading factor */
    if (!isValidSpreadingFactor(config.spreading_factor)) {
        fprintf(stderr, "lora: Invalid frequency.\n");
        return -1;
    }
    sprintf(buffer, "radio set sf sf%u\r\n", config.spreading_factor);
    if (send_cmd(buffer, true) < 0) {
        fprintf(stderr, "lora: Failed to set spreading factor.\n");
        return -1;
    }

    /* Set auto freq correction */
    if (config.auto_freq_band >= LORA_CLICK_AUTO_FREQ_BAND_COUNT) {
        fprintf(stderr, "lora: Invalid auto freq band.\n");
        return -1;
    }
    sprintf(buffer, "radio set afcbw %s\r\n", auto_freq_str[config.auto_freq_band]);
    if (send_cmd(buffer, true) < 0) {
        fprintf(stderr, "lora: Failed to set auto freq band.\n");
        return -1;
    }

    /* Set coding rate */
    if (config.coding_rate >= LORA_CLICK_CODING_RATE_COUNT) {
        fprintf(stderr, "lora: Invalid coding rate.\n");
        return -1;
    }
    sprintf(buffer, "radio set cr %s\r\n", coding_rate_str[config.coding_rate]);
    if (send_cmd(buffer, true) < 0) {
        fprintf(stderr, "lora: Failed to set coding rate.\n");
        return -1;
    }

    /* Set bandwidth */
    switch (config.bandwidth) {
    case LORA_CLICK_BANDWIDTH_125KHZ:
        if (send_cmd("radio set bw 125\r\n", false) < 0)
            return -1;
        break;
    case LORA_CLICK_BANDWIDTH_250KHZ:
        if (send_cmd("radio set bw 250\r\n", false) < 0)
            return -1;
        break;
    case LORA_CLICK_BANDWIDTH_500KHZ:
        if (send_cmd("radio set bw 500\r\n", false) < 0)
            return -1;
        break;
    default:
        fprintf(stderr, "lora: Invalid bandwidth.\n");
        return -1;
    }

    /* Set power */
    if (!isValidPower(config.power)) {
        fprintf(stderr, "lora: Invalid power.\n");
        return -1;
    }
    sprintf(buffer, "radio set pwr %d\r\n", config.power);
    if (send_cmd(buffer, true) < 0) {
        fprintf(stderr, "lora: Failed to set power.\n");
        return -1;
    }

    /* Set bitrate */
    sprintf(buffer, "radio set bitrate %u\r\n", config.bitrate);
    if (send_cmd(buffer, true) < 0) {
        fprintf(stderr, "lora: Failed to set bitrate.\n");
        return -1;
    }

    /* Set frequency deviation */
    sprintf(buffer, "radio set fdev %u\r\n", config.freq_deviation);
    if (send_cmd(buffer, true) < 0) {
        fprintf(stderr, "lora: Failed to set frequency deviation.\n");
        return -1;
    }

    /* Set preamble length */
    sprintf(buffer, "radio set prlen %u\r\n", config.preamble_length);
    if (send_cmd(buffer, true) < 0) {
        fprintf(stderr, "lora: Failed to set preamble length.\n");
        return -1;
    }

    /* Enable/disable CRC header */
    if (config.enable_crc_header)
        sprintf(buffer, "radio set crc on\r\n");
    else
        sprintf(buffer, "radio set crc off\r\n");
    if (send_cmd(buffer, true) < 0) {
        fprintf(stderr, "lora: Failed to enable/disable crc header.\n");
        return -1;
    }

    return 0;
}

int lora_click_send(uint8_t *data, uint32_t count)
{
    uint32_t byte_sent_count = 0;

    if (data == NULL) {
        fprintf(stderr, "lora: Cannot send data using null pointer.\n");
        return -1;
    }

    if (count == 0)
        return 0;

    while (byte_sent_count < count) {
        int ret = 0;
        char buffer[BUFFER_LENGTH];
        uint32_t chunk_length = count;
        uint32_t i = 0;

        if (chunk_length > MAX_CHUNK_LENGTH)
            chunk_length = MAX_CHUNK_LENGTH;

        strcpy(buffer, "radio tx ");
        for (i = 0; i < chunk_length; ++i)
            sprintf(&buffer[9+i*2], "%02X", data[byte_sent_count + i]);
        strcpy(&buffer[9+chunk_length*2], "\r\n");
        if ((ret = send_cmd(buffer, true)) < 0) {
            fprintf(stderr, "lora: Failed to send data.\n");
            return ret;
        }

        if (wait_for_answer("radio_tx_ok\r\n") < 0) {
            fprintf(stderr, "lora: Failed to send data.\n");
            return -1;
        }

        byte_sent_count += chunk_length;
    }

    return byte_sent_count;
}

int lora_click_receive(uint8_t *data, uint32_t count)
{
    uint32_t byte_received_count = 0;

    if (data == NULL) {
        fprintf(stderr, "lora: Cannot receive data using null pointer.\n");
        return -1;
    }

    if (count == 0)
        return 0;

    while (byte_received_count < count) {
        int ret = 0;
        uint8_t i = 0;
        char buffer[65];

        if ((ret = send_cmd("radio rx 0\r\n", true)) < 0) {
            fprintf(stderr, "lora: Failed to receive data.\n");
            return ret;
        }

        if (receive_line(buffer) < 0)
            return -1;

        if (strcmp(buffer, "busy\r\n") == 0)
            continue;
        if (strcmp(buffer, "invalid_param\r\n") == 0)
            return -1;

        /* Assume at this point, previous answer was ok */

        if (receive_line(buffer) < 0)
            return -1;

        if(strncmp("radio_err", buffer, 9) == 0)
            return -1;

        while (buffer[i] != '\r' && byte_received_count < count) {
            data[byte_received_count++] = buffer[10+i];
            ++i;
        }
    }

    return byte_received_count;
}

int lora_click_disable(void)
{
    return -1;
}
