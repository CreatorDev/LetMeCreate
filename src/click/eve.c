#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <letmecreate/click/common.h>
#include <letmecreate/click/eve.h>
#include <letmecreate/click/ft800_defs.h>
#include <letmecreate/core/common.h>
#include <letmecreate/core/gpio.h>
#include <letmecreate/core/gpio_monitor.h>
#include <letmecreate/core/spi.h>

#define FIFO_SIZE               (4096)  /* 4KB */
#define FIFO_CMD_SIZE           (4)     /* 4bytes */
#define FIFO_RAM_OFFSET(OFFSET) (FT800_RAM_CMD + ((OFFSET) & (FIFO_SIZE - FIFO_CMD_SIZE)))


static bool ft800_enabled = false;
static int int_callback_id = -1;
static bool fifo_empty = true;

/* Co-processor FIFO */
static uint32_t cmds[FIFO_SIZE / FIFO_CMD_SIZE];
static uint32_t cmd_cnt = 0;
static bool cmd_buffering = true;
static bool use_timeout = true;

static void (*touch_callback)(uint16_t, uint16_t) = NULL;
static void (*touch_event_callback)(void) = NULL;

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
    int ret;
    va_list args;

    va_start(args, cmd);
    ret = parse_display_list_cmd(cmd, args);
    va_end(args);

    return ret;
}

/* @param[in] offset from last command in buffer (FIFO_CMD_SIZE * cmd_cnt)
 * @param[in] byte_count Number of bytes to insert
 */
static bool check_enough_space_buffer(uint32_t offset, uint8_t byte_count)
{
    /* Check that there is enough space in cmds array */
    if (FIFO_SIZE - (FIFO_CMD_SIZE * cmd_cnt + offset) < byte_count)  {
        fprintf(stderr, "eve: Not enough space in cmd buffer.\n");
        return false;
    }

    return true;
}

static int parse_16bits_arg(uint16_t arg, uint32_t offset)
{
    uint8_t *dest = (uint8_t*)(&cmds[cmd_cnt]);

    if (check_enough_space_buffer(offset, 2) == false)
        return -1;

    memcpy(&dest[offset], &arg, 2);
    return 0;
}

static int parse_32bits_arg(uint32_t arg, uint32_t offset)
{
    uint8_t *dest = (uint8_t*)(&cmds[cmd_cnt]);

    if (check_enough_space_buffer(offset, 4) == false)
        return -1;

    memcpy(&dest[offset], &arg, 4);
    return 0;
}

static int parse_font(uint16_t font, uint32_t offset)
{
    uint8_t *dest = (uint8_t*)(&cmds[cmd_cnt]);
    if (font > 31) {
        fprintf(stderr, "eve: Invalid font index, it must be in range 0-31.\n");
        return -1;
    }

    if (check_enough_space_buffer(offset, 2) == false)
        return -1;

    memcpy(&dest[offset], &font, 2);
    return 0;
}

static bool check_option_opcode(uint32_t opcode, uint16_t options)
{
    bool ret = true;

    switch (opcode) {
    case FT800_BUTTON:
    case FT800_SLIDER:
    case FT800_DIAL:
    case FT800_TOGGLE:
    case FT800_PROGRESS:
    case FT800_SCROLLBAR:
        if ((options & ~(FT800_OPT_FLAT | FT800_OPT_3D)) != 0)
           ret = false;
        break;
    case FT800_LOADIMAGE:
        if ((options & ~(FT800_OPT_RGB565 | FT800_OPT_MONO | FT800_OPT_NODL)) != 0)
            ret = false;
        break;
    case FT800_NUMBER:
        if ((options & ~(FT800_OPT_SIGNED | FT800_OPT_CENTERX | FT800_OPT_CENTERY | FT800_OPT_RIGHTX)) != 0)
            ret = false;
        break;
    case FT800_CLOCK:
        if ((options & ~(FT800_OPT_FLAT | FT800_OPT_3D | FT800_OPT_NOBACK | FT800_OPT_NOTICKS | FT800_OPT_NOHM | FT800_OPT_NOSECS | FT800_OPT_NOHANDS)) != 0)
            ret = false;
        break;
    case FT800_KEYS:
        if ((options & ~(FT800_OPT_FLAT | FT800_OPT_3D | FT800_OPT_CENTERX | FT800_OPT_CENTERY | FT800_OPT_RIGHTX)) != 0)
            ret = false;
        break;
    case FT800_GAUGE:
        if ((options & ~(FT800_OPT_FLAT | FT800_OPT_3D | FT800_OPT_CENTERX | FT800_OPT_CENTERY | FT800_OPT_RIGHTX)) != 0)
            ret = false;
        break;
    case FT800_TEXT:
        if ((options & ~(FT800_OPT_CENTERX | FT800_OPT_CENTERY | FT800_OPT_RIGHTX)) != 0)
            ret = false;
        break;
    default:
        ret = false;
        break;
    }

    if (!ret)
        fprintf(stderr, "eve: Option does not apply to command.\n");

    return ret;
}

