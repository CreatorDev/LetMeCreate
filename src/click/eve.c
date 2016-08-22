#include <linux/spi/spidev.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "letmecreate/click/eve.h"
#include "letmecreate/click/ft800_defs.h"
#include "letmecreate/core/common.h"
#include "letmecreate/core/gpio.h"
#include "letmecreate/core/gpio_monitor.h"
#include "letmecreate/core/spi.h"

#define MAX_FIFO_SIZE       (4096)

static bool ft800_enabled = false;

/* Co-processor FIFO */
static uint32_t cmds[1024];
static uint32_t cmd_cnt = 0;
static bool cmd_buffering = true;

static void sleep_ms(unsigned int ms)
{
    struct timespec rem, req;
    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms - req.tv_sec * 1000) * 1000000;

    while (nanosleep(&req, &rem))
        req = rem;
}

static int send_command(uint8_t cmd)
{
    uint8_t buffer[3];
    buffer[0] = cmd;
    if (cmd != FT800_ACTIVE)
        buffer[0] |= FT800_CMD;
    buffer[1] = 0;
    buffer[2] = 0;

    return spi_transfer(buffer, NULL, sizeof(buffer));
}

/* TODO: add extra parameters for dest buffer */
static int write_str_to_cmd_fifo(char *str)
{
    uint32_t str_length = 0;

    if (str == NULL)
        return -1;

    str_length = strlen(str) + 1;
    /* TODO: add check that there is enough space in dest */

    strcpy((char*)(&cmds[cmd_cnt]), str);

    return (str_length + 3) / 4;
}

static bool is_display_list_cmd(uint32_t cmd)
{
    switch (cmd & 0xC0000000) {
    case FT800_VERTEX2F:
    case FT800_VERTEX2II:
        return true;
    }

    switch (cmd & 0xFF000000) {
    case FT800_ALPHA_FUNC:
    case FT800_BEGIN:
    case FT800_BITMAP_HANDLE:
    case FT800_BITMAP_LAYOUT:
    case FT800_BITMAP_SIZE:
    case FT800_BITMAP_SOURCE:
    case FT800_BITMAP_TRANSFORM_A:
    case FT800_BITMAP_TRANSFORM_B:
    case FT800_BITMAP_TRANSFORM_C:
    case FT800_BITMAP_TRANSFORM_D:
    case FT800_BITMAP_TRANSFORM_E:
    case FT800_BLEND_FUNC:
    case FT800_CALL:
    case FT800_CLEAR:
    case FT800_CLEAR_COLOR_A:
    case FT800_CLEAR_COLOR_RGB:
    case FT800_CLEAR_STENCIL:
    case FT800_COLOR_A:
    case FT800_COLOR_MASK:
    case FT800_COLOR_RGB:
    case FT800_DISPLAY:
    case FT800_END:
    case FT800_JUMP:
    case FT800_LINE_WIDTH:
    case FT800_MACRO:
    case FT800_POINT_SIZE:
    case FT800_RESTORE_CONTEXT:
    case FT800_RETURN:
    case FT800_SAVE_CONTEXT:
    case FT800_SCISSOR_SIZE:
    case FT800_SCISSOR_XY:
    case FT800_STENCIL_FUNC:
    case FT800_STENCIL_MASK:
    case FT800_STENCIL_OP:
    case FT800_TAG:
    case FT800_TAG_MASK:
        return true;
    default:
        return false;
    }
}

static bool is_coprocessor_gfx_cmd(uint32_t cmd)
{
    switch (cmd) {
    case FT800_BGCOLOR:
    case FT800_BUTTON:
    case FT800_CLOCK:
    case FT800_DIAL:
    case FT800_FGCOLOR:
    case FT800_GAUGE:
    case FT800_GRADCOLOR:
    case FT800_GRADIENT:
    case FT800_KEYS:
    case FT800_NUMBER:
    case FT800_PROGRESS:
    case FT800_SCROLLBAR:
    case FT800_SLIDER:
    case FT800_TEXT:
    case FT800_TOGGLE:
        return true;
    default:
        return false;
    }
}

