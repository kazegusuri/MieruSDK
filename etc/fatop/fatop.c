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

#include "fatop.h"
#include <dirent.h>

#define SECTOR_SIZE 512
#define DIRENTRY_SIZE 32
#define MAX_FILE 4

typedef unsigned int uint;

// master boot record
typedef struct {
    unsigned char  bootable;
    char           dummy1[3];
    unsigned char  filesystem;
    char           dummy2[3];
    unsigned int   firstsector;
    unsigned int   sectornum;
} __attribute__((packed)) partition_t;

// BIOS Parameter Block
typedef struct {
    char           dummy1[11];
    unsigned short sectorsize;  // byte / sector
    unsigned char  clustersize; // sector / cluster
    unsigned short fatbegin;
    unsigned char  fatnum;
    unsigned short rootentry;
    unsigned short sectornum;   // # of sector (< 65536)
    unsigned char  media;
    unsigned short fatsize;     // sector / fat
    unsigned int   dummy2;
    unsigned int   hidden;      // # of hidden sector
    unsigned int   sectornum2;  // # of sector (>= 65536)
} __attribute__((packed)) bpb_t;

// working directory and opening files
//FILE *fp = NULL;
char *mmc;
char dir[256];
int dircluster, direntry;
fatinfo_t info;
int filecluster[MAX_FILE], fileentry[MAX_FILE];

/**********************************************************************/
/* Private functions                                                  */
/**********************************************************************/
// Read a sector from filesystem.
void fat_readsector(char *dest, int sec)
{
    if (!dest) return;
    memcpy(dest, &mmc[sec * SECTOR_SIZE], SECTOR_SIZE);
}

/**********************************************************************/
// Write a sector to filesystem.
void fat_writesector(char *src, int sec)
{
    int i;
    char nullchar = '\0';

    if (src) {
        memcpy(&mmc[sec * SECTOR_SIZE], src, SECTOR_SIZE);
    } else {
        for (i = 0; i < SECTOR_SIZE; i++)
            memcpy(&mmc[sec * SECTOR_SIZE + i], &nullchar, sizeof(char));
    }   
}

/**********************************************************************/
// Read a directory entry from filesystem.
void fat_readdirentry(direntry_t *dest, int off)
{
    if (!dest) return;
    memcpy(dest, &mmc[off], DIRENTRY_SIZE);
}

/**********************************************************************/
// Write a directory entry to filesystem.
void fat_writedirentry(direntry_t *src, int off)
{
    int i;
    char nullchar = '\0';

    if (src) {
        memcpy(&mmc[off], src, DIRENTRY_SIZE);
    } else {
        for (i = 0; i < DIRENTRY_SIZE; i++)
            memcpy(&mmc[off], &nullchar, sizeof(char));
    }   
}

/**********************************************************************/
// Return 'index'th FAT entry.
int fat_getnextentry(int index)
{
    unsigned char temp[SECTOR_SIZE], lastbyte;
    int sec, mod;
    if (index <= 0 || index >= info.cnum)
        return -1;
    if (info.fattype == FAT12) {
        // (0xab,0xcd,0xef) are translated to (0xdab,0xefc)
        sec = info.fbegin + index * 3 / 2 / SECTOR_SIZE;
        mod = (index * 3 / 2) % SECTOR_SIZE;
        fat_readsector((char *) temp, sec);
        if (mod == SECTOR_SIZE - 1) {
            lastbyte = temp[SECTOR_SIZE - 1];
            fat_readsector((char *) temp, sec + 1);
            if (index % 2 == 0)
                return ((temp[0] << 8) | lastbyte) & 0xfff;
            else
                return ((temp[0] << 4) | (lastbyte >> 4)) & 0xfff;
        } else {
            if (index % 2 == 0)
                return ((temp[mod + 1] << 8) | temp[mod]) & 0xfff;
            else
                return ((temp[mod + 1] << 4) | (temp[mod] >> 4)) & 0xfff;
        }
    } else {
        sec = info.fbegin + index * 2 / SECTOR_SIZE;
        mod = (index * 2) % SECTOR_SIZE;
        fat_readsector((char *) temp, sec);
        return ((temp[mod + 1] << 8) | temp[mod]) & 0xffff;
    }
    return -1;
}

/**********************************************************************/
// Set 'index'th FAT entry.
int fat_setnextentry(int index, int value)
{
    char temp[SECTOR_SIZE];
    int i, sec, mod;
    if (index <= 0 || index >= info.cnum)
        return -1;
    for (i = 0; i < 2; i++) {
        if (info.fattype == FAT12) {
            // (0xab,0xcd,0xef) are translated to (0xdab,0xefc)
            sec = info.fbegin + info.fsize * i + index * 3 / 2 / SECTOR_SIZE;
            mod = (index * 3 / 2) % SECTOR_SIZE;
            fat_readsector(temp, sec);
            if (mod == SECTOR_SIZE - 1) {
                if (index % 2 == 0)
                    temp[mod] = value & 0xff;
                else
                    temp[mod] = (temp[mod] & 0xf) | ((value << 4) & 0xf0); 
                fat_writesector(temp, sec);

                fat_readsector(temp, sec + 1);
                if (index % 2 == 0)
                    temp[0] = (temp[0] & 0xf0) | ((value >> 8) & 0xf);
                else
                    temp[0] = (value >> 4) & 0xff;
                fat_writesector(temp, sec + 1);
            } else {
                if (index % 2 == 0) {
                    temp[mod] = value & 0xff;
                    temp[mod + 1] = ((temp[mod + 1] & 0xf0) |
                                     ((value >> 8) & 0xf));
                } else {
                    temp[mod] = (temp[mod] & 0xf) | ((value << 4) & 0xf0);
                    temp[mod + 1] = (value >> 4) & 0xff;
                }
                fat_writesector(temp, sec);
            }   
        } else {
            sec = info.fbegin + info.fsize * i + index * 2 / SECTOR_SIZE;
            mod = (index * 2) % SECTOR_SIZE;
            fat_readsector(temp, sec);
            temp[mod] = value & 0xff;
            temp[mod + 1] = (value >> 8) & 0xff;
            fat_writesector(temp, sec);
        }
    }
    return 0;
}

