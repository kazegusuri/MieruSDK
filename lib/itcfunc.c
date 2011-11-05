/******************************************************************************/
/* MieruPC Project: LCD Library itcfunc v3.4                                  */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-07-15 */
/******************************************************************************/

/*
 * Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*! @file
 *@brief MieruLCD用コマンドライブラリ
 *
 *MieruLCD用のコマンドライブラリ
 */

#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wcast-qual"
#include <mierulib.h>

#ifdef MIERU_OS
using namespace system;
#endif

static display_t display;
static char buf[BUF_SIZE];
static char large_hex[] = "0123456789ABCDEF";

#define hex2ascii(n) (large_hex[(n&0xf)])


/**********************************************************************/
void lcd_sendchar(char c)
{
    volatile char *ptr = (volatile char *) MP_MMAP_LCD;
    while (!*ptr);
    *ptr = c;
}

/**********************************************************************/
void lcd_sendcmd(char *arg)
{
    volatile char *ptr = (volatile char *) MP_MMAP_LCD;
    for (; *arg; arg++) {
        while (!*ptr);
        *ptr = *arg;
    }
}

/**********************************************************************/
display_t lcd_getdisplay()
{
    return display;
}

/**********************************************************************/
void lcd_setdisplay(display_t disp)
{
    display = disp;
}

/**********************************************************************/
void lcd_setscrollmode(int on)
{
    display.scroll_mode = on;
}

/**********************************************************************/
void lcd_reset(void)
{
    int i;
    for(i = 0; i < 256; i++){
        lcd_setpalette(i,
                       ((i & 0xe0)     ) | ((i & 0x80) >> 3),
                       ((i & 0x1c) << 3) | ((i & 0x10)     ),
                       ((i & 0x03) << 6) | ((i & 0x03) << 4));
    }
    lcd_setscrollmode(0);
    lcd_settransparent(0);
    lcd_clearpage(0);
    lcd_clearpage(1);
    lcd_clearpage(2);
    lcd_setforeground(COL_WHITE);
    lcd_setbackground(COL_BLACK);
    lcd_locate(0, 0);
    lcd_setpos(0, 0);
    lcd_setshowpage(0);
    lcd_setdrawpage(0);
    lcd_setcursor(0);
}

/**********************************************************************/
int lcd_init(void)
{

#ifdef MIERUPC_APPLICATION
    struct task_report repo;

    // load display's variables before staring this application
    sys_init(&repo);
    lcd_setdisplay(repo.disp);

#else
    mpc_usleep(200000);          // 200msec wait, MieruPC start up may be too fast
    
    display.width  = DEFAULT_WIDTH;
    display.height = DEFAULT_HEIGHT;

    lcd_setscrollmode(0);
    lcd_setcursor(0);
    lcd_settransparent(0);
    lcd_setforeground(COL_WHITE);
    lcd_setbackground(COL_BLACK);

    lcd_clearpage(0);
    lcd_clearpage(1);
    lcd_clearpage(2);

    lcd_locate(0, 0);
    lcd_setpos(0, 0);

    lcd_setshowpage(0);
    lcd_setdrawpage(0);
#endif
    
    // flush input buffer
    while(mpc_getchar() != 0);
    return 1;
}

/* open and initalize tty, and send SV command if rotate is non-zero */
/**********************************************************************/
int lcd_ttyopen(int rotate __UNUSED__)
{
    lcd_init();
    return 0;
}


/**********************************************************************/
void lcd_ttyclose()
{
}

/**********************************************************************/
void lcd_rotate(int r __UNUSED__)
{
}

/**********************************************************************/
int lcd_getwidth()
{
    return display.width;
}

/**********************************************************************/
int lcd_getheight()
{
    return display.height;
}

/**********************************************************************/
int lcd_getshowpage()
{
    return display.showpage;
}

/**********************************************************************/
void lcd_setshowpage(int page)
{
    if(0 <= page && page < PAGE_SIZE){
        lcd_sendchar('P');
        lcd_sendchar(hex2ascii(page));
        display.showpage = page;
    }
}

/**********************************************************************/
int lcd_getdrawpage()
{
    return display.drawpage;
}