static int parse_display_list_cmd(uint32_t cmd, va_list args)
{
    switch (cmd & 0xFF000000) {
    case FT800_ALPHA_FUNC:
        cmds[cmd_cnt++] = FT800_ALPHA_FUNC
                        | ((va_arg(args, int) & 0x7) << 8)
                        | (va_arg(args, int) & 0xFF);
        return 1;
    case  FT800_BEGIN:
        cmds[cmd_cnt++] = FT800_BEGIN
                        | (va_arg(args, int) & 0xF);
        return 1;
    case FT800_BITMAP_HANDLE:
        cmds[cmd_cnt++] = FT800_BITMAP_HANDLE
                        | (va_arg(args, int) & 0x1F);
        return 1;
    case FT800_BITMAP_LAYOUT:
        cmds[cmd_cnt++] = FT800_BITMAP_LAYOUT
                        | ((va_arg(args, int) & 0x1F) << 19)
                        | ((va_arg(args, int) & 0x3FF) << 9)
                        | (va_arg(args, int) & 0x1FF);
        return 1;
    case FT800_BITMAP_SIZE:
        cmds[cmd_cnt++] = FT800_BITMAP_SIZE
                        | ((va_arg(args, int) & 0x1) << 20)
                        | ((va_arg(args, int) & 0x1) << 19)
                        | ((va_arg(args, int) & 0x1) << 18)
                        | ((va_arg(args, int) & 0x1FF) << 9)
                        | (va_arg(args, int) & 0x1FF);
        return 1;
    case FT800_BITMAP_SOURCE:
        cmds[cmd_cnt++] = FT800_BITMAP_SOURCE
                        | (va_arg(args, int) & 0xFFFFF);
        return 1;
    case FT800_BITMAP_TRANSFORM_A:
    case FT800_BITMAP_TRANSFORM_B:
    case FT800_BITMAP_TRANSFORM_C:
    case FT800_BITMAP_TRANSFORM_D:
    case FT800_BITMAP_TRANSFORM_E:
        return 1;
    case FT800_BLEND_FUNC:
        cmds[cmd_cnt++] = FT800_BLEND_FUNC
                        | ((va_arg(args, int) & 0x7) << 3)
                        | (va_arg(args, int) & 0x7);
        return 1;
    case FT800_CALL:
        cmds[cmd_cnt++] = FT800_CALL
                        | (va_arg(args, int) & 0xFFFF);
        return 1;
    case FT800_CLEAR:
        cmds[cmd_cnt++] = FT800_CLEAR
                        | ((va_arg(args, int) & 0x01) << 2)
                        | ((va_arg(args, int) & 0x01) << 1)
                        | (va_arg(args, int) & 0x01);
        return 1;
    case FT800_CLEAR_COLOR_A:
        cmds[cmd_cnt++] = FT800_CLEAR_COLOR_A
                        | (va_arg(args, int) & 0xFF);
        return 1;
    case FT800_CLEAR_COLOR_RGB:
        cmds[cmd_cnt++] = FT800_CLEAR_COLOR_RGB
                        | ((va_arg(args, int) & 0xFF) << 16)
                        | ((va_arg(args, int) & 0xFF) << 8)
                        | (va_arg(args, int) & 0xFF);
        return 1;
    case FT800_CLEAR_STENCIL:
        cmds[cmd_cnt++] = FT800_CLEAR_STENCIL
                        | (va_arg(args, int) & 0xFF);
        return 1;
    case FT800_COLOR_A:
        cmds[cmd_cnt++] = FT800_COLOR_A
                        | (va_arg(args, int) & 0xFF);
        return 1;
    case FT800_COLOR_MASK:
        cmds[cmd_cnt++] = FT800_COLOR_MASK
                        | ((va_arg(args, int) & 0x1) << 3)
                        | ((va_arg(args, int) & 0x1) << 2)
                        | ((va_arg(args, int) & 0x1) << 1)
                        | (va_arg(args, int) & 0x1);
        return 1;
    case FT800_COLOR_RGB:
        cmds[cmd_cnt++] = FT800_COLOR_RGB
                        | ((va_arg(args, int) & 0xFF) << 16)
                        | ((va_arg(args, int) & 0xFF) << 8)
                        | (va_arg(args, int) & 0xFF);
        return 1;
    case FT800_DISPLAY:
        cmds[cmd_cnt++] = FT800_DISPLAY;
        return 1;
    case FT800_END:
        cmds[cmd_cnt++] = FT800_END;
        return 1;
    case FT800_JUMP:
        cmds[cmd_cnt++] = FT800_JUMP
                        | (va_arg(args, int) & 0xFFFF);
        return 1;
    case FT800_LINE_WIDTH:
        cmds[cmd_cnt++] = FT800_LINE_WIDTH
                        | (va_arg(args, int) & 0x3FF);
        return 1;
    case FT800_MACRO:
        cmds[cmd_cnt++] = FT800_MACRO
                        | (va_arg(args, int) & 0x1);
        return 1;
    case FT800_POINT_SIZE:
        cmds[cmd_cnt++] = FT800_POINT_SIZE
                        | (va_arg(args, int) & 0x1FFFF);
        return 1;
    case FT800_RESTORE_CONTEXT:
        cmds[cmd_cnt++] = FT800_RESTORE_CONTEXT;
        return 1;
    case FT800_RETURN:
        cmds[cmd_cnt++] = FT800_RETURN;
        return 1;
    case FT800_SAVE_CONTEXT:
        cmds[cmd_cnt++] = FT800_SAVE_CONTEXT;
        return 1;
    case FT800_SCISSOR_SIZE:
        cmds[cmd_cnt++] = FT800_SCISSOR_SIZE
                        | ((va_arg(args, int) & 0x3FF) << 10)
                        | (va_arg(args, int) & 0x3FF);
        return 1;
    case FT800_SCISSOR_XY:
        cmds[cmd_cnt++] = FT800_SCISSOR_XY
                        | ((va_arg(args, int) & 0x1FF) << 9)
                        | (va_arg(args, int) & 0x1FF);
        return 1;
    case FT800_STENCIL_FUNC:
        cmds[cmd_cnt++] = FT800_STENCIL_FUNC
                        | ((va_arg(args, int) & 0x0F) << 16)
                        | ((va_arg(args, int) & 0xFF) << 8)
                        | (va_arg(args, int) & 0xFF);
        return 1;
    case FT800_STENCIL_MASK:
        cmds[cmd_cnt++] = FT800_STENCIL_MASK
                        | (va_arg(args, int) & 0xFF);
        return 1;
    case FT800_STENCIL_OP:
        cmds[cmd_cnt++] = FT800_STENCIL_OP
                        | ((va_arg(args, int) & 0x7) << 3)
                        | (va_arg(args, int) & 0x7);
        return 1;
    case FT800_TAG:
        cmds[cmd_cnt++] = FT800_TAG
                        | (va_arg(args, int) & 0xFF);
        return 1;
    case FT800_TAG_MASK:
        cmds[cmd_cnt++] = FT800_TAG_MASK
                        | (va_arg(args, int) & 0x1);
        return 1;
    }

    switch (cmd & 0xC0000000) {
    case FT800_VERTEX2F:
        cmds[cmd_cnt++] = FT800_VERTEX2F
                        | ((va_arg(args, int) & 0x7FFF) << 15)
                        | (va_arg(args, int) & 0x7FFF);
        return 1;
    case FT800_VERTEX2II:
        cmds[cmd_cnt++] = FT800_VERTEX2II
                        | ((va_arg(args, int) & 0x1FF) << 21)
                        | ((va_arg(args, int) & 0x1FF) << 12)
                        | ((va_arg(args, int) & 0x1F) << 7)
                        | (va_arg(args, int) & 0x7F);
        return 1;
    }

    return -1;
}

