/******************************************************************************/
/* MieruOS: FatFile v0.1                                                      */
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
#include <file.h>
#include <fat.h>



/******************************************************************************/
void convertToStat(FatEntry *fat, Stat *stat){
    char *p;
    uint cyear, cmon, cday, chour, cmin, csec;
    uint uyear, umon, uday, uhour, umin, usec;
    uint ayear, amon, aday;

    if (!fat || !stat) return;

    p = stat->name;
    for(int i = 0; i < 8; i++){
        if(fat->name[i] == ' ' || fat->name[i] == 0)
            break;
        
        *p++ = fat->name[i];
    }
    
    if(fat->ext[0] != ' ' && fat->ext[0] != 0)
        *p++ = '.';

    for(int i = 0; i < 3; i++){
        if(fat->ext[i] == ' ' || fat->ext[i] == 0)
            break;
        
        *p++ = fat->ext[i];
    }

    *p = 0;

    cyear = (fat->cdate >> 9);
    cmon  = (fat->cdate >> 5) & 0xf; 
    cday  = fat->cdate & 0x1f;       
    chour = fat->ctime >> 11;        
    cmin  = (fat->ctime >> 5) & 0x3f;
    csec  = (fat->ctime << 1) & 0x3e;
    uyear = (fat->udate >> 9);
    umon  = (fat->udate >> 5) & 0xf; 
    uday  = fat->udate & 0x1f;       
    uhour = fat->utime >> 11;        
    umin  = (fat->utime >> 5) & 0x3f;
    usec  = (fat->utime << 1) & 0x3e;
    ayear = (fat->adate >> 9);
    amon  = (fat->adate >> 5) & 0xf; 
    aday  = fat->adate & 0x1f;

    stat->inode = fat->cluster;
    stat->mode  = fat->attr;
    stat->ctime = 0;
    stat->atime = 0;
    stat->utime = 0;
    stat->size  = fat->size;
}

/******************************************************************************/
void convertToFileEntry(FatEntry *fat, FileEntry *file){
    char *p;
    if (!fat || !file) return;

    p = file->name;
    for(int i = 0; i < 8; i++){
        if(fat->name[i] == ' ' || fat->name[i] == 0)
            break;
        
        *p++ = fat->name[i];
    }
    
    if(fat->ext[0] != ' ' && fat->ext[0] != 0)
        *p++ = '.';

    for(int i = 0; i < 3; i++){
        if(fat->ext[i] == ' ' || fat->ext[i] == 0)
            break;
        
        *p++ = fat->ext[i];
    }

    *p = 0;

    file->inode  = fat->cluster;
    file->mode   = fat->attr;
    file->offset = sizeof(FileEntry);
    file->len    = sizeof(FileEntry);
}

/******************************************************************************/
FatFile::FatFile(){
    use=0;
}

/******************************************************************************/
FatFile::FatFile(FatFile &fatfile){
    this->cluster = fatfile.cluster;
    this->entry = fatfile.entry;
    this->entry_sec = fatfile.entry_sec;
    this->entry_off = fatfile.entry_off;
    this->parent = fatfile.parent;    
    this->exist = 1;
    this->isdir = fatfile.isdir;
}

/******************************************************************************/
FatFile::FatFile(int cl){
    set(cl);
}

/******************************************************************************/
FatFile::FatFile(FatEntry ent, int exi, int par, int sec, int nth){
    set(ent, exi, par, sec, nth);
}

/******************************************************************************/
FatFile::~FatFile(){
    use = 0;
}

/******************************************************************************/
void *FatFile::operator new(uint size){
    int i;
    FatFile *file;
    for(i = 0; i < MAX_FILE; i++){
        file = &kernel.files[i];
        if(!file->use)
            break;
    }
    if(i == MAX_FILE){
        lcd_dprintf("File MAX\n");
        return NULL;
    }

    file->user = 0;
    file->use = 1;
    file->num = i;
    return file;
}

/******************************************************************************/
void FatFile::operator delete(void *address){
}

