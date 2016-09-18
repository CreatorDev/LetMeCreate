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

#include "letmecreate/letmecreate.h"

/*
 * Alpha clicker. Examples:
 * Write a running text on the clicker
 * ./main -r "Hi Creator" 
 *
 * Read the temperature clicker
 * ./main -t
 */
int
main(int argc, char *argv[])
{
	char ch;
	char *str;
	char temp[STR_MAX];
	alphanum_init(MIKROBUS_2);

	while ((ch = getopt(argc, argv, "r:c:t")) != -1) {
		switch(ch) {
			/* Write running text */
			case 'r':
				str = optarg;
				if (strlen(str) < 3) {
					printf("Error: please provide a longer text argument!\n");
					return -1;
				}
				printf("Writing: %s\n", str);
				alphanum_write_running_text(str, 0);
				break;
			/* Display two chars */
			case 'c':
				str = optarg;
				if (strlen(str) < 1) {
					printf("Error: please provide a text argument!\n");
					return -1;
				}
				printf("Writing: %s\n", str);
				if (strlen(str) < 2) {
					alphanum_write(alphanum_get_char(toupper(str[0])), 0x0);
				} else {
					alphanum_write(alphanum_get_char(toupper(str[0])), alphanum_get_char(toupper(str[1])));
				}
				alphanum_switch_cycles(0);
				break;
			/* Read temperature and display */
			case 't':
				while (1) {
					memset(temp, 0, STR_MAX);
					alphanum_get_temperature(temp);
					printf("Writing temperature: %s C\n", temp);
					alphanum_write_running_text(temp, 1);
				}
				break;
			default:
				fprintf(stderr, "Error: Wrong parameter\n");
				return -1;
				break;
		}
	} 

	return 0;
}
