/******************************************************************************/
/* MieruPC Project: Keysymbol definition file v0.1                            */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-06-14 */
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
 *@brief キーシンボル定義ファイル
 *
 *mpc_getkey,mpc_getcharで得られるキーシンボルの値を定義する.
 */


/* Misc keys */
#define MKS_BackSpace                     (0x80 | 0x00 | 0x01)  ///< Back space, back char
#define MKS_Tab                           (0x80 | 0x00 | 0x02)  ///< Tab key
#define MKS_Linefeed                      (0x80 | 0x00 | 0x03)  ///< Linefeed, LF
#define MKS_Clear                         (0x80 | 0x00 | 0x04)
#define MKS_Enter                         (0x80 | 0x00 | 0x05)  ///< Return, Enter key
#define MKS_Pause                         (0x80 | 0x00 | 0x06)  ///< Pause, hold
#define MKS_Scroll_Lock                   (0x80 | 0x00 | 0x07)
#define MKS_Sys_Req                       (0x80 | 0x00 | 0x08)
#define MKS_Escape                        (0x80 | 0x00 | 0x09)  ///< Escape key
#define MKS_Delete                        (0x80 | 0x00 | 0x0a)  ///< Delete key
#define MKS_Insert                        (0x80 | 0x00 | 0x0b)  ///< Insert key
#define MKS_PrintScreen                   (0x80 | 0x00 | 0x0c)  ///< Print Screen
#define MKS_Num_Lock                      (0x80 | 0x00 | 0x0d)  ///< Num Lock
#define MKS_Caps_Lock                     (0x80 | 0x00 | 0x0e)  ///< Caps lock 
#define MKS_Shift_Lock                    (0x80 | 0x00 | 0x0f)  ///< Shift lock 

#define MKS_Left                          (0x80 | 0x10 | 0x00)  ///< Move left, left arrow
#define MKS_Up                            (0x80 | 0x10 | 0x01)  ///< Move up, up arrow
#define MKS_Right                         (0x80 | 0x10 | 0x02)  ///< Move right, right arrow
#define MKS_Down                          (0x80 | 0x10 | 0x03)  ///< Move down, down arrow
#define MKS_Page_Up                       (0x80 | 0x10 | 0x04)  ///< Page up key
#define MKS_Page_Down                     (0x80 | 0x10 | 0x05)  ///< Page down key
#define MKS_Home                          (0x80 | 0x10 | 0x06)  ///< Home key
#define MKS_End                           (0x80 | 0x10 | 0x07)  ///< End key
#define MKS_Nazo                          (0x80 | 0x10 | 0x0f)  ///< Nazo no key

/* F key 0xa0 - 0xaf */
#define MKS_F1                            (0x80 | 0x20 | 0x01)
#define MKS_F2                            (0x80 | 0x20 | 0x02)
#define MKS_F3                            (0x80 | 0x20 | 0x03)
#define MKS_F4                            (0x80 | 0x20 | 0x04)
#define MKS_F5                            (0x80 | 0x20 | 0x05)
#define MKS_F6                            (0x80 | 0x20 | 0x06)
#define MKS_F7                            (0x80 | 0x20 | 0x07)
#define MKS_F8                            (0x80 | 0x20 | 0x08)
#define MKS_F9                            (0x80 | 0x20 | 0x09)
#define MKS_F10                           (0x80 | 0x20 | 0x0a)
#define MKS_F11                           (0x80 | 0x20 | 0x0b)
#define MKS_F12                           (0x80 | 0x20 | 0x0c)

/* Modifiers 0xf0 - 0xff */
#define MKS_Shift_L                       (0x80 | 0x70 | 0x00)  ///< Left shift 
#define MKS_Shift_R                       (0x80 | 0x70 | 0x01)  ///< Right shift 
#define MKS_Control_L                     (0x80 | 0x70 | 0x02)  ///< Left control 
#define MKS_Control_R                     (0x80 | 0x70 | 0x03)  ///< Right control 
#define MKS_Meta_L                        (0x80 | 0x70 | 0x04)  ///< Left meta 
#define MKS_Meta_R                        (0x80 | 0x70 | 0x05)  ///< Right meta 
#define MKS_Alt_L                         (0x80 | 0x70 | 0x06)  ///< Left alt 
#define MKS_Alt_R                         (0x80 | 0x70 | 0x07)  ///< Right alt 
#define MKS_Super_L                       (0x80 | 0x70 | 0x08)  ///< Left super 
#define MKS_Super_R                       (0x80 | 0x70 | 0x09)  ///< Right super

