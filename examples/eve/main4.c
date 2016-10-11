/**
 * This example shows how to display a JPEG image using the EVE Click wrapper
 * of the LetMeCreate library.
 *
 * It displays an image and exits after 10 seconds.
 *
 * Before running this program:
 *   - the EVE Click must be inserted in Mikrobus 1
 *   - a WQVGA screen must be connected to the EVE Click.
 */

#include <linux/spi/spidev.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <letmecreate/letmecreate.h>

#define IMAGE_MEMORY_ADDRESS    (0)
#define IMAGE_WIDTH             (256)
#define IMAGE_HEIGHT            (180)

static uint8_t *buffer = NULL;
static uint32_t buffer_length = 0;

static int load_image_from_file(char *filepath)
{
    FILE *file = fopen(filepath, "rb");
    if (file == NULL)
        return -1;

    fseek(file, 0, SEEK_END);
    buffer_length = ftell(file);
    rewind(file);

    buffer = malloc(buffer_length);
    if (buffer == NULL) {
        fclose(file);
        return -1;
    }

    if (fread(buffer, 1, buffer_length, file) != buffer_length) {
        fclose(file);
        return -1;
    }
    fclose(file);

    return 0;
}

int main(void)
{
    if (load_image_from_file("/usr/bin/letmecreate_examples/data/image.jpg") < 0) {
        fprintf(stderr, "Failed to load image.\n");
        return -1;
    }

    spi_init();
    spi_set_mode(MIKROBUS_1, SPI_MODE_0);

    if (eve_click_enable(MIKROBUS_1) < 0) {
        free(buffer);
        return -1;
    }

    /* First, the image must be decompressed by the FT800 chip and store in the
     * memory of the FT800.
     */
    eve_click_load_image(IMAGE_MEMORY_ADDRESS,     /* Address in memory to store decompressed image */
                         FT800_OPT_RGB565,
                         buffer,
                         buffer_length);

    /* Once the image is stored in FT800 memory, there is no need to keep image
     * in Ci40's memory.
     */
    free(buffer);

    eve_click_clear(0, 0, 0);

    /* This specifies where the image is stored in the memory of the FT800 chip.
     * It must match with the first argument of eve_click_load_image
     */
    eve_click_draw(FT800_BITMAP_SOURCE, IMAGE_MEMORY_ADDRESS);

    /* Each pixel takes 2 bytes (5bits for red, 6bits for green and 5bits for
     * blue). Hence, each line of the image takes 2*IMAGE_WIDTH bytes.
     */
    eve_click_draw(FT800_BITMAP_LAYOUT, FT800_RGB565, IMAGE_WIDTH*2, IMAGE_HEIGHT);

    /* This configures which part of the image to draw and how to draw it.
     * Since we display the entire image, the last two arguments are equal to
     * the size of the image but they could be smaller or bigger */
    eve_click_draw(FT800_BITMAP_SIZE, FT800_NEAREST, FT800_BORDER, FT800_BORDER, IMAGE_WIDTH, IMAGE_HEIGHT);


    eve_click_draw(FT800_BEGIN, FT800_BITMAPS);
    eve_click_draw(FT800_VERTEX2II,
                   100,     /* x */
                   50,     /* y */
                   0,       /* handle */
                   0        /* cell */
                   );
    eve_click_draw(FT800_END);

    eve_click_draw(FT800_TEXT,
                   240,                 /* x */
                   26,                  /* y */
                   23,                  /* font */
                   FT800_OPT_CENTER,    /* options */
                  "Example 4");

    eve_click_draw(FT800_TEXT,
                   240,                 /* x */
                   245,                  /* y */
                   20,                  /* font */
                   FT800_OPT_CENTER,    /* options */
                  "J. M. W. Turner - The battle of Trafalgar (1822)");

    eve_click_display();

    sleep(10);

    eve_click_disable(MIKROBUS_1);
    spi_release();

    return 0;
}