/**********************************************************************/
// Get the index of an empty entry (count_mode = 0)
// Get the number of empty entries (count_mode = 1)
int fat_getemptyentry(int count_mode)
{
    char temp[SECTOR_SIZE], lastbyte, check;
    int i, sec, mod, cnt;
    lastbyte = 0;
    sec = info.fbegin;
    mod = cnt = 0;
    fat_readsector(temp, sec);

    for (i = 0; i < info.cnum; i++) {
        if (info.fattype == FAT12) {
            if (mod == -1) {
                if (i % 2 == 0)
                    check = lastbyte | (temp[0] & 0xf);
                else
                    check = (lastbyte & 0xf0) | temp[0];
            } else {
                if (i % 2 == 0)
                    check = temp[mod] | (temp[mod + 1] & 0xf);
                else
                    check = (temp[mod] & 0xf0) | temp[mod + 1];
            }
        } else {
            check = temp[mod] | temp[mod + 1];
        }
        if (check == 0) {
            if (count_mode)
                cnt++;
            else
                return i;
        }

        if (info.fattype == FAT12 && i % 2 == 0)
            mod++;
        else 
            mod += 2;
        if (mod >= SECTOR_SIZE - 1) {
            mod -= SECTOR_SIZE;
            sec++;
            lastbyte = temp[SECTOR_SIZE - 1];
            fat_readsector(temp, sec);
        }
    }
    if (count_mode)
        return cnt;
    else 
        return -1;
}

/**********************************************************************/
// Create an empty cluster.
// if 'attach_to' is valid, the new cluster is attached to that cluster.
int fat_createcluster(int attach_to)
{
    int i, cl, off;
    cl = fat_getemptyentry(0);
    if (cl == -1)
        return -1;
    fat_setnextentry(cl, 0xffff);
    if (attach_to >= 2 && attach_to < info.cnum)
        fat_setnextentry(attach_to, cl);

    off = (info.cbegin + (cl - 2) * info.csize);
    for (i = 0; i < info.csize; i++)
        fat_writesector(NULL, off + i);

    return cl;
}

/**********************************************************************/
// fill 'name' and 'ext' width space.
int fat_clear83(char *name, char *ext)
{
    int i;
    for (i = 0; i < 8; i++)
        name[i] = ' ';
    for (i = 0; i < 3; i++)
        ext[i] = ' ';
    return 0;
}

/**********************************************************************/
// Convert to 8.3 file name. if 'arg' is invalid, returns 1.
int fat_to83(const char *arg, char *name, char *ext)
{
    char forbid_char[15] = ".\"/\\[]:;|=, *?";
    char temp[8];
    int i, j, k;
    if (name == NULL)
        name = temp;
    if (ext == NULL)
        ext = temp;
    fat_clear83(name, ext);

    // name
    for (i = 0; i < 8; i++) {
        if (arg[i] == '\0' || arg[i] == '.')
            break;
        for (k = 0; k < 14; k++)
            if (arg[i] == forbid_char[k])
                return 1;
        if (arg[i] >= 'a' && arg[i] <= 'z')
            name[i] = arg[i] - 0x20;
        else 
            name[i] = arg[i];
    }
    if (i == 0)
        return 1;
    if (arg[i] == '\0')
        return 0;
    if (arg[i] != '.')
        return 1;

    // extension
    for (i++, j = 0; j < 3; i++, j++) {
        if (arg[i] == '\0')
            return 0;
        for (k = 0; k < 14; k++)
            if (arg[i] == forbid_char[k])
                return 1;
        if (arg[i] >= 'a' && arg[i] <= 'z')
            ext[j] = arg[i] - 0x20;
        else 
            ext[j] = arg[i];
    }
    if (arg[i] == '\0')
        return 0;
    return 1;
}

/**********************************************************************/
// Convert from 8.3 file name.
int fat_from83(char *ret, const char *name, const char *ext)
{
    int i, j;

    // name
    for (i = 0; i < 8; i++) {
        if (name[i] == '\0' || name[i] == ' ')
            break;
        ret[i] = name[i];
    }

    // extension
    if (ext[0] != '\0' && ext[0] != ' ') {
        ret[i] = '.';
        for (i++, j = 0; j < 3; i++, j++) {
            if (ext[j] == '\0' || ext[j] == ' ')
                break;
            ret[i] = ext[j];
        }
    }
    ret[i] = '\0';
    return 0;
}

/**********************************************************************/
// Find a file in the sector. This is called by fat_findfilebyname.
int fat_findfileinsectorbyname(const char *name, const char *ext, int off)
{
    int i, j;
    direntry_t entry;
    for (i = 0; i < SECTOR_SIZE / DIRENTRY_SIZE; i++) {
        fat_readdirentry(&entry, off * SECTOR_SIZE + i * DIRENTRY_SIZE);
        if (name[0]) {
            if (entry.name[0] == 0x00 || entry.name[0] == 0x05 ||
                entry.name[0] == (char) 0xe5 ||
                entry.attr & FAT_FILE_ATTR_VOLUME)
                continue;
            // if (strncmp(entry.name, name, 8) != 0 ||
            //     strncmp(entry.ext , ext , 3) != 0) continue;
            for (j = 0; j < 8; j++)
                if (entry.name[j] != name[j])
                    break;
            if (j < 8)
                continue;
            for (j = 0; j < 3; j++)
                if (entry.ext[j] != ext[j])
                    break;
            if (j < 3)
                continue;
            return i;
        } else {
            if (entry.name[0] == 0x00 || entry.name[0] == (char) 0xe5)
                return i;
        }
    }
    return -1;
}

