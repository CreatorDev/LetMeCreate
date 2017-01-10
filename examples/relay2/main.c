/**
 * This examples shows how to use the Relay2 Click wrapper of the LetMeCreate
 *library.
 *
 * It switches on/off relays in the following order:
 *    - relay 1 off, relay 2 off
 *    - relay 1 on, relay 2 off
 *    - relay 1 off, relay 2 on
 *    - relay 1 on, relay 2 on
 *
 * Relay2 Click must be inserted in mikrobus 1 before running this program.
 */

#include <unistd.h>
#include <letmecreate/letmecreate.h>

int main(void)
{
    int counter = 0;
    while (1) {
        switch (counter % 4) {
        case 0:
            relay2_click_disable_relay(MIKROBUS_1, RELAY2_CLICK_RELAY_1);
            relay2_click_disable_relay(MIKROBUS_1, RELAY2_CLICK_RELAY_2);
            break;
        case 1:
            relay2_click_enable_relay(MIKROBUS_1, RELAY2_CLICK_RELAY_1);
            relay2_click_disable_relay(MIKROBUS_1, RELAY2_CLICK_RELAY_2);
            break;
        case 2:
            relay2_click_disable_relay(MIKROBUS_1, RELAY2_CLICK_RELAY_1);
            relay2_click_enable_relay(MIKROBUS_1, RELAY2_CLICK_RELAY_2);
            break;
        case 3:
            relay2_click_enable_relay(MIKROBUS_1, RELAY2_CLICK_RELAY_1);
            relay2_click_enable_relay(MIKROBUS_1, RELAY2_CLICK_RELAY_2);
            break;
        }

        sleep(1);
        ++counter;
    }
}
