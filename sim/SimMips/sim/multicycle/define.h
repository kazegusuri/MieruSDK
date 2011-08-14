/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "../core/define.h"
#define L_NAME_MC "Multi Cycle Extension"
#define L_VER_MC  "2010-05-20"

/**********************************************************************/
enum {
    CPU_IF = 1,
    CPU_ID = 2,
    CPU_RF = 3,
    CPU_EX = 4,
    CPU_MS = 5,
    CPU_MR = 6,
    CPU_WB = 7,
};

/**********************************************************************/
class BoardMC : public Board {
 protected:
    virtual void setupchip();
 public:
    virtual void versioninfo();
};

/**********************************************************************/
class ChipMC : public Chip {
 public:
    ChipMC(BoardMC *board) : Chip(board) {}
    virtual void init();
};

/**********************************************************************/
class MipsMC : public Mips {
 public:
    MipsMC(BoardMC *board, ChipMC *chip) : Mips(board, chip) {}
    virtual void drive();
    virtual void update();
};

/**********************************************************************/