static int parse_options(uint32_t opcode, uint16_t options, uint32_t offset)
{
    uint8_t *dest = (uint8_t*)(&cmds[cmd_cnt]);

    /* Check that option can be applied to this command */
    if (check_option_opcode(opcode, options) == false)
        return -1;

    if (check_enough_space_buffer(offset, 2) == false)
        return -1;

    memcpy(&dest[offset], &options, 2);
    return 0;
}

static int parse_str(char *str, uint32_t offset)
{
    char *dest = (char*)(&cmds[cmd_cnt]);

    if (check_enough_space_buffer(offset, strlen(str) + 1) == false)
        return -1;

    strcpy(&dest[offset], str);
    return (strlen(str) / FIFO_CMD_SIZE) + 1;
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
        ||  parse_options(opcode, va_arg(args, int), 14) < 0)
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
        ||  parse_options(opcode, va_arg(args, int), 10) < 0
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
        ||  parse_options(opcode, va_arg(args, int), 10) < 0
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
        ||  parse_options(opcode, va_arg(args, int), 10) < 0
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
        ||  parse_options(opcode, va_arg(args, int), 14) < 0)
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
        ||  parse_options(opcode, va_arg(args, int), 10) < 0
        ||  parse_32bits_arg(va_arg(args, int), 12) < 0)   /* n */
            return -1;
        cmd_length += 3;
        break;
    case FT800_PROGRESS:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_16bits_arg(va_arg(args, int), 8) < 0      /* width */
        ||  parse_16bits_arg(va_arg(args, int), 10) < 0     /* height */
        ||  parse_options(opcode, va_arg(args, int), 12) < 0
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
        ||  parse_options(opcode, va_arg(args, int), 12) < 0
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
        ||  parse_options(opcode, va_arg(args, int), 12) < 0
        ||  parse_16bits_arg(va_arg(args, int), 14) < 0     /* value */
        ||  parse_16bits_arg(va_arg(args, int), 16) < 0)    /* range */
            return -1;
        cmd_length += 4;
        break;
    case FT800_TEXT:
        if (parse_16bits_arg(va_arg(args, int), 4) < 0      /* x */
        ||  parse_16bits_arg(va_arg(args, int), 6) < 0      /* y */
        ||  parse_font(va_arg(args, int), 8) < 0
        ||  parse_options(opcode, va_arg(args, int), 10) < 0)
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
        ||  parse_options(opcode, va_arg(args, int), 12) < 0)
            return -1;

        {
            uint16_t state = va_arg(args, int);
            if (state > 0)  /* state must equal 65535 to turn on toggle */
                state = 65535;

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
    int ret;
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

static void recover_coprocessor_engine_fault(void)
{
    fprintf(stderr, "eve: recovering from coprocessor engine fault.\n");

    if (write_8bit_reg(FT800_REG_CPURESET, 1) < 0
    ||  write_32bit_reg(FT800_REG_CMD_READ, 0) < 0
    ||  write_32bit_reg(FT800_REG_CMD_WRITE, 0) < 0
    ||  write_8bit_reg(FT800_REG_CPURESET, 0) < 0)
        fprintf(stderr, "eve: Failed to recover.\n");
}

static void wait_for_coprocessor(void)
{
    uint16_t reg_cmd_read = 0;
    uint16_t timeout = 1000; /* timeout of 1s */

    while (fifo_empty == false) {
        sleep_ms(1);
        --timeout;
        if (use_timeout && timeout == 0) {
            fprintf(stderr, "eve: Timeout while waiting for coprocessor.\n");
            break;
        }
    }

    /* Check if a fault happened in the coprocessor engine */
    if (read_16bit_reg(FT800_REG_CMD_READ, &reg_cmd_read) < 0)
        return;
    reg_cmd_read &= 0xFFF;
    if (reg_cmd_read == 0xFFF)
        recover_coprocessor_engine_fault();
}

static uint16_t compute_fifo_freespace(void)
{
    uint16_t reg_cmd_write = 0, reg_cmd_read = 0;

    if (read_16bit_reg(FT800_REG_CMD_WRITE, &reg_cmd_write) < 0
    ||  read_16bit_reg(FT800_REG_CMD_READ, &reg_cmd_read) < 0)
        return 0;

    return (FIFO_SIZE - FIFO_CMD_SIZE) - ((reg_cmd_write - reg_cmd_read) & 0xFFF);
}

static int cmd_fifo_send(uint32_t *cmd_buffer, uint32_t cmd_buffer_cnt)
{
    while (cmd_buffer_cnt > 0) {
        uint32_t offset = 0;
        uint16_t fifo_freespace = compute_fifo_freespace() / FIFO_CMD_SIZE;
        uint32_t chunk_length = cmd_buffer_cnt;

        if (chunk_length > fifo_freespace)
            chunk_length = fifo_freespace;

        if (read_32bit_reg(FT800_REG_CMD_WRITE, &offset) < 0)
            return -1;

        if (memory_write(FIFO_RAM_OFFSET(offset), (uint8_t*)cmd_buffer, FIFO_CMD_SIZE*chunk_length) < 0)
            return -1;

        offset += FIFO_CMD_SIZE * chunk_length;
        offset &= 0xFFC;

        fifo_empty = false;
        if (write_32bit_reg(FT800_REG_CMD_WRITE, offset) < 0)
            return -1;

        wait_for_coprocessor();

        cmd_buffer_cnt -= chunk_length;
        cmd_buffer += chunk_length;
    }

    return 0;
}

static void interrupt_handler(uint8_t __attribute__ ((unused))event)
{
    uint8_t flags;

    if (read_8bit_reg(FT800_REG_INT_FLAGS, &flags) < 0) {
        fprintf(stderr, "eve: Failed to read interrupt flags register.\n");
        return;
    }

    if (flags & FT800_INT_CMD_FIFO_EMPTY)
        fifo_empty = true;

    if (flags & FT800_INT_CONVCOMPLETE) {
        if (touch_callback != NULL) {
            uint32_t tmp = 0;
            if (read_32bit_reg(FT800_REG_TOUCH_SCREEN_XY, &tmp) < 0)
                return;

            touch_callback(tmp >> 16, tmp);
        }
    }

    if (flags & FT800_INT_TOUCH) {
        if (touch_event_callback != NULL)
            touch_event_callback();
    }
}

static int attach_interrupt_handler(uint8_t mikrobus_index)
{
    uint8_t int_pin;

    if (gpio_get_pin(mikrobus_index, TYPE_INT, &int_pin) < 0)
        return -1;

    if (gpio_init(int_pin) < 0
    ||  gpio_monitor_init() < 0)
        return -1;

    if ((int_callback_id = gpio_monitor_add_callback(int_pin, GPIO_FALLING, interrupt_handler)) < 0)
        return -1;

    return 0;
}

static int detach_interrupt_handler(uint8_t mikrobus_index)
{
    uint8_t int_pin;

    if (gpio_get_pin(mikrobus_index, TYPE_INT, &int_pin) < 0)
        return -1;

    if (int_callback_id < 0)
        return 0;

    if (gpio_monitor_remove_callback(int_callback_id) < 0)
        return -1;

    int_callback_id = -1;

    if (gpio_release(int_pin) < 0)
        return -1;

    return 0;
}

static int flush_buffer(void)
{
    int ret = cmd_fifo_send(cmds, cmd_cnt);
    cmd_cnt = 0;
    return ret;
}

static int ft800_power(uint8_t bus_index, bool enable)
{
    uint8_t pd_n_pin;

    if (gpio_get_pin(bus_index, TYPE_RST, &pd_n_pin) < 0)
        return -1;

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

    if (gpio_get_pin(bus_index, TYPE_AN, &mode_pin) < 0)
        return -1;

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

    /* Enable touch screen */
    if (write_8bit_reg(FT800_REG_TOUCH_OVERSAMPLE, 15) < 0   /* Max oversampling */
    ||  write_8bit_reg(FT800_REG_TOUCH_ADC_MODE, 1) < 0 /* Differential mode for better accuracy */
    ||  write_8bit_reg(FT800_REG_TOUCH_MODE, FT800_TOUCH_MODE_FRAME_SYNC) < 0) {
        fprintf(stderr, "eve: Failed to set touch controller mode.\n");
        return -1;
    }

    /* Configure interrupt */
    int_callback_id = -1;
    if (attach_interrupt_handler(mikrobus_index) < 0) {
        fprintf(stderr, "eve: Failed to attach interrupt handler.\n");
        return -1;
    }
    if (write_8bit_reg(FT800_REG_INT_MASK, FT800_INT_CMD_FIFO_EMPTY | FT800_INT_CONVCOMPLETE | FT800_INT_TOUCH) < 0
    ||  write_8bit_reg(FT800_REG_INT_EN, 1) < 0)
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

    if (detach_interrupt_handler(mikrobus_index) < 0) {
        fprintf(stderr, "eve: Failed to detach interrupt handler.\n");
        return -1;
    }

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

    if (!eve_click_is_buffering_enabled())
        return flush_buffer();
     else
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
        return flush_buffer();
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

    return flush_buffer();
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

int eve_click_inflate(uint32_t ptr, const uint8_t *data, uint32_t count)
{
    uint32_t buffer_count = 0;
    uint32_t *buffer = NULL;
    int ret = 0;

    /*
      data is decompressed in main graphics ram (0x0 - 0x3FFFF), so ptr must
      point to a location in this range.
    */
    if (ptr >= 0x40000) {
        fprintf(stderr, "eve: Invalid pointer to decompress data");
        return -1;
    }

    if (data == NULL) {
        fprintf(stderr, "eve: Cannot decompress data with null pointer.\n");
        return -1;
    }

    if (count == 0)
        return 0;

    buffer_count = 3 + (count + 2) / 4;
    buffer = malloc(sizeof(uint32_t) * buffer_count);
    if (buffer == NULL)
        return -1;
    buffer[0] = FT800_INFLATE;
    buffer[1] = ptr;
    buffer[buffer_count - 1] = 0;
    memcpy(&buffer[2], data, count);

    if (cmd_fifo_send(buffer, buffer_count) < 0)
        ret = -1;

    free(buffer);

    return ret;
}

int eve_click_get_ptr(uint32_t *ptr)
{
    uint16_t offset = 0;
    uint32_t buffer[2];

    if (ptr == NULL) {
        fprintf(stderr, "eve: cannot get ptr with null pointer.\n");
        return -1;
    }

    if (read_16bit_reg(FT800_REG_CMD_WRITE, &offset) < 0)
        return -1;

    buffer[0] = FT800_GETPTR;
    buffer[1] = 0;
    if (cmd_fifo_send(buffer, 2) < 0)
        return -1;

    return read_32bit_reg(FIFO_RAM_OFFSET(offset + 4), ptr);
}

int eve_click_load_identity(void)
{
    if (ft800_enabled == false)
        return -1;

    if (cmd_buffering) {
        return parse_coprocessor_vcmd(FT800_LOADIDENTITY);
    } else {
        uint32_t cmd = FT800_LOADIDENTITY;
        return cmd_fifo_send(&cmd, 1);
    }
}

int eve_click_scale(int32_t sx, int32_t sy)
{
    if (ft800_enabled == false)
        return -1;

    if (cmd_buffering) {
        return parse_coprocessor_vcmd(FT800_SCALE, sx, sy);
    } else {
        uint32_t buffer[3];
        buffer[0] = FT800_SCALE;
        buffer[1] = sx;
        buffer[2] = sy;
        return cmd_fifo_send(buffer, 3);
    }
}

int eve_click_translate(int32_t tx, int32_t ty)
{
    if (ft800_enabled == false)
        return -1;

    if (cmd_buffering) {
        return parse_coprocessor_vcmd(FT800_TRANSLATE, tx, ty);
    } else {
        uint32_t buffer[3];
        buffer[0] = FT800_TRANSLATE;
        buffer[1] = tx;
        buffer[2] = ty;
        return cmd_fifo_send(buffer, 3);
    }
}

int eve_click_rotate(int32_t angle)
{
    if (ft800_enabled == false)
        return -1;

    if (cmd_buffering) {
        return parse_coprocessor_vcmd(FT800_ROTATE, angle);
    } else {
        uint32_t buffer[2];
        buffer[0] = FT800_ROTATE;
        buffer[1] = angle;
        return cmd_fifo_send(buffer, 2);
    }
}

int eve_click_get_matrix(int32_t *a, int32_t *b, int32_t *c,
                         int32_t *d, int32_t *e, int32_t *f)
{
    uint32_t buffer[7];
    uint16_t offset;
    uint32_t a_loc = 0, b_loc = 0, c_loc = 0, d_loc = 0, e_loc = 0, f_loc = 0;

    if (ft800_enabled == false)
        return -1;

    if (a == NULL || b == NULL || c == NULL
    ||  d == NULL || e == NULL || f == NULL)
        return -1;

    if (read_16bit_reg(FT800_REG_CMD_WRITE, &offset) < 0)
        return -1;

    buffer[0] = FT800_GETMATRIX;
    memset(&buffer[1], 0, 24);
    if (cmd_fifo_send(buffer, 7) < 0)
        return -1;

    a_loc = FIFO_RAM_OFFSET(offset + 4);
    b_loc = FIFO_RAM_OFFSET(offset + 8);
    c_loc = FIFO_RAM_OFFSET(offset + 12);
    d_loc = FIFO_RAM_OFFSET(offset + 16);
    e_loc = FIFO_RAM_OFFSET(offset + 20);
    f_loc = FIFO_RAM_OFFSET(offset + 24);

    if (read_32bit_reg(a_loc, (uint32_t*)a) < 0
    ||  read_32bit_reg(b_loc, (uint32_t*)b) < 0
    ||  read_32bit_reg(c_loc, (uint32_t*)c) < 0
    ||  read_32bit_reg(d_loc, (uint32_t*)d) < 0
    ||  read_32bit_reg(e_loc, (uint32_t*)e) < 0
    ||  read_32bit_reg(f_loc, (uint32_t*)f) < 0)
        return -1;

    return 0;
}

int eve_click_set_matrix(void)
{
    if (ft800_enabled == false)
        return -1;

    if (cmd_buffering) {
        return parse_coprocessor_vcmd(FT800_SETMATRIX);
    } else {
        uint32_t cmd = FT800_SETMATRIX;
        return cmd_fifo_send(&cmd, 1);
    }
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

    return read_32bit_reg(FIFO_RAM_OFFSET(offset + 12), crc);
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

    if (write_32bit_reg(FIFO_RAM_OFFSET(offset), cmd) < 0)
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
    uint32_t maximum_tranfer_length = 0;

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

    if (spi_get_maximum_tranfer_length(&maximum_tranfer_length) < 0)
        maximum_tranfer_length = FIFO_SIZE;
    else
        maximum_tranfer_length &= ~0x3;
    maximum_tranfer_length -= FIFO_CMD_SIZE;

    while (total_bytes_to_read > 0) {
        uint32_t count = total_bytes_to_read;
        if (count > maximum_tranfer_length)
            count = maximum_tranfer_length;

        if (memory_read(ptr, data, count) < 0)
            return -1;

        ptr += count;
        data += count;
        total_bytes_to_read -= count;
    }

    return 0;
}

int eve_click_spinner(int16_t x, int16_t y, uint16_t style, uint16_t scale)
{
    if (ft800_enabled == false)
        return -1;

    cmds[cmd_cnt++] = FT800_SPINNER;
    cmds[cmd_cnt++] = (y << 16) | x;
    cmds[cmd_cnt++] = (scale << 16) | style;
    return 0;
}

int eve_click_stop(void)
{
    uint32_t cmd = FT800_STOP;

    if (ft800_enabled == false)
        return -1;

    return cmd_fifo_send(&cmd, 1);
}

int eve_click_coldstart(void)
{
    uint32_t cmd = FT800_COLDSTART;

    if (ft800_enabled == false)
        return -1;

    return cmd_fifo_send(&cmd, 1);
}

int eve_click_screensaver(void)
{
    uint32_t cmd = FT800_SCREENSAVER;

    if (ft800_enabled == false)
        return -1;

    return cmd_fifo_send(&cmd, 1);
}

int eve_click_set_backlight_intensity(uint8_t intensity)
{
    if (intensity > 128)
        intensity = 128;

    return write_8bit_reg(FT800_REG_PWM_DUTY, intensity);
}

void eve_click_attach_touch_callback(void (*callback)(uint16_t, uint16_t))
{
    touch_callback = callback;
}

void eve_click_attach_touch_event_callback(void (*callback)(void))
{
    touch_event_callback = callback;
}

int eve_click_calibrate(void)
{
    bool prev_buffering = eve_click_is_buffering_enabled();
    bool calibration_failure = false;
    uint16_t offset = 0;
    uint32_t result = 0;

    /* Discard any previous content in cmds */
    cmd_cnt = 0;

    eve_click_enable_buffering();

    if (parse_coprocessor_vcmd(FT800_DLSTART) < 0
    ||  parse_display_list_vcmd(FT800_CLEAR, 1, 1, 1) < 0
    ||  parse_coprocessor_vcmd(FT800_TEXT, 240, 136, 27, FT800_OPT_CENTER, "Please tap on the dot") < 0)
        return -1;

    cmds[cmd_cnt++] = FT800_CALIBRATE;
    cmds[cmd_cnt++] = 0;

    /* Flush buffer to send commands to device
       Disable timeout because it will take at least a few seconds for the user
       to go through the calibration and CMD_CALIBRATE will not terminate until
       this calibration phase is finished.
     */
    use_timeout = false;
    if (flush_buffer() < 0) {
        use_timeout = true;
        return -1;
    }
    use_timeout = true;

    /* Check result of calibration */
    if (read_16bit_reg(FT800_REG_CMD_READ, &offset) < 0)
        return -1;
    if (read_32bit_reg(FIFO_RAM_OFFSET(offset + (FIFO_SIZE - FIFO_CMD_SIZE)), &result) < 0)
        return -1;
    if (result == 0) {
        fprintf(stderr, "eve: Failed to calibrate touch screen.\n");
        calibration_failure = true;
   }

    /* Restore previous buffering setting */
    if (prev_buffering)
        eve_click_enable_buffering();
    else
        eve_click_disable_buffering();

    if (calibration_failure)
        return -1;

    return 0;
}

int eve_click_get_calibration_matrix(uint32_t *a, uint32_t *b, uint32_t *c,
                                     uint32_t *d, uint32_t *e, uint32_t *f)
{
    if (a == NULL || b == NULL || c == NULL
    ||  d == NULL || e == NULL || f == NULL) {
        fprintf(stderr, "eve: Cannot store calibration matrix using null pointer.\n");
        return -1;
    }

    if (read_32bit_reg(FT800_REG_TOUCH_TRANSFORM_A, a) < 0
    ||  read_32bit_reg(FT800_REG_TOUCH_TRANSFORM_B, b) < 0
    ||  read_32bit_reg(FT800_REG_TOUCH_TRANSFORM_C, c) < 0
    ||  read_32bit_reg(FT800_REG_TOUCH_TRANSFORM_D, d) < 0
    ||  read_32bit_reg(FT800_REG_TOUCH_TRANSFORM_E, e) < 0
    ||  read_32bit_reg(FT800_REG_TOUCH_TRANSFORM_F, f) < 0) {
        fprintf(stderr, "eve: Failed to read calibration matrix.\n");
        return -1;
    }

    return 0;
}

int eve_click_set_calibration_matrix(uint32_t a, uint32_t b, uint32_t c,
                                     uint32_t d, uint32_t e, uint32_t f)
{
    if (write_32bit_reg(FT800_REG_TOUCH_TRANSFORM_A, a) < 0
    ||  write_32bit_reg(FT800_REG_TOUCH_TRANSFORM_B, b) < 0
    ||  write_32bit_reg(FT800_REG_TOUCH_TRANSFORM_C, c) < 0
    ||  write_32bit_reg(FT800_REG_TOUCH_TRANSFORM_D, d) < 0
    ||  write_32bit_reg(FT800_REG_TOUCH_TRANSFORM_E, e) < 0
    ||  write_32bit_reg(FT800_REG_TOUCH_TRANSFORM_F, f) < 0) {
        fprintf(stderr, "eve: Failed to write calibration matrix.\n");
        return -1;
    }

    return 0;
}
