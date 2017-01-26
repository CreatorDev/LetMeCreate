#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <letmecreate/letmecreate.h>


static void receive(void)
{
    char buffer[16];
    lora_click_receive((uint8_t*)buffer, sizeof(buffer) - 1);
    buffer[15] = '\0';
    printf("Received \"%s\"\n", buffer);
}

static void send(unsigned int n)
{
    char buffer[255];
    sprintf(buffer, "Hello, World! %u", n);
    lora_click_send((uint8_t*)buffer, strlen(buffer));
    printf("Sent \"%s\"\n", buffer);
}

int main(int argc, char** argv)
{
    uint32_t n = 0;
    char mode;
    if (argc < 2) {
        fprintf(stderr, "%s r|s\n", argv[0]);
        return -1;
    }

    mode = argv[1][0];
    if (mode != 's' && mode != 'r') {
        fprintf(stderr, "Invalid mode.\n");
        return -1;
    }

    uart_init();
    uart_select_bus(MIKROBUS_1);
    uart_set_baudrate(UART_BD_57600);
    lora_click_init(MIKROBUS_1, lora_click_get_default_configuration());

    while (1) {
        if (mode == 's') {
            send(n);
            sleep(1);
        }
        else if (mode == 'r')
            receive();

        ++n;
    }

    uart_release();

    return 0;
}
