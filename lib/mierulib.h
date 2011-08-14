/******************************************************************************/
/* MieruPC Project: MieruPc Standard Library v1.4                             */
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
 *@brief MieruPC用基本ライブラリ
 *
 *MieruPC用のアプリケーションで使われる基本的なライブラリ．
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "rename.h"
#include "types.h"
#include "mmap.h"
#include "keysym.h"
#include "itcfunc.h"
#include "syscall.h"
#include "xmalloc.h"
#include "bmp.h"

#ifdef __cplusplus
}
#endif

#define OPT_SHT_BIT 0x1 ///< Shift keyの押下状態を示すビット
#define OPT_CTL_BIT 0x2 ///< Control keyの押下状態を示すビット
#define OPT_ALT_BIT 0x4 ///< Alt keyの押下状態を示すビット
#define OPT_WIN_BIT 0x8 ///< Windows keyの押下状態を示すビット

#define MLIB_VERSION "v1.1" ///< MieruLibのバージョン


/*************************************************************************
 ***    string fucntions           
 *************************************************************************/

/*! 
 *@brief 数値をn進数の文字列に変換する．
 *
 *数値numをbase進数の文字列に変換し，bufに書き込む．
 *文字列が終端endを越える場合は切り詰められる．
 *文字列の長さが最低サイズsizeを満たない場合は，
 *文字padを先頭に挿入する．
 *
 *@param buf 出力先バッファ
 *@param end 文字列の終端
 *@param num 変換する値
 *@param base 変換する進数
 *@param size 文字列の最低サイズ
 *@param pad 空白を埋める文字
 *@return 書き込まれた文字数
 */ 
char *ntos(char *buf, char *end, uint num, int base, int size, int pad);

/*! 
 *@brief 指定された書式に変換して出力．
 *
 *フォーマット文字列にしたがって出力を生成し，バッファに書き込む．
 *sizeを越える場合は切り詰める．
 *
 *@param buf 出力先バッファ
 *@param size バッファの最大サイズ
 *@param fmt フォーマット文字列
 *@param args 引数のリスト
 *@return 書き込まれた文字数
 */ 
int mpc_vsnprintf(char *buf, unsigned int size, const char *fmt, va_list args);


/*! 
 *@brief 指定された書式に変換して出力．
 *
 *フォーマット文字列にしたがって出力を生成し，バッファに書き込む．
 *sizeを越える場合は切り詰める．
 *
 *@param buf 出力先バッファ
 *@param size バッファの最大サイズ
 *@param fmt フォーマット文字列
 *@return 書き込まれた文字数
 */ 
int mpc_snprintf(char *buf, unsigned int size, const char *fmt, ...);

/*! 
 *@brief 文字列をコピーする．
 *
 *srcのnバイトをdstにコピーする．
 *srcがnバイトより短い場合は残りをヌルバイトで埋める．
 *
 *@param dst 出力先バッファ
 *@param src 出力元バッファ
 *@param n コピーするバイト数
 *@return dstへのポインタ
 */ 
char *mpc_strncpy(char *dst, const char *src, unsigned int n);

/*! 
 *@brief 文字列をコピーする．
 *
 *srcの文字列をdstにコピーする．
 *
 *@param dst 出力先バッファ
 *@param src 出力元バッファ
 *@return dstへのポインタ
 */ 
char *mpc_strcpy(char *dst, const char *src);

/*! 
 *@brief 文字列の長さを計算する．
 *
 *文字列sの長さを計算する．
 *終端文字は計算に含まれない．
 *
 *@param s 文字列
 *@return sの文字数
 */ 
unsigned int mpc_strlen(const char *s);

/*! 
 *@brief 2つの文字列を比較する．
 *
 *2つの文字列を先頭からnバイト比較する．
 *s1がs2に比べて小さいか，等しいか，大きいかを返す．
 *
 *@param s1 比較する文字列
 *@param s2 比較する文字列
 *@param n 比較する文字数
 *@return 比較結果を示す符号付整数
 */ 
int mpc_strncmp(const char *s1, const char *s2, unsigned int n);

/*! 
 *@brief 2つの文字列を比較する．
 *
 *2つの文字列を比較する．
 *s1がs2に比べて小さいか，等しいか，大きいかを返す．
 *
 *@param s1 比較する文字列
 *@param s2 比較する文字列
 *@return 比較結果を示す符号付整数
 */ 
int mpc_strcmp(const char *s1, const char *s2);

/*! 
 *@brief 文字列中の文字の位置を特定する．
 *
 *文字列s1中で最初に文字cが現れる位置を見つける．
 *
 *@param s 文字列
 *@param c 文字
 *@return 部分文字列の開始位置のポインタ
 */ 
char *mpc_strchr(const char *s, int c);

/*! 
 *@brief 部分文字列の位置を示す．
 *
 *部分文字列s2が文字列s1中で最初に現れる位置を見つける．
 *
 *@param s1 文字列
 *@param s2 部分文字列
 *@return 部分文字列の開始位置のポインタ
 */ 