static int parse_display_list_vcmd(uint32_t cmd, ...)
{
    int ret = -1;
    va_list args;

    va_start(args, cmd);
    ret = parse_display_list_cmd(cmd, args);
    va_end(args);

    return ret;
}

static int parse_16bits_arg(uint16_t arg, uint32_t offset)
{
    uint8_t *dest = (uint8_t*)(&cmds[cmd_cnt]);
    /* Check that there is enough space in cmds array */
    memcpy(&dest[offset], &arg, 2);
    return 0;
}

static int parse_32bits_arg(uint32_t arg, uint32_t offset)
{
    uint8_t *dest = (uint8_t*)(&cmds[cmd_cnt]);
    /* Check that there is enough space in cmds array */
    memcpy(&dest[offset], &arg, 4);
    return 0;
}

static int parse_font(uint16_t font, uint32_t offset)
{
    /* TODO add checks */

    uint8_t *dest = (uint8_t*)(&cmds[cmd_cnt]);
    memcpy(&dest[offset], &font, 2);
    return 0;
}

static int parse_options(uint16_t options, uint32_t offset)
{
    /* TODO add checks */

    uint8_t *dest = (uint8_t*)(&cmds[cmd_cnt]);
    memcpy(&dest[offset], &options, 2);
    return 0;
}

static int parse_str(char *str, uint32_t offset)
{
    char *dest = (char*)(&cmds[cmd_cnt]);

    /* TODO add checks */
    strcpy(&dest[offset], str);

    return (strlen(str) / 4) + 1;
}

