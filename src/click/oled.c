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

#include "letmecreate/click/oled.h"
#include "letmecreate/click/common.h"
#include "letmecreate/core.h"

/* Command and data registers */
const uint16_t oled_cmd_addr = 0x3C;
const uint16_t oled_data_addr = 0x3D;

/*
 * Translate character into raster font graphics.
 */
uint8_t *
oled_get_char(char c)
{ 
	if ((c >= '!') && (c <= '~')) {
		return oled_char_table[c - '!'];
	}

	return NULL;
}

/*
 * Write to the oled command register.
 */
int
oled_cmd(uint8_t cmd)
{
	int ret = 0;

	if ((ret = i2c_write_register(oled_cmd_addr, 0b0000000, cmd)) < 0) {
		fprintf(stderr, "Error: i2c write failed with: %i\n", ret);
	}

	return ret;
}

/*
 * Write data to a page.
 */
int
oled_data(uint8_t data){
	int ret = 0;

	if ((ret = i2c_write_register(oled_cmd_addr, 0b1100000, data)) < 0) {
		fprintf(stderr, "Error: i2c write data failed with: %i\n", ret);
	}

	return ret; 
}

/* 
 * Initialize the oled display.
 */
int
oled_init(uint8_t bus)
{
	int ret = 0;

	i2c_select_bus(bus); /* select bus */

	ret = oled_cmd(SSD1306_DISPLAYOFF);             /* 0xAE Set OLED Display Off */
	ret += oled_cmd(SSD1306_SETDISPLAYCLOCKDIV);     /* 0xD5 Set Display Clock Divide Ratio/Oscillator Frequency */
	ret += oled_cmd(0x80);
	ret += oled_cmd(SSD1306_SETMULTIPLEX);           /* 0xA8 Set Multiplex Ratio */
	ret += oled_cmd(0x27);
	ret += oled_cmd(SSD1306_SETDISPLAYOFFSET);       /* 0xD3 Set Display Offset */
	ret += oled_cmd(0x00);
	ret += oled_cmd(SSD1306_SETSTARTLINE);           /* 0x40 Set Display Start Line */
	ret += oled_cmd(SSD1306_CHARGEPUMP);             /* 0x8D Set Charge Pump */
	ret += oled_cmd(0x14);                           /* 0x14 Enable Charge Pump */
	ret += oled_cmd(SSD1306_COMSCANDEC);             /* 0xC8 Set COM Output Scan Direction */
	ret += oled_cmd(SSD1306_SETCOMPINS);             /* 0xDA Set COM Pins Hardware Configuration */
	ret += oled_cmd(0x12);
	ret += oled_cmd(SSD1306_SETCONTRAST);            /* 0x81 Set Contrast Control */
	ret += oled_cmd(0xAF);
	ret += oled_cmd(SSD1306_SETPRECHARGE);           /* 0xD9 Set Pre-Charge Period */
	ret += oled_cmd(0x25);
	ret += oled_cmd(SSD1306_SETVCOMDETECT);          /* 0xDB Set VCOMH Deselect Level */
	ret += oled_cmd(0x20);
	ret += oled_cmd(SSD1306_DISPLAYALLON_RESUME);    /* 0xA4 Set Entire Display On/Off */
	ret += oled_cmd(SSD1306_NORMALDISPLAY);          /* 0xA6 Set Normal/Inverse Display */
	ret += oled_cmd(SSD1306_DISPLAYON);              /* 0xAF Set OLED Display On */

	if (ret < 0) {
		printf("Error: Cannot setup OLED display.\n");
	}

	return ret;
}

/*
 * Set the current page address.
 */
int
oled_set_page_addr(uint8_t add)
{
	int ret = 0;
	add = 0xb0 | add;
	ret = oled_cmd(add);

	return ret;
}

/* 
 * Write a picture saved in returned raster graphics to the oled
 * display.
 */
int 
oled_write_pic(uint8_t *pic)
{
    unsigned char i, j;
	unsigned char num=0;
	int ret = 0;

	for (i = 0; i < 0x05; i++) {
		oled_set_page_addr(i);
		oled_cmd(0x10);
		oled_cmd(0x40);
		for (j = 0; j < 0x60; j++){
			if ((ret = oled_data(pic[i * 0x60 + j])) < 0 ) {
				printf("Error: Cannot write to oled display\n");
				return ret;
			}
		}
	}

	return ret;
}

/*
 * Write a text string to the oled display.
 * The display is divided with the default monospace font into two lines a
 * 8 characters per line. The whole (visible) set of ASCII characters are 
 * available.
 */
void
oled_write_text(char *str)
{
	unsigned char i,j;
	uint8_t data;
	uint8_t *ch;
	int ch_addr;
	int ch_num;
	int start, end;
	int width = 11;
	int col_offset = -6;
	int char_per_line = 8;

	int str_len = strlen(str);

	printf("Writing: %s (length: %i)\n", str, str_len);

	for (i = 0 ; i < 5; ++i) {
		oled_set_page_addr(i);
		oled_cmd(0x10);
		oled_cmd(0x40);

		data = 0x00;
		ch_num = char_per_line - 1;
		for (j = 0; j < 96; ++j) {
			/* Line 1 */
			if (i == 0 || i == 1) {
				start = (char_per_line - ch_num) * width + col_offset;
				end = (char_per_line - ch_num) * width + width - 1 + col_offset;
				if (ch_num < str_len && ch_num >= 0) {
					if (j >= start && j <= end) {
						ch_addr = (j - start) + width * (i - 0);
						if (str[ch_num] == ' ') {
							data = 0x00;
						} else {
							ch = oled_get_char(str[ch_num]);
							if (ch == NULL) {
								data = 0x00;
							} else {
								data = ch[ch_addr];
							}
						}
					} else {
						data = 0x00;
					}
				} else {
					data = 0x00;
				}
				if (j == end) {
					--ch_num;
				}
			/* Line 2 */
			} else if ((i == 2 || i == 3) && ((str_len - char_per_line) > 0)) {
				start = (char_per_line - ch_num) * width + col_offset;
				end = (char_per_line - ch_num) * width + width - 1 + col_offset;
				if (ch_num < (str_len - char_per_line) && ch_num >= 0) {
					if (j >= start && j <= end) {
						ch_addr = (j - start) + width * (i - 2);
						if (str[ch_num + char_per_line] == ' ') {
							data = 0x00;
						} else {
							ch = oled_get_char(str[ch_num + char_per_line]);
							if (ch == NULL) {
								data = 0x00;
							} else {
								data = ch[ch_addr];
							}
						}
					} else {
						data = 0x00;
					}
				} else {
					data = 0x00;
				}
				if (j == end) {
					--ch_num;
				}
			} else {
				data = 0x00;
			}

			oled_data(data);
		}
	}
}
