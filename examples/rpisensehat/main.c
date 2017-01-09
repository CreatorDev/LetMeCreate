#include <stdio.h>
#include <unistd.h>
#include <letmecreate/letmecreate.h>

int main(void)
{
    float t = 0.f, h = 0.f, p = 0.f;

    i2c_init();
    rpisensehat_init();

    rpisensehat_get_temperature(&t);
    rpisensehat_get_humidity(&h);
    rpisensehat_get_pressure(&p);

    printf("temperature: %f degrees celsius\n", t);
    printf("humidity: %f%%\n", h);
    printf("pressure: %fhPa\n", p);

    rpisensehat_display_rainbow();

    sleep(5);

    rpisensehat_release();
    i2c_release();

    return 0;
}