static int parse_coprocessor_cmd(uint32_t opcode, va_list args)
{
    /* Command length in words (4 bytes) */
    uint32_t cmd_length = 0;

    if (parse_32bits_arg(opcode, 0) < 0)
        return -1;
    ++cmd_length;

    /* Parse arguments of command */
    switch (opcode) {
    case FT800_BGCOLOR:
        if (parse_32bits_arg(va_arg(args, int), 4) < 0)   /* 24-bit color */
            return -1;
        ++cmd_length;
        break;
    case FT800_BUTTON:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_16bits_arg(va_arg(args, int), 8) < 0      /* w */
        ||  parse_16bits_arg(va_arg(args, int), 10) < 0     /* h */
        ||  parse_font(va_arg(args, int), 12) < 0
        ||  parse_options(va_arg(args, int), 14) < 0)
            return -1;
        cmd_length += 3;
        {
            int tmp = parse_str(va_arg(args, char*), 16);
            if (tmp < 0)
                return -1;
            cmd_length += tmp;
        }
        break;
    case FT800_CLOCK:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_16bits_arg(va_arg(args, int), 8) < 0      /* radius */
        ||  parse_options(va_arg(args, int), 10) < 0
        ||  parse_16bits_arg(va_arg(args, int), 12) < 0     /* hour */
        ||  parse_16bits_arg(va_arg(args, int), 14) < 0     /* minute */
        ||  parse_16bits_arg(va_arg(args, int), 16) < 0     /* second */
        ||  parse_16bits_arg(va_arg(args, int), 18) < 0)    /* millisecond */
            return -1;
        cmd_length += 4;
        break;
    case FT800_COLDSTART:
        break;
    case FT800_DIAL:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_16bits_arg(va_arg(args, int), 8) < 0      /* radius */
        ||  parse_options(va_arg(args, int), 10) < 0
        ||  parse_16bits_arg(va_arg(args, int), 12) < 0)    /* value */
            return -1;
        cmd_length += 3;
        break;
    case FT800_DLSTART:
        break;
    case FT800_FGCOLOR:
        if (parse_32bits_arg(va_arg(args, int), 4) < 0)   /* 24-bit color */
            return -1;
        ++cmd_length;
        break;
    case FT800_GAUGE:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_16bits_arg(va_arg(args, int), 8) < 0      /* radius */
        ||  parse_options(va_arg(args, int), 10) < 0
        ||  parse_16bits_arg(va_arg(args, int), 12) < 0     /* major */
        ||  parse_16bits_arg(va_arg(args, int), 14) < 0     /* minor */
        ||  parse_16bits_arg(va_arg(args, int), 16) < 0     /* value */
        ||  parse_16bits_arg(va_arg(args, int), 18) < 0)    /* range */
            return -1;
        cmd_length += 4;
        break;
    case FT800_GRADCOLOR:
        if (parse_32bits_arg(va_arg(args, int), 4) < 0)   /* 24-bit color */
            return -1;
        ++cmd_length;
        break;
    case FT800_GRADIENT:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x0 */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y0 */
        ||  parse_32bits_arg(va_arg(args, int), 8) < 0      /* 24-bit color 0 */
        ||  parse_16bits_arg(va_arg(args, int), 12) < 0     /* x1 */
        ||  parse_16bits_arg(va_arg(args, int), 14) < 0     /* y1 */
        ||  parse_32bits_arg(va_arg(args, int), 16) < 0)    /* 24-bit color 1 */
            return -1;
        cmd_length += 4;
        break;
    case FT800_INTERRUPT:
        if (parse_32bits_arg(va_arg(args, int), 4) < 0)     /* delay in ms */
            return -1;
        ++cmd_length;
        break;
    case FT800_KEYS:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_16bits_arg(va_arg(args, int), 8) < 0      /* width */
        ||  parse_16bits_arg(va_arg(args, int), 10) < 0     /* height */
        ||  parse_font(va_arg(args, int), 12) < 0
        ||  parse_options(va_arg(args, int), 14) < 0)
            return -1;
        cmd_length += 3;
        {
            int tmp = parse_str(va_arg(args, char*), 16);
            if (tmp < 0)
                return -1;
            cmd_length += tmp;
        }
        break;
    case FT800_LOADIDENTITY:
        break;
    case FT800_NUMBER:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_font(va_arg(args, int), 8) < 0
        ||  parse_options(va_arg(args, int), 10) < 0
        ||  parse_32bits_arg(va_arg(args, int), 12) < 0)   /* n */
            return -1;
        cmd_length += 3;
        break;
    case FT800_PROGRESS:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_16bits_arg(va_arg(args, int), 8) < 0      /* width */
        ||  parse_16bits_arg(va_arg(args, int), 10) < 0     /* height */
        ||  parse_options(va_arg(args, int), 12) < 0
        ||  parse_16bits_arg(va_arg(args, int), 14) < 0     /* value */
        ||  parse_16bits_arg(va_arg(args, int), 16) < 0)    /* range */
            return -1;
        cmd_length += 4;
        break;
    case FT800_ROTATE:
        if (parse_32bits_arg(va_arg(args, int), 4) < 0)     /* angle */
            return -1;
        ++cmd_length;
        break;
    case FT800_SCALE:
        if (parse_32bits_arg(va_arg(args, int), 4) < 0      /* x scale factor */
        ||  parse_32bits_arg(va_arg(args, int), 8) < 0)     /* y scale factor */
            return -1;
        cmd_length += 2;
        break;
    case FT800_SCREENSAVER:
    case FT800_SCROLLBAR:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_16bits_arg(va_arg(args, int), 8) < 0      /* width */
        ||  parse_16bits_arg(va_arg(args, int), 10) < 0     /* height */
        ||  parse_options(va_arg(args, int), 12) < 0
        ||  parse_16bits_arg(va_arg(args, int), 14) < 0     /* value */
        ||  parse_16bits_arg(va_arg(args, int), 16) < 0     /* size */
        ||  parse_16bits_arg(va_arg(args, int), 18) < 0)    /* range */
            return -1;
        cmd_length += 4;
        break;
    case FT800_SETMATRIX:
        break;
    case FT800_SKETCH:
    case FT800_SLIDER:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_16bits_arg(va_arg(args, int), 8) < 0      /* width */
        ||  parse_16bits_arg(va_arg(args, int), 10) < 0     /* height */
        ||  parse_options(va_arg(args, int), 12) < 0
        ||  parse_16bits_arg(va_arg(args, int), 14) < 0     /* value */
        ||  parse_16bits_arg(va_arg(args, int), 16) < 0)    /* range */
            return -1;
        cmd_length += 4;
        break;
    case FT800_TEXT:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_font(va_arg(args, int), 8) < 0
        ||  parse_options(va_arg(args, int), 10) < 0)
            return -1;
        cmd_length += 2;
        {
            int tmp = parse_str(va_arg(args, char*), 12);
            if (tmp < 0)
                return -1;
            cmd_length += tmp;
        }
        break;
    case FT800_TOGGLE:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_16bits_arg(va_arg(args, int), 8) < 0      /* w */
        ||  parse_font(va_arg(args, int), 10) < 0
        ||  parse_options(va_arg(args, int), 12) < 0)
            return -1;

        {
            uint16_t state = va_arg(args, int);
            /* TODO: add checks */
            if (parse_16bits_arg(state, 14) < 0)
                return -1;
        }
        cmd_length += 3;
        {
            int tmp = parse_str(va_arg(args, char*), 16);
            if (tmp < 0)
                return -1;
            cmd_length += tmp;
        }
        break;
    case FT800_TRACK:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_16bits_arg(va_arg(args, int), 8) < 0      /* width */
        ||  parse_16bits_arg(va_arg(args, int), 10) < 0     /* height */
        ||  parse_16bits_arg(va_arg(args, int), 12) < 0)    /* tag */
            return -1;
        cmd_length += 3;
        break;
    case FT800_TRANSLATE:
        if (parse_32bits_arg(va_arg(args, int), 4) < 0      /* tx */
        ||  parse_32bits_arg(va_arg(args, int), 8) < 0)     /* ty */
            return -1;
        cmd_length += 2;
        break;
    }

    cmd_cnt += cmd_length;
    return cmd_length;
}