/**********************************************************************/
// Return the offset of the directory entry specified by 'filename'.
int fat_findfilebyname(char *filename)
{
    int i, ret, cl, off;
    char name[8], ext[3];
    if (filename) {
        if (filename[0] == '.') {
            // beginning with '.': "." and ".." are only acceptable
            fat_clear83(name, ext);
            name[0] = '.';
            if (filename[1] != '\0') {
                if (filename[1] != '.' || filename[2] != '\0')
                    return -1;
                name[1] = '.';
            }
        } else if (fat_to83(filename, name, ext)) {
            return -1;
        }
    } else {
        name[0] = '\0';
    }
    if (direntry) { // working directory is not root?
        cl = dircluster;
        while (cl > 0 && cl < info.last) {
            for (i = 0; i < info.csize; i++) {
                off = info.cbegin + (cl - 2) * info.csize + i;
                if ((ret = fat_findfileinsectorbyname(name, ext, off)) != -1)
                    return off * SECTOR_SIZE + ret * DIRENTRY_SIZE;
            }
            cl = fat_getnextentry(cl);
        }
    } else {
        for (i = 0; i < info.rsize; i++) {
            off = info.rbegin + i;
            if ((ret = fat_findfileinsectorbyname(name, ext, off)) != -1)
                return off * SECTOR_SIZE + ret * DIRENTRY_SIZE;
        }
    }
    return -1;
}

/**********************************************************************/
// Find a file in the sector. This is called by fat_findfilebyindex.
int fat_findfileinsectorbyindex(int *index, int off)
{
    int i;
    direntry_t entry;
    for (i = 0; i < SECTOR_SIZE / DIRENTRY_SIZE; i++) {
        fat_readdirentry(&entry, off * SECTOR_SIZE + i * DIRENTRY_SIZE);
        if (entry.name[0] == 0x00 || entry.name[0] == 0x05 ||
            entry.name[0] == (char) 0xe5 ||
            entry.attr & FAT_FILE_ATTR_VOLUME)
            continue;
        if (*index == 0)
            return i;
        else
            (*index)--;
    }
    return -1;
}

/**********************************************************************/
// Return the offset of the directory entry specified by index.
int fat_findfilebyindex(int index)
{
    int i, ret, cl, off;
    if (index < 0)
        return -1;

    if (direntry) { // working directory is not root?
        cl = dircluster;
        while (cl > 0 && cl < info.last) {
            for (i = 0; i < info.csize; i++) {
                off = info.cbegin + (cl - 2) * info.csize + i;
                if ((ret = fat_findfileinsectorbyindex(&index, off)) != -1)
                    return off * SECTOR_SIZE + ret * DIRENTRY_SIZE;
            }
            cl = fat_getnextentry(cl);
        }
    } else {
        for (i = 0; i < info.rsize; i++) {
            off = info.rbegin + i;
            if ((ret = fat_findfileinsectorbyindex(&index, off)) != -1)
                return off * SECTOR_SIZE + ret * DIRENTRY_SIZE;
        }
    }
    return -1;
}

/**********************************************************************/
// Convert direntry_t to fat_file_t, useful format
int fat_convertfileinfo(direntry_t *entry, fat_file_t *ret)
{
    if (!ret)
        return 0;
    fat_from83(ret->name, entry->name, entry->ext);
    ret->attr = entry->attr;
    ret->c_year   = (entry->cdate >> 9) + 1980;
    ret->c_month  = (entry->cdate >> 5) & 0xf; 
    ret->c_day    = entry->cdate & 0x1f;       
    ret->c_hour   = entry->ctime >> 11;        
    ret->c_minute = (entry->ctime >> 5) & 0x3f;
    ret->c_second = (entry->ctime << 1) & 0x3e;
    ret->u_year   = (entry->udate >> 9) + 1980;
    ret->u_month  = (entry->udate >> 5) & 0xf; 
    ret->u_day    = entry->udate & 0x1f;       
    ret->u_hour   = entry->utime >> 11;        
    ret->u_minute = (entry->utime >> 5) & 0x3f;
    ret->u_second = (entry->utime << 1) & 0x3e;
    ret->size     = entry->size;
    return 0;
}

/**********************************************************************/
/* Public functions                                                   */
/**********************************************************************/
// Initialization. Access 'filename' as an image of FAT12/16. 
int fat_init() 
{
    partition_t par;
    bpb_t bpb;
    unsigned short sig;
    int i, off, snum;


    // check MBR
    memcpy(&sig, &mmc[0x1fe], sizeof(unsigned short));
    if (sig != 0xaa55)
        return FAT_ERROR_MBR_FAILURE;
    

    //fseek(fp, 0x1be, SEEK_SET);
    //fread(&par, sizeof(partition_t), 1, fp);
    memcpy(&par, &mmc[0x1be], sizeof(partition_t));
    off = par.firstsector;
    info.fattype = ((par.filesystem == 1) ? FAT12 : 
                    (par.filesystem == 4) ? FAT16 :
                    (par.filesystem == 6) ? FAT16 :
                    (par.filesystem == 14) ? FAT12 : OTHERTYPE);
    if (info.fattype == OTHERTYPE)
        return FAT_ERROR_MBR_FAILURE;
    

    // check BPB
    memcpy(&sig, &mmc[off * SECTOR_SIZE + 0x1fe], sizeof(unsigned short));
    if (sig != 0xaa55)
        return FAT_ERROR_BPB_FAILURE;

    memcpy(&bpb, &mmc[off * SECTOR_SIZE], sizeof(bpb_t));
    if (bpb.sectorsize != 512)
        return FAT_ERROR_SECTOR_SIZE_INVALID;
    

    // Write to Filesystem Information
    info.fsize = bpb.fatsize;
    info.rsize = (uint) bpb.rootentry * DIRENTRY_SIZE / bpb.sectorsize;
    info.csize = bpb.clustersize;
    info.fbegin = off + bpb.fatbegin;
    info.rbegin = info.fbegin + (uint) bpb.fatnum * bpb.fatsize;
    info.cbegin = info.rbegin + info.rsize;
    snum = (bpb.sectornum2 == 0) ?  bpb.sectornum : bpb.sectornum2;
    info.cnum = (snum - info.cbegin + off) / info.csize + 2;
    info.last = (info.fattype == FAT12) ? 0xff8 : 0xfff8;

    // initialize working directory and opening files
    fat_chdir("");
    for (i = 0; i < MAX_FILE; i++)
        filecluster[i] = fileentry[i] = 0;

    return 0;
}

