/******************************************************************************/
/* MieruPC Project: MieruPc Standard Library v1.4                             */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-07-09 */
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
 *@brief MieruPC用基本ライブラリ
 *
 *MieruPC用のアプリケーションで使われる基本的なライブラリ．
 */

#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wcast-qual"
#include <mierulib.h>
#include <keycode.h>
#include <keytable.h>

extern int main(void);


static uint key_queue[32];
static int key_num = 0;
static int key_begin = 0;
static int key_end = 0;
static uchar key_status = 0;

static uint rand_seed = 1;

/****************************************************************************/
/***                           string functions                           ***/
/****************************************************************************/

/**********************************************************************/
char *ntos(char *buf, char *end, uint num, int base, int size, int pad){
    static const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    const char *digits;
    int i, len;
    char c,sign,tmp[48];

    c = (pad) ? '0' : ' ';

    digits = small_digits;
    if(base<2 || base>36)
        return 0;

    // if num is negative in decimal number, sign is 1
    sign = 0;
    if( base == 10 && (signed int) num < 0 ){
        sign = '-';
        num = - (signed int) num;
    }else{
        num &= 0xffffffff;
    }

    len = 0;
    if(num == 0)
        tmp[len++] = '0';

    // get string of 'num base' number
    while (num != 0){
        tmp[len++] = digits[num % base];
        num = num / base;
    }

    // put sign character
    if(sign){
        if(buf <= end)
            *buf = sign;
        ++buf;
        size--;
    }

    // put zero or space to fill blank
    for(i = len; i < size; i++){
        if(buf <= end)
            *buf = c;
        ++buf;
    }

    // put string reversely
    while(len-- > 0){
        if(buf <= end)
            *buf = tmp[len];
        ++buf;
    }

    return buf;
}

/**********************************************************************/
int mpc_vsnprintf(char *buf, unsigned int size, const char *fmt, va_list args){
    char *str, *end;
    int base;
    char c;
    char *s;
    int len;
    int i;
    uint num;
    int zeropad;
    int field_width;

    str = buf;
    end = buf + size - 1;

    if(end < buf - 1){
        end = (char *)(-1);
        size = end - buf + 1;
    }

    for(; *fmt; ++fmt){
        if(*fmt != '%'){
            if(str <= end)
                *str++ = *fmt;
            continue;
        }
        fmt++;

        zeropad = 0;
        if(*fmt == '0'){
            zeropad = 1;
            fmt++;
        }

        field_width = -1;
        if(mpc_isdigit(*fmt)){
            field_width = 0;
            while(mpc_isdigit(*fmt))
                field_width = field_width * 10 + *fmt++ - '0';
        }

        base = 10;
        switch(*fmt){
        case 'c':
            c = (unsigned char ) va_arg(args, int);
            if(c != 0 && str <= end)
                *str++ = c;
            continue;
        case 's':
            s = va_arg(args, char *);
            len = mpc_strlen(s);
            for(i=0; i<field_width-len; i++){
                *str = ' ';
                str++;
            }
            for(i=0; i<len; i++){
                if(str <= end)
                    *str = *s;
                ++str; ++s;
            }

            continue;
        case 'p':
            if(str+1 <= end){
                *str++ = '0';
                *str++ = 'x';
            }
            base = 16;
            field_width = 8;
            zeropad = 1;
            break;
        case 'o':
            base = 8;
            break;
        case 'd':
            break;
        case 'x':
            base = 16;
            break;
        case '%':
            if(str <= end)
                *str++ = '%';
            continue;
        default:
            if(str <= end)
                *str++ = '%';
            fmt--;
            continue;
        }
        num = (unsigned int)va_arg(args, unsigned int);
        str = ntos(str, end, num, base, field_width, zeropad);
    }
    if(str <= end)
        *str = 0;
    else if(size > 0)
        *end = 0;

    return str-buf;
}

/**********************************************************************/
int mpc_snprintf(char *buf, unsigned int size, const char *fmt, ...){
    va_list args;
    int r;

    va_start(args, fmt);
    r = mpc_vsnprintf(buf,size,fmt,args);
    va_end(args);
    return r;
}

/**********************************************************************/
char *mpc_strncpy(char *dst, const char *src, unsigned int n){
    char *s = dst;
    if (!src || !dst || n <= 0)
        return s; 

    for(;*src && n > 0;src++, n--)
        *dst++ = *src;
    for(; n > 0; n--)
        *dst++ = '\0';
    return s;
}