static int parse_coprocessor_vcmd(uint32_t opcode, ...)
{
    int ret = -1;
    va_list args;
    va_start(args, opcode);
    ret = parse_coprocessor_cmd(opcode, args);
    va_end(args);
    return ret;
}

static int memory_write(uint32_t address, const void *data, uint32_t size)
{
    uint8_t *buffer = NULL;
    uint32_t buffer_size = size + 3;

    /* Check if address can be stored in 22 bits */
    if (address >= 0x400000)
        return -1;

    buffer = malloc(buffer_size);
    if (buffer == NULL)
        return -1;
    buffer[0] = FT800_MEM_WRITE | (address >> 16);
    buffer[1] = address >> 8;
    buffer[2] = address;

    memcpy(&buffer[3], data, size);

    return spi_transfer(buffer, NULL, buffer_size);
}

static int memory_read(uint32_t address, void *data, uint32_t size)
{
    uint8_t *tx_buffer = NULL;
    uint8_t *rx_buffer = NULL;
    uint32_t buffer_size = size + 4;

    tx_buffer = malloc(buffer_size);
    if (tx_buffer == NULL) {
        fprintf(stderr, "eve: Could not allocate memory for tx buffer.\n");
        return -1;
    }
    rx_buffer = malloc(buffer_size);
    if (rx_buffer == NULL) {
        fprintf(stderr, "eve: Could not allocate memory for rx buffer.\n");
        free(tx_buffer);
        return -1;
    }

    tx_buffer[0] = FT800_MEM_READ | (address >> 16);
    tx_buffer[1] = address >> 8;
    tx_buffer[2] = address;
    memset(&tx_buffer[3], 0, buffer_size - 3);

    if (spi_transfer(tx_buffer, rx_buffer, buffer_size) < 0) {
        fprintf(stderr, "eve: Error during spi transfer while reading from memory.\n");
        return -1;
    }

    memcpy(data, &rx_buffer[4], size);

    return 0;
}

static int write_8bit_reg(uint32_t reg_address, uint8_t value)
{
    return memory_write(reg_address, &value, sizeof(value));
}

static int write_16bit_reg(uint32_t reg_address, uint16_t value)
{
    return memory_write(reg_address, (uint8_t*)&value, sizeof(value));
}

static int write_32bit_reg(uint32_t reg_address, uint32_t value)
{
    return memory_write(reg_address, (uint8_t*)&value, sizeof(value));
}

static int read_8bit_reg(uint32_t reg_address, uint8_t *value)
{
    return memory_read(reg_address, (uint8_t*)value, 1);
}

static int read_16bit_reg(uint32_t reg_address, uint16_t *value)
{
    return memory_read(reg_address, (uint8_t*)value, 2);
}

static int read_32bit_reg(uint32_t reg_address, uint32_t *value)
{
    return memory_read(reg_address, (uint8_t*)value, 4);
}

static void wait_for_coprocessor(void)
{
    uint16_t reg_cmd_read = 0, reg_cmd_write = 0;

    /* TODO: Add a timeout */
    do {
        read_16bit_reg(FT800_REG_CMD_WRITE, &reg_cmd_write);
        read_16bit_reg(FT800_REG_CMD_READ, &reg_cmd_read);
        reg_cmd_write &= 0xFFF;
        reg_cmd_read &= 0xFFF;

        if (reg_cmd_read == 0xFFF)
            printf("coprocessor engine fault\n");
    } while(reg_cmd_write != reg_cmd_read);
}

static uint16_t compute_fifo_freespace(void)
{
    uint16_t reg_cmd_write = 0, reg_cmd_read = 0;

    if (read_16bit_reg(FT800_REG_CMD_WRITE, &reg_cmd_write) < 0
    ||  read_16bit_reg(FT800_REG_CMD_READ, &reg_cmd_read) < 0)
        return 0;

    return (4096 - 4) - ((reg_cmd_write - reg_cmd_read) & 0xFFF);
}

