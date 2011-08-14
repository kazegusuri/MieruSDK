/******************************************************************************/
/* MieruPC Project: LCD Library itcfunc v3.4                                  */
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
 *@brief MieruLCD用コマンドライブラリ
 *
 *MieruLCD用のコマンドライブラリ
 */

#define DEFAULT_WIDTH  480   ///< ディスプレイの横サイズ
#define DEFAULT_HEIGHT 272   ///< ディスプレイの縦サイズ
#define BUF_SIZE 256         ///< printfで使用するバッファサイズ
#define PAGE_SIZE 3          ///< 最大ページサイズ

#define CURSOR_WIDTH  (DEFAULT_WIDTH  / 8)  ///< カーソルの横サイズ
#define CURSOR_HEIGHT (DEFAULT_HEIGHT / 16) ///< カーソルの縦サイズ

#define COL_BLACK       0x00 ///< 色指定に使用
#define COL_DARKRED     0x80 ///< 色指定に使用
#define COL_DARKGREEN   0x10 ///< 色指定に使用
#define COL_DARKBLUE    0x02 ///< 色指定に使用
#define COL_DARKCYAN    0x12 ///< 色指定に使用
#define COL_DARKMAGENTA 0x82 ///< 色指定に使用
#define COL_DARKYELLOW  0x90 ///< 色指定に使用
#define COL_GRAY        0xb6 ///< 色指定に使用
#define COL_DARKGRAY    0x6d ///< 色指定に使用
#define COL_RED         0xe0 ///< 色指定に使用
#define COL_GREEN       0x1c ///< 色指定に使用
#define COL_BLUE        0x03 ///< 色指定に使用
#define COL_CYAN        0x1f ///< 色指定に使用
#define COL_MAGENTA     0xe3 ///< 色指定に使用
#define COL_YELLOW      0xfc ///< 色指定に使用
#define COL_WHITE       0xff ///< 色指定に使用


/*!
 *  ディスプレイの状態変数を格納する構造体．
 */
typedef struct {
    int width, height;
    int showpage, drawpage;
    int scrollx[PAGE_SIZE], scrolly[PAGE_SIZE];
    int cursorx, cursory;
    int posx, posy;
    int scroll_mode;
    int cursor_mode;
    int foreground, background;
} display_t;


void lcd_sendchar(char);
void lcd_sendcmd(char *);

/*! 
 *@brief ディスプレイ変数を取得する．
 *ディスプレイ変数を取得する．
 *@return ディスプレイ変数
 */ 
display_t lcd_getdisplay();

/*! 
 *@brief ディスプレイ変数を設定する．
 *ディスプレイ変数を設定する．
 *@param disp ディスプレイ変数
 */ 
void lcd_setdisplay(display_t disp);

/*! 
 *@brief スクロールモードの設定
 *
 *カーソルが画面の一番下にある時に改行を行った場合
 *画面をスクロールするかどうか．
 *スクロールモードがonのときはスクロールし，
 *offの時はカーソルが画面の一番上に来る．
 * - on=0 スクロールモードを解除
 * - on=1 スクロールモードにする
 *
 *@param on スクロールモード
 */ 
void lcd_setscrollmode(int on);

/*! 
 *@brief 液晶のリセット
 *
 *液晶の状態を初期状態へリセットする．
 */ 
void lcd_reset(void);

/*! 
 *@brief 画面初期化
 *
 *各ページや座標などを初期化する．
 *プログラム開始時に1回だけ呼び出す必要がある．
 *
 *@return 真を返す
 */ 
int  lcd_init(void);

/*! 
 *@brief 画面初期化
 *
 *各ページや座標などを初期化する．
 *プログラム開始時に1回だけ呼び出す必要がある．
 *
 *@param rotate 回転するかどうか
 *
 *@return 真を返す
 */ 
int  lcd_ttyopen(int rotate);

/*! 
 *@brief 終了処理
 *
 *現時点では空の関数である．
 */ 
