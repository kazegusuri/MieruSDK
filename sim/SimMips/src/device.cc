/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

/**********************************************************************/
enum {
    MIERU_KEY         = 0x00,
    MIERU_LCD         = 0x04,
    MIERU_MMC_CONTROL = 0x08,
    MIERU_CNT         = 0x0c,
    MIERU_OLDKEY      = 0x10,

    MIERU_POLL_CYCLE  = 0x1000,
};

/**********************************************************************/
void MieruIO::init(Board *board)
{
    unsigned int i;
    
    this->board = board;
    for (MemoryMap *temp = board->mmap; temp != NULL; temp = temp->next)
        if ((mmccon = dynamic_cast<MMCController *>(temp->dev)) != NULL)
            break;
    dispcon = new DisplayController();
    dispcon->init(board);

    counter = 0;
    cntdiv = 1000;
    keystate = 0;
    for (i = 0; i < MIERU_NUM_KEYBUF; i++)
        kbbuf[i] = 0;

    BoardMP *b = dynamic_cast<BoardMP *>(board);
    if (b)
        if (b->lcd_type == 0)
            cntdiv = 10;
}

/**********************************************************************/
MieruIO::~MieruIO()
{
    DELETE(dispcon);
}

/**********************************************************************/
void MieruIO::update()
{
    if (++counter < MIERU_POLL_CYCLE)
        return;
    counter = 0;
    getasciicode();
}

/**********************************************************************/
void MieruIO::getasciicode()
{
    int alt = 0, ctrl = 0, shift = 0;
    int i;
    char swbuf = 0;
    char *p;
    char key_arrow[5] = "DACB";
    char key_special[33] = " $-.#!\"    pqrst uvwxy z{       ";
    char key_sh_num[11] = ")!@#$%^&*(";
    char key_sp1[17] = ";=,-./``:+<_>?~~";
    char key_sp2[9] = "[\\]'{|}\"";

    if (::read(STDIN_FILENO, &swbuf, sizeof(swbuf)) == 0)
        return;
    if (!swbuf)
        return;

    // Standard Input -> MieruPC Keycode
    if (swbuf == 0x1b) {
        swbuf = (keystate == 0x1b) ? 0x1b : 0;
        keystate = 0x1b;
    } else if ((keystate == 0x1b) && (swbuf == 0x5b)) {
        keystate = 0x5b;
        swbuf = 0;
    } else if (keystate == 0x5b) {
        if ((p = strchr(key_arrow, (int) swbuf)) != NULL) {
            keystate = 0;
            swbuf = 0x25 + (int) (p - key_arrow);
        } else if ((swbuf >= '0') && (swbuf <= '9')) {
            keystate = 0x80 + swbuf - '0';
            swbuf = 0;
        } else {
            keystate = 0;
        }
    } else if (keystate >= 0x80) {
        if ((swbuf >= '0') && (swbuf <= '9')) {
            keystate = 0x80 + ((keystate - 0x80) % 3) * 10 + (swbuf - '0');
            swbuf = 0;
        } else {
            swbuf = key_special[keystate - 0x80];
            keystate = 0;
        }
    } else {
        alt = (keystate == 0x1b);
        keystate = 0;
        if ((swbuf < 0x20) && (swbuf != 0x09) && (swbuf != 0x0d)) {
            ctrl = 1;
            swbuf += 0x40;
            if(swbuf == 'J'){
                ctrl = 0;
                swbuf = 0x0d;
            }
        } else if ((swbuf >= 'A') && (swbuf <= 'Z')) {
            shift = 1;
        } else if ((swbuf >= 'a') && (swbuf <= 'z')) {
            swbuf -= 0x20;
        } else if ((p = strchr(key_sh_num, (int) swbuf)) != NULL) {
            shift = 1;
            swbuf = '0' + (int) (p - key_sh_num);
        } else if ((p = strchr(key_sp1, (int) swbuf)) != NULL) {
            shift = (((int) (p - key_sp1)) >= 8);
            swbuf = 0xba + ((int) (p - key_sp1) & 0x07);
        } else if ((p = strchr(key_sp2, (int) swbuf)) != NULL) {
            shift = (((int) (p - key_sp2)) >= 4);
            swbuf = 0xdb + ((int) (p - key_sp2) & 0x03);
        } else if (swbuf == 0x7f) {
            swbuf = 0x08;
        }
    }
    
    // Enqueue Key Buffer
    if (swbuf) {
        for (i = 0; kbbuf[i] && i < MIERU_NUM_KEYBUF - 1; i++);
        if (i > MIERU_NUM_KEYBUF - (alt + ctrl + shift + 1) * 2)
            return;
        if (alt)
            kbbuf[i++] = 0xa4;
        if (ctrl)
            kbbuf[i++] = 0xa2;
        if (shift)
            kbbuf[i++] = 0xa0;
        kbbuf[i++] = (swbuf & 0xff);
        kbbuf[i++] = (swbuf & 0xff) << 8;
        if (shift)
            kbbuf[i++] = 0xa000;
        if (ctrl)
            kbbuf[i++] = 0xa200;
        if (alt)
            kbbuf[i++] = 0xa400;
    }
}

