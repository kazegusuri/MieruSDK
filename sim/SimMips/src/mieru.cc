/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define2.h"

/**********************************************************************/
BoardMP::BoardMP()
{
    lcd_path = NULL;
    lcd_type = 1;
    mmc = NULL;
    ttyc = NULL;

}

/**********************************************************************/
BoardMP::~BoardMP()
{
    DELETE(mmc);
    DELETE(ttyc);
}

/**********************************************************************/
int BoardMP::init()
{
    if (Board::init())
        return 1;
    ttyc = new ttyControl();
    return 0;
}

/**********************************************************************/
void BoardMP::setupchip()
{
    chip = new ChipMP(this);
    chip->init();
}

/**********************************************************************/
void BoardMP::usage()
{
    Board::usage();
    char usagemessage[] = "\
 [MieruPC Mode Option]\n\
  -l [name]: specify the LCD Emulator Pipe\n\
  -t[num]  : specify the type of LCD (0: old, 1: new(default))\n\
\n";
    printf("%s", usagemessage);
}

/**********************************************************************/
int BoardMP::checkopt(char **argv, int *index)
{
    char *opt = argv[*index];
    if (opt[1] == 'l') {
        if (lcd_path) {
            fprintf(stderr, "## multiple -l options\n");
            return 1;
        }
        if ((lcd_path = argv[++*index]) == NULL) {
            fprintf(stderr, "## -l option: no file specified\n");
            return 1;
        }
        return 0;
    } else if (opt[1] == 't') {
        lcd_type = (opt[2] != '0');
        return 0;
    }
    return Board::checkopt(argv, index);
}

/**********************************************************************/
int BoardMP::parsememfile(MemoryMap **now, char *memfile,
                           char *line, int linecnt)
{
    uint032_t size;
    char *endptr;
    char suffix, *p;

    if (strncmp(line, "@mmc", 4) == 0){
        if (!now)
            return 0;
        size = strtoul(&line[5], &endptr, 10);
        suffix = endptr[0];
        p = &endptr[0];
        if (suffix == 'G' || suffix == 'g'){
            size *= 1024*1024*1024;
            p++;
        }else if (suffix == 'M' || suffix == 'm'){
            size *= 1024*1024;
            p++;
        }else if (suffix == 'K' || suffix == 'k'){
            size *= 1024;
            p++;
        }
        
        while (*p == ' ') p++;
        if (*p == 0)
            p = NULL;
        
        if (!mmc && size > 0){
            mmc = new MultiMediaCard(p, size);
        }
        return 0;
    }
    return Board::parsememfile(now, memfile, line, linecnt);
}

/**********************************************************************/
MMDevice *BoardMP::setmmdevice(char *devname, uint032_t size)
{
    if (strcmp(devname, "MIERU_IO") == 0)
        return new MieruIO();
    if (strcmp(devname, "MMC_IO") == 0)
        return new MMCController();
    return Board::setmmdevice(devname, size);
}

/**********************************************************************/
void BoardMP::versioninfo()
{
    Board::versioninfo();
    printf("## - %s %s\n", L_NAME_MP, L_VER_MP);    
}

/**********************************************************************/
void BoardMP::print()
{
    if (debug_state) {
        chip->mips->as->print();
        static_cast<ChipMP *>(chip)->cp0->print();
        chip->mc->print();
    }
    if (imix_mode)
        chip->mips->ss->print();
}

/**********************************************************************/
void ChipMP::init()
{
    mc = new MemoryController(mmap);
    cp0 = new MipsCp0Lite(static_cast<BoardMP *>(board));
    mips = new MipsMP(static_cast<BoardMP *>(board), this);
    mips->init();
}

/**********************************************************************/
void ChipMP::drive()
{
    mips->drive();
    for (MemoryMap *temp = mmap; temp != NULL; temp = temp->next)
        temp->dev->drive();
    mc->drive();
}

/**********************************************************************/
void ChipMP::update()
{
    mips->update();
    cp0->step();
    for (MemoryMap *temp = mmap; temp != NULL; temp = temp->next)
        temp->dev->update();
    mc->update();
    cycle++;
}

/**********************************************************************/
MipsMP::MipsMP(BoardMP *board, ChipMP *chip) : Mips(board, chip)
{
    this->cp0 = chip->cp0;
    exc_occur = 0;
}

/**********************************************************************/
void MipsMP::drive()
{
    if (cp0->checkinterrupt()) {
        exception(EXC_INT____);
        return;
    }
    Mips::drive();
}

/**********************************************************************/
void MipsMP::execute()
{
    Mips::execute();
    if (inst->op == MFC0_____)
        inst->rslt32 = cp0->readreg(inst->rd);
}

/**********************************************************************/
void MipsMP::writeback()
{
    Mips::writeback();
    if (inst->op == MTC0_____)
        cp0->writereg(inst->rd, inst->rslt32);
}

/**********************************************************************/
void MipsMP::setnpc()
{
    if (exc_occur) {
        as->pc = cp0->doexception(exc_code, as->pc, 
                                  inst->vaddr, (as->delay_npc) ? 1 : 0);
        as->delay_npc = 0;
        exc_occur = 0;
    } else {
        Mips::setnpc();
    }
    if (inst->op == ERET_____) {
        as->pc = cp0->readreg(CP0_EPC_____);
        if (!as->pc) {
            printf("!! Branch to zero. stop.\n");
            board->simerror();
        }
        cp0->modifyreg(CP0_SR______, 0, 0x2);
    }
}

/**********************************************************************/
void MipsMP::syscall()
{
    if (as->r[REG_V0] < 4000) {
        exception(EXC_SYSCALL);
    } else if (as->r[REG_V0] == SYS_MMCSYNC) {
        static_cast<BoardMP *>(board)->mmc->writeback();
        as->r[REG_V0] = 0;        
    } else {
        Mips::syscall();
    }        
}

/**********************************************************************/
void MipsMP::exception(int code)
{
    if (exc_occur)
        return;
    exc_occur = 1;
    exc_code = code;
    inst->clear();
}

/**********************************************************************/
