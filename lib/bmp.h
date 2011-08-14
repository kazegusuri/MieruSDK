/******************************************************************************/
/* MieruPC Project: Bitmap Library v1.3                                       */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-07-15 */
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
 *@brief ビットマップ描画ライブラリ
 *
 *ビットマップ描画ライブラリ．
 *このライブラリはOS用のアプリケーションの時のみ使用可能である．
 */

typedef struct bmp_file_hdr{
    uchar type[2];     ///! シグネチャ "BM"
    uint size;         ///! ファイルサイズ
    ushort resv1;      ///! 予約領域 常に0
    ushort resv2;      ///! 予約領域 常に0
    uint offset;       ///! ファイル先頭から画像データまでのオフセット
} __attribute__((packed)) bmp_file_hdr_t;

typedef struct bmp_info_hdr{
    uint size;        ///! 情報ヘッダのサイズ 12を指す
    int width;        ///! 画像の幅(ピクセル)
    int height;       ///! 画像の高さ(ピクセル)
    ushort plane;     ///! プレーン数
    ushort bitcount;  ///! 1画素あたりのデータサイズ(8だと256色)
    uint compression; ///! 圧縮形式 0だと無圧縮
    uint imagesize;   ///! 画像データ部のサイズ
    int hresolution;  ///! 横方向解像度
    int vresolution;  ///! 縦方向解像度
    uint palette;     ///! 実際に使用されているカラーパレットの色数
    uint important;   ///! 重要なパレットのインデックス
}  bmp_info_hdr_t;

/*!
 *  8ビットカラー時に使用される構造体.
 *  BGRの3バイトで1つの色を表現する.
 */
typedef struct rgbquad{
    uchar blue;
    uchar green;
    uchar red;
    uchar resv;
}rgbquad_t;

/*!
 *@brief 256色bmpで使われているパレットをロードする
 *
 *256色bmpで使われているパレットをロードする
 *ただし，RGB各色の上位4ビットしか使用しない
 *bmpに256色bmpファイル全体を読み込んでいる必要がある
 *
 *@param palette bmpのパレットデータ
 *
 */ 
void bmp_loadpalette(const rgbquad_t *palette);

/*!
 *@brief 256色bmpファイルの描画を行う
 *
 *256色bmpファイルの描画を行う
 *bmpに256色bmpファイル全体を読み込んでいる必要がある
 *
 *@param bmp 256色bmpファイルを読み込んだバッファ
 *@param x0 描画先の左上の座標（x)
 *@param y0 描画先の左上の座標（y）
 *@param bmp_x0 描画元の左上の座標（x）
 *@param bmp_y0 描画元の左上の座標（y）
 *@param xsize 描画サイズ（幅）
 *@param ysize 描画サイズ（高さ）
 *
 *@return エラー時に1, 正常時に0を返す
 */ 
int bmp_draw(const char *bmp, int x0, int y0, int bmp_x0, int bmp_y0, int xsize, int ysize);

/*!
 *@brief 256色bmpファイルの幅（ピクセル数）を取得
 *
 *256色bmpファイルの幅（ピクセル数）を取得
 *bmpに256色bmpファイルのヘッダを読み込んでいる必要がある
 *
 *@param bmp 256色bmpファイルのヘッダを読み込んだバッファ
 *
 *@return 256色bmpファイルの幅（ピクセル数）を返す
 */
int bmp_getwidth(const char *bmp);

/*!
 *@brief 256色bmpファイルの高さ（ピクセル数）を取得
 *
 *256色bmpファイルの高さ（ピクセル数）を取得
 *bmpに256色bmpファイルのヘッダを読み込んでいる必要がある
 *
 *@param bmp 256色bmpファイルのヘッダを読み込んだバッファ
 *
 *@return 256色bmpファイルの高さ（ピクセル数）を返す
 */
int bmp_getheight(const char *bmp);

/*!
 *@brief 256色bmpファイルのファイルサイズ（バイト単位）を取得
 *
 *@param bmp 256色bmpファイルのヘッダを読み込んだバッファ
 *
 *@return 256色bmpファイルのファイルサイズ（バイト単位）を返す
 */
int bmp_getsize(const char *bmp);
