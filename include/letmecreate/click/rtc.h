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

#include <stdint.h>
#include <letmecreate/click/export.h>

enum WEEKDAY {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
};

enum MONTH {
    JANUARY,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
};

/** Structure to represent a date */
struct date {
    uint16_t year;          /**< Must be in range year_offset..year_offset+3 */
    unsigned int month:4;   /**< Must be in range 0..11 */
    unsigned int day:5;     /**< Must be in range 1..31 */
    unsigned int weekday:4; /**< Must be in range 0..6 */
    unsigned int hour:5;    /**< Must be in range 0..23 */
    unsigned int minute:6;  /**< Must be in range 0..59 */
    unsigned int second:6;  /**< Must be in range 0..59 */
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

#endif