/**********************************************************************/
char *mpc_strcpy(char *dst, const char *src){
    return mpc_strncpy(dst, src, mpc_strlen(src) + 1);
}

/**********************************************************************/
unsigned int mpc_strlen(const char *s){
    const char *ss;
    for(ss = s; *ss != 0; ss++);
    return ss - s;
}

/**********************************************************************/
int mpc_strncmp(const char *s1, const char *s2, unsigned int n){
    const char *end = s1 + n ;
    while(s1 != end){
        if(*s1 == 0 || *s1 != *s2)
            return *s1 - *s2;

        s1++;
        s2++;
    }
    return 0;
}

/**********************************************************************/
int mpc_strcmp(const char *s1, const char *s2){
    while(*s1 !=0 && *s1 == *s2){
        s1++;
        s2++;
    }

    return *s1 - *s2;
}

/**********************************************************************/
char *mpc_strchr(const char *s, int c){
    const char target = (char)c;
    do{
        if (*s == target)
            return (char *)s;
    }while (*s++ != 0);
    return NULL;
}

/**********************************************************************/
char *mpc_strstr(const char *s1, const char *s2){
    const char c = *s2;
    const int len = mpc_strlen(s2);
    const char *p = s1;

    while(p != NULL){
        p = mpc_strchr(p, c);
        if(p == NULL)
            break;

        if(!mpc_strncmp(p, s2, len))
            return (char *)p;
        p++;
    }
    return NULL;
}

/**********************************************************************/
char *mpc_strcat(char *s1, const char *s2){
    mpc_strcpy(s1 + mpc_strlen(s1), s2);
    return s1;
}

/**********************************************************************/
char *mpc_strncat(char *s1, const char *s2, int n){
    int len = mpc_strlen(s1);
    mpc_strncpy(s1 + len, s2, n);
    s1[len + n] = 0;
    return s1;
}

/**********************************************************************/
int mpc_atoi(char *a){
    int ret = 0;
    for(; '0' <= *a && *a <= '9'; a++)
        ret = ret * 10 + *a - '0';
    return ret;
}

/****************************************************************************/
/***                        character functions                           ***/
/****************************************************************************/

/**********************************************************************/
int mpc_iscntrl(char c){
    if((0x00 <= c && c <= 0x1f) || (c == 0x7f))
        return 1;
    return 0;
}

/**********************************************************************/
int mpc_isprint(char c){
    if(0x20 <= c && c < 0x7f)
        return 1;
    return 0;
}

/**********************************************************************/
int mpc_isdigit(char c){
    return (('0' <= c) && (c <= '9'));  
}

/**********************************************************************/
int mpc_isspace(char c){
    return ((c == ' ' ) || (c == '\t') || 
            (c == '\v') || (c == '\f') || 
            (c == '\n') || (c == '\r') );
}

/**********************************************************************/
char mpc_tolower(char c){
    if('A' <= c && c <= 'Z')
        c += 0x20;
    return c;
}

/**********************************************************************/
char mpc_toupper(char c){
    if('a' <= c && c <= 'z')
        c -= 0x20;
    return c;
}

/**********************************************************************/
int mpc_islower(char c){
    if('a' <= c && c <= 'z')
        return 1;
    return 0;
}

/**********************************************************************/
int mpc_isupper(char c){
    if('A' <= c && c <= 'Z')
        return 1;
    return 0;
}

/**********************************************************************/
void *mpc_memset(void *s, int c, unsigned int n){
    uchar *p, *end;
    if(n == 0) return s;

    p = (uchar *)s;
    end = p + n;

    for(; p != end; p++)
        *p = (uchar)c;
    return s;
}

/**********************************************************************/
int mpc_memcmp(const void *s1, const void *s2, unsigned int n){
    const uchar *ss1, *ss2, *end;
    int result = 0;
    ss1 = (const unsigned char *)s1;
    ss2 = (const unsigned char *)s2;
    for(end = ss2 + n;
        ss2 != end && (result = *ss1 - *ss2) == 0;
        ss1++, ss2++)
        ;
    return result;
}

/**********************************************************************/
void *mpc_memcpy(void *dst, const void *src, unsigned int n){
    char *ss1 = (char *)dst;
    const char *ss2 = (const char *)src;
    const char *t = ss2 + n;

    while(ss2 != t)
        *ss1++ = *ss2++;
    return dst;
}

/****************************************************************************/
/***                        other functions                               ***/
/****************************************************************************/

/**********************************************************************/
#ifdef __cplusplus
extern "C"
#endif
void mpc_startup(){

#ifndef MIERUPC_APPLICATION
    main();
#else
    int ret = main();
    sys_exit(ret);
#endif
}