static int cmd_fifo_send(uint32_t *cmd_buffer, uint32_t cmd_buffer_cnt)
{
    uint32_t i = 0, offset = 0;

    wait_for_coprocessor();

    while (cmd_buffer_cnt > 0) {
        uint32_t offset = 0;
        uint16_t fifo_freespace = compute_fifo_freespace() / 4;
        uint32_t chunk_length = cmd_buffer_cnt;

        if (chunk_length > fifo_freespace)
            chunk_length = fifo_freespace;

        if (read_32bit_reg(FT800_REG_CMD_WRITE, &offset) < 0)
            return -1;

        memory_write(FT800_RAM_CMD + offset, (uint8_t*)cmd_buffer, 4*chunk_length);

        offset += 4 * chunk_length;
        offset &= 0xFFC;

        if (write_32bit_reg(FT800_REG_CMD_WRITE, offset) < 0)
            return -1;

        wait_for_coprocessor();

        cmd_buffer_cnt -= chunk_length;
        cmd_buffer += chunk_length;
    }

    return 0;
}

static int ft800_power(uint8_t bus_index, bool enable)
{
    uint8_t pd_n_pin;

    switch (bus_index) {
    case MIKROBUS_1:
        pd_n_pin = MIKROBUS_1_RST;
        break;
    case MIKROBUS_2:
        pd_n_pin = MIKROBUS_2_RST;
        break;
    default:
        return -1;
    }

    if (gpio_init(pd_n_pin) < 0
    || gpio_set_direction(pd_n_pin, GPIO_OUTPUT) < 0
    || gpio_set_value(pd_n_pin, enable ? 1 : 0) < 0)
        return -1;

    sleep_ms(25);

    return 0;
}

static int ft800_use_spi(uint8_t bus_index)
{
    uint8_t mode_pin;

    switch (bus_index) {
    case MIKROBUS_1:
        mode_pin = MIKROBUS_1_AN;
        break;
    case MIKROBUS_2:
        mode_pin = MIKROBUS_2_AN;
        break;
    default:
        return -1;
    }

    if (gpio_init(mode_pin) < 0
    || gpio_set_direction(mode_pin, GPIO_OUTPUT) < 0
    || gpio_set_value(mode_pin, 0) < 0)
        return -1;

    return 0;
}

static int check_device_id(void)
{
    uint8_t device_id;

    if (memory_read(FT800_REG_ID, &device_id, 1) < 0) {
        fprintf(stderr, "eve: Failed to read REG_ID.\n");
        return -1;
    }
    if (device_id != FT800_DEVICE_ID) {
        fprintf(stderr, "eve: Invalid Device ID.\n");
        return -1;
    }

    return 0;
}

int eve_click_enable(uint8_t mikrobus_index)
{
    if (ft800_enabled)
        return 0;

    if (ft800_use_spi(mikrobus_index) < 0) {
        fprintf(stderr, "eve: Failed to configure ft800 chip to use spi.\n");
        return -1;
    }

    if (ft800_power(mikrobus_index, false) < 0
    ||  ft800_power(mikrobus_index, true) < 0) {
        fprintf(stderr, "eve: Failed to power up ft800 chip.\n");
        return -1;
    }

    if (send_command(FT800_ACTIVE) < 0) {
        fprintf(stderr, "eve: Failed to activate ft800 chip.\n");
        return -1;
    }

    /* Configure PLL */
    if (send_command(FT800_CLKEXT) < 0
    ||  send_command(FT800_CLK48M) < 0) {
        fprintf(stderr, "eve: Failed to enable PLL on ft800 chip.\n");
        return -1;
    }

    if (check_device_id() < 0)
        return -1;

    /* Turn off LCD screen */
    if (write_8bit_reg(FT800_REG_GPIO, 0x00) < 0) {
        fprintf(stderr, "eve: Failed to turn off LCD screen.\n");
        return -1;
    }

    /* Disable pixel clock output */
    if (write_8bit_reg(FT800_REG_PCLK, 0) < 0) {
        fprintf(stderr, "eve: Failed to disable pixel clock output.\n");
        return -1;
    }

    /* Clock settings for WQVGA screen */
    if (write_8bit_reg(FT800_REG_PCLK_POL, 1) < 0
    ||  write_16bit_reg(FT800_REG_HSIZE, 480) < 0
    ||  write_16bit_reg(FT800_REG_HCYCLE, 548) < 0
    ||  write_16bit_reg(FT800_REG_HOFFSET, 43) < 0
    ||  write_16bit_reg(FT800_REG_HSYNC0, 0) < 0
    ||  write_16bit_reg(FT800_REG_HSYNC1, 41) < 0
    ||  write_16bit_reg(FT800_REG_VSIZE, 272) < 0
    ||  write_16bit_reg(FT800_REG_VCYCLE, 292) < 0
    ||  write_16bit_reg(FT800_REG_VOFFSET, 12) < 0
    ||  write_16bit_reg(FT800_REG_VSYNC0, 0) < 0
    ||  write_16bit_reg(FT800_REG_VSYNC1, 10) < 0
    ||  write_8bit_reg(FT800_REG_CSPREAD, 0) < 0) {
        fprintf(stderr, "eve: Failed to set clock settings.\n");
        return -1;
    }

    eve_click_enable_buffering();


    /* Display black screen */
    cmd_cnt = 0;
    parse_coprocessor_vcmd(FT800_DLSTART);
    parse_display_list_vcmd(FT800_CLEAR_COLOR_RGB, 0, 0, 0);
    parse_display_list_vcmd(FT800_CLEAR, 1, 1, 1);
    parse_display_list_vcmd(FT800_DISPLAY);
    parse_coprocessor_vcmd(FT800_SWAP);
    if (cmd_fifo_send(cmds, cmd_cnt) < 0)
        return -1;

    /* Turn on LCD screen */
    if (write_8bit_reg(FT800_REG_GPIO_DIR, 0x80) < 0
    ||  write_8bit_reg(FT800_REG_GPIO, 0x80) < 0) {
        fprintf(stderr, "eve: Failed to turn on LCD screen.\n");
        return -1;
    }

    /* Configure backlighting to maximum */
    if (write_16bit_reg(FT800_REG_PCLK, 0x00FA) < 0
    ||  write_8bit_reg(FT800_REG_PWM_DUTY, 0x80) < 0) {
        fprintf(stderr, "eve: Failed to configure backlighting.\n");
        return -1;
    }

    /* Enable pixel clock output */
    if (write_8bit_reg(FT800_REG_PCLK, 5) < 0) {
        fprintf(stderr, "eve: Failed to enable pixel clock output.\n");
        return -1;
    }

    ft800_enabled = true;

    /* Wait 100 ms for co-processor */
    sleep_ms(100);

    return 0;
}