/**********************************************************************/
// Finalization.
int fat_fini()
{
    int i;
    for (i = 0; i < MAX_FILE; i++)
        fat_close(i, -1);
    return 0;
}

/**********************************************************************/
// Change working directory.
int fat_chdir(char *targ)
{
    int i, j, off;
    direntry_t entry;

    if (!targ)
        return FAT_ERROR_INVALID_FILE_NAME;

    if (targ[0] == '.' && targ[1] == '\0') // "."
        return 0;

    if (!targ[0]) {
        //targ == "": change working directory to root
        dir[0] = '/';
        dir[1] = '\0';
        dircluster = 0;
        direntry = 0;
    } else {
        off = fat_findfilebyname(targ);
        if (off == -1)
            return FAT_ERROR_NO_SUCH_FILE;
        fat_readdirentry(&entry, off);
        if (!(entry.attr & FAT_FILE_ATTR_DIR))
            return FAT_ERROR_NOT_DIRECTORY;
        if (entry.name[0] == '.' && entry.name[1] == '.') { // ".."
            for (i = 0; i < 256; i++)
                if (dir[i] == '\0')
                    break;
            for (; i && dir[i] != '/'; i--);
            if (i == 0)
                i++;
        } else {
            for (i = 0; i < 247; i++)
                if (dir[i] == '\0')
                    break;
            if (i == 1)
                i--;
            if (i == 247)
                return FAT_ERROR_DIRECTORY_TOO_LONG;
            dir[i] = '/';
            for (i++, j = 0; j < 8; i++, j++) {
                if (targ[j] == '\0' || targ[j] == '.')
                    break;
                dir[i] = targ[j];
            }
        }
        dir[i] = '\0';
        dircluster = entry.cluster;
        direntry = (dircluster) ? off : 0;
    }
    return 0;
}

/**********************************************************************/
// Get the file information and write it to 'ret'.
int fat_fileinfobyname(char *filename, fat_file_t *ret)
{
    int off;
    direntry_t entry;
    if (!filename)
        return FAT_ERROR_INVALID_FILE_NAME;
    off = fat_findfilebyname(filename);
    if (off == -1)
        return FAT_ERROR_NO_SUCH_FILE;
    fat_readdirentry(&entry, off);
    fat_convertfileinfo(&entry, ret);
    return 0;
}

int fat_fileinfobyindex(int index, fat_file_t *ret)
{
    int off;
    direntry_t entry;
    off = fat_findfilebyindex(index);
    if (off == -1)
        return FAT_ERROR_NO_SUCH_FILE;
    fat_readdirentry(&entry, off);
    fat_convertfileinfo(&entry, ret);
    return 0;
}

/**********************************************************************/
// Open a file. Return the file descriptor.
int fat_open(char *filename)
{
    if (!filename)
        return -FAT_ERROR_INVALID_FILE_NAME;

    int i, index, off;
    direntry_t entry;
    for (index = 0; index < MAX_FILE; index++) {
        if (fileentry[index] == 0)
            break;
    }
    if (index == MAX_FILE)
        return -FAT_ERROR_TOO_MANY_FILES;
    off = fat_findfilebyname(filename);
    if (off == -1)
        return -FAT_ERROR_NO_SUCH_FILE;
    for (i = 0; i < MAX_FILE; i++)
        if (fileentry[i] == off)
            return -FAT_ERROR_FILE_ALREADY_OPEN;

    fat_readdirentry(&entry, off);
    if (entry.attr & FAT_FILE_ATTR_DIR)
        return -FAT_ERROR_IS_DIRECTORY;
    filecluster[index] = entry.cluster;
    fileentry[index] = off;
    return index;
}

/**********************************************************************/
// Close a file. if 'size' is not -1, expand or contract the cluster
// chain so that the file size will be equivalent to 'size'.
int fat_close(int index, int size)
{
    int i, cl, prevcl, maxcl;
    direntry_t entry;

    if (fileentry[index] == 0)
        return FAT_ERROR_FILE_NOT_OPEN;

    if (size != -1) {
        fat_readdirentry(&entry, fileentry[index]);
        maxcl = size + SECTOR_SIZE * info.csize - 1;
        maxcl /= SECTOR_SIZE * info.csize;
        cl = prevcl = filecluster[index];
        // count # of cluster
        for (i = 0; i < maxcl; i++) {
            if (cl <= 0 || cl >= info.last)
                break;
            prevcl = cl;
            cl = fat_getnextentry(cl);
        }
        // when # of cluster is less than specified
        if (i < maxcl) {
            cl = prevcl;
            if (fat_getemptyentry(1) <= maxcl - i)
                return FAT_ERROR_DISC_IS_FULL;
            for (; i < maxcl; i++) {
                cl = fat_createcluster(cl);
                if (i == 0)
                    filecluster[index] = cl;
            }
            cl = 0;
        }
        // when # of cluster is more than specified
        if (cl > 0 && cl < info.last) {
            if (i == 0)
                filecluster[index] = 0;
            else
                fat_setnextentry(prevcl, 0xffff);
            while (cl > 0 && cl < info.last) {
                prevcl = cl;
                cl = fat_getnextentry(cl);
                fat_setnextentry(prevcl, 0); 
            }
        }
        entry.size = size;
        entry.cluster = filecluster[index];
        fat_writedirentry(&entry, fileentry[index]);
    }
    fileentry[index] = filecluster[index] = 0;
    return 0;
}

