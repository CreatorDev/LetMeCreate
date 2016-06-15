#include <linux/spi/spidev.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "click/accel.h"
#include "core/common.h"
#include "core/spi.h"

static volatile bool running = true;

static void exit_program(int signo)
{
    running = false;
}

static void sleep_100ms(void)
{
    struct timespec rem, req = {
        .tv_sec = 0,
        .tv_nsec = 100000000
    };

    while (nanosleep(&req, &rem))
        req = rem;
}

int main(void)
{
    /* Set signal handler to exit program when Ctrl+c is pressed */
    struct sigaction action = {
        .sa_handler = exit_program,
        .sa_flags = 0
    };
    sigemptyset(&action.sa_mask);
    sigaction (SIGINT, &action, NULL);

    spi_init();
    spi_select_bus(MIKROBUS_1);
    accel_click_enable();

    printf("Press Ctrl+c to exit program.\n");
    printf("  X        Y        Z\n");
    while (running) {
        float x, y, z;
        accel_click_get_measure(&x, &y, &z);

        /* Print measurement */
        printf("\33[2K\r");     /* Erase current line */
        printf("%-9.3f%-9.3f%-9.3f", x, y, z);
        fflush(stdout);
        sleep_100ms();
    }

    accel_click_disable();
    spi_release();

    return 0;
}
