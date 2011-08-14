/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

/**********************************************************************/
void BoardMC::setupchip()
{
    chip = new ChipMC(this);
    chip->init();
}

/**********************************************************************/
void BoardMC::versioninfo()
{
    Board::versioninfo();
    printf("## - %s %s\n", L_NAME_MC, L_VER_MC);    
}

/**********************************************************************/
void ChipMC::init()
{
    mc = new MemoryController(mmap);
    mips = new MipsMC(static_cast<BoardMC *>(board), this);
    mips->init();
}

/**********************************************************************/
void MipsMC::drive()
{
    if        (state == CPU_IF) {
        fetch();
    } else if (state == CPU_ID) {
        decode();
    } else if (state == CPU_RF) {
        regfetch();
    } else if (state == CPU_EX) {
        execute();
    } else if (state == CPU_MS) {
        memsend();
    } else if (state == CPU_MR) {
        memreceive();
    } else if (state == CPU_WB) {
        writeback();
        setnpc();
    }
}

/**********************************************************************/
void MipsMC::update()
{
    if (!running())
        return;
    else if (state == CPU_EX)
        state = (inst->attr & LOADSTORE) ? CPU_MS : CPU_WB;
    else if (state == CPU_WB)
        state = CPU_IF;
    else
        state++;
}

/**********************************************************************/
