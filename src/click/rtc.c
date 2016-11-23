#include <stdio.h>
#include <letmecreate/core/i2c.h>
#include <letmecreate/click/rtc.h>

#define PCF8583_ADDRESS         (0x50)

/* Register address */
#define PCF8583_CONTROL_REG     (0x00)
#define PCF8583_SECOND_REG      (0x02)
#define PCF8583_TIMER_REG       (0x07)

#define TWELVE_HOUR_FORMAT          (0x80)
#define PM_FLAG             (0x40)

static uint16_t year_offset = 0;

static uint8_t bcd_to_binary(uint8_t byte)
{
    return ((byte & 0xF0) >> 4) * 10 + (byte & 0x0F);
}

static uint8_t binary_to_bcd(uint8_t byte)
{
    return ((byte / 10) << 4) | (byte % 10);
}

int rtc_click_init(uint16_t _year_offset)
{
    uint8_t buffer[2];

    /* Disable alarm */
    buffer[0] = PCF8583_CONTROL_REG;
    buffer[1] = 0;
    if (i2c_write(PCF8583_ADDRESS, buffer, sizeof(buffer)) < 0)
        return -1;

    /* Disable timer */
    buffer[0] = PCF8583_TIMER_REG;
    buffer[1] = 0;
    if (i2c_write(PCF8583_ADDRESS, buffer, sizeof(buffer)) < 0)
        return -1;

    year_offset = _year_offset;
    return 0;
}

int rtc_click_get_date(struct date *date)
{
    uint8_t buffer[5];

    if (date == NULL) {
        fprintf(stderr, "rtc: Cannot store date using null pointer.\n");
        return -1;
    }

    if (i2c_write_byte(PCF8583_ADDRESS, PCF8583_SECOND_REG) < 0
    ||  i2c_read(PCF8583_ADDRESS, buffer, sizeof(buffer)) < 0)
        return -1;

    date->second = bcd_to_binary(buffer[0]);
    date->minute = bcd_to_binary(buffer[1]);
    date->hour = bcd_to_binary(buffer[2] & 0x3F);

    /* Convert hour in 24h format */
    if (buffer[2] & TWELVE_HOUR_FORMAT) {
        if (buffer[2] & PM_FLAG)
            date->hour += 12;
    }

    date->day = bcd_to_binary(buffer[3] & 0x3F);
    date->year = buffer[3] >> 6;
    date->year += year_offset;
    date->month = bcd_to_binary(buffer[4] & 0x1F);
    --date->month; /* Device store month in range 1..12, decrement to get it in range 0..11 (easier for indexing) */
    date->weekday = buffer[4] >> 5;

    return 0;
}

int rtc_click_set_date(struct date date)
{
    uint8_t buffer[6];
    buffer[0] = PCF8583_SECOND_REG;

    ++date.month;
    date.year -= year_offset;

    if (date.second > 59
    ||  date.minute > 59
    ||  date.hour > 23
    ||  date.day == 0
    ||  date.month == 0
    ||  date.month > 12
    ||  (date.day > 31 && (date.month == 1 || date.month == 3 || date.month == 5 || date.month == 7 || date.month == 8 || date.month == 10 || date.month == 12))
    ||  (date.day > 30 && (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11))
    ||  (date.day > 29 && date.year == 0)
    ||  (date.day > 28 && date.year != 0)
    ||  date.weekday > 6
    ||  date.year > 3) {
        fprintf(stderr, "Invalid date.\n");
        return -1;
    }

    buffer[1] = binary_to_bcd(date.second);
    buffer[2] = binary_to_bcd(date.minute);
    buffer[3] = binary_to_bcd(date.hour);
    buffer[4] = (date.year << 6) | binary_to_bcd(date.day);
    buffer[5] = (date.weekday << 5) | binary_to_bcd(date.month);

    return i2c_write(PCF8583_ADDRESS, buffer, sizeof(buffer));
}
