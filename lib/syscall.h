/******************************************************************************/
/* MieruPC Project: System Call Library v0.3                                  */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-06-14 */
/******************************************************************************/
#pragma once
#include <mierulib.h>

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
 *@brief システムコール用ライブラリ
 *
 *システムコールを使うために必要なライブラリ．
 *このライブラリはOS用のアプリケーションの時のみ使用可能である．
 */



/*! 
 *@brief システムコール呼び出しインタフェース
 *
 *システムコールを呼び出すためのインタフェース.
 *第一引数にシステムコール番号を指定する.
 *残りの引数にシステムコールの引数を格納する.
 *使わない引数には0を代入すること.
 *
 *@param syscall_nr システムコール番号
 *@param arg1 システムコールの第1引数
 *@param arg2 システムコールの第2引数
 *@param arg3 システムコールの第3引数
 *@param arg4 システムコールの第4引数
 *@return 呼び出したシステムコールの返り値
 */ 
#ifdef MIERU_OS
namespace system{
#endif
int _syscall(unsigned int syscall_nr,
             unsigned int arg1,
             unsigned int arg2,
             unsigned int arg3,
             unsigned int arg4);
#ifdef MIERU_OS
}
#endif

#ifdef MIERUPC_APPLICATION
#define SYSCALL_NULL      0  ///< sys_nullのシステムコール番号
#define SYSCALL_INIT      1  ///< sys_initのシステムコール番号
#define SYSCALL_FIN       2  ///< sys_finのシステムコール番号
#define SYSCALL_EXIT      3  ///< sys_exitのシステムコール番号
#define SYSCALL_READ      4  ///< sys_readのシステムコール番号
#define SYSCALL_WRITE     5  ///< sys_writeのシステムコール番号
#define SYSCALL_OPEN      6  ///< sys_openのシステムコール番号
#define SYSCALL_CLOSE     7  ///< sys_closeのシステムコール番号
#define SYSCALL_STAT      8  ///< sys_statのシステムコール番号
#define SYSCALL_CREAT     9  ///< sys_creatのシステムコール番号
#define SYSCALL_RENAME    10 ///< sys_renameのシステムコール番号
#define SYSCALL_REMOVE    11 ///< sys_removeのシステムコール番号
#define SYSCALL_EXECVE    12 ///< sys_execveのシステムコール番号
#define SYSCALL_CHDIR     13 ///< sys_chdirのシステムコール番号
#define SYSCALL_GETCWD    14 ///< sys_getcwdのシステムコール番号
#define SYSCALL_LSEEK     15 ///< sys_lseekのシステムコール番号
#define SYSCALL_BRK       16 ///< sys_brkのシステムコール番号
#define SYSCALL_MKDIR     17 ///< sys_mkdirのシステムコール番号
#define SYSCALL_ABORT   4001 ///< sys_mkdirのシステムコール番号


#define SEEK_SET	0 ///< sys_lseekで使用:ファイルの先頭からのオフセット
#define SEEK_CUR	1 ///< sys_lseekで使用:ファイルの現在位置からのオフセット
#define SEEK_END	2 ///< sys_lseekで使用:ファイルの最後からのオフセット

#define O_ACCMODE   0x007     ///< sys_openなどで使用:O_RDONLYなどの比較用
#define O_RDONLY    0x001     ///< sys_openなどで使用:読み込み専用で開く
#define O_WRONLY    0x002     ///< sys_openなどで使用:書き込み専用で開く
#define O_RDWR      0x004     ///< sys_openなどで使用:読み書きで開く
#define O_CREAT     0x008     ///< sys_openなどで使用:ファイルが無い場合は生成する
#define O_TRUNK     0x0c8     ///< sys_openなどで使用:ファイルが存在する場合はサイズを0にする
#define O_APPEND    0x010     ///< sys_openなどで使用:追加モードでオープン
#define O_DIRECTORY 0x020     ///< sys_openなどで使用:ディレクトリを開く
#define O_NONBLOCK  0x040     ///< sys_openなどで使用:ノンブロッキングモード 

struct task_report{
    display_t disp;
};

typedef struct {
    char name[16];
    uint inode;
    uint mode;
    time_t ctime;
    time_t utime;
    time_t atime;
    uint size;
}Stat;

typedef struct{
    char name[16];
    uint inode;
    uint mode;
    uint offset;
    uint len;
}FileEntry;


/*! 
 *@brief アプリケーションの状態を取得する．
 *
 *OSから現在のアプリケーションの状態を取得する
 *
 *@param repo 状態を格納する構造体
 *@return 常に0
 */ 
int sys_init(struct task_report *repo);


/*! 
 *@brief アプリケーションの状態を報告する．
 *
 *OSに現在のアプリケーションの状態を報告する
 *
 *@param repo 状態を格納する構造体
 *@return 常に0
 */ 
int sys_fin(const struct task_report *repo);

/*! 
 *@brief シミュレーションを強制終了する.
 *
 *@param status 未使用
 */ 
int sys_abort(int status);

/*! 
 *@brief アプリケーションを終了する．
 *
 *アプリケーションを終了する．
 *statusは現在利用されていない．
 *
 *@param status 未使用
 *@return 常に0
 */ 
int sys_exit(int status);

/*! 
 *@brief ファイルを生成する．
 *
 *引数argで指定したファイルを生成する．
 *成功時には0を，失敗時には負数を返す．
 *引数flagsは現在利用されていない．
 *
 *@param path ファイルパス
 *@param mode 未使用
 *@return 成功時は0，失敗時は負数
 */ 
