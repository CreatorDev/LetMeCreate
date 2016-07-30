#include <unistd.h>
#include <letmecreate/letmecreate.h>

int main(void)
{
    int counter = 0;
    while (1) {
        switch (counter % 4) {
        case 0:
            relay2_click_disable_relay_1(MIKROBUS_1);
            relay2_click_disable_relay_2(MIKROBUS_1);
            break;
        case 1:
            relay2_click_enable_relay_1(MIKROBUS_1);
            relay2_click_disable_relay_2(MIKROBUS_1);
            break;
        case 2:
            relay2_click_disable_relay_1(MIKROBUS_1);
            relay2_click_enable_relay_2(MIKROBUS_1);
            break;
        case 3:
            relay2_click_enable_relay_1(MIKROBUS_1);
            relay2_click_enable_relay_2(MIKROBUS_1);
            break;
        }

        sleep (1);
        ++counter;
    }
}
