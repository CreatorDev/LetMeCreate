#ifndef __LETMECREATE_CLICK_ALPHANUM_H__
#define __LETMECREATE_CLICK_ALPHANUM_H__

#include <stdint.h>
#include <letmecreate/click/export.h>

/* Switch interval in ms */
#define ALPHANUM_SWITCH_INTERVAL 8

/* Max string length for running text */
#define STR_MAX 10

/* Init the alphanum clicker */
int LETMECREATE_CLICK_EXPORT alphanum_click_init(uint8_t);

/* Converts a char into a 14 segment display value */
uint16_t LETMECREATE_CLICK_EXPORT alphanum_click_get_char(char);

/* Write the two chars to */
int LETMECREATE_CLICK_EXPORT alphanum_click_write(char, char);

/* Raw write  */
int LETMECREATE_CLICK_EXPORT alphanum_click_raw_write(uint16_t, uint16_t);

/* This peridically switch between segment a and b to keep the illusion of a
 * simultanous display of both values
 */
void LETMECREATE_CLICK_EXPORT alphanum_click_switch_cycles(int);

/* Write a text with more than two characters */
void LETMECREATE_CLICK_EXPORT alphanum_click_write_running_text(const char *, int);

#endif