/******************************************************************************/
int FatFile::updateEntry(){
    kernel.fat.readSector(this->entry_sec);
    kernel.fat.writeEntry(&this->entry, this->entry_off);
    return 0;
}

/******************************************************************************/
void FatFile::set(int cl){
    uchar *p;
    int ret, sec, off;
    FatEntry ent;
    FatFileName fatname;
    FatCluster parent;
    int parentcl;

    this->exist = 1;
    this->isdir = 1;

    // copy the entry if root directory
    if(cl == 0)
        return set( kernel.root->entry, 1, 0, 0, 0);

    // create this dir's cluster
    this->cluster.set(this->isdir, cl);

    // search ".." entry in this directory
    fatname.set("..");
    p = cluster.seek(0);
    ret = searchEntry(fatname, &ent);
   
    // this dir must be root directory if not found
    if(ret < 0)
        return set( kernel.root->entry, 1, 0, 0, 0); // impossible to come here

    // create parent cluster
    parentcl = ent.cluster;
    parent.set(true, parentcl);

    // search own entry in parent cluster
    sec = off = 0;
    for(p = parent.seek(0); p != NULL; p = parent.next()){
        off = searchEntry(cl, &ent);
        if(off < 0) continue;
        sec = parent.getSector();
        break;
    }

    this->entry = ent;
    this->entry_sec = sec;
    this->entry_off = off;
    this->parent = parentcl;
    printEntry();
}

/******************************************************************************/
void FatFile::set(FatEntry ent, int exi, int par, int sec, int nth){

    this->entry = ent;
    this->exist = exi;
    this->parent = par;
    this->entry_sec = sec;
    this->entry_off = nth;
    this->isdir = ((this->entry.attr & FAT_FILE_ATTR_DIR) != 0);

    // root directory
    if(exist)
        this->cluster.set(this->isdir, this->entry.cluster);
    else
        this->cluster.clear();
}

/******************************************************************************/
int FatFile::searchEntry(FatFileName fatname, FatEntry *entry){
    int i;

    for(i = 0; i < Fat::SECTOR_SIZE / Fat::ENTRY_SIZE; i++) {
        kernel.fat.readEntry(entry, i);
        if(entry->name[0] == 0x00 || entry->name[0] == 0x05 ||
           entry->name[0] == (char) 0xe5 ||
           entry->attr & FAT_FILE_ATTR_VOLUME)
            continue;


        if(strncmp(entry->name, fatname.name, 8) != 0)
            continue;
        if(strncmp(entry->ext, fatname.ext, 3) != 0)
            continue;
        return i;
    }
    return -1;
}

/******************************************************************************/
int FatFile::searchEntry(int cl, FatEntry *entry){
    int i;

    for(i = 0; i < Fat::SECTOR_SIZE / Fat::ENTRY_SIZE; i++) {
        kernel.fat.readEntry(entry, i);
        if(entry->name[0] == 0x00 || entry->name[0] == 0x05 ||
           entry->name[0] == (char) 0xe5 ||
           entry->attr & FAT_FILE_ATTR_VOLUME)
            continue;

        if( cl != entry->cluster)
            continue;
        return i;
    }
    return -1;
}

/******************************************************************************/
int FatFile::searchEntry(){
    int i;
    FatEntry entry;

    for(i = 0; i < Fat::SECTOR_SIZE / Fat::ENTRY_SIZE; i++) {
        kernel.fat.readEntry(&entry, i);
        if (entry.name[0] == 0x00 || 
            entry.name[0] == (char) 0x05 || 
            entry.name[0] == (char) 0xe5)
            return i;
    }
    return -1;
}

/******************************************************************************/
FatFile *FatFile::getParent(int cl){
    return new FatFile(cl);
}

/******************************************************************************/
FatFile *FatFile::getCopy(){
    return new FatFile(*this);
}