/**********************************************************************/
void lcd_setdrawpage(int page)
{
    if(0 <= page && page < PAGE_SIZE){
        lcd_sendchar('W');
        lcd_sendchar(hex2ascii(page));
        display.drawpage = page;
    }
}

/**********************************************************************/
void lcd_clear()
{
    lcd_clearpage(display.drawpage);
}

/**********************************************************************/
void lcd_clearpage(int page)
{
    lcd_sendchar('C');
    lcd_sendchar(hex2ascii(page));
    mpc_usleep(6000); // 6msec
    
    lcd_locate(0, 0);
    lcd_setpos(0, 0);

    if(0 <= page && page < PAGE_SIZE){
        display.scrollx[page] = 0;
        display.scrolly[page] = 0;
    }
}

/**********************************************************************/
void lcd_cls()
{
    lcd_clear();
}

/**********************************************************************/
void lcd_copypage(int src, int dest)
{
    lcd_sendchar('V');
    lcd_sendchar(hex2ascii(src));
    lcd_sendchar(hex2ascii(dest));
    mpc_usleep(11000); // 11msec
}

/**********************************************************************/


/********** charcter functions ****************************************/
/**********************************************************************/
/**********************************************************************/
void lcd_settransparent(int t)
{
    lcd_sendchar('T');
    lcd_sendchar(hex2ascii(t));
}

/**********************************************************************/
void lcd_setreverse(int flag __UNUSED__)
{
    // not implemented
}

/**********************************************************************/
void lcd_setcursor(int t)
{
    display.cursor_mode = t;

    lcd_sendchar('c');
    lcd_sendchar(hex2ascii(t));
}


/**********************************************************************/
void lcd_setcolor(int color)
{
    lcd_setforeground(color);
}

/**********************************************************************/
void lcd_setforeground(int col)
{
    char u,l;
    display.foreground = col;
    l = col & 0xf;
    u = (col >> 4) & 0xf;
    lcd_sendchar('F');
    lcd_sendchar(hex2ascii(u));
    lcd_sendchar(hex2ascii(l));
}

/**********************************************************************/
void lcd_setbackground(int col)
{
    char u,l;
    display.background = col;
    l = col & 0xf;
    u = (col >> 4) & 0xf;
    lcd_sendchar('B');
    lcd_sendchar(hex2ascii(u));
    lcd_sendchar(hex2ascii(l));
}

/**********************************************************************/
int lcd_getcurx()
{
    return display.cursorx;
}

/**********************************************************************/
int lcd_getcury()
{
    return display.cursory;
}

/**********************************************************************/
void lcd_locate(int x, int y)
{
    int xx, yy;
    xx = x + display.scrollx[display.drawpage];
    if (xx >= CURSOR_WIDTH)
        xx -= CURSOR_WIDTH;
    yy = y + display.scrolly[display.drawpage];
    if (yy >= CURSOR_HEIGHT)
        yy -= CURSOR_HEIGHT;
    lcd_sendchar('X');
    lcd_sendchar(hex2ascii((xx/10) % 10));
    lcd_sendchar(hex2ascii((xx   ) % 10));
    lcd_sendchar('Y');
    lcd_sendchar(hex2ascii((yy/10) % 10));
    lcd_sendchar(hex2ascii((yy   ) % 10));

    if(x < CURSOR_WIDTH)
        display.cursorx = x;
    if(y < CURSOR_HEIGHT)
        display.cursory = y;
}

/**********************************************************************/
void lcd_nextline()
{
    lcd_sendchar('J');
    lcd_sendchar('X');
    lcd_sendchar(hex2ascii((display.scrollx[display.drawpage]/10) % 10));
    lcd_sendchar(hex2ascii((display.scrollx[display.drawpage]   ) % 10));

    display.cursorx = 0;
    display.cursory++;

    if(display.cursory >= CURSOR_HEIGHT){
        if(!display.scroll_mode){
            display.cursory = 0;
            lcd_sendchar('Y');
            lcd_sendchar(hex2ascii((display.scrolly[display.drawpage]/10) % 10));
            lcd_sendchar(hex2ascii((display.scrolly[display.drawpage]   ) % 10));
        }else{
            lcd_vscroll(1, 1);
            lcd_locate(0, CURSOR_HEIGHT - 1);
        }
    }
}

