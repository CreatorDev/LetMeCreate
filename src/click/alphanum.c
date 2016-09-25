#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "letmecreate/click/alphanum.h"
#include "letmecreate/core.h"

static uint8_t gpio_pin_le2;
static uint8_t gpio_pin_oe;
static uint8_t gpio_pin_oe2;

/* 
 * Sleep for switch cycles.
 */
static void
alphanum_sleep_cycles(void)
{
	struct timespec cycles;
	struct timespec slept;

	cycles.tv_sec = ALPHANUM_SWITCH_INTERVAL / 1000;
	cycles.tv_nsec = ALPHANUM_SWITCH_INTERVAL * 1000 * 1000 - (cycles.tv_sec * 1000);

	while (nanosleep(&cycles, &slept) != 0) {
		cycles = slept;
	}
}

/* 
 * Convert char to 14 segment display value.
 */
uint16_t
alphanum_get_char(char c)
{
	if ((c >= '-') && (c <= '_')) {
		return alphanum_char_table[c - '-'];
	} else {
		return 0;
	}
}

/*
 * Write value on segments a (left) and b (right).
 */
int 
alphanum_raw_write(uint16_t a, uint16_t b)
{
	/* Set all GPIO to 1 */
	if (gpio_set_value(gpio_pin_le2, 1)) {
		printf("Error: cannot set value\n");
		return -1;
	}

	if (gpio_set_value(gpio_pin_oe, 1)) {
		printf("Error: cannot set value\n");
		return -1;
	}

	if (gpio_set_value(gpio_pin_oe2, 1)) {
		printf("Error: cannot set value\n");
		return -1;
	}

	/* Write b */
	if (spi_transfer((uint8_t *) &b, NULL, sizeof(b)) < 0) {
		fprintf(stderr, "spi write register failed.\n");
		return -1;
	}

	/* Enable output A */
	if (gpio_set_value(gpio_pin_oe, 0)) {
		printf("Error: cannot set value\n");
		return -1;
	}

	/* Write a */
	if (spi_transfer((uint8_t *) &a, NULL, sizeof(a)) < 0) {
		fprintf(stderr, "spi write register failed.\n");
		return -1;
	}

	/* Latch B */
	if (gpio_set_value(gpio_pin_le2, 0)) {
		printf("Error: cannot set value\n");
		return -1;
	}

	/* Disable Latch B */
	if (gpio_set_value(gpio_pin_le2, 1)) {
		printf("Error: cannot set value\n");
		return -1;
	}

	/* Enable, disable B */
	if (gpio_set_value(gpio_pin_oe2, 0)) {
		printf("Error: cannot set value\n");
		return -1;
	}

	if (gpio_set_value(gpio_pin_oe2, 1)) {
		printf("Error: cannot set value\n");
		return -1;
	}

	/* Enable a */
	if (gpio_set_value(gpio_pin_oe, 0)) {
		printf("Error: cannot set value\n");
		return -1;
	}

	return 0;
}

/*
 * Write 2 chars on the segment display.
 * Wrapper around alphanum_raw_write().
 */
int
alphanum_write(char a, char b)
{
	alphanum_raw_write(alphanum_get_char(toupper(a)), alphanum_get_char(toupper(b)));
}

/* 
 * Init the alphanum clicker.
 */
int
alphanum_init(uint8_t bus)
{
	int ret = 0;

	/* Setup GPIO */
	switch(bus) {
		case MIKROBUS_1:
			gpio_pin_le2 = MIKROBUS_1_RST;
			gpio_pin_oe = MIKROBUS_1_INT;
			gpio_pin_oe2 = MIKROBUS_1_PWM;
			break;
		case MIKROBUS_2:
			gpio_pin_le2 = MIKROBUS_2_RST;
			gpio_pin_oe = MIKROBUS_2_INT;
			gpio_pin_oe2 = MIKROBUS_2_PWM;
			break;
		default:
			printf("Error: Bus not found!\n");
			return -1;
	}

	/* Init GPIO pins */
	if ((ret = gpio_init(gpio_pin_le2)) != 0) {
		printf("Error: Cannot initialize gpio\n");
		return ret;
	}
	if ((ret = gpio_init(gpio_pin_oe)) != 0) {
		printf("Error: Cannot initialize gpio\n");
		return ret;
	}
	if ((ret = gpio_init(gpio_pin_oe2)) != 0) {
		printf("Error: Cannot initialize gpio\n");
		return ret;
	}

	/* Set directions for the GPIO pins */
	if ((ret = gpio_set_direction(gpio_pin_le2, GPIO_OUTPUT)) != 0) {
		printf("Error: Cannot gpio directions\n");
		return ret;
	}
	if ((ret = gpio_set_direction(gpio_pin_oe, GPIO_OUTPUT)) != 0) {
		printf("Error: Cannot gpio directions\n");
		return ret;
	}
	if ((ret = gpio_set_direction(gpio_pin_oe2, GPIO_OUTPUT)) != 0) {
		printf("Error: Cannot gpio directions\n");
		return ret;
	}

	spi_select_bus(bus);

	return ret;
}

/* 
 * Periodically switch between segments a and b to keep the illusion of
 * a simultaneous display of both values.
 */
void
alphanum_switch_cycles(int num)
{
	int i = 0;
	while (i < num || num == 0) {
		if (gpio_set_value(gpio_pin_oe, 1)) {
			printf("Error: cannot set value\n");
			return;
		}
		if (gpio_set_value(gpio_pin_oe2, 0)) {
			printf("Error 1: cannot set value le2\n");
			return;
		}

		alphanum_sleep_cycles();

		if (gpio_set_value(gpio_pin_oe2, 1)) {
			printf("Error 2: cannot set value le2\n");
			return;
		}
		if (gpio_set_value(gpio_pin_oe, 0)) {
			printf("Error: cannot set value\n");
			return;
		}

		alphanum_sleep_cycles();
		++i;
	}
}

/*
 * Write 2 or more characters on the alphanum clicker as a 
 * "running text".
 */
void
alphanum_write_running_text(const char *s, int ntimes)
{
	int i, cnt;
	int cycles = 25;
	char *str;

	if ((str = malloc((strlen(s) + 1) * sizeof(char))) == NULL) {
		printf("Error: malloc()\n");
		return;
	}

	for (i = 0; i < strlen(s); ++i) {
		str[i] = toupper(s[i]);
	}

	i = 0;
	cnt = 1;
	while (1) {
		if ((i + 1) >= strlen(str)) {
			if ((i + 1) == (strlen(str) + 1)) {
				alphanum_raw_write(0x00, 0x00);
				alphanum_switch_cycles(cycles);
				i = 0;
				if (ntimes == cnt && ntimes > 0) {
					break;
				} else {
					++cnt;
				}
			} else {
				alphanum_raw_write(alphanum_get_char(str[i]), 0x00);
				alphanum_switch_cycles(cycles);
				++i;
			}
		} else {
			alphanum_raw_write(alphanum_get_char(str[i]), alphanum_get_char(str[i + 1]));
			alphanum_switch_cycles(cycles);
			++i;
		}
	}
}