void lcd_ttyclose();

/*! 
 *@brief 画面表示の切り替え
 *
 *現在この関数は無効化されている．
 *初期化後に動的に切り替えたい場合などに使用する．
 * - r=0 画面を縦表示にする
 * - r=1 画面を横表示にする
 *
 *@param r 表示モード
 */ 
void lcd_rotate(int r);

/*! 
 *@brief 画面の横サイズを返す
 *
 *縦表示なら272, 横表示なら480が返るはず
 *@return 画面の横サイズ
 */ 
int lcd_getwidth();

/*! 
 *@brief 画面の縦サイズを返す
 *
 *縦表示なら480, 横表示なら272が返るはず
 *@return 画面の縦サイズ
 */ 
int lcd_getheight();


/* page functions */


/*! 
 *@brief 現在の表示ページ番号を返す
 *
 *デフォルトでは表示ページ番号は0になっている
 *@return 現在の表示ページ番号(0~2)
 */ 
int lcd_getshowpage();

/*! 
 *@brief 表示ページをセットする
 *
 *デフォルトでは表示ページ番号は0になっている
 *@param page セットする表示ページ番号(0~2)
 */ 
void lcd_setshowpage(int page);

/*! 
 *@brief 現在の描画ページ番号を返す
 *
 *デフォルトでは描画ページ番号は0になっている
 *@return 現在の描画ページ番号(0~2)
 */ 
int lcd_getdrawpage();

/*! 
 *@brief 描画ページをセットする
 *
 *デフォルトでは描画ページ番号は0になっている
 *@param page セットする描画ページ番号(0~2)
 */ 
void lcd_setdrawpage(int page);

/*! 
 *@brief 現在の描画ページを背景色でクリアする
 *
 *背景色は，lcd_setbackground()で指定する
 */ 
void lcd_clear();

/*! 
 *@brief 指定した番号のページを背景色でクリアする
 *
 *背景色は，lcd_setbackground()で指定する
 *@param page クリアするページの番号(0~2)
 */ 
void lcd_clearpage(int page);

/*! 
 *@brief 現在の描画ページを背景色でクリアし，かつカーソル位置を(0,0)に戻す
 *
 *背景色は，lcd_setbackground()で指定する
 */ 
void lcd_cls();

/*! 
 *@brief ページの間コピー
 *
 *srcのページをdestのページに丸ごとコピーする
 *@param src コピー元のページ番号
 *@param dest コピー先のページ番号
 */ 
void lcd_copypage(int src, int dest);


/* character functions */

/*! 
 *@brief 文字の表示モードの設定
 *
 * - t=0 文字色と背景色の両方を描画
 * - t=1 文字色のみ描画
 *
 *@param t 表示モード
 */ 
void lcd_settransparent(int t);

/*! 
 *@brief カーソルの方式を設定
 *
 * - t=0 カーソル非表示
 * - t=1 下線が点滅表示
 * - t=2 全体が点滅表示
 *
 *@param t 表示モード
 */ 
void lcd_setcursor(int t);

/*! 
 *@brief 前景色の設定
 *
 *前景色の指定を行う．
 *以降，文字の色とドットの色はこの関数で指定した色になる．
 *色を変えたい場合は再びこの関数で指定する．
 *
 *色番号の指定はcolorの下位8ビットに対して，RRRGGGBBとする．
 *つまり指定できる範囲は，R(000~111),G(000~111),B(00~11)となる．
 *@param color 色番号
 */ 
void lcd_setcolor(int color);

/*! 
 *@brief 前景色の設定
 *
 *lcd_setcolorと全く同じ
 *@param color 色番号
 */ 
void lcd_setforeground(int color);

/*! 
 *@brief 背景色の設定
 *
 *背景色の指定を行う．
 *以降，文字の背景色とクリアする色はこの関数で指定した色になる．
 *色を変えたい場合は再びこの関数で指定する．
 *
 *色番号の指定は前景色と同様はcolorの下位8ビットに対して，RRRGGGBBとする．
 *つまり指定できる範囲は，R(000~111),G(000~111),B(00~11)となる．
 *@param color 色番号
 */ 