/**********************************************************************/
// Create a file(isdir = 0) or a directory(isdir = 1)
int fat_create(char *filename, int isdir)
{
    int off, newoff, cl, prevcl;
    direntry_t entry;

    if (fat_to83(filename, NULL, NULL) != 0)
        return FAT_ERROR_INVALID_FILE_NAME;
    if (fat_findfilebyname(filename) != -1)
        return FAT_ERROR_NO_SUCH_FILE;

    off = fat_findfilebyname(NULL);
    if (off == -1) { // is there no empty entry in working directory?
        if (direntry == 0)
            return FAT_ERROR_DISC_IS_FULL; // root can't be extended
        cl = prevcl = dircluster;
        while (cl > 0 && cl < info.last) {
            prevcl = cl;
            cl = fat_getnextentry(cl);
        }
        if ((cl = fat_createcluster(prevcl)) == -1)
            return FAT_ERROR_DISC_IS_FULL;
        off = (info.cbegin + (cl - 2) * info.csize) * SECTOR_SIZE;
    }

    entry.attr = FAT_FILE_ATTR_ARC | ((isdir) ? FAT_FILE_ATTR_DIR : 0);
    entry.ctimecs = 0;
    entry.ctime = entry.utime = 0;                    //   00:00:00
    entry.cdate = entry.udate = entry.adate = 0x3a21; // 2009/01/01
    entry.size = 0;
    if (isdir) {
        // Create entries for "." and ".."
        if ((cl = fat_createcluster(0)) == -1)
            return FAT_ERROR_DISC_IS_FULL;
        entry.cluster = cl;
        entry.cluster2 = 0;
        fat_clear83(entry.name, entry.ext);

        entry.name[0] = '.'; // "."
        newoff = (info.cbegin + (cl - 2) * info.csize) * SECTOR_SIZE;
        fat_writedirentry(&entry, newoff);

        entry.cluster = dircluster;
        entry.name[1] = '.'; // ".."
        fat_writedirentry(&entry, newoff + DIRENTRY_SIZE);

        entry.cluster = cl;
    } else {
        entry.cluster = entry.cluster2 = 0;
    }
    fat_to83(filename, entry.name, entry.ext);
    fat_writedirentry(&entry, off);
    return 0;
}

/**********************************************************************/
// Rename a file.
int fat_rename(char *filename_from, char *filename_to)
{
    int off;
    direntry_t entry;

    if (fat_to83(filename_to, NULL, NULL) != 0)
        return FAT_ERROR_INVALID_FILE_NAME;
    if ((off = fat_findfilebyname(filename_from)) == -1)
        return FAT_ERROR_NO_SUCH_FILE;

    fat_readdirentry(&entry, off);
    fat_to83(filename_to, entry.name, entry.ext);
    fat_writedirentry(&entry, off);
    return 0;
}

/**********************************************************************/
// Remove a file.
int fat_kill(char *filename)
{
    int i, ret, cl, prevcl, off;
    direntry_t entry;

    if (!filename)
        return FAT_ERROR_INVALID_FILE_NAME;
    if ((off = fat_findfilebyname(filename)) == -1)
        return FAT_ERROR_NO_SUCH_FILE;
    fat_readdirentry(&entry, off);
    if (entry.attr & FAT_FILE_ATTR_DIR) {
        // directory has more than two("." and "..") entries?
        fat_chdir(filename);
        ret = fat_findfilebyindex(2);
        fat_chdir("..");
        if (ret != -1)
            return FAT_ERROR_DIRECTORY_NOT_EMPTY;
    }
    for (i = 0; i < MAX_FILE; i++)
        if (fileentry[i] == off)
            return -FAT_ERROR_FILE_ALREADY_OPEN;

    fat_writedirentry(NULL, off);
    cl = prevcl = entry.cluster;
    while (cl > 0 && cl < info.last) {
        prevcl = cl;
        cl = fat_getnextentry(cl);
        fat_setnextentry(prevcl, 0);
    }
    return 0;
}

/**********************************************************************/
// Read a sector from the file opened by fat_open().
int fat_getsector(char *dest, int index, int sector)
{
    int i, cl;
    if (fileentry[index] == 0)
        return FAT_ERROR_FILE_NOT_OPEN;
    if (sector < 0)
        return FAT_ERROR_INVALID_OFFSET;
    
    cl = filecluster[index];
    while (sector >= info.csize) {
        if (cl <= 0 || cl >= info.last)
            break;
        sector -= info.csize;
        cl = fat_getnextentry(cl);
    }
    if (cl <= 0 || cl >= info.last) {
        for (i = 0; i < SECTOR_SIZE; i++)
            dest[i] = 0;
    } else {
        sector += info.cbegin + (cl - 2) * info.csize;
        fat_readsector(dest, sector);
    }
    return 0;
}

/**********************************************************************/
// Write a sector to the file opened by fat_open().
int fat_setsector(char *src, int index, int sector)
{
    int cl, prevcl;
    direntry_t dent;

    if (fileentry[index] == 0)
        return FAT_ERROR_FILE_NOT_OPEN;
    if (sector < 0)
        return FAT_ERROR_INVALID_OFFSET;
    
    cl = prevcl = filecluster[index];
    while (sector >= info.csize) {
        if (cl <= 0 || cl >= info.last)
            break;
        sector -= info.csize;
        prevcl = cl;
        cl = fat_getnextentry(cl);
    }
    if (cl <= 0 || cl >= info.last) {
        if (fat_getemptyentry(1) * info.csize <= sector)
            return FAT_ERROR_DISC_IS_FULL;
        cl = fat_createcluster(prevcl);
        if (prevcl == 0){
            filecluster[index] = cl;
            fat_readdirentry(&dent, fileentry[index]);
            dent.cluster = cl;
            fat_writedirentry(&dent, fileentry[index]);
        }
        while (sector >= info.csize) {
            sector -= info.csize;
            cl = fat_createcluster(cl);
        }
    }
    sector += info.cbegin + (cl - 2) * info.csize;
    fat_writesector(src, sector);
    return 0;
}