int sys_creat(const char *path, int mode);

/*! 
 *@brief ファイルを開く．
 *
 *引数argで指定したファイルを開くする．
 *成功時には非負数のファイルディスクリプタを返す．
 *失敗時には負数を返す．
 *引数flagsは現在利用されていない．
 *常にリードライトで開かれる．
 *
 *@param path ファイルパス
 *@param flags 未使用
 *@return ファイルディスクリプタ
 */ 
int sys_open(const char *path, int flags);

/*! 
 *@brief ファイルから読み込む．
 *
 *ファイルディスクリプタfdで指定したファイルからポインタstrにcountバイト読み込む．
 *正常に読み込んだバイト数を返す．
 *エラー時には負数を返す．
 *
 *@param fd ファイルディスクリプタ
 *@param str 読み込先アドレス
 *@param count 読み込むバイト数
 *@return 読み込んだバイト数
 */ 
int sys_read(int fd, char *str, int count);

/*! 
 *@brief ファイルに書き込む．
 *
 *ファイルディスクリプタfdで指定したファイルにポインタstrの先頭countバイト書き込む．
 *正常に書き込んだバイト数を返す．
 *エラー時には負数を返す．
 *
 *@param fd ファイルディスクリプタ
 *@param str 書き込み元アドレス
 *@param count 書き込むバイト数
 *@return 書き込んだバイト数
 */ 
int sys_write(int fd, const char *str, int count);

/*! 
 *@brief ファイルを閉じる．
 *
 *ファイルディスクリプタfdで指定したファイルを閉じる．
 *成功時には0，エラー時には負数を返す．
 *
 *@param fd ファイルディスクリプタ
 *@return 成功時には0，エラー時には負数を返す
 */ 
int sys_close(int fd);

/*! 
 *@brief ファイルの状態を取得する．
 *
 *pathnameで指定したファイルの状態を取得する．
 *成功時には0，エラー時には負数を返す．
 *
 *@param pathname ファイルパス
 *@param st Stat構造体へのパス
 *@return 成功時には0，エラー時には負数を返す
 */ 
int sys_stat(const char *pathname, Stat *st);

/*! 
 *@brief ヒープ領域のサイズを変更する.
 *
 *ヒープ領域の位置をbrkで指定した値に設定する.
 *値が更新された場合は設定した値を返す.
 *更新されない場合は以前の値を返す.
 *
 *@param brk 設定するヒープ位置
 *@return 現在のヒープ位置
 */ 
int sys_brk(unsigned int brk);

/*! 
 *@brief カレントディレクトリを変更する．
 *
 *引数strで指定したパスにカレントディレクトリを変更する．
 *
 *成功時には0，エラー時には負数を返す．
 *
 *@param str ディレクトリパス
 *@return 成功時には0，エラー時には負数を返す
 */ 
int sys_chdir(const char *str);

/*! 
 *@brief ファイル名をを変更する．
 *
 *oldpathで指定したファイルをnewpathで指定したファイル名に変更する．
 *
 *成功時には0，エラー時には負数を返す．
 *
 *@param oldpath ファイルパス
 *@param newpath ファイルパス
 *@return 成功時には0，エラー時には負数を返す
 */ 
int sys_rename(const char *oldpath, const char *newpath);

/*! 
 *@brief ファイルを削除する．
 *
 *pathnameで指定したファイルを削除する．
 *
 *成功時には0，エラー時には負数を返す．
 *
 *@param pathname ファイルパス
 *@return 成功時には0，エラー時には負数を返す
 */ 
int sys_remove(const char *pathname);

/*! 
 *@brief ファイルの読み書きオフセットの位置を変更する．
 *
 *pathnameで指定したファイルを削除する．
 *ファイルディスクリプタfdに対応するオープンされたファイルのオフセットを、
 *whenceに基づきoffsetの位置へ変更する．
 *whenceは以下の3通りである．
 *-SEEK_SET
 * ファイルの先頭からoffsetバイトに設定
 *-SEEK_CUR
 * 現在のオフセットからoffsetバイトを足した位置に設定
 *-SEEK_END
 * ファイルのサイズにoffsetバイトを足した位置に設定
 *
 *成功時には現在位置を，エラー時には負数を返す．
 *
 *@param fd ファイルディスクリプタ
 *@param offset 相対位置を指定
 *@param whence オフセットの種類を指定
 *@return 成功時には現在位置，エラー時には負数を返す
 */ 
int sys_lseek(int fd, int offset, int whence);

/*! 
 *@brief 現在のディレクトリの絶対パスを取得する.
 *
 *現在のディレクトリの絶対パスを取得する.
 *bufには十分なサイズのメモリを予め確保しておく必要がある.
 *取得するパスの長さがsize以上の場合はエラーを返す.
 *成功時にはパスの長さを示す正の値を返す.
 *エラー時には負数を返す.
 *
 *@param buf パスを格納するポインタ
 *@param size 格納できる最大文字数
 *@return 成功時には正，エラー時には負数を返す
 */ 
int sys_getcwd(char *buf, unsigned int size);

/*! 
 *@brief ファイルを生成する．
 *
 *引数argで指定したファイルを生成する．
 *成功時には0を，失敗時には負数を返す．
 *引数flagsは現在利用されていない．
 *
 *@param path ファイルパス
 *@param mode 未使用
 *@return 成功時は0，失敗時は負数
 */ 
int sys_mkdir(const char *path, int mode);

#endif