/**********************************************************************/
void lcd_puts(const char *s)
{
    int i = 0;
    while(s[i] != 0) {
        if(s[i] == '\n'){
            lcd_nextline();
            i++;
            continue;
        }else if(s[i] == '\t'){
            lcd_puts("  ");
            i++;
            continue;
        }
        lcd_sendchar('D');
        lcd_sendchar(s[i++]);

        display.cursorx++;
        if(display.cursorx == CURSOR_WIDTH){
            display.cursorx = 0;
            display.cursory++;
        }
        if(display.cursory >= CURSOR_HEIGHT){
            if(!display.scroll_mode){
                display.cursory = 0;
            }else{
                lcd_vscroll(1, 1);
                lcd_locate(0, CURSOR_HEIGHT - 1);
            }
        }
    }
}

/**********************************************************************/
void lcd_print(const char *str)
{
    lcd_puts(str);
}

/**********************************************************************/
int lcd_printf(const char *fmt, ...)
{
    int ret;
    va_list arg;

    va_start(arg, fmt);
    ret = mpc_vsnprintf(buf, BUF_SIZE, fmt, arg);
    va_end(arg);
    lcd_puts(buf);

    return ret;
}

/**********************************************************************/
int lcd_dprintf(const char *fmt, ...)
{
    int ret;
    va_list arg;
    int len;

    va_start(arg, fmt);
    ret = mpc_vsnprintf(buf, BUF_SIZE, fmt, arg);
    va_end(arg);
    
    len = mpc_strlen(buf);
    _syscall(4004, 1, (uint)buf, len, 0);

    return ret;
}

/**********************************************************************/
void lcd_moveright()
{
    lcd_sendchar('L');
    display.cursorx++;
    if(display.cursorx == CURSOR_WIDTH){
        display.cursorx = 0;
        display.cursory++;
    }
    if(display.cursory >= CURSOR_HEIGHT){
        if(!display.scroll_mode){
            display.cursory = 0;
        }else{
            lcd_vscroll(1, 1);
            lcd_locate(0, CURSOR_HEIGHT - 1);
        }
    }
}

/**********************************************************************/
void lcd_moveleft()
{
    lcd_sendchar('H');
    display.cursorx--;
    if(display.cursorx < 0){
        display.cursorx = CURSOR_WIDTH - 1;
        display.cursory--;
    }
    if(display.cursory < 0){
        if(!display.scroll_mode){
            display.cursory = CURSOR_HEIGHT - 1;
        }else{
            // not scroll and not move
            display.cursory = 0;
        }
    }
}

/**********************************************************************/


/***** graphic functions **********************************************/
/**********************************************************************/
/**********************************************************************/
void lcd_setpalette(int i, uchar red, uchar green, uchar blue)
{
    if(i < 0 || 256 <= i) return;
    lcd_sendchar('p');
    lcd_sendchar(large_hex[(i/16) % 16]);
    lcd_sendchar(large_hex[(i   ) % 16]);
    lcd_sendchar(large_hex[(red   >> 4) & 0xf]);
    lcd_sendchar(large_hex[(green >> 4) & 0xf]);
    lcd_sendchar(large_hex[(blue  >> 4) & 0xf]);
}

/**********************************************************************/
void lcd_drawdot()
{
    lcd_sendchar('d');

    display.posx++;
    if(display.posx == display.width)
        display.posx = 0;
}

/**********************************************************************/
void lcd_drawdotcolor(int col)
{
    lcd_setforeground(col);
    lcd_drawdot();
}

/**********************************************************************/
// for backward compatibility
void lcd_drawdot4(int c0, int c1, int c2, int c3){
    lcd_drawdotcolor(c0);
    lcd_drawdotcolor(c1);
    lcd_drawdotcolor(c2);
    lcd_drawdotcolor(c3);
}

/**********************************************************************/
int lcd_getposx()
{
    return display.posx;
}

/**********************************************************************/
int lcd_getposy()
{
    return display.posy;
}