int eve_click_disable(uint8_t mikrobus_index)
{
    if (ft800_enabled == false)
        return 0;

    /* Disable pixel clock output */
    if (write_8bit_reg(FT800_REG_PCLK, 0) < 0) {
        fprintf(stderr, "eve: Failed to disable pixel clock output.\n");
        return -1;
    }

    /* Turn off LCD screen */
    if (write_8bit_reg(FT800_REG_GPIO, 0x00) < 0) {
        fprintf(stderr, "eve: Failed to turn off LCD screen.\n");
        return -1;
    }

    /* Disable backlighting */
    if (write_8bit_reg(FT800_REG_PWM_DUTY, 0x00) < 0) {
        fprintf(stderr, "eve: Failed to disable backlighting.\n");
        return -1;
    }

    /* Set chip in standby mode */
    if (send_command(FT800_STANDBY) < 0) {
        fprintf(stderr, "eve: Failed to set ft800 chip in standby mode.\n");
        return -1;
    }

    /* Power down eve click */
    if (ft800_power(mikrobus_index, false) < 0) {
        fprintf(stderr, "eve: Failed to power down ft800 chip.\n");
        return -1;
    }

    ft800_enabled = false;
    return 0;
}

void eve_click_disable_buffering(void)
{
    /* Flush buffer */
    if (cmd_cnt == 0)
        cmd_fifo_send(cmds, cmd_cnt);
    cmd_cnt = 0;

    cmd_buffering = false;
}

void eve_click_enable_buffering(void)
{
    cmd_buffering = true;
}

bool eve_click_is_buffering_enabled(void)
{
    return cmd_buffering;
}

int eve_click_clear(uint8_t r, uint8_t g, uint8_t b)
{
    if (ft800_enabled == false)
        return -1;

    cmd_cnt = 0;

    if (parse_coprocessor_vcmd(FT800_DLSTART) < 0
    ||  parse_display_list_vcmd(FT800_CLEAR_COLOR_RGB, r, g, b) < 0
    ||  parse_display_list_vcmd(FT800_CLEAR_COLOR_A, 0xFF) < 0
    ||  parse_display_list_vcmd(FT800_CLEAR, 1, 1, 1) < 0)
        return -1;

    return 0;
}

int eve_click_draw(uint32_t cmd, ...)
{
    int ret = -1;
    va_list args;

    if (ft800_enabled == false) {
        fprintf(stderr, "eve: Cannot append cmd when eve click is disabled.\n");
        return -1;
    }
    va_start(args, cmd);
    if (is_display_list_cmd(cmd))
        ret = parse_display_list_cmd(cmd, args);
    else if (is_coprocessor_gfx_cmd(cmd))
        ret = parse_coprocessor_cmd(cmd, args);
    va_end(args);

    if (ret < 0)
        return ret;

    if (!eve_click_is_buffering_enabled())
        return cmd_fifo_send(cmds, cmd_cnt);
     else
        return 0;
}

int eve_click_display(void)
{
    if (ft800_enabled == false)
        return -1;

    if (parse_display_list_vcmd(FT800_DISPLAY) < 0
    ||  parse_coprocessor_vcmd(FT800_SWAP) < 0)
        return -1;

    return cmd_fifo_send(cmds, cmd_cnt);
}

