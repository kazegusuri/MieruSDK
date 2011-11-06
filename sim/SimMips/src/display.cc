/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

enum {
    DISPLAY_CMD_BUF_SIZE = 256,
};

/**********************************************************************/
DisplayController::~DisplayController()
{
    DELETE_ARRAY(cmdbuf);
    DELETE_ARRAY(tmpbuf);
    DELETE(trans);
}

/**********************************************************************/
void DisplayController::init(Board *b)
{
    BoardMP *board = static_cast<BoardMP *>(b);
    gui_pipe = -1;
    if (board->lcd_path) {
        int res = open(board->lcd_path, O_WRONLY);
        if (res < 0) {
            fprintf(stderr, "## Can't open LCD pipe file: %s\n",
                    board->lcd_path);
        } else {
            gui_pipe = res;
            if (::write(gui_pipe, "I", 1) != 1) { // LCD Reset
                perror(NULL);
            }
        }
    }
    cmdbuf = new char[DISPLAY_CMD_BUF_SIZE + 1];
    tmpbuf = new char[DISPLAY_CMD_BUF_SIZE + 1];
    cmdbuf[0] = '\0';

    if (board->lcd_type == 0)
        trans = new DisplayTransFromOld();
    else
        trans = new DisplayTrans();
}

/**********************************************************************/
int DisplayController::write(uint032_t addr, uint size, uint064_t data)
{
    if (addr != 0 || size != 1)
        return 1;

    int len = strlen(cmdbuf);
    if (len >= DISPLAY_CMD_BUF_SIZE)
        len = DISPLAY_CMD_BUF_SIZE - 1;
    cmdbuf[len++] = (uint008_t) data;
    cmdbuf[len] = '\0';

    len = trans->run(cmdbuf, tmpbuf, len);
    if (gui_pipe != -1 && len) {
        if (::write(gui_pipe, tmpbuf, len) != len) {
            return 1;
        }
    }
    return 0;
}

/**********************************************************************/
int DisplayTrans::run(char *src, char *dst, int len)
{
    for (int i = 0; i < len; i++)
        dst[i] = src[i];
    src[0] = '\0';
    return len;
}

/**********************************************************************/
DisplayTransFromOld::DisplayTransFromOld()
{
    currentpage = sourcepage = 0;
    cursorx = cursory = 0;
}

