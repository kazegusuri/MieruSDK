/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

/************************************************************************/
MainMemory::MainMemory(uint032_t mem_size)
{
    npage = (mem_size + MM_PAGE_SIZE - 1) / MM_PAGE_SIZE;
    mem_size = npage * MM_PAGE_SIZE;
    pagetable = new uint032_t*[npage];
    external = new int[npage];
    for (uint i = 0; i < npage; i++) {
        pagetable[i] = NULL;
        external[i] = 0;
    }
}

/************************************************************************/
MainMemory::~MainMemory()
{
    for (uint i = 0; i < npage; i++)
        if (!external[i]) {
            DELETE_ARRAY(pagetable[i]);
        }
    DELETE_ARRAY(pagetable);
    DELETE_ARRAY(external);
}

/************************************************************************/
uint032_t* MainMemory::setpageentry(uint032_t addr, uint032_t *array)
{
    if (addr >= mem_size)
        return NULL;
    if (!external[addr / MM_PAGE_SIZE])
        DELETE_ARRAY(pagetable[addr / MM_PAGE_SIZE]);
    pagetable[addr / MM_PAGE_SIZE] = array;
    external[addr / MM_PAGE_SIZE] = 1;
    return array;
}

/************************************************************************/
inline uint032_t* MainMemory::newpage(uint032_t addr)
{
    uint032_t *page = new uint032_t[MM_PAGE_SIZE / sizeof(uint032_t)];
    // for (uint i = 0; i < MM_PAGE_SIZE / sizeof(uint032_t); i++)
    //     page[i] = 0;
    pagetable[addr / MM_PAGE_SIZE] = page;
    return page;
}

/************************************************************************/
inline uint032_t* MainMemory::getrealaddr(uint032_t addr)
{
    uint032_t *page = pagetable[addr / MM_PAGE_SIZE];
    if (page == NULL)
        page = newpage(addr);
    page += (addr % MM_PAGE_SIZE) / sizeof(uint032_t);
    return page;
}

/************************************************************************/
int MainMemory::read(uint032_t addr, uint size, void *data)
{
    if (size == 1)
        read1b(addr, (uint008_t *) data);
    else if (size == 2)
        read2b(addr, (uint016_t *) data);
    else if (size == 4)
        read4b(addr, (uint032_t *) data);
    else if (size == 8)
        read8b(addr, (uint064_t *) data);
    else
        return 1;
    return 0;
}

/************************************************************************/
void MainMemory::read1b(uint032_t addr, uint008_t *data)
{
    uint032_t temp;
    int offset = (addr & 0x3) * 8;
    read4b(addr, &temp);
    *data = (temp >> offset) & 0xff;
}

/************************************************************************/
void MainMemory::read2b(uint032_t addr, uint016_t *data)
{
    uint032_t temp;
    int offset = (addr & 0x2) * 8;
    read4b(addr, &temp);
    *data = (temp >> offset) & 0xffff;
}

/************************************************************************/
void MainMemory::read4b(uint032_t addr, uint032_t *data)
{
    uint032_t *mem = getrealaddr(addr);
    *data = *mem;
}

/************************************************************************/
void MainMemory::read8b(uint032_t addr, uint064_t *data)
{
    addr = addr & ~0x7;
    uint032_t temp_h, temp_l;
    read4b(addr, &temp_l);
    read4b(addr + 4, &temp_h);
    *data = ((uint064_t) temp_h << 32) | temp_l;
}

/************************************************************************/
int MainMemory::write(uint032_t addr, uint size, uint064_t data)
{
    if (size == 1)
        write1b(addr, (uint008_t) data);
    else if (size == 2)
        write2b(addr, (uint016_t) data);
    else if (size == 4)
        write4b(addr, (uint032_t) data);
    else if (size == 8)
        write8b(addr, data);
    else
        return 1;
    return 0;
}

