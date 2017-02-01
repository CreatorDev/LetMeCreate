/**
 * This example shows how to use the Alphanum Click wrapper of the LetMeCreate
 * to display characters.
 *
 * It displays "Ci" during 5 seconds on the Alphanum Click.
 *
 * The Alphanum Click must be inserted in Mikrobus 1 before running this
 * program.
 */

#include <unistd.h>
#include <letmecreate/letmecreate.h>


int main(void)
{
    spi_init();
    spi_select_bus(MIKROBUS_1);

    if (alphanum_click_init(MIKROBUS_1) < 0)
        return -1;

    if (alphanum_click_write('C', 'i') < 0)
        return -1;

    sleep(5);

    alphanum_click_release();
    spi_release();

    return 0;
}
