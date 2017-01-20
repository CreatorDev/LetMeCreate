#include <stdio.h>
#include <string.h>
#include <letmecreate/letmecreate.h>

static void receive(void)
{
    char *cmd = "sys get ver\r\n";
    uint8_t tmp;
    lora_click_receive(&tmp, sizeof(tmp));
    printf("Character received: 0x%02X\n", tmp);
}

static void send(void)
{
    static int n = 0;
    char buffer[30];
    sprintf(buffer, "message %u", n);
    lora_click_send(buffer, strlen(buffer));
    ++n;
}

int main(int argc, char** argv)
{
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
    lora_click_enable(lora_click_get_default_configuration());

     while (1) {
        if (mode == 's') {
            send();
            sleep(1);
        }
        else if (mode == 'r')
            receive();
    }

    uart_release();

    return 0;
}
