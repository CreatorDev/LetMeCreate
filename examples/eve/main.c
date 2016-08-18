#include <linux/spi/spidev.h>
#include <stdio.h>
#include <unistd.h>
#include <letmecreate/letmecreate.h>


int main(void)
{
    spi_init();
    spi_set_mode(MIKROBUS_1, SPI_MODE_0);

    if (eve_click_enable(MIKROBUS_1) < 0)
        return -1;

    /* Display a green screen */
    eve_click_clear(0, 255, 0);
    eve_click_display();

    sleep(3);
    eve_click_disable(MIKROBUS_1);

    spi_release();

    return 0;
}
