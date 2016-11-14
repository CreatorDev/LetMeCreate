#include <stdio.h>
#include <string.h>
#include <letmecreate/letmecreate.h>


int main(void)
{
    char *cmd = "sys get ver\r\n";
    uart_init();
    uart_set_baudrate(UART_BD_57600);
    uart_send(cmd, strlen(cmd));

    while (1) {
        char tmp;
        uart_receive((uint8_t*)&tmp, sizeof(tmp));
        printf("%c\n", tmp);
    }

    uart_release();
    return 0;
}
