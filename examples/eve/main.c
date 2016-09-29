/**
 * This example shows some basic features of the EVE Click wrapper of the
 * LetMeCreate library.
 *
 * It displays a green screen for three seconds, then displays some text for
 * another three seconds.
 *
 * Before running this program:
 *   - the EVE Click must be inserted in Mikrobus 1
 *   - a WQVGA screen must be connected to the EVE Click.
 */
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <letmecreate/letmecreate.h>


int main(void)
{
    spi_init();
    spi_set_mode(MIKROBUS_1, SPI_MODE_0);

    if (eve_click_enable(MIKROBUS_1) < 0)
        return -1;

    /* Display a green screen for 3 seconds */
    eve_click_clear(0, 255, 0);
    eve_click_display();

    sleep(3);

    /* Display some text */
    eve_click_clear(0, 0, 0);   /* black screen */
    eve_click_draw(FT800_TEXT,
                   240,                 /* x */
                   136,                 /* y */
                   31,                  /* font */
                   FT800_OPT_CENTER,    /* options */
                  "Hello World !");
    eve_click_display();

    sleep(3);

    eve_click_disable(MIKROBUS_1);
    spi_release();

    return 0;
}