/**********************************************************************/
// wait key to be put
void mpc_waitkey(void){
    for(;;){
        if(MKS_Enter == mpc_getchar())
            break;
    }
}

/**********************************************************************/
unsigned int mpc_getoldtime(void){
    return *(volatile uint *) MP_MMAP_TIMER;
}

/**********************************************************************/
unsigned int mpc_gettime(void){
    return *(volatile uint *) MP_MMAP_TIMER / 1000;
}

/**********************************************************************/
unsigned int mpc_getutime(void){
    return *(volatile uint *) MP_MMAP_TIMER * 1000;
}

/**********************************************************************/
unsigned int mpc_getmtime(void){
    return *(volatile uint *) MP_MMAP_TIMER;
}

/**********************************************************************/
void mpc_usleep(uint tm){
    volatile uint *p = (volatile uint *) MP_MMAP_TIMER;
    uint end = (uint) *p + tm / 1000 + 1;    
    while (*p < end);
}

/**********************************************************************/
uint mpc_getkey2buf(){
    uint keybuf;
    for(;;){
        keybuf = *(volatile uint *) MP_MMAP_KEYCODE;

        if(keybuf == 0)
            break;

        if(key_num < 32){
            key_num++;
            key_queue[key_end++] = keybuf;
            if(key_end == 32)
                key_end = 0;
        }
    }

    if(key_num == 0) return 0;

    key_num--;
    keybuf = key_queue[key_begin++];
    if(key_begin == 32)
        key_begin = 0;

    return keybuf;
}

/**********************************************************************/
// get key from memory
uchar mpc_getkey(uchar *make){
    uint keybuf;
    uchar key;
    int brk = 0;

    keybuf = mpc_getkey2buf();
    if(keybuf == 0) return 0; // key is zero in most case

    if((keybuf >> 8) != 0){
        brk = 1;
        keybuf = (keybuf >> 8) & 0xff;
    }else{
        brk = 0;
        keybuf = keybuf & 0xff;
    }

    key = key2sym[keybuf]; // ASCII or SCANCODE to keysym
    if(make != NULL) *make = (brk == 0);

    switch(key){
    case MKS_Shift_L:
    case MKS_Shift_R:        
        key_status = brk ? (key_status & ~OPT_SHT_BIT) : (key_status | OPT_SHT_BIT);
        break;

    case MKS_Control_L:
    case MKS_Control_R:
        key_status = brk ? (key_status & ~OPT_CTL_BIT) : (key_status | OPT_CTL_BIT);
        break;

    case MKS_Meta_L:
    case MKS_Meta_R:
        key_status = brk ? (key_status | ~OPT_ALT_BIT) : (key_status & OPT_ALT_BIT);
        break;

    case MKS_Super_L:
    case MKS_Super_R:
        key_status = brk ? (key_status | ~OPT_WIN_BIT) : (key_status & OPT_WIN_BIT);
        break;
    }

    return key;
}

/**********************************************************************/
unsigned int mpc_getchar(){
    uchar key, make;

    // get key
    key = mpc_getkey(&make);

    // key is zero in most case
    if(key == 0) return 0;    

    // ignore break key
    if(!make) return 0;

    // ignore modifiers (upper 4bit on)
    if( (key >> 4) == 0xf) return 0;

    // shift key
    if(mpc_onshift()) key = shiftkey[key];

    return key;
}

/**********************************************************************/
int mpc_onshift(){
    return (key_status & OPT_SHT_BIT);
}

/**********************************************************************/
int mpc_oncontrol(){
    return ((key_status & OPT_CTL_BIT) >> 1);
}

/**********************************************************************/
int mpc_onalt(){
    return ((key_status & OPT_ALT_BIT) >> 2);
}

/**********************************************************************/
int mpc_onsuper(){
    return ((key_status & OPT_WIN_BIT) >> 3);
}

/**********************************************************************/
int mpc_rand(void){
    rand_seed = rand_seed*1103515245+12345;
    return (uint)(rand_seed / 65536) % 32768;
}

/**********************************************************************/
void mpc_srand(uint seed){
    rand_seed = seed;
}

/**********************************************************************/
uint mpc_read_gpio(int n){
    return *(volatile uchar *)( MP_MMAP_GPIO_0 + n );
}

/**********************************************************************/
void mpc_write_gpio(int n, uint val){
    *(volatile uchar *)( MP_MMAP_GPIO_0 + n ) = (val & 0xff);
}

/**********************************************************************/