/******************************************************************************/
File *FatFile::lookup(const char *name){
    FatEntry ent;
    FatFileName fatname;
    int i, sec, off, ret;
    bool found = false;
    uchar *buffer;

    if(!name || !name[0]) return NULL;
    if(!isdir || !this->exist) return NULL;
    
    ret = fatname.set(name);
    if(ret < 0) return NULL;

    sec = off = 0;
    for(buffer = cluster.seek(0); buffer != NULL; buffer = cluster.next()){
        off = searchEntry(fatname, &ent);
        if(off < 0) continue;

        found = true;
        sec = cluster.getSector();
        break;
    }

    if(fatname.name[0] == '.'){
        if(!this->isdir) return NULL;

        if(fatname.name[1] == '.')
            return getParent(parent);
        else
            return getCopy();
    }
    
    if(!found){
        memset(&ent, 0, Fat::ENTRY_SIZE);
        for(i = 0; i < 8; i++)
            ent.name[i] = fatname.name[i];
        for(i = 0; i < 3; i++)
            ent.ext[i] = fatname.ext[i];
        sec = off = 0;
        
    }

    return new FatFile(ent, (int)found, this->entry.cluster, sec, off);
}

/******************************************************************************/
int FatFile::open(int mode){
    int ret;
    if(!this->exist){
        if(!(mode & O_CREAT)){
            return -FAT_ERROR_NO_SUCH_FILE;
        }

        ret = this->create(0, 0);
        if(ret < 0)
            return ret;
    }

    // check if file is directory
    if(this->isdir && !(mode & O_DIRECTORY))
        return -FAT_ERROR_IS_DIRECTORY;
    if(!this->isdir && (mode & O_DIRECTORY))
        return -FAT_ERROR_NOT_DIRECTORY;

    // set file size 0 in trunk mode
    if(mode & O_TRUNK)
        this->size = 0;
    else
        this->size = this->entry.size;

    this->mode = mode;
    this->pos = 0;
    // lcd_printf("open %d\n", this->size);
    // int i;
    // for(i=0;i<8;i++){
    //     lcd_printf("%c", this->entry.name[i]);
    // }
    // lcd_printf("\n");
  
    return 0;
}

/******************************************************************************/
int FatFile::create(int mode, int isdir){
    FatCluster cl;
    uchar *p;
    bool found = false;
    int sec, off, ret;

    if(this->exist == 1)
        return -FAT_ERROR_FILE_ALREADY_EXIST;

    // create parent cluster list
    cl.set(true, parent);

    // search empty entry in parent dir
    sec = off = 0;
    for(p = cl.seek(0); p != NULL; p = cl.next()){
        off = searchEntry();
        if(off < 0) continue;

        sec = cl.getSector();
        found = true;
        break;
    }

    // add new cluster and search again if not found
    if(!found){
        ret = cl.add(1);
        if(ret < 0)
            return -FAT_ERROR_DISC_IS_FULL;

        for(p = cl.seek(0); p != NULL; p = cl.next()){
            off = searchEntry();
            if(off < 0) continue;

            sec = cl.getSector();
            break;
        }
    }

    this->exist = 1;
    this->entry_sec = sec;
    this->entry_off = off;
    this->entry.size = 0;
    this->entry.ctimecs = 0;
    this->entry.ctime = this->entry.utime = 0;                    //   00:00:00
    this->entry.cdate = this->entry.udate = this->entry.adate = 0x3a21; // 2009/01/01
    this->entry.attr = FAT_FILE_ATTR_ARC | ((isdir) ? FAT_FILE_ATTR_DIR : 0);
    this->entry.cluster = this->entry.cluster2 = Fat::UNDEFINED_CLUSTER;
    this->isdir = isdir;

    if(isdir){
        ret = this->cluster.add(1);
        if(ret != 1)
            return FAT_ERROR_DISC_IS_FULL;

        // allocate new cluster
        this->entry.cluster = this->cluster.getTop();

        // update entry
        updateEntry();
        
        FatEntry ent;
        ent = this->entry;
        // initialize names
        for(int i=0;i<8;i++) ent.name[i] = ' ';
        for(int i=0;i<3;i++) ent.ext[i] = ' ';

        // "." entry is always assigned here
        ent.name[0] = '.'; // "."
        this->cluster.seek(0);
        kernel.fat.writeEntry(&ent, 0);
        

        // ".." entry is always assigned here
        ent.cluster = this->parent;
        ent.name[1] = '.'; // ".."
        kernel.fat.writeEntry(&ent, 1);
    }else{
        updateEntry();
    }

    return 0;
}