/**********************************************************************/
int DisplayTransFromOld::run(char *src, char *dst, int len)
{
    char hexchr[17] = "0123456789ABCDEF";
    dst[0] = '\0';
    if (src[0] == '\n') {
        src[0] = '\0';
        return 0;
    }
    if (src[len - 1] != '\r')
        return 0;

    int rslt = 0, dstlen = 0;

    if (src[0] == 'C' && src[1] == 'L') {        // 1.1 CL
        char colchr[8] = "RGBYMCW";
        int colnum[7] = {0xe0, 0x1c, 0x03, 0xfc, 0xe3, 0x1f, 0xff};
        int col = 6;
        for (int i = 0; i < 7; i++)
            if (src[2] == colchr[i])
                col = i;
        rslt |= addcmd('F', dst, &dstlen);
        rslt |= addcmd(hexchr[colnum[col] / 16], dst, &dstlen);
        rslt |= addcmd(hexchr[colnum[col] % 16], dst, &dstlen);
    } else if (src[0] == 'C' && src[1] == 'H') { // 1.2 CH
        int colnum[16] = {0x00, 0xe0, 0x1c, 0x03, 0xfc, 0xe3, 0x1f, 0xff,
                          0x49, 0x80, 0x10, 0x02, 0x90, 0x82, 0x12, 0xb6};
        int col = 7;
        for (int i = 0; i < 16; i++)
            if (src[2] == hexchr[i])
                col = i;
        rslt |= addcmd('F', dst, &dstlen);
        rslt |= addcmd(hexchr[colnum[col] / 16], dst, &dstlen);
        rslt |= addcmd(hexchr[colnum[col] % 16], dst, &dstlen);
    } else if (src[0] == 'C' && src[1] == 'S') { // 1.3 CS
        rslt |= addcmd('F', dst, &dstlen);
        rslt |= addcmd(src[2], dst, &dstlen);
        rslt |= addcmd(src[3], dst, &dstlen);
    } else if (src[0] == 'D' && src[1] == 'S') { // 1.4 DS
        int page = src[2] - '0';
        if (page >= 0 && page <= 2) {
            rslt |= addcmd('P', dst, &dstlen);
            rslt |= addcmd(src[2], dst, &dstlen);
        }
    } else if (src[0] == 'P' && src[1] == 'S') { // 1.5 PS
        int page = src[2] - '0';
        if (page >= 0 && page <= 2) {
            rslt |= addcmd('W', dst, &dstlen);
            rslt |= addcmd(src[2], dst, &dstlen);
            currentpage = page;
        }
    } else if (src[0] == 'E' && src[1] == 'R') { // 1.6 ER
        rslt |= addcmd('C', dst, &dstlen);
        rslt |= addcmd('0' + currentpage, dst, &dstlen);
        rslt |= addcursorposcmd(0, 0, dst, &dstlen);
    } else if (src[0] == 'H' && src[1] == 'H') { // 1.7 HH
        rslt |= addcursorposcmd(0, 0, dst, &dstlen);
    } else if (src[0] == 'M' && src[1] == 'S') { // 1.10 MS
        rslt |= addcmd("T1", dst, &dstlen);
    } else if (src[0] == 'M' && src[1] == 'C') { // 1.11 MC
        rslt |= addcmd("T0", dst, &dstlen);
    } else if (src[0] == 'H' && src[1] == 'W') { // 2.9 HW
        for (int i = 3; i < len - 2; i++) {
            rslt |= addcmd('D', dst, &dstlen);
            rslt |= addcmd(src[i], dst, &dstlen);
            cursorx++;
            if (cursorx >= 40)
                rslt |= addcursorposcmd(0, cursory + 1, dst, &dstlen);
        }
    } else if (src[0] == 'H' && src[1] == 'P') { // 2.10 HP
        int x, y;
        int pos = 2;
        x = (src[pos++] & 0xf);
        if (src[pos] != ',')
            x = x * 10 + (src[pos++] & 0xf);
        pos++;
        y = (src[pos++] & 0xf);
        if (src[pos] != '\r')
            y = y * 10 + (src[pos++] & 0xf);
        rslt |= addcursorposcmd(x, y, dst, &dstlen);
    } else if (src[0] == 'H' && src[1] == 'F') { // 2.11 HF
        rslt |= addcursorposcmd(cursorx, cursory + 1, dst, &dstlen);
    } else if (src[0] == 'H' && src[1] == 'R') { // 2.12 HR
        rslt |= addcursorposcmd(0, cursory, dst, &dstlen);
    } else if (strncmp(src, "BS0,0,", 6) == 0) {      // 4.2 BS
        sourcepage = src[6] & 0x7;
    } else if (strncmp(src, "BD0,0,", 6) == 0) {      // 4.3 BD
        int destpage = src[6] & 0x7;
        rslt |= addcmd('V', dst, &dstlen);
        rslt |= addcmd('0' + sourcepage, dst, &dstlen);
        rslt |= addcmd('0' + destpage, dst, &dstlen);        
    }
    if (rslt) {
        dstlen = 0;
        dst[0] = '\0';
    }
    src[0] = '\0';
    return dstlen;
}

/**********************************************************************/
int DisplayTransFromOld::addcmd(char src, char *dst, int *len)
{
    if (*len == DISPLAY_CMD_BUF_SIZE - 1)
        return 1;
    dst[*len] = src;
    (*len)++;
    dst[*len] = '\0';
    return 0;
}

/**********************************************************************/
int DisplayTransFromOld::addcmd(const char *src, char *dst, int *len)
{
    while (*src) {
        if (addcmd(*src, dst, len))
            return 1;
        src++;
    }
    return 0;
}

/**********************************************************************/
int DisplayTransFromOld::addcursorposcmd(int x, int y, char *dst, int *len)
{
    int rslt = 0;
    if (x > 40) {
        x = 0;
        y++;
    }
    if (y > 15)
        y = 0;

    if (x == 0) {
        rslt |= addcmd("X10", dst, len);
    } else if (x == cursorx + 1) {
        rslt |= addcmd('L', dst, len);
    } else if (x == cursorx - 1) {
        rslt |= addcmd('H', dst, len);
    } else {
        rslt |= addcmd('X', dst, len);
        rslt |= addcmd('1' + (x / 10), dst, len);
        rslt |= addcmd('0' + (x % 10), dst, len);
    }

    if (y == 0) {
        rslt |= addcmd("Y01", dst, len);
    } else if (y == cursory + 1) {
        rslt |= addcmd('J', dst, len);
    } else if (y == cursory - 1) {
        rslt |= addcmd('K', dst, len);
    } else {
        rslt |= addcmd('Y', dst, len);
        rslt |= addcmd('0' + ((y + 1) / 10), dst, len);
        rslt |= addcmd('0' + ((y + 1) % 10), dst, len);
    }

    cursorx = x;
    cursory = y;
    return rslt;
}

/**********************************************************************/
