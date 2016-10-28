/**
 * This example shows how to use the RTC Click wrapper of the LetMeCreate to
 * obtain the current date.
 *
 * It sets the date to Friday 28th October, 9:44:0. Then, it prints the current
 * date read from the RTC Click every second. Ctrl+C must be pressed to exit
 * the program.
 *
 * The RTC Click must be inserted in Mikrobus 1 before running this program.
 */

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <letmecreate/letmecreate.h>


static char *weekday_str[] = {
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};

static char *month_str[] = {
    "January",
    "Februay",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

static volatile bool running = true;

static void exit_program(int __attribute__ ((unused))signo)
{
    running = false;
}

int main(void)
{
    struct date date = {
        .second = 0,
        .minute = 44,
        .hour   = 9,
        .weekday = FRIDAY,
        .day = 28,
        .month = OCTOBER,
        .year = 2016
    };
    struct sigaction action = {
        .sa_handler = exit_program,
        .sa_flags = 0
    };
    sigemptyset(&action.sa_mask);
    sigaction (SIGINT, &action, NULL);

    i2c_init();
    rtc_click_init(2016);
    rtc_click_set_date(date);

    while (running) {
        rtc_click_get_date(&date);
        printf("%s %u %s %u, %u:%u:%u\n", weekday_str[date.weekday],
                                          date.day,
                                          month_str[date.month],
                                          date.year,
                                          date.hour, date.minute, date.second);
        sleep(1);
    }

    i2c_release();

    return 0;
}
