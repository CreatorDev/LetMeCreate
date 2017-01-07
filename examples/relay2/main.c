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
