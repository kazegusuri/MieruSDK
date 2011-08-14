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

#include <kernel.h>
#include <fat.h>

static unsigned char *buffer;
volatile uint *mmc_data = (volatile uint *) MIERU_MMC_DATA_S;

/******************************************************************************/
int Fat::init(void){
    FatPartition par;
    FatBpb bpb;
    unsigned char sig[2];
    int sec, snum;

    // memory map
    buffer = (uchar *)mmc_data;

    // check MBR
    readSector(0);
    sig[0] = buffer[0x1fe];
    sig[1] = buffer[0x1ff];
    if(sig[0] != 0x55 || sig[1] != 0xaa)
        return -FAT_ERROR_MBR_INVALID;

    // check fat type
    memcpy(&par, &buffer[0x1be], sizeof(FatPartition));
    sec = par.firstsector;
    type = ((par.filesystem ==  1) ? FAT12 : 
            (par.filesystem ==  4) ? FAT16 :
            (par.filesystem ==  6) ? FAT16 :
            (par.filesystem == 14) ? FAT12 : OTHERTYPE);
    if (type == OTHERTYPE)
        return -FAT_ERROR_FAT_TYPE_INVALID;

    // check BPB(BIOS Parameter Block)
    readSector(sec);
    sig[0] = buffer[0x1fe];
    sig[1] = buffer[0x1ff];
    if(sig[0] != 0x55 || sig[1] != 0xaa)
        return -FAT_ERROR_BPB_INVALID;

    memcpy( &bpb, &buffer[0], sizeof(FatBpb));
    if (bpb.sectorsize != Fat::SECTOR_SIZE) 
        return -FAT_ERROR_SECTOR_SIZE_INVALID;

    // save Filesystem Information
    fatsize = bpb.fatsize;
    rootsize = (uint) bpb.rootentry * Fat::ENTRY_SIZE / bpb.sectorsize;
    clustersize = bpb.clustersize;
    fatbegin = sec + bpb.fatbegin;
    rootbegin = fatbegin + (uint) bpb.fatnum * bpb.fatsize;
    clusterbegin = rootbegin + rootsize;
    snum = (bpb.sectornum2 == 0) ?  bpb.sectornum : bpb.sectornum2;
    clusternum = (snum - clusterbegin + sec) / clustersize + 2;
    last = (type == FAT12) ? 0xff8 : 0xfff8;

    // create static root entry
    FatEntry root;
    root.name[0] = 0;
    root.ext[0] = 0;
    root.attr = 0x1f; // special use
    root.cluster = 0;
    root.cluster2 = 0;
    root.size = 0;
    
    // create root file
    kernel.root = new FatFile(root, 1, 0, 0, 0);

    // create current dir file
    kernel.current = new FatFile(root, 1, 0, 0, 0);
    
    return 0;
}

/******************************************************************************/
void Fat::changemmcblock(uint addr){
    volatile unsigned int *control = (volatile unsigned int *) MP_MMAP_MMC_CONTROL;

    while((*control & 1) != 1);
    *control = addr & 0xFFFFFE00;
    mpc_usleep(1);
    while((*control & 1) != 1);
}

/******************************************************************************/
void Fat::readSector(int sec){
    changemmcblock(sec * Fat::SECTOR_SIZE);
}

/******************************************************************************/
void Fat::writeSector(int sec){
}

/******************************************************************************/
void Fat::readEntry(FatEntry *dest, int off){
    if (!dest) return;    

    off = off * Fat::ENTRY_SIZE;
    memcpy(dest, buffer + off, Fat::ENTRY_SIZE); 

}

/******************************************************************************/
void Fat::writeEntry(FatEntry *src, int off){
    if (!src) return;    

    off = off * Fat::ENTRY_SIZE;
    memcpy(buffer + off, src, Fat::ENTRY_SIZE);
}