#ifdef _DEBUG_
/**********************************************************************/
/* functions for Debug                                                */
/**********************************************************************/
void fat_printinfo()
{
    printf("[[File System Info]]\n");
    printf("File System: %s\n",
           (info.fattype == FAT12) ? "FAT12" :
           (info.fattype == FAT16) ? "FAT16" : "Unknown");
    printf("Total Logical Size: %d Bytes\n",
           (info.cnum - 2) * info.csize * SECTOR_SIZE);
    printf("Cluster Size: %d Bytes\n",
           info.csize * SECTOR_SIZE);
    printf("Start sector for FATs: %d\n", info.fbegin);
    printf("Root Directory Size: %d Sectors\n", info.rsize);
    printf("Size of FAT: %d Sectors\n", info.fsize);
    printf("2nd Cluster Start Sector: %d\n", info.cbegin);
    printf("Index of Last Cluster: %d\n", info.cnum - 1);
    printf("\n");
}

void fat_pwd()
{
    printf(dir);
}

/**********************************************************************/
#endif

void usage(void){
    char usagemessage[] = "\
  info [mmc_file]   : displate contents of the mmc file\n\
  cat  [mmc_file]   [file_name] : cat file in the mmc file\n\
  in   [mmc_master] [mmc_root]  [out_file] [prior_file] : make mmc image from directory\n\
  out  [mmc_file]   [dirctory]  : extract mmc file to target directory\n\
\n";

    printf("Usage: fatop \n");
    printf("%s", usagemessage);
}

int writefile(char *path, char *file){
    FILE *fp;
    int size, ret, fd, i;
    char name[12];
    char sector[SECTOR_SIZE];
    direntry_t dent;
    
    // open and calculate file size
    fp = fopen(path, "r");
    if(fp == NULL){
        printf("##Warning : %s was not found\n",  path);
        return 1;
    }
    fseek(fp, 0, SEEK_END); 
    size = ftell(fp); 
    fseek(fp, 0, SEEK_SET);

    
    // fat create and open
    ret = fat_create(file, 0);
    if(ret > 0){
        printf("##Warning : create error (%s). skip.\n", file);
        fclose(fp);
        return 1;
    }
    
    fd = fat_open(file);
    if(fd < 0){
        printf("##Warning : open error (%s). skip.\n", file);
        fclose(fp);
        return 1;
    }
            
    
    // fat write
    for(i=0; i*SECTOR_SIZE < size; i++){
        memset(sector, 0, SECTOR_SIZE);
        fread(sector, SECTOR_SIZE, sizeof(char), fp);
        fat_setsector(sector, fd, i);
    }
    

    // fat write directory entry
    fat_readdirentry(&dent, fileentry[fd]);
    dent.size = size;
    fat_writedirentry(&dent, fileentry[fd]);

    fat_from83(name, dent.name, dent.ext);
    printf(">> %s %d\n", name, dent.size);
    
    fat_close(fd, -1);
    
    fclose(fp);

    return 0;
}

int writedir(char *parent, char *priority){
    DIR *dir;
    struct dirent *dp;
    char path[1024];
    char *p;
    int ret;
    printf(">> %s is directory\n", parent);
    strcpy(path, parent);
    for(p = path; *p != 0; p++);

    // add files to mmc image
    dir = opendir(parent);
    if(dir == NULL)
        return;
    
    // fat image creation
    for(dp = readdir(dir); dp != NULL; dp = readdir(dir)){
        if(!strcmp(".",     dp->d_name) || 
           !strcmp("..",    dp->d_name) )
            continue;

        if((priority != NULL) && !strcmp(priority, dp->d_name))
            continue;

        strcpy(p, dp->d_name);
        printf("< %s\n",path);

        if(dp->d_type == DT_DIR){
            char *tmp;
            for(tmp = p; *tmp != 0; tmp++);

                // fat create and open
            ret = fat_create(dp->d_name, 1);
            if(ret > 0){
                printf("##Warning : create dir error (%s). skip.\n", dp->d_name);
                continue;
            }
    
            fat_chdir(dp->d_name);
            *tmp++ = '/';
            *tmp = 0;
            writedir(path, NULL);
            
        }else if(dp->d_type == DT_REG){
            writefile(path, dp->d_name);
            printf("\n");
        }        
    }
    closedir(dir);

    ret = fat_chdir("..");
}

int readoutdir(char *parent){
    FILE *fp;
    DIR *dir;
    struct dirent *dp;
    int i, j, off;
    int fsize, size, ret, fd;
    direntry_t dent;
    char name[13];

    char sector[513];

    char path[1024], *p;

    // make mmcroot address
    strcpy(path, parent);
    for(p = path;*p != 0; p++);
    if(*(p-1) != '/'){
        *p = '/';
        p++;
        *p = 0;
    }


    // create file from mmc data (max 100 files only)
    for(j = 0; j < 100; j++){
        off = fat_findfilebyindex(j);
        if(off < 0)
            break;

        fat_readdirentry(&dent, off);
        fat_from83(name, dent.name, dent.ext);
        strcpy(p, name);
        printf("< %s %d\n",name, dent.size);
        printf("> %s\n\n", path);

        
        if (dent.attr & 0x10){ //directory 
            char *tmp;
            for(tmp = p; *tmp != 0; tmp++);
            *tmp++ = '/';
            *tmp = 0;

            // make mmcroot directory
            if(mkdir(path, 
                     S_IRUSR | S_IWUSR | S_IXUSR |
                     S_IRGRP | S_IWGRP | S_IXGRP |
                     S_IROTH | S_IXOTH | S_IXOTH ) == 0){
                printf("create directory (%s)\n", path);
                fat_chdir(name);
                readoutdir(path);
            }

        }else{
            // create file
            fp = fopen(path, "w");
            if(fp == NULL){
                printf("##Warning : can't open file (%s). skip.\n",
                       path);
                fclose(fp);
                continue;
            }


            // fat file
            fd = fat_open(name);
            if(fd < 0){
                printf("##Warning : can't open file (%s). skip.\n",
                       name);
                fclose(fp);
                continue;
            }
        
        
            // fat to file
            for(i=0; i*SECTOR_SIZE < dent.size; i++){
                memset(sector, 0, SECTOR_SIZE);
                fat_getsector(sector, fd, i);
                fwrite(sector, SECTOR_SIZE, sizeof(char), fp);
            }
        
            fat_close(fd, -1);
            fclose(fp);

            // change file size
            truncate(path, dent.size);
        }
    }

    fat_chdir("..");
}


