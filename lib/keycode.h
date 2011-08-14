/******************************************************************************/
/* MieruPC Project: Keycode definition file v0.1                              */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-06-14 */
/*                                                                            */
/* DON'T use these macros in application directly.                            */
/* You shoud use keysyms (in keysym.h)                                        */
/******************************************************************************/
#pragma once

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
 *@brief スキャンコード定義ファイル
 *
 *スキャンコード定義ファイル．
 *アプリケーションからスキャンコードを直接使用しないこと．
 */


/* special keys */
#define MIERU_KEYCODE_SHT       0xa0 // left  shift key
#define MIERU_KEYCODE_SHTR      0xa1 // right shift key
#define MIERU_KEYCODE_CTL       0xa2 // left  control key
#define MIERU_KEYCODE_CTLR      0xa3 // right control key
#define MIERU_KEYCODE_ALT       0xa4 // left  alt key
#define MIERU_KEYCODE_ALTR      0xa5 // right alt key
#define MIERU_KEYCODE_SPECIAL   0x5b // left  windows key
#define MIERU_KEYCODE_SPECIALR  0x5c // right windows key

#define MIERU_KEYCODE_MINUS     0xbd // '-'
#define MIERU_KEYCODE_EQUAL     0xbb // '='
#define MIERU_KEYCODE_BKSLASH   0xdc // '\'
#define MIERU_KEYCODE_BKQUATE   0xc0 // '`'
#define MIERU_KEYCODE_LBRACE    0xdb // '['
#define MIERU_KEYCODE_RBRACE    0xdd // ']'
#define MIERU_KEYCODE_SEMICOLON 0xba // ';'
#define MIERU_KEYCODE_SQUOTE    0xde // '''
#define MIERU_KEYCODE_COMMA     0xbc // ','
#define MIERU_KEYCODE_PERIOD    0xbe // '.'
#define MIERU_KEYCODE_SLASH     0xbf // '/'
#define MIERU_KEYCODE_BKSPACE   0x08 // BS key (delete key)
#define MIERU_KEYCODE_ENTER     0x0d // enter key
#define MIERU_KEYCODE_SPACE     0x20 // space key
#define MIERU_KEYCODE_DELETE    0x2e // delete key (Fn+~)
#define MIERU_KEYCODE_ESCAPE    0x1b // escape key
#define MIERU_KEYCODE_TAB       0x09 // tab key

#define MIERU_KEYCODE_PAGEUP    0x21 // PageUp key
#define MIERU_KEYCODE_PAGEDOWN  0x22 // PageDown key
#define MIERU_KEYCODE_END       0x23 // END key
#define MIERU_KEYCODE_HOME      0x24 // HOME key

#define MIERU_KEYCODE_LEFT      0x25 // left  arrow key
#define MIERU_KEYCODE_UP        0x26 // up    arrow key
#define MIERU_KEYCODE_RIGHT     0x27 // right arrow key
#define MIERU_KEYCODE_DOWN      0x28 // down  arrow key

/* keypad keys */
#define MIERU_KEYCODE_PAD_0      0x60 // 
#define MIERU_KEYCODE_PAD_1      0x61 // 
#define MIERU_KEYCODE_PAD_2      0x62 // 
#define MIERU_KEYCODE_PAD_3      0x63 // 
#define MIERU_KEYCODE_PAD_4      0x64 // 
#define MIERU_KEYCODE_PAD_5      0x65 // 
#define MIERU_KEYCODE_PAD_6      0x66 // 
#define MIERU_KEYCODE_PAD_7      0x67 // 
#define MIERU_KEYCODE_PAD_8      0x68 // 
#define MIERU_KEYCODE_PAD_9      0x69 // 
//#define MIERU_KEYCODE_PAD_00      0x00 // can't use this key

#define MIERU_KEYCODE_PAD_DOT     0x6e // '.'
#define MIERU_KEYCODE_PAD_ENTER   0x6c // enter key
#define MIERU_KEYCODE_PAD_PLUS    0x6b // '+'
#define MIERU_KEYCODE_PAD_MINUS   0x6d // '-'
#define MIERU_KEYCODE_PAD_BKSPACE 0x08 // BS key
#define MIERU_KEYCODE_PAD_AST     0x6a // '*'
#define MIERU_KEYCODE_PAD_SLASH   0x6f // '/'
#define MIERU_KEYCODE_PAD_TAB     0x09 // TAB key
#define MIERU_KEYCODE_PAD_NUMLOCK 0x90 // NumLock key

