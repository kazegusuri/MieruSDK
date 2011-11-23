/******************************************************************************/
/* MieruPC Project: Memory Map Definition v0.1                                */
/* written by Masahiro Sano Arch Lab. Tokyo Tech                   2010-06-14 */
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
 *@brief メモリマップ定義ファイル
 *
 *メモリマップ定義ファイル
 *MieruPC2010のメモリマップ．
 */


/* MieruPC2010 Memory Map */
#pragma once

/* Block RAM range */
#define MIERU_BLOCK_S    0x00000000 ///< 物理メモリの開始アドレス
#define MIERU_BLOCK_E    0x00080000 ///< 物理メモリの終了アドレス

/* Memory Mapped I/O */

// For MieruPC 2010
#define MP_MMAP_OFFSET       0x00800000
#define MP_MMAP_KEYCODE      (MP_MMAP_OFFSET + 0x100) ///< キーボードのメモリマップ
#define MP_MMAP_LCD          (MP_MMAP_OFFSET + 0x104) ///< 液晶のメモリマップ
#define MP_MMAP_MMC_CONTROL  (MP_MMAP_OFFSET + 0x108) ///< MMCへのコマンドのメモリマップ
#define MP_MMAP_TIMER        (MP_MMAP_OFFSET + 0x10c) ///< 現在時刻のメモリマップ

#define MP_MMAP_TLBMODE      (MP_MMAP_OFFSET + 0x120) ///< TLBを使用するか

#define MP_MMAP_GPIO_0       (MP_MMAP_OFFSET + 0x1f0) ///< GPIOのメモリマップ
#define MP_MMAP_GPIO_4       (MP_MMAP_OFFSET + 0x1f4) ///< GPIOのメモリマップ
#define MP_MMAP_GPIO_8       (MP_MMAP_OFFSET + 0x1f8) ///< GPIOのメモリマップ
#define MP_MMAP_GPIO_C       (MP_MMAP_OFFSET + 0x1fc) ///< GPIOのメモリマップ

#define MIERU_MMC_DATA_S     (MP_MMAP_OFFSET + 0x200) ///< MMCの領域の開始アドレス
#define MIERU_MMC_DATA_E     (MP_MMAP_OFFSET + 0x400) ///< MMCの領域の終了アドレス
#define MIERU_MAP_END        (MP_MMAP_OFFSET + 0xFFF)