/******************************************************************************/
int FatFile::close(){
    _syscall(5000, 0,0,0,0);
    return 0;
}

/******************************************************************************/
int FatFile::stat(Stat *st){
    convertToStat( &this->entry, st);
    return 0;
}

/******************************************************************************/
int FatFile::seek(int offset, uint whence){
    int to;
    if(this->isdir)
        return -FAT_ERROR_IS_DIRECTORY;

    if(!this->exist)
        return -FAT_ERROR_NO_SUCH_FILE;

    switch(whence){
    case SEEK_SET:
        to = 0;
        break;
    case SEEK_CUR:
        to = this->pos;
        break;
    case SEEK_END:
        to = this->size;
        break;
    default:
        return -FAT_ERROR_INVALID_FLAG ;
    }
    
    to += offset;
    if(to < 0 || (uint)to > this->size)
        return -FAT_ERROR_INVALID_OFFSET;
    
    this->pos = to;
    return to;
}

/******************************************************************************/
int FatFile::read(void *buf, uint count){
    if(!this->exist)
        return -FAT_ERROR_NO_SUCH_FILE;

    if( buf == NULL )
        return -FAT_ERROR_INVALID_POINTER;

    if(isdir)
        return readDir(buf, count);
    else{
        if( (this->mode & O_ACCMODE) != O_RDONLY &&
            (this->mode & O_ACCMODE) != O_RDWR  )
            return -FAT_ERROR_INVALID_FLAG;

        return readFile(buf, count);
    }
}

/******************************************************************************/
int FatFile::readDir(void *buf, uint count){
    uint first, from_sec, offset;
    int i, remain;
    uchar *buffer;
    FatEntry ent;
    FileEntry *p;

    //set reading points
    first = this->pos;
    remain = count / sizeof(FileEntry);

    if(remain <= 0)
        return 0;

    p = (FileEntry *)buf;
    from_sec = first / Fat::SECTOR_SIZE;
    offset = first % Fat::SECTOR_SIZE; // first & 0x1ff 
    this->pos -= offset;

    for(buffer = cluster.seek(from_sec); buffer != NULL; buffer = cluster.next()){
        for(i = offset / Fat::ENTRY_SIZE; i < Fat::SECTOR_SIZE / Fat::ENTRY_SIZE; i++) {
            kernel.fat.readEntry(&ent, i);
            if (ent.name[0] == 0x00 || ent.name[0] == (char) 0x05 ||
                ent.name[0] == (char) 0xe5 || 
                ent.attr & FAT_FILE_ATTR_VOLUME)
                continue;

            convertToFileEntry(&ent, p);
            p++;
            remain--;
            if(remain == 0)
                break;
        }

        if(remain == 0)
            break;
        
        this->pos += Fat::SECTOR_SIZE;
        offset = 0;
    }

    this->pos += (i+1) * Fat::ENTRY_SIZE;
    return count - remain * Fat::ENTRY_SIZE;
}

