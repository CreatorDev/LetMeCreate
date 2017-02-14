/**
 * @file rtc.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 *
 * @example rtc/main.c
 */

#ifndef __LETMECREATE_CLICK_RTC_H__
#define __LETMECREATE_CLICK_RTC_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <letmecreate/click/export.h>

/** Structure to represent a date */
struct date {
    uint16_t year;          /**< Must be in range year_offset..year_offset+3 */
    uint8_t month;          /**< Must be in range 0..11 */
    uint8_t day;            /**< Must be in range 1..31 */
    uint8_t weekday;        /**< Must be in range 0..6 */
    uint8_t hour;           /**< Must be in range 0..23 */
    uint8_t minute;         /**< Must be in range 0..59 */
    uint8_t second;         /**< Must be in range 0..59 */
};

/**
 * @brief Initialise the RTC Click.
 *
 * @param[in] year_offset RTC Click stores year on 2 bits only. year_offset is
 * used to compute year as year_offset + 2bits from RTC click.
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT rtc_click_init(uint16_t year_offset);

/**
 * @brief Read current date from RTC Click.
 *
 * @param[out] date
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT rtc_click_get_date(struct date *date);

/**
 * @brief Set current date of RTC Click.
 *
 * @param[in] date
 * @return 0 if successful, -1 otherwise
 */
int LETMECREATE_CLICK_EXPORT rtc_click_set_date(struct date date);

#ifdef __cplusplus
}
#endif

#endif
