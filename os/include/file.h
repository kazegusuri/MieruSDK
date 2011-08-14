/******************************************************************************/
/* MieruOS: File v0.1                                                         */
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

#pragma once

#define MAX_FILE 32

#define SEEK_SET	0	/* seek relative to beginning of file */
#define SEEK_CUR	1	/* seek relative to current file position */
#define SEEK_END	2	/* seek relative to end of file */

#define O_ACCMODE   0x007     ///< sys_openなどで使用:O_RDONLYなどの比較用
#define O_RDONLY    0x001     ///< sys_openなどで使用:読み込み専用で開く
#define O_WRONLY    0x002     ///< sys_openなどで使用:書き込み専用で開く
#define O_RDWR      0x004     ///< sys_openなどで使用:読み書きで開く
#define O_CREAT     0x008     ///< sys_openなどで使用:ファイルが無い場合は生成する
#define O_TRUNK     0x0c8     ///< sys_openなどで使用:ファイルが存在する場合はサイズを0にする
#define O_APPEND    0x010     ///< sys_openなどで使用:追加モードでオープン
#define O_DIRECTORY 0x020     ///< sys_openなどで使用:ディレクトリを開く
#define O_NONBLOCK  0x040     ///< sys_openなどで使用:ノンブロッキングモード 


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

class File{
public:
    int exist;
    int isdir;

    int num;

    uint pos;
    uint size;
    uint mode;
    uint user;

    virtual ~File(){}
    void operator delete(void *address){}
    virtual File *lookup(const char *name)         = 0;
    virtual int open(int flags)                    = 0;
    virtual int create(int mode, int isdir)        = 0;
    virtual int close()                            = 0;
    virtual int stat(Stat *st)                     = 0;
    virtual int seek(int offset, uint whence)      = 0;
    virtual int read(void *buf, uint count)        = 0;
    virtual int write(const void *buf, uint count) = 0;
    virtual int remove()                           = 0;
    virtual int rename(File *dest)                 = 0;
};