/******************************************************************************/
int FatFile::readFile(void *buf, uint count){
    uint first, last, from_sec, to_sec;
    uint read_size, offset;
    int remain;
    uchar *buffer, *p;

    // set reading points
    first = this->pos;
    last  = this->pos + count;
    remain = count;

    // in case of over size
    if(last > this->size){
        last = this->size;
        remain = last - first;
    }

    if(remain <= 0)
        return 0;

    from_sec = first / Fat::SECTOR_SIZE;
    to_sec   = (last - 1)  / Fat::SECTOR_SIZE;

    // set for first read
    p = (uchar *)buf;
    offset = first % Fat::SECTOR_SIZE; // first & 0x1ff 
    read_size = Fat::SECTOR_SIZE - offset;
    if(read_size > (uint)remain)
        read_size = remain;
    count = 0;
    
    buffer = cluster.seek(from_sec);
    for(uint i = from_sec; i <= to_sec; i++){
        if(buffer == NULL)
            return -FAT_ERROR_KERNEL_BUG; // impossible to come here

        // read from buffer
        memcpy(p, buffer + offset, read_size);
        count += read_size;

        // set for next read
        remain -= read_size;
        p += read_size;
        offset = 0;
        if( remain > Fat::SECTOR_SIZE)
            read_size = Fat::SECTOR_SIZE;
        else
            read_size = remain;

        buffer = cluster.next();
    }

    this->pos += count;
    return count;
}

/******************************************************************************/
int FatFile::write(const void *buf, uint count){
    if(!this->exist)
        return -FAT_ERROR_NO_SUCH_FILE;

    if(isdir)
        return -FAT_ERROR_IS_DIRECTORY;
    else{
        if( (this->mode & O_ACCMODE) != O_WRONLY &&
            (this->mode & O_ACCMODE) != O_RDWR  )
            return -FAT_ERROR_INVALID_FLAG;

        return writeFile(buf, count);
    }
    
    return 0;
}

/******************************************************************************/
int FatFile::writeFile(const void *buf, uint count){
    uint first, last, from_sec, to_sec;
    uint write_size, offset;
    int remain;
    uchar *buffer, *p;

    if( buf == NULL )
        return -FAT_ERROR_INVALID_POINTER;

    // file pos is always last in append mode
    if(this->mode & O_APPEND)
        this->pos = this->size;

    // set writing points
    first = this->pos;
    last  = this->pos + count;
    remain = count;

    if(remain <= 0)
        return 0;

    from_sec = first / Fat::SECTOR_SIZE;
    to_sec   = (last - 1)  / Fat::SECTOR_SIZE;

    // allocate new cluster if there is not enough
    if(to_sec >= (uint)cluster.getSize()){
        int tmp, prev, ret;
        tmp = (to_sec - cluster.getSize()) / kernel.fat.clustersize + 1;
        prev = cluster.getTop();
        ret = cluster.add(tmp);
        if(ret != tmp)
            return -FAT_ERROR_DISC_IS_FULL;

        if(prev != cluster.getTop()){
            entry.cluster = cluster.getTop();
            updateEntry();
        }
    }

    // set for first write
    p = (uchar *)buf;
    offset = first % Fat::SECTOR_SIZE; // first & 0x1ff 
    write_size = Fat::SECTOR_SIZE - offset;
    if(write_size > (uint)remain)
        write_size = remain;
    count = 0;

    buffer = cluster.seek(from_sec);
    for(uint i = from_sec; i <= to_sec; i++){

        if(buffer == NULL)
            return -FAT_ERROR_KERNEL_BUG; // impossible to come here

        // write to buffer
        memcpy(buffer + offset, p, write_size);
        count += write_size;

        // set for next write
        remain -= write_size;
        p += write_size;
        offset = 0;
        if( remain > Fat::SECTOR_SIZE)
            write_size = Fat::SECTOR_SIZE;
        else
            write_size = remain;

        buffer = cluster.next();
    }

    this->pos += count;
    if(this->size < this->pos) {
        this->size = this->pos;
        this->entry.size = this->size;
    }

    updateEntry();
    return count;
}

/******************************************************************************/
int FatFile::remove(){
    if(!this->exist) return -FAT_ERROR_NO_SUCH_FILE;
    
    if(this->isdir){
        uchar *p;
        FatEntry ent;
        bool isEmpty = true;

        if(cluster.isRoot())
            return -FAT_ERROR_INVALID_FILE_NAME;

        // check if this dir is empty
        for(p = cluster.seek(0); p != NULL; p = cluster.next()){
            for(int i = 0; i < Fat::SECTOR_SIZE / Fat::ENTRY_SIZE; i++) {
                kernel.fat.readEntry(&ent, i);
                if(ent.name[0] == '.'){
                    isEmpty = false;
                    break;
                }
            }
            
            if(isEmpty == false)
                break;
        }
        
        if(isEmpty == false)
            return -FAT_ERROR_DIRECTORY_NOT_EMPTY;
        
        //chdir("..");
    }
    
    memset(&this->entry, 0, Fat::ENTRY_SIZE);
    updateEntry();
    this->cluster.remove();

    return 0;
}

