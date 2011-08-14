/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

/**********************************************************************/
void BoardFP::setupchip()
{
    chip = new ChipFP(this);
    chip->init();
}

/**********************************************************************/
void BoardFP::versioninfo()
{
    Board::versioninfo();
    printf("## - %s %s\n", L_NAME_FP, L_VER_FP);    
}

/**********************************************************************/
void ChipFP::init()
{
    mc = new MemoryController(mmap);
    mips = new MipsFP(static_cast<BoardFP *>(board), this);
    mips->init();
}

/**********************************************************************/