/************************************************************************/
void MainMemory::write1b(uint032_t addr, uint008_t data)
{
    int offset = (addr & 0x3) * 8;
    uint032_t ins = (uint032_t) data << offset;
    uint032_t mask = 0xff << offset;
    uint032_t temp;
    read4b(addr, &temp);
    temp = ins | (temp & ~mask);
    write4b(addr, temp);
}

/************************************************************************/
void MainMemory::write2b(uint032_t addr, uint016_t data)
{
    int offset = (addr & 0x2) * 8;
    uint032_t ins = (uint032_t) data << offset;
    uint032_t mask = 0xffff << offset;
    uint032_t temp;
    read4b(addr, &temp);
    temp = ins | (temp & ~mask);
    write4b(addr, temp);
}

/************************************************************************/
void MainMemory::write4b(uint032_t addr, uint032_t data)
{
    uint032_t *mem = getrealaddr(addr);
    *mem = data;
}

/************************************************************************/
void MainMemory::write8b(uint032_t addr, uint064_t data)
{
    addr = addr & ~0x7;
    write4b(addr, (uint032_t) data);
    write4b(addr + 4, (uint032_t) (data >> 32));
}

/************************************************************************/
void MainMemory::print()
{
    uint032_t *page;
    for (uint i = 0; i < npage; i++) {
        page = pagetable[i];
        if (page != NULL) {
            printf("[MEMORY BLOCK: 0x%08x]", i);
            for (uint j = 0; j < MM_PAGE_SIZE / sizeof(uint032_t); j++) {
                if ((j % 4) == 0)
                    printf("\n%08x: ", (uint)
                           (i * MM_PAGE_SIZE + j * sizeof(uint032_t)));
                printf("%02x%02x%02x%02x ",
                       page[j] & 0xff , (page[j] >> 8) & 0xff,
                       (page[j] >> 16) & 0xff , (page[j] >> 24) & 0xff);
            }
            printf("\n\n");
        }
    }
}
/************************************************************************/
MemoryMap::MemoryMap()
{
    addr = size = 0;
    dev = NULL;
    next = NULL;
}

/************************************************************************/
MemoryMap::~MemoryMap()
{
    DELETE(dev);
    DELETE(next);
}

/************************************************************************/
MemoryController::MemoryController(MemoryMap *mmap)
{
    this->mmap = mmap;
    this->mode = mode;
}

/************************************************************************/
McInst::McInst()
{
    dev = NULL;
}

/************************************************************************/
int MemoryController::read(uint064_t addr, uint size, void *data)
{
    for (MemoryMap *cur = mmap; cur; cur = cur->next) {
        if (addr - cur->addr >= cur->size)
            continue;
        uint032_t devaddr = (uint032_t) addr - cur->addr;
        return cur->dev->read(devaddr, size, data);
    }
    return 1;
}

/************************************************************************/
int MemoryController::write(uint064_t addr, uint size, uint064_t data)
{
    MemoryMap *cur;

    for (cur = mmap; cur; cur = cur->next) {
        if (addr - cur->addr < cur->size)
            break;
    }
    if (!cur)
        return 1;

    for (int i = 0; i < NUM_MCINST; i++) {
        if (!inst[i].dev) {
            inst[i].dev = cur->dev;
            inst[i].addr = addr - cur->addr;
            inst[i].size = size;
            inst[i].data = data;
            return 0;
        }    
    }
    
    fprintf(stderr, "!! MEMORY CONTROLLER QUEUE IS FULL");
    return 1;
}

/************************************************************************/
void MemoryController::update()
{
    for (int i = 0; i < NUM_MCINST; i++) {
        if (inst[i].dev) {
            inst[i].dev->write(inst[i].addr, inst[i].size, inst[i].data);
            inst[i].dev = NULL;
        }
    }
}
/************************************************************************/
void MemoryController::print()
{
    for (MemoryMap *cur = mmap; cur != NULL; cur = cur->next)
        cur->dev->print();
}

/************************************************************************/