uint getfatentry(FILE *fp, uint index, fatinfo_t *info)
{
    unsigned short temp;
    if (info->fattype == FAT12) {
        fseek(fp, info->off + info->fbegin * info->ssize + index * 3 / 2,
              SEEK_SET);
        fread(&temp, sizeof(unsigned short), 1, fp);
        if (index % 2 == 0)
            return (uint) temp & 0xfff;
        else
            return (uint) temp >> 4;
    } else {
        fseek(fp, info->off + info->fbegin * info->ssize + index * 2,
              SEEK_SET);
        fread(&temp, sizeof(unsigned short), 1, fp);
        return (uint) temp;
    }
}

void readfat(FILE *fp, uint depth, uint soff, fatinfo_t *info)
{
    uint i, j, last, cnt, cl;
    direntry_t entry;
    char tmpstr[13];
    tmpstr[8] = ' ';
    tmpstr[12] = '\0';

    for (i = 0; i < info->ssize; i += DIRENTRY_SIZE) {
        fseek(fp, info->off + soff * info->ssize + i, SEEK_SET);
        fread(&entry, sizeof(direntry_t), 1, fp);
        if (entry.name[0] == 0x00 || entry.name[0] == 0x05 ||
            entry.name[0] == (char) 0xe5 || entry.attr & 0x08)
            continue;

        // file information
        for (j = 0; j < depth * 2; j++)
            putchar(' ');
        for (j = 0; j < 8; j++)
            tmpstr[j] = entry.name[j];
        for (j = 0; j < 3; j++)
            tmpstr[j + 9] = entry.ext[j];
        printf("%s  ", tmpstr);
        if (entry.attr & 0x10)
            printf("     <dir>  ");
        else 
            printf("%10u  ", entry.size);
        printf("%02u/%02u/%02u %02u:%02u:%02u",
               ((entry.udate >> 9) + 80) % 100, (entry.udate >> 5) & 0xf,
               entry.udate & 0x1f, entry.utime >> 11,
               (entry.utime >> 5) & 0x3f, (entry.utime << 1) & 0x3e);

        // cluster chain
        cnt = 0;
        cl = entry.cluster;
        last = (info->fattype == FAT12) ? 0xff8 : 0xfff8;
#ifdef _CLUSTER_
        if (getfatentry(fp, cl, info) >= last) {
            printf(" [%04x]", cl);
        } else {
            while (cl < last) {
                if (cnt % 8 == 0) {
                    printf("\n");
                    for (j = 0; j < depth * 2 + 1; j++)
                        putchar(' ');
                }
                printf("[%04x] ", cl);
                cl = getfatentry(fp, cl, info);
                cnt++;
            }
        }
#endif
        printf("\n");
        // recursive call for directory 
        if ((entry.attr & 0x10) && (entry.name[0] != '.')) {
            cl = entry.cluster;
            while (cl < last) {
                for (j = 0; j < info->csize; j++)
                    readfat(fp, depth + 1,
                            info->cbegin + (cl - 2) * info->csize + j,
                            info);
                cl = getfatentry(fp, cl, info);
            }
        }
    }
}

int fat_in(int argc, char *argv[]){
    FILE *fp;
    DIR *dir;
    struct dirent *dp;
    char path[1024];
    char *p;

    char *mmcfile;
    char *mmcroot;
    char *outfile;
    char *priority = NULL;
    int i;
    int fsize, size, ret, fd;
    direntry_t dent;
    char name[12];

    char sector[513];

    if(argc != 5 && argc != 6){
        usage();
        return 1;
    }

    mmcfile = argv[2];
    mmcroot = argv[3];
    outfile = argv[4];

    fsize  = mmc_init(mmcfile);
    if(fsize < 0)
        return 1;
    

    // make mmcroot address
    strcpy(path, mmcroot);
    for(p = path;*p != 0; p++);
    if(*(p-1) != '/'){
        *p = '/';
        p++;
        *p = 0;
    }

    // add 1st priority file to mmc image
    if(argc == 6){
        priority = argv[5];
        strcpy(p, priority);
        printf("%s is first write file\n", priority);

        strcpy(p, priority);
        printf("< %s\n",path);
        
        writefile(path, priority);
    }

    // write directory recursively
    writedir(mmcroot, priority);
    
    
    fp = fopen(outfile,"w");
    ret = fwrite(mmc, sizeof(char), fsize, fp);
    if(ret != fsize)
        printf("## Warning: write to mmc file\n");
    
    fclose(fp);
    free(mmc);
}

int fat_out(int argc, char *argv[]){
    FILE *fp;
    DIR *dir;
    struct dirent *dp;
    char path[512];
    char *p;

    char *mmcfile;
    char *mmcroot;
    char *outfile;
    char *prior = NULL;
    int i, j, off;
    int fsize, size, ret, fd;
    direntry_t dent;
    char name[12];

    char sector[513];

    if(argc != 4){
        usage();
        return 1;
    }

    mmcfile = argv[2];
    mmcroot = argv[3];

    fsize  = mmc_init(mmcfile);
    if(fsize < 0){
        printf("## fat init error (%d).\n", fsize);
        return 1;
    }
    

    // make mmcroot address
    strcpy(path, mmcroot);
    for(p = path;*p != 0; p++);
    if(*(p-1) != '/'){
        *p = '/';
        p++;
        *p = 0;
    }

    // make mmcroot directory
    if(mkdir(mmcroot, 
             S_IRUSR | S_IWUSR | S_IXUSR |
             S_IRGRP | S_IWGRP | S_IXGRP |
             S_IROTH | S_IXOTH | S_IXOTH ) == 0){
        printf("create directory (%s)\n", mmcroot);
    }

    readoutdir(path);

    
    free(mmc);
    return 0;
}

