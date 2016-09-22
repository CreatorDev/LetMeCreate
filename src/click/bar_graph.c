#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "letmecreate/click/bar_graph.h"
#include "letmecreate/core/common.h"
#include "letmecreate/core/spi.h"



int bar_graph_click_send_data(uint8_t data)
{

	int ret; 
	ret = spi_transfer(&data, NULL, 1);
	if(ret < 0)
		return -1;
	else
		return 0;
}