/* printable characters 0x20-0x7f */
#define MKS_space                         0x20  ///< ' '
#define MKS_exclam                        0x21  ///< '!'
#define MKS_quotedbl                      0x22  ///< '"'
#define MKS_numbersign                    0x23  ///< '#'
#define MKS_dollar                        0x24  ///< '$'
#define MKS_percent                       0x25  ///< '%'
#define MKS_ampersand                     0x26  ///< '^'
#define MKS_apostrophe                    0x27  ///< '''
#define MKS_quoteright                    0x27  ///< '''
#define MKS_parenleft                     0x28  ///< '('
#define MKS_parenright                    0x29  ///< ')'
#define MKS_asterisk                      0x2a  ///< '*'
#define MKS_plus                          0x2b  ///< '+'
#define MKS_comma                         0x2c  ///< ','
#define MKS_minus                         0x2d  ///< '-'
#define MKS_period                        0x2e  ///< '.'
#define MKS_slash                         0x2f  ///< '/'
#define MKS_0                             0x30
#define MKS_1                             0x31
#define MKS_2                             0x32
#define MKS_3                             0x33
#define MKS_4                             0x34
#define MKS_5                             0x35
#define MKS_6                             0x36
#define MKS_7                             0x37
#define MKS_8                             0x38
#define MKS_9                             0x39
#define MKS_colon                         0x3a  ///< ':'
#define MKS_semicolon                     0x3b  ///< ';'
#define MKS_less                          0x3c  ///< '<'
#define MKS_equal                         0x3d  ///< '='
#define MKS_greater                       0x3e  ///< '>'
#define MKS_question                      0x3f  ///< '?'
#define MKS_at                            0x40  ///< '@'
#define MKS_A                             0x41
#define MKS_B                             0x42
#define MKS_C                             0x43
#define MKS_D                             0x44
#define MKS_E                             0x45
#define MKS_F                             0x46
#define MKS_G                             0x47
#define MKS_H                             0x48
#define MKS_I                             0x49
#define MKS_J                             0x4a
#define MKS_K                             0x4b
#define MKS_L                             0x4c
#define MKS_M                             0x4d
#define MKS_N                             0x4e
#define MKS_O                             0x4f
#define MKS_P                             0x50
#define MKS_Q                             0x51
#define MKS_R                             0x52
#define MKS_S                             0x53
#define MKS_T                             0x54
#define MKS_U                             0x55
#define MKS_V                             0x56
#define MKS_W                             0x57
#define MKS_X                             0x58
#define MKS_Y                             0x59
#define MKS_Z                             0x5a
#define MKS_bracketleft                   0x5b  ///< '['
#define MKS_backslash                     0x5c  ///< '\'
#define MKS_bracketright                  0x5d  ///< ']'
#define MKS_asciicircum                   0x5e  ///< '^'
#define MKS_underscore                    0x5f  ///< '_'
#define MKS_quoteleft                     0x60  ///< '`'
#define MKS_a                             0x61
#define MKS_b                             0x62
#define MKS_c                             0x63
#define MKS_d                             0x64
#define MKS_e                             0x65
#define MKS_f                             0x66
#define MKS_g                             0x67
#define MKS_h                             0x68
#define MKS_i                             0x69
#define MKS_j                             0x6a
#define MKS_k                             0x6b
#define MKS_l                             0x6c
#define MKS_m                             0x6d
#define MKS_n                             0x6e
#define MKS_o                             0x6f
#define MKS_p                             0x70
#define MKS_q                             0x71
#define MKS_r                             0x72
#define MKS_s                             0x73
#define MKS_t                             0x74
#define MKS_u                             0x75
#define MKS_v                             0x76
#define MKS_w                             0x77
#define MKS_x                             0x78
#define MKS_y                             0x79
#define MKS_z                             0x7a
#define MKS_braceleft                     0x7b  ///< '{'
#define MKS_bar                           0x7c  ///< '|'
#define MKS_braceright                    0x7b  ///< '}'


/* Keypad functions, keypad numbers cleverly chosen to map to ASCII */
/* 0xc0 - 0xef */
#define MKS_PAD_Space                      (0x00 | 0xc0)  ///< Space on numpad
#define MKS_PAD_Tab                        (0x01 | 0xc0)
#define MKS_PAD_Enter                      (0x02 | 0xc0)  ///< Enter on numpad
#define MKS_PAD_F1                         (0x03 | 0xc0)  
#define MKS_PAD_F2                         (0x04 | 0xc0)
#define MKS_PAD_F3                         (0x05 | 0xc0)
#define MKS_PAD_F4                         (0x06 | 0xc0)
#define MKS_PAD_Home                       (0x07 | 0xc0)  ///< Home on numpad
#define MKS_PAD_Left                       (0x08 | 0xc0)  ///< left arrow on numpad
#define MKS_PAD_Up                         (0x09 | 0xc0)  ///< up arrow on numpad
#define MKS_PAD_Right                      (0x0a | 0xc0)  ///< right arrow on numpad
#define MKS_PAD_Down                       (0x0b | 0xc0)  ///< down arrow on numpad
#define MKS_PAD_Prior                      (0x0c | 0xc0)
#define MKS_PAD_Page_Up                    (0x0d | 0xc0)
#define MKS_PAD_Next                       (0x0e | 0xc0)
#define MKS_PAD_Page_Down                  (0x0f | 0xc0)
#define MKS_PAD_End                        (0x10 | 0xc0)  ///< End on numpad
#define MKS_PAD_Begin                      (0x11 | 0xc0)
#define MKS_PAD_Insert                     (0x12 | 0xc0)  ///< Insert on numpad
#define MKS_PAD_Delete                     (0x13 | 0xc0)  ///< Delete on numpad
#define MKS_PAD_Equal                      (0x14 | 0xc0) 
#define MKS_PAD_Multiply                   (0x15 | 0xc0)  ///< '*'
#define MKS_PAD_Add                        (0x16 | 0xc0)  ///< '+'
#define MKS_PAD_Separator                  (0x17 | 0xc0)  ///< '/'
#define MKS_PAD_Subtract                   (0x18 | 0xc0)  ///< '-'
#define MKS_PAD_Decimal                    (0x19 | 0xc0)  ///< '.'
#define MKS_PAD_Divide                     (0x1a | 0xc0)  ///< '/'

#define MKS_PAD_0                          (0x20 | 0xc0)
#define MKS_PAD_1                          (0x21 | 0xc0)
#define MKS_PAD_2                          (0x22 | 0xc0)
#define MKS_PAD_3                          (0x23 | 0xc0)
#define MKS_PAD_4                          (0x24 | 0xc0)
#define MKS_PAD_5                          (0x25 | 0xc0)
#define MKS_PAD_6                          (0x26 | 0xc0)
#define MKS_PAD_7                          (0x27 | 0xc0)
#define MKS_PAD_8                          (0x28 | 0xc0)
#define MKS_PAD_9                          (0x29 | 0xc0)
