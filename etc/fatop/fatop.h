/******************************************************************************/
/* MieruSDK: Fatop v0.3                                                       */
/* written by Masahiro Sano Arch Lab. Tokyo Tech                   2010-06-14 */
/******************************************************************************/

/*
 * fatop: this program handles FAT image with some operations.
 * Copyright (C) 2010  Arch Lab. Tokyo Institute of Technology.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct {
    char name[13];
    int attr;
    int c_year;
    int c_month;
    int c_day;
    int c_hour;
    int c_minute;
    int c_second;
    int u_year;
    int u_month;
    int u_day;
    int u_hour;
    int u_minute;
    int u_second;
    unsigned int size;
} fat_file_t;

typedef struct {
    char name[8];
    char ext[3];
    unsigned char  attr;
    char dummy;
    unsigned char  ctimecs; // 1/100sec
    unsigned short ctime;
    unsigned short cdate;
    unsigned short adate;
    unsigned short cluster2;
    unsigned short utime;
    unsigned short udate;
    unsigned short cluster;
    unsigned int   size;
} __attribute__((packed)) direntry_t;

typedef enum {
    FAT12,
    FAT16,
    OTHERTYPE,
} fattype_t;

// Filesystem Information for calculating offset
typedef struct {
    int off;
    int fbegin;
    int rbegin;
    int cbegin;
    int ssize;
    int fsize;
    int rsize;
    int csize;
    int cnum;
    fattype_t fattype;
    int last;
} fatinfo_t;

enum {
    FAT_FILE_ATTR_ARC    = 0x20,
    FAT_FILE_ATTR_DIR    = 0x10,
    FAT_FILE_ATTR_VOLUME = 0x8,
    FAT_FILE_ATTR_HIDDEN = 0x4,
    FAT_FILE_ATTR_SYSTEM = 0x2,
    FAT_FILE_ATTR_RONLY  = 0x1,

    FAT_ERROR_NO_SUCH_FILE        = 1,
    FAT_ERROR_INVALID_FILE_NAME   = 2,
    FAT_ERROR_NOT_DIRECTORY       = 3,
    FAT_ERROR_IS_DIRECTORY        = 4,
    FAT_ERROR_DIRECTORY_TOO_LONG  = 5,
    FAT_ERROR_TOO_MANY_FILES      = 6,
    FAT_ERROR_FILE_NOT_OPEN       = 7,
    FAT_ERROR_FILE_ALREADY_OPEN   = 8,
    FAT_ERROR_DISC_IS_FULL        = 9,
    FAT_ERROR_INVALID_OFFSET      = 10,
    FAT_ERROR_DIRECTORY_NOT_EMPTY = 11,
    FAT_ERROR_MBR_FAILURE         = 100,
    FAT_ERROR_BPB_FAILURE         = 101,
    FAT_ERROR_SECTOR_SIZE_INVALID = 102,
};

int main(int argc, char *argv[]);
int fat_in(int argc, char *argv[]);
int fat_out(int argc, char *argv[]);
int fat_cat(int argc, char*argv[]);
int fat_info(int argc, char*argv[]);
void usage(void);
int writefile(char *path, char *file);
uint getfatentry(FILE *fp, uint index, fatinfo_t *info);
void readfat(FILE *fp, uint depth, uint soff, fatinfo_t *info);


int fat_init();
int fat_fini();
int fat_chdir(char *);
int fat_fileinfobyname(char *, fat_file_t *);
int fat_fileinfobyindex(int, fat_file_t *);
int fat_open(char *);
int fat_close(int, int);
int fat_create(char *, int);
int fat_rename(char *, char *);
int fat_kill(char *);
int fat_getsector(char *, int, int);
int fat_setsector(char *, int, int);

#ifdef _DEBUG_
// these functions set to be private
void fat_writesector(char *, int);
void fat_readdirentry(direntry_t *, int);
void fat_writedirentry(direntry_t *, int);
int fat_getnextentry(int);
int fat_setnextentry(int, int);
int fat_getemptyentry(int);
int fat_createcluster(int);
int fat_clear83(char *, char *);
int fat_to83(const char *, char *, char *);
int fat_from83(char *, const char *, const char *);
int fat_findfileinsectorbyname(const char *, const char *, int);
int fat_findfilebyname(char *);
int fat_findfileinsectorbyindex(int *, int);
int fat_findfilebyindex(int);
int fat_convertfileinfo(direntry_t *, fat_file_t *);
// these functions are for debug(uncomment when debug)
void fat_printinfo();
void fat_pwd();
#endif