/**********************************************************************/
void lcd_setpos(int x, int y)
{
    int xx, yy;
    xx = x + display.scrollx[display.drawpage] * 8;
    if (xx >= display.width)
        xx -= display.width;
    yy = y + display.scrolly[display.drawpage] * 16;
    if (yy >= display.height)
        yy -= display.height;

    lcd_sendchar('x');
    lcd_sendchar(hex2ascii((xx/100) % 10));
    lcd_sendchar(hex2ascii((xx/10 ) % 10));
    lcd_sendchar(hex2ascii((xx    ) % 10));
    lcd_sendchar('y');
    lcd_sendchar(hex2ascii((yy/100) % 10));
    lcd_sendchar(hex2ascii((yy/10 ) % 10));
    lcd_sendchar(hex2ascii((yy    ) % 10));

    if(x < display.width)
        display.posx = x;
    if(y < display.height)
        display.posy = y;
}

/**********************************************************************/
/* mode 0:normal 1:transparent ****************************************/
void lcd_copyblock(int p0, int x0, int y0, int p1, int x1, int y1, int mode)
{
    int oldpage = lcd_getdrawpage();

    lcd_setdrawpage(p1);
    lcd_locate(x1, y1);
    lcd_sendchar('v');
    lcd_sendchar(hex2ascii(p0));
    lcd_sendchar(hex2ascii((x0/10) % 10));
    lcd_sendchar(hex2ascii((x0   ) % 10));
    lcd_sendchar(hex2ascii((y0/10) % 10));
    lcd_sendchar(hex2ascii((y0   ) % 10));
    lcd_sendchar(hex2ascii(mode & 0x1));

    lcd_setdrawpage(oldpage);
    
}

/**********************************************************************/
void lcd_copyregion(int srcp, int sx, int sy,
                    int dstp, int dx, int dy,
                    int w, int h, int mode)
{
    int i, j;
    for(i = 0; i < w; i++)
        for(j = 0; j < h; j++)
            lcd_copyblock(srcp, sx+i, sy+j, dstp, dx+i, dy+j, mode);
}

/**********************************************************************/
void lcd_vscroll(int sc, int clear)
{
    int i, j;
    int oldx, oldy;
    int d;
    int old_mode;

    if(sc == 0) return;

    d = sc;
    if(d < 0)
        d = CURSOR_HEIGHT + sc;
    
    lcd_sendchar('s');
    lcd_sendchar(hex2ascii((d/10) % 10));
    lcd_sendchar(hex2ascii((d   ) % 10));

    display.scrolly[display.drawpage] += d;
    if (display.scrolly[display.drawpage] >= CURSOR_HEIGHT)
        display.scrolly[display.drawpage] -= CURSOR_HEIGHT;

    if(clear){
        oldx = lcd_getcurx();
        oldy = lcd_getcury();
        old_mode = display.scroll_mode;

        if(sc < 0){
            sc = -sc;
            lcd_locate(0, 0);
        }else{
            lcd_locate(0, CURSOR_HEIGHT - sc);
        }
        lcd_setscrollmode(0);
        for(i = 0; i < sc; i++)
            for(j = 0; j < CURSOR_WIDTH; j++)
                lcd_puts(" ");
        lcd_setscrollmode(old_mode);

        lcd_locate(oldx, oldy);
    }
}

/**********************************************************************/
void lcd_hscroll(int sc, int clear)
{
    int i, j;
    int oldx, oldy;
    int d, pos;
    int old_mode;
    
    if(sc == 0) return;

    d = sc;
    if(sc < 0)
        d = CURSOR_WIDTH + sc;

    lcd_sendchar('S');
    lcd_sendchar(hex2ascii((d/10) % 10));
    lcd_sendchar(hex2ascii((d   ) % 10));

    display.scrollx[display.drawpage] += d;
    if (display.scrollx[display.drawpage] >= CURSOR_WIDTH)
        display.scrollx[display.drawpage] -= CURSOR_WIDTH;

    if(clear){
        oldx = lcd_getcurx();
        oldy = lcd_getcury();
        old_mode = display.scroll_mode;

        if(sc < 0){
            sc = -sc;
            pos = 0;
        }else{
            pos = CURSOR_WIDTH - sc;        
        }
        lcd_setscrollmode(0);
        for(i = 0; i < CURSOR_HEIGHT; i++){
            lcd_locate(pos, i);
            for(j = 0; j < sc; j++)
                lcd_puts(" ");
        }
        lcd_setscrollmode(old_mode);

        lcd_locate(oldx, oldy);
    }
}

/**********************************************************************/