char *mpc_strstr(const char *s1, const char *s2);


/*! 
 *@brief 2つの文字列を連結する．
 *
 *文字列s1の後に文字列s2を付け加え，終端文字\0を加える．
 *
 *@param s1 文字列
 *@param s2 追加する文字列
 *@return 連結してできた文字列ののポインタ
 */ 
char *mpc_strcat(char *s1, const char *s2);

/*! 
 *@brief 2つの文字列を連結する．
 *
 *文字列s1の後に文字列s2を付け加え，終端文字\0を加える．
 *
 *@param s1 文字列
 *@param s2 追加する文字列
 *@param n 追加する最大文字数
 *@return 連結してできた文字列ののポインタ
 */ 
char *mpc_strncat(char *s1, const char *s2, int n);

/*! 
 *@brief 文字列を整数型に変換する．
 *
 *文字列aの初めの部分をint型整数に変換する．
 *
 *@param a 文字列
 *@return 変換された値
 */ 
int mpc_atoi(char *a);

/*************************************************************************
 ***    character fucntions           
 *************************************************************************/

/*! 
 *@brief 文字を分類する．
 *
 *文字cが制御文字か調べる．
 *制御文字の場合は1を，そうでない場合は0を返す．
 *
 *@param c 文字
 *@return 制御文字の場合は1，それ以外は0．
 */ 
int mpc_iscntrl(char c);

/*! 
 *@brief 文字を分類する．
 *
 *文字cが表示可能文字か調べる．
 *表示可能文字の場合は1を，そうでない場合は0を返す．
 *
 *@param c 文字
 *@return 表示可能文字の場合は1，それ以外は0．
 */ 
int mpc_isprint(char c);

/*! 
 *@brief 文字を分類する．
 *
 *文字cが数字か調べる．
 *数字の場合は1を，そうでない場合は0を返す．
 *
 *@param c 文字
 *@return 数字の場合は1，それ以外は0．
 */ 
int mpc_isdigit(char c);

/*! 
 *@brief 文字を分類する．
 *
 *文字cがホワイトスペースか調べる．
 *ホワイトスペースの場合は1を，そうでない場合は0を返す．
 *
 *@param c 文字
 *@return ホワイトスペースの場合は1，それ以外は0．
 */ 
int mpc_isspace(char c);

/*! 
 *@brief 文字を小文字にする．
 *
 *文字cが小文字に変換可能なら小文字にする．
 *変換できない場合は変換しない．
 *
 *@param c 文字
 *@return 変換後の文字cを返す．
 */ 
char mpc_tolower(char c);

/*! 
 *@brief 文字を大文字にする．
 *
 *文字cが大文字に変換可能なら大文字にする．
 *変換できない場合は変換しない．
 *
 *@param c 文字
 *@return 変換後の文字cを返す．
 */ 
char mpc_toupper(char c);

/*! 
 *@brief 文字を分類する．
 *
 *文字cが大文字か調べる．
 *大文字の場合は1を，そうでない場合は0を返す．
 *
 *@param c 文字
 *@return 大文字の場合は1，それ以外は0．
 */ 
int mpc_islower(char c);

/*! 
 *@brief 文字を分類する．
 *
 *文字cが小文字か調べる．
 *小文字の場合は1を，そうでない場合は0を返す．
 *
 *@param c 文字
 *@return 小文字の場合は1，それ以外は0．
 */ 
int mpc_isupper(char c);

/*************************************************************************
 ***    memory fucntions           
 *************************************************************************/

/*! 
 *@brief 指定した値でメモリ領域を埋める．
 *
 *バッファsの先頭からnバイトをcで埋める．
 *
 *@param s バッファ
 *@param c 書き込む値
 *@param n 書き込むバイト数
 *@return 変換後の文字c
 */ 
void *mpc_memset(void *s, int c, unsigned int n);

/*! 
 *@brief メモリ領域を比較する．
 *
 *メモリ領域s1とs2の最初のnバイトを比較する．
 *s1がs2より小さいか，等しいか，大きいかを返す．
 *
 *@param s1 比較するバッファ
 *@param s2 比較するバッファ
 *@param n 比較するバイト数
 *@return 比較結果を示す符号付整数
 */
int mpc_memcmp(const void *s1, const void *s2, unsigned int n);

/*! 
 *@brief メモリ領域をコピーする．
 *
 *メモリ領域s1の先頭nバイトをメモリ領域dstにコピーする．
 *
 *@param dst コピー先メモリ領域
 *@param src コピー元メモリ領域
 *@param n コピーするバイト数
 *@return dstへのポインタ
 */
#ifdef __cplusplus
extern "C" {
#endif
void *mpc_memcpy(void *dst, const void *src, unsigned int n);
#ifdef __cplusplus
}
#endif

