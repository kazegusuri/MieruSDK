/******************************************************************************/
/* MieruPC Project: Memory Allocation Library v1.1                            */
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
 *@brief メモリアロケーションライブラリ
 *
 *いわゆるmallocのライブラリ．
 *このライブラリはOS用のアプリケーションの時のみ使用可能である．
 */


/*!
 *@brief ヒープ領域の管理構造体
 */
struct memory_control_block {
    int is_available; ///< ブロックが使用できるかどうか
    int size;         ///< ブロックサイズ
    struct memory_control_block *prev; ///< 前のブロックへのポインタ
    struct memory_control_block *next; ///< 次のブロックへのポインタ
};

#ifdef MIERUPC_APPLICATION

/*! 
 *@brief ヒープ領域のサイズを変更する．
 *
 *req_sizeで指定された量のヒープ領域を確保する．
 *領域を確保できなかった場合は-1を返す．
 *確保できた場合は確保した領域の先頭アドレスを返す．
 *req_sizeを0で指定した場合は現在のbrkの位置を返す．
 *
 *@param req_size 確保する領域のサイズ
 *@return 確保した領域のアドレス
 */ 
void *mpc_sbrk(int req_size);

/*! 
 *@brief メモリ空間を解放する．
 *
 *pointerで指定したメモリ空間を解放する．
 *mpc_mallocで確保したアドレスでなければならない．
 *
 *@param pointer 解放するアドレス
 */ 
void mpc_free(void *pointer);

/*! 
 *@brief メモリ領域を確保する．
 *
 *req_sizeで指定された量のメモリ領域を確保する．
 *領域を確保できなかった場合はNULLを返す．
 *確保できた場合は確保した領域の先頭アドレスを返す．
 *
 *@param req_size 確保する領域のサイズ
 *@return 確保した領域のアドレス
 */ 
void *mpc_malloc(long int req_size);

/*! 
 *@brief メモリ領域を再確保する．
 *
 *sizeで指定された量のメモリ領域を確保する．
 *領域を確保できなかった場合はNULLを返す．
 *確保できた場合は確保した領域の先頭アドレスを返す．
 *また，以前のメモリ領域の内容をコピーする．
 *
 *@param pointer 確保する領域
 *@param size 確保する領域のサイズ
 *@return 確保した領域のアドレス
 */ 
void *mpc_realloc(void *pointer, int size);

/*! 
 *@brief ヒープ領域の初期化を行う．
 *
 *ヒープ領域の初期化を行う．
 *mpc_malloc初回呼び出し時に呼び出される．
 */ 
void malloc_init();

#endif /* MIERUPC_APPLICATION */