int fat_cat(int argc, char*argv[]){
    int fd;
    int i,ret, size, fsize;
    char buf[513];

    char *mmcfile, *catfile;
    direntry_t dent;

    if(argc != 4){
        usage();
        return 1;
    }

    mmcfile = argv[2];
    catfile = argv[3];
    
    fsize = mmc_init(mmcfile);
    if(fsize < 0){
        fprintf(stderr, "## fat init error (%d).\n",ret);
        return 1;
    }

    fd = fat_open(catfile);
    if(fd < 0){
        printf("## %s was not found",catfile);
        free(mmc);
        return 1;
    }

    fat_readdirentry(&dent, fileentry[fd]);
    size= dent.size;

    printf("%d\n",size);

    // fat read
    for(i=0; i*512 < size; i++){
        memset(buf, 0, 513);
        //fread(sector, 512, sizeof(char), fp);
        fat_getsector(buf, fd, i);
        printf("%s", buf);
    }
    fat_close(fd, -1);
    

    free(mmc);
    
    return 0;
}

int fat_info(int argc, char *argv[]){
    FILE *fp;
    partition_t par;
    bpb_t bpb;
    fatinfo_t info;
    unsigned short sig;
    uint i;

    char *mmcfile;

    if (argc != 3) {
        usage();
        return 1;
    }

    mmcfile = argv[2];

    if ((fp = fopen(mmcfile, "rb")) == NULL) {
        printf("##Error : can't open file %s\n", mmcfile);
        return 1;
    }
    
    printf("[[MBR Info]]\n");
    // check MBR signature
    fseek(fp, 0x1fe, SEEK_SET);
    fread(&sig, sizeof(unsigned short), 1, fp);
    printf("signature: 0x%04x (must be 0xaa55)\n", sig);
    if (sig != 0xaa55) {
        fclose(fp);
        return 0;
    }
    
    fseek(fp, 0x1be, SEEK_SET);
    fread(&par, sizeof(partition_t), 1, fp);
    info.off = par.firstsector * SECTOR_SIZE;
    info.fattype = ((par.filesystem == 1) ? FAT12 : 
                    (par.filesystem == 4) ? FAT16 :
                    (par.filesystem == 6) ? FAT16 :
                    (par.filesystem == 14) ? FAT12 : OTHERTYPE);

    printf("Total Physical Size: %u Bytes\n", 
           (par.firstsector + par.sectornum) * SECTOR_SIZE);
    printf("Total Partition Size: %u Bytes\n", 
           par.sectornum * SECTOR_SIZE);
    printf("File System: %s\n",
           (info.fattype == FAT12) ? "FAT12" :
           (info.fattype == FAT16) ? "FAT16" : "Unknown");
    printf("Partition Start Sector: %u\n\n", par.firstsector);
    
    if (info.fattype == OTHERTYPE) {
        fclose(fp);
        return 0;
    }

    printf("[[BPB Info]]\n");
    // check BPB signature
    fseek(fp, info.off + 0x1fe, SEEK_SET);
    fread(&sig, sizeof(unsigned short), 1, fp);
    printf("signature: 0x%04x (must be 0xaa55)\n", sig);
    if (sig != 0xaa55) {
        fclose(fp);
        return 0;
    }

    // print BPB summary
    fseek(fp, info.off, SEEK_SET);
    fread(&bpb, sizeof(bpb_t), 1, fp);
    if (bpb.sectornum2 == 0)
        bpb.sectornum2 = bpb.sectornum;

    info.ssize = bpb.sectorsize;
    info.csize = bpb.clustersize;
    info.rsize = (uint) bpb.rootentry * DIRENTRY_SIZE / bpb.sectorsize;
    info.fbegin = bpb.fatbegin;
    info.rbegin = info.fbegin + (uint) bpb.fatnum * bpb.fatsize;
    info.cbegin = info.rbegin + info.rsize;

    printf("Total Physical Size: %u Bytes\n", 
           (uint) bpb.sectorsize * bpb.sectornum2);
    printf("Total Logical Size: %u Bytes\n",
           (uint)(bpb.sectornum2 - info.cbegin)
           / bpb.clustersize * bpb.clustersize * bpb.sectorsize);
    printf("Sector Size: %u Bytes\n", bpb.sectorsize);
    printf("Cluster Size: %u Bytes\n",
           (uint) bpb.sectorsize * bpb.clustersize);
    printf("Number of FAT: %u\n", bpb.fatnum);
    printf("Size of FAT: %u Sectors\n", bpb.fatsize);
    printf("Start sector for FATs: %u\n", bpb.fatbegin);
    printf("Root Directory Entry Start: %u\n", info.rbegin);
    printf("Root Directory Entry: %u\n", bpb.rootentry);
    printf("Root Directory Size: %u Sectors\n", info.rsize);
    printf("2nd Cluster Start Sector: %u\n\n", info.cbegin);

    printf("[[File Organization Info]]\n");
    // read root directory
    for (i = 0; i < info.rsize; i++) {
        readfat(fp, 0, info.rbegin + i, &info);
    }

    fclose(fp);
    return 0;
}



int mmc_init(char *file){
    FILE *fp;
    int fsize, ret;

    // open base mmcfile
    fp = fopen(file, "r");
    if(fp == NULL){
        fprintf(stderr, "## can't open mmcfile: %s\n", file);
        return -1;
    }

    // calculate file size
    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // make base mmc image
    mmc = (char *)malloc(sizeof(char) * fsize);
    ret = fread(mmc, sizeof(char), fsize, fp);
    if(ret != fsize)
        fprintf(stderr, "## Warning: read from mmc file\n");
    fclose(fp);

    // fat initialization
    ret = fat_init();
    if(ret > 0){
        fprintf(stderr, "## fat init error (%d)\n",ret);
        return -1;
    }

    return fsize;
}

int main(int argc, char *argv[]){

    if(argc < 2){
        usage();
        return 1;
    }

    if(!strcmp(argv[1], "in")){
        fat_in(argc, argv);
    }else if(!strcmp(argv[1], "out")){
        fat_out(argc, argv);
    }else if(!strcmp(argv[1], "cat")){
        fat_cat(argc, argv);
    }else if(!strcmp(argv[1], "info")){
        fat_info(argc, argv);
    }else{
        usage();
        return 1;
    }
    
    return 0;
}
