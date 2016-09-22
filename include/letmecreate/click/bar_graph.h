/**
 * @file bar_graph.h
 * @author Jai Shiv
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example bar_graph/main.c
 */

#ifndef __LETMECREATE_CLICK_BARGRAPH_H__
#define __LETMECREATE_CLICK_BARGRAPH_H__

#include <stdint.h>


/**
 * @brief Send Data to  bar_graph click.
 *
 * The bar_graph click must be enabled before calling this function.
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int bar_graph_click_send_data(const uint8_t );


#endif