/******************************************************************************/
int FatFile::rename(File *dest){
    int ret;
    FatFile *dst = (FatFile *)dest;

    if(!this->exist) return -FAT_ERROR_NO_SUCH_FILE;
    if(dst->exist) return -FAT_ERROR_FILE_ALREADY_EXIST;

    // create as 'file' temporarily for allocating directory entry
    ret = dst->create(0, 0);
    if(ret < 0) return ret;

    dst->entry.attr = this->entry.attr;
    dst->entry.dummy = this->entry.ctimecs;
    dst->entry.ctime = this->entry.ctime;
    dst->entry.cdate = this->entry.cdate;
    dst->entry.adate = this->entry.adate;
    dst->entry.cluster2 = this->entry.cluster2;
    dst->entry.utime = this->entry.utime;
    dst->entry.cluster = this->entry.cluster;
    dst->entry.size = this->entry.size;

    dst->cluster = this->cluster;
    if(this->isdir)
        dst->isdir = 1;

    memset(&this->entry, 0, Fat::ENTRY_SIZE);
    this->updateEntry();
    dst->updateEntry();

    return 0;
}

/******************************************************************************/
void FatFile::printEntry(){
    int i;
    lcd_dprintf("name:");
    for(i = 0; i < 8; i++)
        lcd_dprintf("%c", entry.name[i]);

    lcd_dprintf("  ext:");
    for(i = 0; i < 3; i++)
        lcd_dprintf("%c", entry.ext[i]);

    lcd_dprintf("\n");
    
    lcd_dprintf("cluster %d, parent %d\n", this->entry.cluster, this->parent);

}

/******************************************************************************/
void FatFileName::clear(){
    int i;
    for (i = 0; i < 8; i++)
        name[i] = ' ';
    for (i = 0; i < 3; i++)
        ext[i] = ' ';
}

/******************************************************************************/
int FatFileName::set(const char *string){
    static char forbid_char[] = "\"\\[]:;|=, *?";
    int i;
    const char *p;

    if(string == NULL || string[0] == 0)
        return -1;

    // initialize
    clear();

    // check illegal character
    for(p = (char *)string; *p != 0; p++){
        for(i = 0; i < 12; i++)
            if(*p == forbid_char[i])
                return -1;
    }
    p = string;

    // check special file name "." and ".."
    if(*p == '.'){
        name[0] = '.';
        p++;
        if(*p == '.'){
            name[1] = '.';
            p++;
        }

        if(*p != 0) return -1;
        return 0;
    }

    // name
    for(i = 0; i < 8; i++){
        name[i] = toupper(*p);
        p++;

        if(*p == 0) return 0;
        if(*p == '.') break;
    }

    // pass to read '.'
    p++;

    // extension
    for(i = 0; i < 3; i++){
        if(*p == 0) return 0;
        if(*p == '.') return -1;

        ext[i] = toupper(*p);
        p++;
    }

    // long ext name error
    if(*p != 0) return -1;
    return 0;
}

/******************************************************************************/
int FatFileName::get(char *string){
    char *p1 = string;
    int i;

    // name
    for(i = 0; i < 8; i++)
        if(name[i] != ' ' && name[i] != 0)
            *p1++ = name[i];

    // in case that not existing ext
    if(ext[0] == ' ' || ext[0] == 0){
        *p1 = 0;
        return 0;
    }

    *p1++ = '.';

    // ext
    for(i = 0; i < 3; i++)
        if(ext[i] != ' ' && ext[i] != 0)
            *p1++ = ext[i];

    *p1 = 0;
    return 0;
}

/******************************************************************************/
