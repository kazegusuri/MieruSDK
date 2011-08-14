/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

#define READBITS(reg, pos) ((readreg(reg) >> pos ## _SH) & pos ## _MASK)
#define MODIFYBITS(reg, pos, code) modifyreg(reg, (code) << pos ## _SH,\
                                             pos ## _MASK << pos ## _SH)

/**********************************************************************/
enum {
    CP0_POLL_CYCLE = 0x100,
    EXC_HANDLE_BASE = 0x100,
    COMPARE_CONNECTED = 7,
};

/**********************************************************************/
MipsCp0Lite::MipsCp0Lite(BoardMP *board)
{
    this->board = board;
    this->chip = static_cast<ChipMP *>(board->chip);
    count = compare = sr = cause = epc = 0;
    lastcount = 0;
    timerint = 0;
    counter = 0;
}

/**********************************************************************/
void MipsCp0Lite::step()
{
    if ((++counter) < CP0_POLL_CYCLE)
        return;
    counter = 0;

    count = (uint032_t) (board->gettime() / 10);
    if ((int032_t) (count - compare) >= 0 &&
        (int032_t) (lastcount - compare) < 0)
        setinterrupt(COMPARE_CONNECTED);
}

/**********************************************************************/
uint032_t MipsCp0Lite::readreg(int x)
{
    if (x == CP0_COUNT___)
        return count;
    else if (x == CP0_COMPARE_)
        return compare;
    else if (x == CP0_SR______)
        return sr;
    else if (x == CP0_CAUSE___)
        return cause;
    else if (x == CP0_EPC_____)
        return epc;
    else
        return 0;
}

/**********************************************************************/
void MipsCp0Lite::writereg(int x, uint032_t value)
{
    if (x == CP0_COUNT___)
        count   = value;
    else if (x == CP0_COMPARE_)
        compare = value;
    else if (x == CP0_SR______)
        sr      = value;
    else if (x == CP0_CAUSE___)
        cause   = value;
    else if (x == CP0_EPC_____)
        epc     = value;
}

/**********************************************************************/
void MipsCp0Lite::modifyreg(int x, uint032_t value, uint032_t mask)
{
    writereg(x, (value & mask) | (readreg(x) & ~mask));
}

/**********************************************************************/
uint032_t MipsCp0Lite::doexception(int code, uint032_t epc,
                               uint032_t vaddr, int delay)
{
    if (code == EXC_INT____)
        clearinterrupt(COMPARE_CONNECTED);
    MODIFYBITS(CP0_CAUSE___, CAUSE_EXC, code);
    if (!READBITS(CP0_SR______, SR_EXL)) {
        writereg(CP0_EPC_____, (delay) ? epc - 4 : epc);
        MODIFYBITS(CP0_CAUSE___, CAUSE_BD, (delay) ? 1 : 0);
    } else board->simerror();
    MODIFYBITS(CP0_SR______, SR_EXL, 1);
    return EXC_HANDLE_BASE;
}

/**********************************************************************/
void MipsCp0Lite::setinterrupt(int num)
{
    timerint = 1;
}

/**********************************************************************/
void MipsCp0Lite::clearinterrupt(int num)
{
    timerint = 0;
}

/**********************************************************************/
int MipsCp0Lite::checkinterrupt()
{
    return (timerint &&
            READBITS(CP0_SR______, SR_EXL) == 0 &&
            READBITS(CP0_SR______, SR_IE) != 0);
}

/**********************************************************************/
void MipsCp0Lite::print()
{
    printf("COUNT    COMPARE  SR       EPC      CAUSE\n");
    printf("%08x %08x %08x %08x %08x \n\n",
           count, compare, sr, epc, cause);
}

/**********************************************************************/
