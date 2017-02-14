#include <stddef.h>
#include <time.h>
#include <letmecreate/click/common.h>
#include <letmecreate/core/spi.h>


int spi_write_register(uint8_t reg_address, uint8_t data)
{
    uint8_t tx_buffer[2];

    tx_buffer[0] = reg_address;
    tx_buffer[1] = data;
    return spi_transfer(tx_buffer, NULL, sizeof(tx_buffer));
}

void sleep_ms(uint32_t ms)
{
    struct timespec rem, req;
    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms - req.tv_sec * 1000) * 1000 * 1000;

    while (nanosleep(&req, &rem))
        req = rem;
}
