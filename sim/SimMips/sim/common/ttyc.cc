/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "ttyc.h"

ttyControl::ttyControl()
{
    struct termios newterm;
    int newfflg;

    tcgetattr(STDIN_FILENO, &oldterm);
    oldfflg = fcntl(STDIN_FILENO, F_GETFL);

    if ((oldterm.c_lflag & (ICANON | ECHO)) == 0) {
        valid = 0;
        return;
    }

    newterm = oldterm;
    newterm.c_lflag &= ~(ICANON | ECHO);
    newterm.c_cc[VMIN] = 0;
    newterm.c_cc[VTIME] = 0;
    newfflg = oldfflg | O_NONBLOCK;

    tcsetattr(STDIN_FILENO, TCSANOW, &newterm);
    fcntl(STDIN_FILENO, F_SETFL, newfflg);
    valid = 1;
}

/**********************************************************************/
ttyControl::~ttyControl()
{
    if (valid) {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);
        fcntl(STDIN_FILENO, F_SETFL, oldfflg);
    }
}

/**********************************************************************/
