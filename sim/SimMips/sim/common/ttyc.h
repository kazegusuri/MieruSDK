/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

class ttyControl {
 private:
    struct termios oldterm;
    int oldfflg;
    int valid;
 public:
    ttyControl();
    ~ttyControl();
};

/**********************************************************************/
