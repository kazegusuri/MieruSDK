/******************************************************************************/
/* MieruPC Project: Key Translation file v0.2                                 */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-06-29 */
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
 *@brief キー変換定義ファイル
 *
 * キーコードからキーシンボルへの変換テーブル.
 * このファイルを変更するとキーの割り当てを変更することができる.
 */

unsigned char key2sym[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, // 0
    MKS_BackSpace, MKS_Tab, 0, 0, 0, MKS_Enter, 0, 0, // 0
    0, 0, 0, MKS_Pause, MKS_Caps_Lock, 0, 0, 0, // 1
    0, 0, 0, MKS_Escape, 0, 0, 0, 0, // 1
    MKS_space, MKS_Page_Up, MKS_Page_Down, MKS_End, MKS_Home, MKS_Left, MKS_Up, MKS_Right, // 2
    MKS_Down, 0, 0, 0, MKS_PrintScreen, MKS_Insert, MKS_Insert, 0, // 2
    MKS_0, MKS_1, MKS_2, MKS_3, MKS_4, MKS_5, MKS_6, MKS_7, // 3
    MKS_8, MKS_9, 0, 0, 0, 0, 0, 0, // 3
    0, MKS_a, MKS_b, MKS_c, MKS_d, MKS_e, MKS_f, MKS_g, // 4
    MKS_h, MKS_i, MKS_j, MKS_k, MKS_l, MKS_m, MKS_n, MKS_o, // 4
    MKS_p, MKS_q, MKS_r, MKS_s, MKS_t, MKS_u, MKS_v, MKS_w, // 5
    MKS_x, MKS_y, MKS_z, MKS_Super_L, MKS_Super_R, MKS_Nazo, 0, 0,  // 5
    MKS_PAD_0, MKS_PAD_1, MKS_PAD_2, MKS_PAD_3, MKS_PAD_4, MKS_PAD_5, MKS_PAD_6, MKS_PAD_7, // 6
    MKS_PAD_8, MKS_PAD_9, MKS_PAD_Multiply, MKS_PAD_Add, MKS_PAD_Enter, MKS_PAD_Subtract, MKS_PAD_Decimal, MKS_PAD_Divide,  // 6
    MKS_F1, MKS_F2, MKS_F3, MKS_F4, MKS_F5, MKS_F6, MKS_F7, MKS_F8,  // 7
    MKS_F9, MKS_F10, MKS_F11, MKS_F12, 0, 0, 0, 0,  // 7
    0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0,  // 8
    MKS_Num_Lock, MKS_Scroll_Lock, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0,  // 9
    MKS_Shift_L, MKS_Shift_R, MKS_Control_L, MKS_Control_R, MKS_Alt_L, MKS_Alt_R, 0, 0,  // a
    0, 0, 0, 0, 0, 0, 0, 0,  // a
    0, 0, 0, 0, 0, 0, 0, 0,  // b
    0, 0, MKS_semicolon, MKS_equal, MKS_comma, MKS_minus, MKS_period, MKS_slash,  // b
    MKS_quoteleft, 0, 0, 0, 0, 0, 0, 0,  // c
    0, 0, 0, 0, 0, 0, 0, 0,  // c
    0, 0, 0, 0, 0, 0, 0, 0,  // d
    0, 0, 0, MKS_bracketleft, MKS_backslash, MKS_bracketright, MKS_quoteright, 0,  // d
    0, 0, 0, 0, 0, 0, 0, 0,  // e
    0, 0, 0, 0, 0, 0, 0, 0,  // e
    0, 0, 0, 0, 0, 0, 0, 0,  // f
    0, 0, 0, 0, 0, 0, 0, 0 // f
};

unsigned char shiftkey[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, '"', 0, 0, 0, 0, '<', '_', '>', '?', 
    ')', '!', '@', '#', '$', '%', '^', '&', '*', '(', 0, ':', 0, '+', 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '{', '|', '}', 0, 0, 
    '~', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