int eve_click_load_image(uint32_t ptr, uint32_t options, const uint8_t *data, uint32_t count)
{
    int ret = 0;
    uint32_t *buffer = NULL;
    uint32_t buffer_count = 0;

    if (ft800_enabled == false)
        return -1;

    if (data == NULL)
        return -1;

    if (count == 0)
        return 0;

    buffer_count = 3 + (count + 3) / 4;
    buffer = malloc(sizeof(uint32_t) * buffer_count);
    if (buffer == NULL)
        return -1;
    buffer[0] = FT800_LOADIMAGE;
    buffer[1] = ptr;
    buffer[2] = options;
    buffer[buffer_count - 1] = 0;
    memcpy(&buffer[3], data, count);

    if (cmd_fifo_send(buffer, buffer_count) < 0)
        ret = -1;

    free(buffer);

    return ret;
}

int eve_click_memcrc(uint32_t ptr, uint32_t byte_count, uint32_t *crc)
{
    uint16_t offset = 0;
    uint32_t buffer[4];

    if (ft800_enabled == false)
        return -1;

    if (crc == NULL)
        return -1;

    if (byte_count == 0)
        return 0;

    if (read_16bit_reg(FT800_REG_CMD_WRITE, &offset) < 0)
        return -1;

    buffer[0] = FT800_MEMCRC;
    buffer[1] = ptr;
    buffer[2] = byte_count;
    buffer[3] = 0;
    if (cmd_fifo_send(buffer, 4) < 0)
        return -1;

    return read_32bit_reg(FT800_RAM_CMD + offset + 12, crc);
}

int eve_click_memset(uint32_t ptr, uint32_t value, uint32_t byte_count)
{
    uint32_t buffer[4];
    if (ft800_enabled == false)
        return -1;

    if (byte_count == 0)
        return 0;

    buffer[0] = FT800_MEMSET;
    buffer[1] = ptr;
    buffer[2] = value;
    buffer[3] = byte_count;
    return cmd_fifo_send(buffer, 4);
}

int eve_click_memcpy(uint32_t dest, uint32_t src, uint32_t byte_count)
{
    uint32_t buffer[4];

    if (ft800_enabled == false)
        return -1;

    if (byte_count == 0)
        return 0;

    buffer[0] = FT800_MEMCPY;
    buffer[1] = dest;
    buffer[2] = src;
    buffer[3] = byte_count;
    return cmd_fifo_send(buffer, 4);
}

int eve_click_memzero(uint32_t ptr, uint32_t byte_count)
{
    uint32_t buffer[3];
    if (ft800_enabled == false)
        return -1;

    if (byte_count == 0)
        return 0;

    buffer[0] = FT800_MEMZERO;
    buffer[1] = ptr;
    buffer[2] = byte_count;
    return cmd_fifo_send(buffer, 3);
}

int eve_click_ftdi_logo(void)
{
    uint32_t cmd = FT800_LOGO;
    uint32_t offset;
    uint16_t reg_cmd_read = 0, reg_cmd_write = 0;

    if (ft800_enabled == false)
        return -1;

    if (read_32bit_reg(FT800_REG_CMD_WRITE, &offset) < 0)
        return -1;
    offset &= 0xFFC;

    if (write_32bit_reg(FT800_RAM_CMD + offset, cmd) < 0)
        return -1;
    offset += 4;
    if (write_32bit_reg(FT800_REG_CMD_WRITE, offset) < 0)
        return -1;

    sleep_ms(2500);

    /* Wait until REG_CMD_READ and REG_CMD_WRITE are equal to 0 */
    do {
        if (read_16bit_reg(FT800_REG_CMD_READ, &reg_cmd_read) < 0
        ||  read_16bit_reg(FT800_REG_CMD_WRITE, &reg_cmd_write) < 0)
            return -1;
        reg_cmd_read &= 0xFFF;
        reg_cmd_write &= 0xFFF;
    } while(reg_cmd_read != 0 && reg_cmd_write != 0);

    return 0;
}

int eve_click_snapshot(uint32_t ptr, uint8_t *data)
{
    uint32_t buffer[2];
    uint32_t total_bytes_to_read = 480 * 272 * 2;

    if (ft800_enabled == false)
        return -1;

    if (data == NULL)
        return -1;

    buffer[0] = FT800_SNAPSHOT;
    buffer[1] = ptr;

    /* Disable pixel clock output */
    if (write_8bit_reg(FT800_REG_PCLK, 0) < 0) {
        fprintf(stderr, "eve: Failed to disable pixel clock output.\n");
        return -1;
    }

    if (cmd_fifo_send(buffer, 2) < 0)
        return -1;

    /* Enable pixel clock output */
    if (write_8bit_reg(FT800_REG_PCLK, 5) < 0) {
        fprintf(stderr, "eve: Failed to disable pixel clock output.\n");
        return -1;
    }

    /* Read in chunk of max 4092 bytes because SPI driver fails to do transfer
     * of more than 4kB.
     */
    while (total_bytes_to_read > 0) {
        uint32_t count = total_bytes_to_read;
        if (count > 4092)
            count = 4092;

        if (memory_read(ptr, data, count) < 0)
            return -1;

        ptr += count;
        data += count;
        total_bytes_to_read -= count;
    }

    return 0;
}