void lcd_setbackground(int color);

/*! 
 *@brief 現在のカーソル位置のx座標を返す
 *
 *返り値はカーソルの座標系で返される(ピクセルではない).
 *つまり返り値の範囲は，横画面の場合0~59である
 *@return 現在のカーソル位置のx座標
 */ 
int lcd_getcurx();


/*! 
 *@brief 現在のカーソル位置のy座標を返す
 *
 *返り値はカーソルの座標系で返される(ピクセルではない).
 *つまり返り値の範囲は，横画面の場合0~16である
 *@return 現在のカーソル位置のy座標
 */ 
int lcd_getcury();


/*! 
 *@brief カーソル位置を設定する
 *
 *カーソル位置を(x,y)の座標にセットする
 *x,yはカーソルの座標系で指定する．(ピクセル指定ではない)
 *
 *@param x カーソル位置のx座標
 *@param y カーソル位置のy座標
 */ 
void lcd_locate(int x, int y);

/*! 
 *@brief カーソルを改行する
 *
 *カーソル位置を改行する
 *厳密にはカーソルのx座標を0, y座標を+1している
 *改行により画面をはみ出す場合，
 *スクロールモードの場合は画面をスクロールし，y座標は16のままである．
 *スクロールモードでない場合はy座標は0となる．
 */ 
void lcd_nextline();


/*! 
 *@brief 指定した文字列を表示する
 *一文字のサイズは8x16
 *
 *printfのような書式は使えないが、\nで改行を行う
 *\nをつけていなくても文字が端までいけば自動的に改行する．
 *一番下まで文字が行った場合，処理はスクロールモードによって異なる．
 *@param s 表示文字列
 */
void lcd_puts(const char *s);


/*! 
 *@brief 指定した文字列を表示する
 *lcd_putsと全く同じである．
 *@param str 表示文字列
 */
void lcd_print(const char *str);


/*! 
 *@brief 指定した文字列を表示する(書式フォーマット指定可能)
 *%c,%d,%s,%x,%pが使用可能．
 *@param fmt 表示文字列
 */
int  lcd_printf(const char *fmt, ...);


/*! 
 *@brief 指定した文字列をデバッグ表示する(書式フォーマット指定可能)
 *%c,%d,%s,%x,%pが使用可能．
 *@param fmt 表示文字列
 */
int  lcd_dprintf(const char *fmt, ...);

/*! 
 *@brief カーソルを右に移動する．
 *
 *カーソルを右に移動する．
 */
void lcd_moveright();

/*! 
 *@brief カーソルを左に移動する．
 *
 *カーソルを左に移動する．
 */
void lcd_moveleft();


/***** graphic functions **********************************************/
/**********************************************************************/
/**********************************************************************/

/*! 
 *@brief パレットカラーを変更する
 *
 *MieruPCで使用する256色カラーパレットの色を変更する．
 *引数i番目の色を12bitカラーで指定することができる．
 *色はrgbを各8bitで指定するが有効なのは上位4bitのみである．
 */ 
void lcd_setpalette(int i, uchar red, uchar green, uchar blue);

/*! 
 *@brief 1ドット描画
 *
 *現在指定されている座標に1ドット描画する．
 *色はlcd_setforeground()で指定した色になる．
 */ 
void lcd_drawdot();


/*! 
 *@brief 1ドット描画(色指定)
 *
 *現在指定されている座標に，指定した色で1ドット描画する．
 *@param col 色番号
 */ 
void lcd_drawdotcolor(int col);

