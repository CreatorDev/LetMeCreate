#include <stdio.h>
#include <string.h>
#include <time.h>
#include <letmecreate/core/gpio.h>
#include <letmecreate/core/i2c.h>
#include <letmecreate/rpisensehat/attiny88.h>

/* I2C slave address */
#define ATTINY88_ADDRESS    (0x46)

/* Commands */
#define KEYS_CMD            (0xF2)
#define LEDS_CMD            (0x00)


/* Values for displaying a rainbow on LED matrix.
 * (copied from https://github.com/raspberrypi/rpi-sense/blob/master/main.c)
 */
static uint8_t rainbow[] = {
    0x1F, 0x1F, 0x1F, 0x1F, 0x14, 0x03, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x12, 0x1F, 0x1F, 0x1F, 0x1F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,

    0x1F, 0x1F, 0x1F, 0x12, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x04, 0x14, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1D,

    0x1F, 0x1F, 0x11, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x15, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0B,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x1F, 0x1F,

    0x1F, 0x0F, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x17, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0A, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0A, 0x1F, 0x1F, 0x1F,

    0x0E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
    0x1F, 0x1F, 0x1F, 0x1F, 0x1D, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x0B, 0x1F, 0x1F, 0x1F, 0x1F,

    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x14,
    0x1F, 0x1F, 0x1F, 0x1B, 0x07, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x0C, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x15, 0x1F,
    0x1F, 0x1F, 0x19, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x12,

    0x00, 0x00, 0x00, 0x00, 0x05, 0x17, 0x1F, 0x1F,
    0x1F, 0x17, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0F, 0x02,
};

static void sleep_50ms(void)
{
    struct timespec rem, req = {
        .tv_sec = 0,
        .tv_nsec = 50000000
    };

    while (nanosleep(&req, &rem))
        req = rem;
}

static int reset(void)
{
    if (gpio_init(GPIO_21) < 0
    ||  gpio_set_direction(GPIO_21, GPIO_OUTPUT) < 0)
        return -1;

    if (gpio_set_value(GPIO_21, 0) < 0)
        return -1;

    sleep_50ms();

    if (gpio_set_value(GPIO_21, 1) < 0)
        return -1;

    sleep_50ms();
    sleep_50ms();

    return 0;
}


int attiny88_enable(void)
{
    uint8_t pixels[64*3];

    /* Reset Atmel Chip otherwise it hangs the I2C bus. */
    if (reset() < 0) {
        fprintf(stderr, "attiny88: Failed to reset chip.\n");
        return -1;
    }

    /* Switch off LED's */
    memset(pixels, 0, sizeof(pixels));
    return attiny88_set_leds_state(pixels);
}

int attiny88_get_joystick_input(uint8_t *state)
{
    if (state == NULL) {
        fprintf(stderr, "attiny88: Cannot store joystick state using null pointer.\n");
        return -1;
    }

    if (i2c_write_byte(ATTINY88_ADDRESS, KEYS_CMD) < 0
    ||  i2c_read_byte(ATTINY88_ADDRESS, state) < 0)
        return -1;

    return 0;
}

int attiny88_set_leds_state(uint8_t *pixels)
{
    uint8_t col = 0, row = 0;
    uint8_t buffer[64*3+1];
    uint8_t *values = NULL;

    if (pixels == NULL) {
        fprintf(stderr, "attiny88: Cannot set leds using null buffer.\n");
        return -1;
    }

    buffer[0] = LEDS_CMD;
    memcpy(&buffer[1], pixels, 64*3);

    /* Pixels need to be sent to the atmel chip using this layout:
     *
     * First column:    RRRRRRRR
     *                  GGGGGGGG
     *                  BBBBBBBB
     *
     * Second column:   RRRRRRRR
     *                  GGGGGGGG
     *                  BBBBBBBB
     *         ....
     *
     * Each value must not be greater than 0x1F.
     */
    values = &buffer[1];
    for (row = 0; row < 8; ++row) {
        uint8_t i = row * 24;
        for (col = 0; col < 8; ++col) {
            uint8_t index = (row*8 + col)*3;
            values[i]    = pixels[index];
            values[i+8]  = pixels[index + 1];
            values[i+16] = pixels[index + 2];

            /* Ensure that not value is greater than 0x1F */
            if (values[i] > 0x1F)
                values[i] = 0x1F;
            if (values[i+8] > 0x1F)
                values[i+8] = 0x1F;
            if (values[i+16] > 0x1F)
                values[i+16] = 0x1F;

            ++i;
        }
    }

    if (i2c_write(ATTINY88_ADDRESS, buffer, sizeof(buffer)) < 0)
        return -1;

    return 0;
}

int attiny88_display_rainbow(void)
{
    uint8_t buffer[64*3+1];
    buffer[0] = LEDS_CMD;
    memcpy(&buffer[1], rainbow, sizeof(rainbow));
    if (i2c_write(ATTINY88_ADDRESS, buffer, sizeof(buffer)) < 0)
        return -1;

    return 0;
}

int attiny88_disable(void)
{
    uint8_t pixels[64*3];

    /* Switch off LED's */
    memset(pixels, 0, sizeof(pixels));
    return attiny88_set_leds_state(pixels);
}
