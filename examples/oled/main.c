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
 * Example how to write a text to the OLED display.
 */
int
main(int argc, char *argv[])
{
	int ret = 0;

	if ((ret = i2c_init()) != 0) {
		printf("Error: Cannot initialize i2c.\n");
		return ret;
	}

	if (oled_init(MIKROBUS_2) < 0) {
		printf("Error: Cannot initialize oled clicker.\n");
		return 1;
	}
	oled_write_text("Hello   Creator!");

	return 0;
}
