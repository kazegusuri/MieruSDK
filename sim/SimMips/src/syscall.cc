/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define2.h"

/**********************************************************************/
void Mips::syscall()
{
    MipsCp0Lite *cp0 = static_cast<ChipMP *>(chip)->cp0;
    uint032_t vaddr;
    uint064_t paddr;
    switch (as->r[REG_V0]) {
    case SYS_EXIT:
        state = CPU_STOP;
        break;
    case SYS_WRITE:
        if (as->r[REG_A0] == STDOUT_FILENO) {
            uint008_t c = 0;
            for (uint i = 0; i < as->r[REG_A2]; i++) {
                vaddr = as->r[REG_A1];
                cp0->getphaddr(vaddr, &paddr, 0);
                mc->read((uint032_t)paddr + i, 1, &c);
                // mc->read(as->r[REG_A1] + i, 1, &c);
                if (!chip->issilent())
                    putchar((int) c);
            }
        }
        as->r[REG_V0] = as->r[REG_A2];
        as->r[REG_A3] = 0;
        break;
    case SYS_IOCTL:
        as->r[REG_V0] = as->r[REG_A3] = 0;
        break;
    default:
        if (!chip->issilent())
            printf("## unknown syscall #%d (see asm/unistd.h). Skip.\n",
                   as->r[REG_V0]);
        as->r[REG_V0] = as->r[REG_A3] = 0;
        break;
    }
}

/**********************************************************************/