/*************************************************************************
 ***    other fucntions           
 *************************************************************************/

/*! 
 *@brief スタートアップ処理を行う．
 *
 *main関数を呼び出すための前処理と後処理を行う．
 */
#ifndef __cplusplus
void mpc_startup(void);
#endif

/*! 
 *@brief キー入力を待つ．
 *
 *エンターキーの入力があるまで処理を止める．
 *
 */
void mpc_waitkey(void);

/*! 
 *@brief 時刻を取得する．
 *
 *この関数は絶対に使用してはいけない.
 *過去の互換性を維持するためだけに使われる.
 *単位は1ミリ秒である．
 *
 *@return 現在時刻
 */
unsigned int mpc_getoldtime(void);

/*! 
 *@brief 時刻を取得する．
 *
 *現在の時刻を取得する．
 *単位は1秒である．
 *
 *@return 現在時刻
 */
unsigned int mpc_gettime(void);

/*! 
 *@brief 時刻を取得する．
 *
 *現在の時刻を取得する．
 *単位は1マイクロ秒である．
 *
 *@return 現在時刻
 */
unsigned int mpc_getutime(void);

/*! 
 *@brief 時刻を取得する．
 *
 *現在の時刻を取得する．
 *単位は1ミリ秒である．
 *
 *@return 現在時刻
 */
unsigned int mpc_getmtime(void);

/*! 
 *@brief マイクロ秒単位で処理を止める．
 *
 *指定した時間，処理を止める．
 *時間の単位はマイクロ秒である．
 *
 *@param tm 処理を止める時間
 */
void mpc_usleep(unsigned int tm);

/*! 
 *@brief 入力されたキーのシンボルを取得する．
 *
 *入力されたキーのキーシンボルを返す．
 *引数がNULL以外の場合は，キーシンボルがメイクコードかブレイクコードかを示す値を格納する．
 *メイクコードの場合は1を，ブレイクコードの場合は0が格納される．
 *キーシンボルはkeysym.hを参照．
 *特殊キー(shift,alt,controlなど)の押下状態を確認するには
 *それぞれmpc_onshift関数，mpc_onalt関数，mpc_oncontrol関数，mpc_onsuper関数を利用する．
 *
 *@param make メイクコードかブレイクコードかを返す
 *@return キーシンボル
 */
uchar mpc_getkey(uchar *make);

/*! 
 *@brief 入力されたキーを取得する．
 *
 *入力されたキーを返す．
 *mpc_getkeyとの違いはshiftキーなどを考慮するかどうかである．
 *また，altなどのModifierのコードを返さない．
 *特殊キー(shift,alt,controlなど)の押下状態を確認するには
 *それぞれmpc_onshift関数，mpc_onalt関数，mpc_oncontrol関数，mpc_onsuper関数を利用する．
 *
 *@return キーシンボル
 */
unsigned int mpc_getchar();

/*! 
 *@brief shiftキーが押されているか調べる．
 *
 *押されている時は1を，そうでない時は0を返す
 *
 *@return  *押されている時は1を，そうでない時は0を返す
 */
int mpc_onshift();

/*! 
 *@brief controlキーが押されているか調べる．
 *
 *押されている時は1を，そうでない時は0を返す
 *
 *@return  *押されている時は1を，そうでない時は0を返す
 */
int mpc_oncontrol();

/*! 
 *@brief altキーが押されているか調べる．
 *
 *押されている時は1を，そうでない時は0を返す
 *
 *@return  *押されている時は1を，そうでない時は0を返す
 */
int mpc_onalt();

/*! 
 *@brief superキーが押されているか調べる．
 *
 *押されている時は1を，そうでない時は0を返す
 *
 *@return  *押されている時は1を，そうでない時は0を返す
 */
int mpc_onsuper();

/*! 
 *@brief 乱数を取得する.
 *
 *乱数を取得する.
 *
 *@return 乱数
 */
int mpc_rand(void);

/*! 
 *@brief 乱数のシードを設定する.
 *
 *乱数のシードを設定する.
 *
 *@param seed シードの値
 */
void mpc_srand(uint seed);

/*! 
 *@brief gpioから1byte読み込む.
 *
 *gpioのn番目から1byte読み込む
 *
 *@param n n番目のgpio
 *@return gpioから読み込んだ値
 */
uint mpc_read_gpio(int n);

/*! 
 *@brief gpioから1byte書き込む
 *
 *gpioのn番目から1byte書き込む
 *
 *@param n n番目のgpio
 *@param val 書き込む値
 */
void mpc_write_gpio(int n, uint val);


#ifdef __cplusplus
extern "C" {
#endif
unsigned long udivmodsi4(unsigned long num, unsigned long den, int modwanted);
long __udivsi3 (long a, long b);
long __umodsi3 (long a, long b);
long __divsi3 (long a, long b);
long __modsi3 (long a, long b);
#ifdef __cplusplus
}
#endif