/*! 
 *@brief 4ドット表示
 *
 *
 *この関数は絶対に使用してはいけない．
 *過去の互換を維持するために実装されている．
 *X方向に4個並べてドットを表示する．
 *途中で画面端にはみ出した場合は，同行の左から残りのドットが打たれる．
 *@param c0 1番目のドットの色番号
 *@param c1 2番目のドットの色番号
 *@param c2 3番目のドットの色番号
 *@param c3 4番目のドットの色番号
 */
void lcd_drawdot4(int c0, int c1, int c2, int c3);

/*! 
 *@brief 現在のグラフィック位置のx座標を返す
 *
 *返り値はピクセル.
 *@return 現在のグラフィック位置のx座標
 */ 
int lcd_getposx();

/*! 
 *@brief 現在のグラフィック位置のy座標を返す
 *
 *返り値はピクセル.
 *@return 現在のグラフィック位置のy座標
 */ 
int lcd_getposy();

/*! 
 *@brief グラフィック位置を設定する
 *
 *引数はピクセル.
 *@param x 設定するグラフィック位置のx座標
 *@param y 設定するグラフィック位置のy座標
 */ 
void lcd_setpos(int x, int y);

/*! 
 *@brief ページ間の部分コピー(サイズ固定)
 *
 *p0のページをp1のページに部分コピーする．
 *コピーするブロックサイズは8x16で固定されている．
 *
 *x0とy0でコピー元のブロックのカーソル座標を指定する．
 *x1とy1でコピー先のブロックのカーソル座標を指定する．
 *
 *modeは
 * - 0:普通コピー
 * - 1:透過コピー(現在設定している背景色はコピーしない)
 *
 *@param p0   コピー元のページ番号
 *@param x0   コピー元のブロックのX座標
 *@param y0   コピー元のブロックのy座標
 *@param p1   コピー先のページ番号
 *@param x1   コピー先のブロックのX座標
 *@param y1   コピー先のブロックのy座標
 *@param mode コピーモード
 */ 
void lcd_copyblock(int p0, int x0, int y0, int p1, int x1, int y1, int mode);

/*! 
 *@brief ページ間の部分コピー
 *
 *p0のページをp1のページに部分コピーする．
 *コピーするブロックサイズは幅w, 高さhで指定する．
 *(ブロック単位(8x16)で指定)
 *
 *x0とy0でコピー元のブロックのカーソル座標を指定する．
 *x1とy1でコピー先のブロックのカーソル座標を指定する．
 *
 *modeは
 * - 0:普通コピー
 * - 1:透過コピー(現在設定している背景色はコピーしない)
 *
 *@param srcp コピー元のページ番号
 *@param sx   コピー元のブロックのX座標
 *@param sy   コピー元のブロックのy座標
 *@param dstp コピー先のページ番号
 *@param dx   コピー先のブロックのX座標
 *@param dy   コピー先のブロックのy座標
 *@param w    コピーサイズ幅    
 *@param h    コピーサイズ高さ
 *@param mode コピーモード
 */ 
void lcd_copyregion(int srcp, int sx, int sy, int dstp, int dx, int dy, int w, int h, int mode);

/*! 
 *@brief 画面の縦方向スクロール
 *
 *現在のページを上下へスクロールする．
 *スクロール量の単位は16dot．
 *引数が正の場合は上へ、負の場合は下へスクロールする．
 *clearはスクロールした部分を背景色で初期化するかどうか
 * - 0:初期化しない
 * - 1:初期化する
 *
 *@param sc スクロール量
 *@param clear 画面をクリアするかどうか
 */ 
void lcd_vscroll(int sc, int clear);

/*! 
 *@brief 画面の横方向スクロール
 *
 *現在のページを左右へスクロールする．
 *スクロール量の単位は8dot．
 *引数が正の場合は左へ、負の場合は右へスクロールする．
 *clearはスクロールした部分を背景色で初期化するかどうか
 * - 0:初期化しない
 * - 1:初期化する
 *
 *@param sc スクロール量
 *@param clear 画面をクリアするかどうか
 */ 
void lcd_hscroll(int sc, int clear);
/**********************************************************************/