/**********************************************************************/
int MieruIO::read(uint032_t addr, uint size, void *data)
{
    if (size == 1)
        read1b(addr, (uint008_t *) data);
    else if (size == 4)
        read4b(addr, (uint032_t *) data);
    return 0;
}

/**********************************************************************/
int MieruIO::write(uint032_t addr, uint size, uint064_t data)
{
    if (size == 1)
        write1b(addr, (uint008_t) data);
    else if (size == 4)
        write4b(addr, (uint032_t) data);
    return 0;
}

/**********************************************************************/
void MieruIO::read1b(uint032_t addr, uint008_t *data)
{
    if (addr == MIERU_LCD) {
        *data = 1;
    } else if (addr == MIERU_KEY) {
        uint032_t keybuf;
        read4b(addr, &keybuf);
        *data = keybuf & 0xff;
    } else if (addr == MIERU_OLDKEY) {
        *data = 0;
        int i;
        for (i = 0; i < MIERU_NUM_KEYBUF; i++) {
            if (!kbbuf[i])
                break;
            char swbuf = kbbuf[i] & 0xff;
            kbbuf[i] = 0;
            *data |= ((swbuf == 'W') ? 32 :
                      (swbuf == 'S') ? 16 :
                      (swbuf == 'A') ? 8 :
                      (swbuf == 'D') ? 4 :
                      (swbuf == 'J') ? 2 :
                      (swbuf == 'K') ? 1 : 0);
        }
    } else {
        *data = 0;
    }
}

/**********************************************************************/
void MieruIO::read4b(uint032_t addr, uint032_t *data)
{
    if (addr == MIERU_CNT) {
        *data = (uint032_t) (board->gettime() / cntdiv);
    } else if (addr == MIERU_KEY) {
        *data = kbbuf[0];
        for (int i = 0; i <  MIERU_NUM_KEYBUF - 1; i++)
            kbbuf[i] = kbbuf[i + 1];
        kbbuf[MIERU_NUM_KEYBUF - 1] = 0;
    } else if (addr == MIERU_MMC_CONTROL && mmccon) {
        *data = mmccon->getstate();
    } else {
        *data = 0;
    }
}

/**********************************************************************/
void MieruIO::write4b(uint032_t addr, uint032_t data)
{
    if (addr == MIERU_MMC_CONTROL && mmccon)
        mmccon->setaddr(data);
}

/**********************************************************************/
void MieruIO::write1b(uint032_t addr, uint008_t data)
{
    if (addr == MIERU_LCD)
        dispcon->write(0, 1, data);
}

