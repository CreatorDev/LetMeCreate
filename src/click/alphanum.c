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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "letmecreate/click/alphanum.h"
#include "letmecreate/click/thermo3.h"
#include "letmecreate/core/common.h"
#include "letmecreate/core/gpio.h"
#include "letmecreate/core/spi.h"
#include "letmecreate/core/i2c.h"

static uint8_t current_mikrobus_index = MIKROBUS_2;

uint8_t gpio_pin_le2;
uint8_t gpio_pin_oe;
uint8_t gpio_pin_oe2;

/* 
 * Convert char to 14 segment display value.
 */
uint16_t
alphanum_get_char(char c)
{
	if ((c >= '-') && (c <= '_')) {
		return char_table[c - '-'];
	} else {
		return 0;
	}
}

/*
 * Write value on display a and b.
 */
int 
alphanum_write(uint16_t a, uint16_t b)
{
	int ret;

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
	if ((ret = spi_transfer((uint8_t *) &b, NULL, sizeof(b))) < 0) {
		fprintf(stderr, "spi write register failed.\n");
		return -1;
	}

	/* Enable output A */
	if (gpio_set_value(gpio_pin_oe, 0)) {
		printf("Error: cannot set value\n");
		return -1;
	}

	/* Write a */
	if ((ret = spi_transfer((uint8_t *) &a, NULL, sizeof(a))) < 0) {
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
}

/* 
 * Init the alphanum clicker
 */
int
alphanum_init(uint8_t bus)
{
	int ret;

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

	gpio_init(gpio_pin_le2);
	gpio_init(gpio_pin_oe);
	gpio_init(gpio_pin_oe2);

	gpio_set_direction(gpio_pin_le2, GPIO_OUTPUT);
	gpio_set_direction(gpio_pin_oe, GPIO_OUTPUT);
	gpio_set_direction(gpio_pin_oe2, GPIO_OUTPUT);

	/* SPI int */
	if ((ret = spi_init()) != 0) {
		printf("Error spi init failed with %i\n", ret);
		return ret;
	}

	spi_select_bus(bus);

	return ret;
}

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

		usleep(SWITCH_INTERVAL);

		if (gpio_set_value(gpio_pin_oe2, 1)) {
			printf("Error 2: cannot set value le2\n");
			return;
		}
		if (gpio_set_value(gpio_pin_oe, 0)) {
			printf("Error: cannot set value\n");
			return;
		}

		usleep(SWITCH_INTERVAL);
		++i;
	}
}

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
				alphanum_write(0x00, 0x00);
				alphanum_switch_cycles(cycles);
				i = 0;
				if (ntimes == cnt && ntimes > 0) {
					break;
				} else {
					++cnt;
				}
			} else {
				alphanum_write(alphanum_get_char(str[i]), 0x00);
				alphanum_switch_cycles(cycles);
				++i;
			}
		} else {
			alphanum_write(alphanum_get_char(str[i]), alphanum_get_char(str[i + 1]));
			alphanum_switch_cycles(cycles);
			++i;
		}
	}
}

/* 
 * Used to get the temperature from the temp clicker
 */
void
alphanum_get_temperature(char *str)
{
	float temperature = 0.f;

	i2c_init();
	i2c_select_bus(MIKROBUS_1);

	thermo3_click_enable(0);
	thermo3_click_get_temperature(&temperature);
	snprintf(str, STR_MAX, "%.2f", temperature);
	
	thermo3_click_disable();

	i2c_release();
}
