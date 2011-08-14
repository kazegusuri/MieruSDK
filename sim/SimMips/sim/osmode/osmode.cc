/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

enum {
    SR_DEF       = 0x10000000,
    PAGEMASK_DEF = 0x00001fff,
    PRID_DEF     = 0x00018001,
    CONFIG_DEF   = 0x80000082,
    CONFIG1_DEF  = 0x1ed96c80,
};

/**********************************************************************/
BoardCZ::BoardCZ()
{
    debug_exctlb = 0;
    ttyc = NULL;
}

/**********************************************************************/
BoardCZ::~BoardCZ()
{
    DELETE(ttyc);
}

/**********************************************************************/
int BoardCZ::init()
{
    // set registers' default value
    if (Board::init())
        return 1;
    chip->mips->as->r[REG_T9] = 0;

    MipsCp0 *cp0 = static_cast<ChipCZ *>(chip)->cp0;
    cp0->writereg(CP0_SR______, SR_DEF);
    cp0->writereg(CP0_PAGEMASK, PAGEMASK_DEF);
    cp0->writereg(CP0_PRID____, PRID_DEF);
    cp0->writereg(CP0_CONFIG__, CONFIG_DEF);
    cp0->writereg(CP0_CONFIG1_, CONFIG1_DEF);

    // check ELF file address
    for (int i = 0; i < ld->memtabnum; i++) {
        if ((ld->memtab[i].addr <  KSEG0_MIN) ||
            (ld->memtab[i].addr >= KSEG2_MIN)) {
            printf("## ERROR: load to unmapped segment: 0x%08x\n",
                   ld->memtab[i].addr);
            return 1;
        }
        ld->memtab[i].addr &= UNMAP_MASK;
    }
    ttyc = new ttyControl();
    return 0;
}

/**********************************************************************/
void BoardCZ::setupchip()
{
    chip = new ChipCZ(this);
    chip->init();
}

/**********************************************************************/
void BoardCZ::usage()
{
    Board::usage();
    char usagemessage[] = "\
 [OS Mode Option]\n\
  -d4: debug mode 4: put the trace of exception and TLB change\n\
\n";
    printf("%s", usagemessage);
}

/**********************************************************************/
int BoardCZ::checkopt(char **argv, int *index)
{
    char *opt = argv[*index];
    if (opt[1] == 'd' && opt[2] == '4') {
        debug_exctlb = 1;
        printf("## debug mode 4: exception and TLB trace.\n");
        return 0;
    }
    return Board::checkopt(argv, index);
}

/**********************************************************************/
MMDevice *BoardCZ::setmmdevice(char *devname, uint032_t size)
{
    if (strcmp(devname, "ISA_IO") == 0)
        return new IsaIO();
    if (strcmp(devname, "ISA_BUS") == 0)
        return new MMDevice();
    return Board::setmmdevice(devname, size);
}

/**********************************************************************/
void BoardCZ::versioninfo()
{
    Board::versioninfo();
    printf("## - %s %s\n", L_NAME_CZ, L_VER_CZ);    
}

/**********************************************************************/
void BoardCZ::print()
{
    if (debug_state) {
        chip->mips->as->print();
        static_cast<ChipCZ *>(chip)->cp0->print();
        chip->mc->print();
    }
    if (imix_mode)
        chip->mips->ss->print();
}

/**********************************************************************/
void ChipCZ::init()
{
    mc = new MemoryController(mmap);
    cp0 = new MipsCp0(static_cast<BoardCZ *>(board), 1);
    mips = new MipsCZ(static_cast<BoardCZ *>(board), this);
    mips->init();
}

/**********************************************************************/
void ChipCZ::drive()
{
    mips->drive();
    for (MemoryMap *temp = mmap; temp != NULL; temp = temp->next)
        temp->dev->drive();
    mc->drive();
}

/**********************************************************************/
void ChipCZ::update()
{
    mips->update();
    cp0->step();
    for (MemoryMap *temp = mmap; temp != NULL; temp = temp->next)
        temp->dev->update();
    mc->update();
    cycle++;
}

/**********************************************************************/
