/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

/**********************************************************************/
int main(int argc, char *argv[])
{
    BoardMP *board = new BoardMP();
    board->versioninfo();

    // execute if successfully initialized
    if (board->siminit(argc, argv) == 0)
        board->exec();
    
    DELETE(board);
    return 0;
}

/**********************************************************************/