/******************************************************************************/
uint Fat::referTable(int index){
    unsigned char lastbyte;
    int sec, mod;
    uint value;

    if (index <= 0 || index >= clusternum) return -1;

    if (type == FAT12) {
        // (0xab,0xcd,0xef) are translated to (0xdab,0xefc)
        sec = fatbegin + index * 3 / 2 / Fat::SECTOR_SIZE;
        mod = (index * 3 / 2) % Fat::SECTOR_SIZE;
        readSector(sec);
        if (mod == Fat::SECTOR_SIZE - 1) {
            lastbyte = buffer[Fat::SECTOR_SIZE - 1];
            readSector(sec + 1);
            if (index % 2 == 0)
                value = ((buffer[0] << 8) | lastbyte) & 0xfff;
            else
                value =  ((buffer[0] << 4) | (lastbyte >> 4)) & 0xfff;
        } else {
            if (index % 2 == 0)
                value = ((buffer[mod + 1] << 8) | buffer[mod]) & 0xfff;
            else
                value = ((buffer[mod + 1] << 4) | (buffer[mod] >> 4)) & 0xfff;
        }
    } else {
        sec = fatbegin + index * 2 / Fat::SECTOR_SIZE;
        mod = (index * 2) % Fat::SECTOR_SIZE;
        readSector(sec);
        value = ((buffer[mod + 1] << 8) | buffer[mod]) & 0xffff;
    }
    
    if(value <= 1 || value >= (uint)last)
        return -1;
    return value;
}

/******************************************************************************/
int Fat::updateTable(int index, int value){
    int i, sec, mod;
    if (index <= 0 || index >= clusternum) return -1;

    for (i = 0; i < 2; i++) {
        if (type == FAT12) {
            // (0xab,0xcd,0xef) are translated to (0xdab,0xefc)
            sec = fatbegin + fatsize * i + index * 3 / 2 / Fat::SECTOR_SIZE;
            mod = (index * 3 / 2) % Fat::SECTOR_SIZE;
            readSector(sec);
            if (mod == Fat::SECTOR_SIZE - 1) {
                if (index % 2 == 0)
                    buffer[mod] = value & 0xff;
                else
                    buffer[mod] = (buffer[mod] & 0xf) | ((value << 4) & 0xf0); 
                writeSector(sec);

                readSector(sec + 1);
                if (index % 2 == 0)
                    buffer[0] = (buffer[0] & 0xf0) | ((value >> 8) & 0xf);
                else
                    buffer[0] = (value >> 4) & 0xff;

                writeSector(sec + 1);
            } else {
                if (index % 2 == 0) {
                    buffer[mod] = value & 0xff;
                    buffer[mod + 1] = ((buffer[mod + 1] & 0xf0) |
                                       ((value >> 8) & 0xf));
                } else {
                    buffer[mod] = (buffer[mod] & 0xf) | ((value << 4) & 0xf0);
                    buffer[mod + 1] = (value >> 4) & 0xff;
                }
                writeSector(sec);
            }   
        } else {
            sec = fatbegin + fatsize * i + index * 2 / Fat::SECTOR_SIZE;
            mod = (index * 2) % Fat::SECTOR_SIZE;
            readSector(sec);
            buffer[mod] = value & 0xff;
            buffer[mod + 1] = (value >> 8) & 0xff;
            writeSector(sec);
        }
    }
    return 0;
}

/******************************************************************************/
int Fat::getEmptyCluster(){

    char lastbyte, check;
    int i, sec, mod, cnt;

    lastbyte = 0;
    sec = fatbegin;
    mod = cnt = 0;

    readSector(sec);

    for (i = 0; i < clusternum; i++) {
        if (type == FAT12) {
            if(mod == -1){
                if (i & 0x1)
                    check = (lastbyte & 0xf0) | buffer[0];
                else
                    check = lastbyte | (buffer[0] & 0xf);
            }else{
                if (i & 0x1)
                    check = (buffer[mod] & 0xf0) | buffer[mod + 1];
                else
                    check = buffer[mod] | (buffer[mod + 1] & 0xf);
            }
        } else {
            check = buffer[mod] | buffer[mod + 1];
        }
        if (check == 0)
            return i;

        if (type == FAT12 && !(i & 0x1))
            mod++;
        else 
            mod += 2;
        if (mod >= Fat::SECTOR_SIZE - 1) {
            mod -= Fat::SECTOR_SIZE;
            sec++;
            lastbyte = buffer[Fat::SECTOR_SIZE - 1];
            readSector(sec);
        }
    }

    return -1;
}

/******************************************************************************/
int Fat::allocNewCluster(int prev){
    int next;

    // get empty cluster
    next = getEmptyCluster();
    if(next < 0) return -1;

    // empty cluster is always last in the list
    updateTable(next, 0xffff);

    // in case that existing prev cluster
    if(prev >= 2 && prev < clusternum)
        updateTable(prev, next);

    // clear sectors
    for(int i = 0; i < clustersize; i++){
        int sec = clusterbegin + (next - 2) * clustersize + i;
        readSector(sec);
        memset(buffer, 0, Fat::SECTOR_SIZE);
    }

    return next;
}