/************************************************************************/
MultiMediaCard::MultiMediaCard(char *filename, uint032_t size)
{
    FILE *fp = NULL;
    char *fullname = NULL;
    char *name;
    uint032_t fsize, ret;
    int len;

    this->size = size;
    this->mem = new uint008_t[size];

    // get the real path of the image file
    if (filename){
        fullname = new char[PATH_MAX + 1];
        if (!realpath(filename, fullname)) {  
            fprintf(stderr, "## Warning: can't find mmc file: %s\n",
                    filename);
            fullname = NULL;
        }
    }

    // set the name of the image file to be written
    if (fullname) {
        len = strlen(fullname);
        name = new char[len + 2];
        strcpy(name, fullname);
        name[len] = '_';
        name[len + 1] = '\0';
        this->filename = name;
    } else {
        this->filename = "_mmc_result";
    }

    // read the image file
    if (fullname) {
        fp = fopen(fullname, "rb");
        if (!fp) {
            fprintf(stderr, "## Warning: can't open mmc file: %s\n",
                    filename);
        } else {
            // get file size
            fseek(fp, 0, SEEK_END); 
            fsize = ftell(fp); 
            fseek(fp, 0, SEEK_SET);
            if(size < fsize)
                size = fsize;
            
            ret = fread(this->mem, sizeof(uint008_t), fsize, fp);
            if (ret != fsize)
                fprintf(stderr, "## Warning: can't read mmc file: %s\n",
                        filename);
            fclose(fp);
        }
    }
    DELETE(fullname);
}

/************************************************************************/
MultiMediaCard::~MultiMediaCard()
{
    DELETE_ARRAY(filename);
    DELETE_ARRAY(mem);
}

/************************************************************************/
int MultiMediaCard::readblock(uint032_t addr, uint008_t *data)
{
    if(addr >= this->size)
        return -1;
    addr = addr & ~(MMC_BLOCK_SIZE - 1);
    memcpy(data, this->mem + addr, MMC_BLOCK_SIZE);
    return 0;
}

/************************************************************************/
int MultiMediaCard::writeblock(uint032_t addr, uint008_t *data)
{
    if(addr >= this->size)
        return -1;
    addr = addr & ~(MMC_BLOCK_SIZE - 1);
    memcpy(this->mem + addr, data, MMC_BLOCK_SIZE);
    return 0;
}

/************************************************************************/
int MultiMediaCard::writeback()
{
    if (!filename)
        return 0;

    FILE *fp;
    int ret;
    
    if((fp = fopen(filename, "wb")) == NULL){
        fprintf(stderr, "## can't open file: %s\n", filename);
        return -1;
    }
    ret = fwrite(mem, size, 1, fp);
    fclose(fp);

    return ret;
}

/************************************************************************/
void MMCController::init(Board *board)
{
    this->board = board;
    mmc = static_cast<BoardMP *>(board)->mmc;
    addr = lastaddr = 0;
    ready = 0;
    if (mmc) {
        mmc->readblock(0, cache);
        ready = 1;
    }
}

/************************************************************************/
void MMCController::update()
{
    if (!ready || !mmc)
        return;

    if (addr != lastaddr) {
        mmc->writeblock(lastaddr, cache);
        mmc->readblock(addr, cache);
        lastaddr = addr;
    }
}

/************************************************************************/
void MMCController::setaddr(uint032_t newaddr)
{
    addr = newaddr & ~(MMC_BLOCK_SIZE - 1);
}

/************************************************************************/
uint032_t MMCController::getstate()
{
    return addr | ready;
}

/************************************************************************/
int MMCController::read(uint032_t addr, uint size, void *data)
{
    uint064_t temp = 0;
    if (size > 8 || addr + size > MMC_BLOCK_SIZE)
        return 1;

    if (!ready) {
        temp = 0;
    } else {
        for (uint i = 0; i < size; i++)
            temp = (temp << 8) | cache[addr + i];
    }
    if (size == 1)
        *(uint008_t *) data = (uint008_t) temp;
    else if (size == 2)
        *(uint016_t *) data = (uint016_t) temp;
    else if (size == 4)
        *(uint032_t *) data = (uint032_t) temp;
    else if (size == 8)
        *(uint064_t *) data = temp;
    return 0;
}

/************************************************************************/
int MMCController::write(uint032_t addr, uint size, uint064_t data)
{
    if (size > 8 || addr + size > MMC_BLOCK_SIZE)
        return 1;
    if (!ready)
        return 0;

    for (uint i = 0; i < size; i++) {
        cache[addr + i] = data & 0xff;
        data >>= 8;
    }
    return 0;
}

/************************************************************************/
