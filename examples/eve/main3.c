/**
 * This example shows some graphic capabilities of the EVE Click wrapper of the
 * LetMeCreate library. You can find more documentation about these commands in
 * the programmer guide available on FTDI website:
 * http://www.ftdichip.com/Support/Documents/ProgramGuides/FT800%20Programmers%20Guide.pdf
 *
 * It displays some text, a button, a gauge, a clock, a dial and a toogle. After
 *  10 seconds, it exits.
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

    eve_click_clear(0, 0, 0);
    eve_click_draw(FT800_TEXT,
                   240,                 /* x */
                   36,                  /* y */
                   23,                  /* font */
                   FT800_OPT_CENTER,    /* options */
                  "Example 3");

    eve_click_draw(FT800_BUTTON,
                   2,       /* x */
                   200,     /* y */
                   76,      /* width */
                   56,      /* height */
                   26,      /* font */
                   0,       /* options, 0 means no options */
                  "button");

    eve_click_draw(FT800_GAUGE,
                   130,     /* x */
                   220,     /* y */
                   30,      /* radius */
                   0,       /* options */
                   5,       /* Number of major subdivisions */
                   4,       /* Number of minor subdivisions */
                   30,      /* value */
                   100      /* range */
                   );

    eve_click_draw(FT800_CLOCK,
                   234,     /* x */
                   215,     /* y */
                   40,      /* radius */
                   0,       /* options */
                   8,       /* hours */
                   32,      /* minutes */
                   15,      /* seconds */
                   400      /* milliseconds */
                  );

    eve_click_draw(FT800_DIAL,
                   335,         /* x */
                   220,         /* y */
                   30,          /* radius */
                   0,           /* options */
                   40000        /* value in range 0..65535 */
                  );

    eve_click_draw(FT800_TOGGLE,
                   405,         /* x */
                   215,         /* y */
                   30,          /* width */
                   22,          /* font */
                   0,           /* options */
                   0,           /* state, 0 for off, 65535 for on */
                   "on""\xff""off");

    eve_click_display();

    sleep(10);

    eve_click_disable(MIKROBUS_1);
    spi_release();

    return 0;
}
