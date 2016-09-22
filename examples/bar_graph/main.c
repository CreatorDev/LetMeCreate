#include <linux/spi/spidev.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <letmecreate/letmecreate.h>
#include <unistd.h>
#include <stdlib.h>
#define DEMO_ITERATIONS 4
#define DEMO_DELAY 4000

static volatile bool running = true;

static void exit_program(int signo)
{
	running = false;
}

static void print_usage(const char *program)
{
	printf("\nUsage: %s [options]\n",program);
	printf("-m select mikroBUS where BarGraph sits (1 or 2)\n");
	printf("-h display this message\n");

}
static int parse_cmd_opts(int argc, char *argv[], int *bus_no)
{
	int opt;
	while(1){
		int tmp =0;
		opt = getopt(argc, argv, "m:h");
		if (opt == -1) {
			break;
		}

		switch (opt) {
			case 'm':
				tmp = strtoul(optarg, NULL, 0);
				if (tmp == 1) {
					*bus_no = 1;
				} else if (tmp == 2) {
					*bus_no = 2;
				} else {
					print_usage(argv[0]); 
					return -1;
				}
				break;
			case 'h':
				print_usage(argv[0]);
				return -1;
				break;
			default:
				print_usage(argv[0]);
				return -1;
				break;
		}
	}
	return 1;
}
int main(int argc, char * argv[])
{
	int bus_no =0,ret;
        int count = 0;
	/* Set signal handler to exit program when Ctrl+c is pressed */
	struct sigaction action = {
		.sa_handler = exit_program,
		.sa_flags = 0
	};
	sigemptyset(&action.sa_mask);
	sigaction (SIGINT, &action, NULL);
	ret = parse_cmd_opts(argc, argv, &bus_no);
	if (ret <= 0) {
		return ret;
	}    
	spi_init();
	if(bus_no == 1)
	{
		pwm_init(MIKROBUS_1);
		pwm_set_period(MIKROBUS_1, 100000);
		pwm_set_duty_cycle(MIKROBUS_1, 50);
		pwm_enable(MIKROBUS_1);
	}
	else if(bus_no == 2)
	{
		pwm_init(MIKROBUS_2);
		pwm_set_period(MIKROBUS_2, 100000);
		pwm_set_duty_cycle(MIKROBUS_2, 50);
		pwm_enable(MIKROBUS_2);
	}
	else
	{
		print_usage(argv[0]);
	}

	while (running && count<5) {
		uint8_t i, j;

		for (j = 0; j < DEMO_ITERATIONS; j++) {
			for (i = 0; i <= 100; i++) {
				if (bar_graph_click_send_data((1 << (i/10)) - 1) < 0) {
					return -1;
				}
				usleep(DEMO_DELAY);
			}

			for (i = 109; i > 0; i--) {
				if (bar_graph_click_send_data((1 << (i/10)) - 1) < 0) {
					return -1;
				}
				usleep(DEMO_DELAY);
			}
		}

		for (i = 0; i <= 100; i++) {
			if (bar_graph_click_send_data((1 << (i/10)) - 1) < 0) {
				return -1;
			}
			usleep(DEMO_DELAY);
		}
		count++;
	}

	if(bus_no == 1)
	{
		pwm_disable(MIKROBUS_1);
		pwm_release(MIKROBUS_1);
	}
	else
	{

		pwm_disable(MIKROBUS_2);
		pwm_release(MIKROBUS_2);
	}
	spi_release();

	return 0;
}
