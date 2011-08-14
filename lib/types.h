/******************************************************************************/
/* MieruPC Project: Type definition v0.2                                      */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-07-09 */
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
 *@brief 型定義ファイル
 *
 *基本的な型を定義する．
 */


#define NULL  0
#define TRUE  1
#define FALSE 0

#define EOF (-1)

typedef char __s8;             ///< 符号付き8bit:charの別名
typedef unsigned char __u8;    ///< 符号無し8bit:unsigned charの別名

typedef short __s16;           ///< 符号付き16bit:shortの別名
typedef unsigned short __u16;  ///< 符号無し16bit:unsigned shortの別名

typedef int __s32;             ///< 符号付き32bit:intの別名
typedef unsigned int __u32;    ///< 符号無し32bit:unsigned intの別名

typedef __s8 int8;             ///< 符号付き8bit
typedef __u8 uint8;            ///< 符号無し16bit
typedef __s16 int16;           ///< 符号付き16bit
typedef __u16 uint16;          ///< 符号無し16bit
typedef __s32 int32;           ///< 符号付き32bit
typedef __u32 uint32;          ///< 符号無し32bit

typedef unsigned char uchar;   ///< unsigned charの別名
typedef unsigned short ushort; ///< unsigned shortの別名
typedef unsigned int uint;     ///< unsigned intの別名

typedef uint size_t;           ///< unsigned intの別名
typedef uint time_t;           ///< unsigned intの別名

#ifndef va_arg
typedef char *va_list;         ///< 可変長引数を扱う型
#define va_size(type) ((sizeof(type) + sizeof(long) - 1) & (~(sizeof(long) - 1)))
#define va_start(argp, last) ((argp) = (va_list)&(last) + va_size(last))
#define va_arg(argp, type) (*(type *)((argp) += va_size(type), (argp) - va_size(type)))
#define va_end(argp)
#endif /* va_arg */