/******************************************************************************/
int Fat::deleteCluster(int cl){
    if(cl < 2 && cl >= clusternum)
        return 1;
    
    updateTable(cl, 0x0);
    return 0;
}

/******************************************************************************/
FatCluster::FatCluster(){
    isroot = 0;
    cl = -1;
    sectors = 0;
}

/******************************************************************************/
bool FatCluster::isRoot(){
    return isroot;
}

/******************************************************************************/
int FatCluster::getTop(){
    return cl;
}

/******************************************************************************/
int FatCluster::getSize(){
    return sectors;
}

/******************************************************************************/
int FatCluster::getSector(){
    if(isroot){
        return kernel.fat.rootbegin + cur_pos;
    }else{
        return kernel.fat.clusterbegin + (cur_cl - 2) * kernel.fat.clustersize 
            +  (cur_pos % kernel.fat.clustersize);
    }
}

/******************************************************************************/
void FatCluster::set(int n){
    cl = n;

    if(n == 0){
        isroot = 1;
        sectors = kernel.fat.rootsize;
    }else{
        isroot = 0;
        sectors = 0;
        while(n > 0){
            n = kernel.fat.referTable(n);
            sectors += kernel.fat.clustersize;
        }
    }
}

/******************************************************************************/
uchar *FatCluster::next(){
    int size, n, off, sec;

    if(sectors <= cur_pos + 1) return NULL;
    cur_pos++;

    // root entry
    if(isroot){
        sec = kernel.fat.rootbegin + cur_pos;
        kernel.fat.readSector(sec);
        return buffer;
    }

    size = kernel.fat.clustersize;
    n = cur_pos / size;
    off = cur_pos % size;

    // get next cluster if needed
    if( (cur_pos % size) == 0 ){
        cur_cl = kernel.fat.referTable(cur_cl);
        if(cur_cl < 0) return NULL;
    }
    
    sec = kernel.fat.clusterbegin + (cur_cl - 2) * size + off;
    kernel.fat.readSector(sec);
    return buffer;
}

/******************************************************************************/
uchar *FatCluster::seek(int nth){
    int size, sec, off, n, i, cur;

    if(nth >= sectors) return NULL;

    // root entry
    if(isroot){
        cur_pos = nth;
        cur_cl = cl;
        sec = kernel.fat.rootbegin + nth;
        kernel.fat.readSector(sec);
        return buffer;
    }

    cur = cl;
    size = kernel.fat.clustersize;
    n = nth / size;
    off = nth % size;

    // get the target clusster
    for(i = 0; i < n; i++){
        cur = kernel.fat.referTable(cur);
        if(cur < 0) return NULL;
    }

    cur_pos = nth;
    cur_cl = cur;

    sec = kernel.fat.clusterbegin + (cur - 2) * size + off;
    kernel.fat.readSector(sec);
    return buffer;
}

/******************************************************************************/
int FatCluster::add(int n){
    int next, remain, tmp;

    remain = n;
    // root entry can't extend
    if(isroot) return -1;

    // in case that there is no cluster in the list
    if(sectors == 0){
        cl = kernel.fat.allocNewCluster(0);
        if(cl < 0) goto out;

        remain--;
    }

    // track the list
    tmp = cl;
    for(;;){
        next = kernel.fat.referTable(tmp);
        if(next < 0) break;
        tmp = next;
    }

    // add new cluster to the list
    for(; remain > 0; remain--){
        tmp = kernel.fat.allocNewCluster(tmp);
        if(tmp < 0) goto out;
    }
out:
    sectors += (n - remain) * kernel.fat.clustersize;
    return n - remain;
}

/******************************************************************************/
void FatCluster::remove(){
    int cur, next;
    if(isroot) return;

    cur = cl;

    // get the target clusster
    while(cur >= 0){
        next = kernel.fat.referTable(cur);
        kernel.fat.deleteCluster(cur);
        cur = next;
    }

    cl = sectors = cur_cl = cur_pos = 0;
}

/******************************************************************************/
void FatCluster::clear(){
    cl = sectors = cur_cl = cur_pos = 0;
}

/******************************************************************************/
