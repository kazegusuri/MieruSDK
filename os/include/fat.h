/******************************************************************************/
/* MieruOS: Fat v0.1                                                          */
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

#include <file.h>


enum {
    FAT_FILE_ATTR_ARC    = 0x20,
    FAT_FILE_ATTR_DIR    = 0x10,
    FAT_FILE_ATTR_VOLUME = 0x8,
    FAT_FILE_ATTR_HIDDEN = 0x4,
    FAT_FILE_ATTR_SYSTEM = 0x2,
    FAT_FILE_ATTR_RONLY  = 0x1,

    FAT_ERROR_NO_SUCH_FILE        = 1,
    FAT_ERROR_FILE_ALREADY_EXIST  = 2,
    FAT_ERROR_INVALID_FILE_NAME   = 3,
    FAT_ERROR_NOT_DIRECTORY       = 4,
    FAT_ERROR_IS_DIRECTORY        = 5,
    FAT_ERROR_DIRECTORY_TOO_LONG  = 6,
    FAT_ERROR_TOO_MANY_FILES      = 7,
    FAT_ERROR_FILE_NOT_OPEN       = 8,
    FAT_ERROR_FILE_ALREADY_OPEN   = 9,
    FAT_ERROR_DISC_IS_FULL        = 10,
    FAT_ERROR_INVALID_OFFSET      = 11,
    FAT_ERROR_DIRECTORY_NOT_EMPTY = 12,
    FAT_ERROR_INVALID_POINTER     = 13,
    FAT_ERROR_INVALID_FLAG        = 14,
    FAT_ERROR_KERNEL_BUG          = 15,
    FAT_ERROR_MBR_INVALID         = 100,
    FAT_ERROR_FAT_TYPE_INVALID    = 101,
    FAT_ERROR_BPB_INVALID         = 102,
    FAT_ERROR_SECTOR_SIZE_INVALID = 103,
};

typedef struct {
    char   name[8];   ///! ファイル名
    char   ext[3];    ///! 拡張子
    uchar  attr;      ///! 属性
    uchar  dummy;     ///! 未使用
    uchar  ctimecs;   ///! 生成時間(10msec) (0-199)
    ushort ctime;     ///! 生成時間
    ushort cdate;     ///! 生成時間
    ushort adate;     ///! アクセス時間
    ushort cluster2;  ///! クラスタ番号(上位16bit)
    ushort utime;     ///! 更新時間
    ushort udate;     ///! 更新時間
    ushort cluster;   ///! クラスタ番号(下位16bit)
    uint   size;      ///! ファイルサイズ
} FatEntry;

// master boot record
typedef struct {

    uchar  bootable;     ///!
    uchar  dummy1[3];    ///! 未使用
    uchar  filesystem;   ///! 
    uchar  dummy2[3];    ///! 未使用

    uint   firstsector;  ///! 
    uint   sectornum;    ///! 
} FatPartition;

// BIOS Parameter Block
typedef struct {
    uchar  dummy1[11];  ///! 未使用
    ushort __attribute__((packed)) sectorsize;  ///! 1セクタ当たりのバイト数
    uchar  clustersize; ///! 1クラスタ当たりのセクタ数
    ushort __attribute__((packed)) fatbegin;    ///! FATの開始クラスタ
    uchar  fatnum;      ///! FATの数
    ushort __attribute__((packed)) rootentry;   ///! ルートディレクトリのエントリ数
    ushort __attribute__((packed)) sectornum;   ///! 総セクタ数 (< 65536)
    uchar  media;       ///! 種類
    ushort __attribute__((packed)) fatsize;     ///! 1つのFAT当たりのセクタ数
    uint   dummy2;      ///! 未使用
    uint   hidden;      ///! The number of hidden sector
    uint   sectornum2;  ///! 総セクタ数 (>= 65536)
} FatBpb;


class FatFileName{
public:
    char name[8];
    char ext[3];

    void clear();
    int get(char *string);
    int set(const char *string);
};


class FatCluster{
    bool isroot;
    int cl;
    int cur_cl, cur_pos;
    int sectors;
public:
    FatCluster();
    bool isRoot();
    int getTop();
    int getSize();
    int getSector();
    void set(bool isdir, int n);
    uchar *next();
    uchar *seek(int nth);
    int add(int n);
    void remove();
    void clear();
};


class Fat{
    void changemmcblock(uint addr);
public:
    enum{
        SECTOR_SIZE = 512,
        ENTRY_SIZE  = 32,
        LAST_CLUSTER_FAT12 = 0xff8,
        LAST_CLUSTER_FAT16 = 0xfff8,

        UNDEFINED_CLUSTER = 0,
        ROOT_CLUSTER = 0,
    };

    typedef enum {
        FAT12,
        FAT16,
        OTHERTYPE,
    } fattype_t;

    int fatbegin;
    int rootbegin;
    int clusterbegin;
    int fatsize;
    int rootsize;
    int clustersize;
    int clusternum;
    int last;
    fattype_t type;
    
    void readSector(int sec);
    void writeSector(int sec);
    void readEntry(FatEntry *dest, int off);
    void writeEntry(FatEntry *src, int off);
    uint referTable(int index);
    int updateTable(int index, int value);
    int getEmptyCluster();
    int allocNewCluster(int prev);
    int deleteCluster(int cl);

    int init(void);
};

class FatFile : public File{

    int updateEntry();
    FatFile *getParent(int cl);
    FatFile *getCopy();
    int readFile(void *buf, uint count);
    int readDir(void *buf, uint count);
    int writeFile(const void *buf, uint count);
    void set(int cl);
    void set(FatEntry ent, int exi, int par, int sec, int nth);
    int searchEntry(FatFileName fatname, FatEntry *entry);
    int searchEntry(int cl, FatEntry *entry);
    int searchEntry();

public:
    int use;
    FatCluster cluster;
    FatEntry entry;
    int entry_sec;
    int entry_off;
    int parent;

    static void *operator new(uint size);
    static void operator delete(void *address);

    FatFile();
    FatFile(FatFile &fatfile);
    FatFile(int cl);
    FatFile(FatEntry ent, int exi, int par, int sec, int nth);
    ~FatFile();

    virtual File *lookup(const char *name);
    int open(int mode);
    virtual int create(int mode, int isdir);
    virtual int close();
    virtual int stat(Stat *st);
    virtual int seek(int offset, uint whence);
    virtual int read(void *buf, uint count);
    virtual int write(const void *buf, uint count);
    virtual int remove();
    virtual int rename(File *dest);

    void printEntry();
};
