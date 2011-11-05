/*
 * Copyright (c) 2011 Masahiro Sano.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*! @file sandbox.cpp
 *@brief temporary test file
 *@author Masahiro Sano
 *@since 2011/11/04
 *@date 2011/11/04
 */

#include <kernel.h>
#include <mierulib.h>
#include <cache.h>
#include <task.h>

#include <list.hpp>
#include <initializer_list>

void process1(){
    for(;;){
        lcd_dprintf("process1\n");
        usleep(100000);
    }

    Kernel::getInstance()->taskmanager.switchContext(
        Kernel::getInstance()->taskmanager.getCurrentTask(), 
        &Kernel::getInstance()->taskmanager.tasks[2]);
    lcd_dprintf("process1\n");
    for(;;);
}

void process2(){
    for(;;){
        lcd_dprintf("process2\n");
        usleep(100000);
    }
    lcd_dprintf("process2\n");
    //usleep(1000000);
    Kernel::getInstance()->taskmanager.switchContext(
        Kernel::getInstance()->taskmanager.getCurrentTask(), 
        &Kernel::getInstance()->taskmanager.tasks[1]);
    lcd_dprintf("process2\n");

    for(;;);
}

void sandbox() {

    lcd_dprintf("aaa\n");
    mpc::list<int> a = {1, 2, 3, 4};
    for(auto it = a.begin(); it != a.end(); it++) {
        lcd_dprintf("%d ", *it);
    }
    lcd_dprintf("\n");

    for(;;);
    Kernel *kernel = Kernel::getInstance();
    // Task *current = kernel->taskmanager.getCurrentTask();
    // unsigned int gp;

    //MemoryAllocator::getInstance()->_unitTest();
    //for(;;);
    
    //ret = system::_syscall(0,104,111,1200,0xffff);
    //lcd_dprintf("ret %d\n", ret);
    //for(;;);

    Task *t1 = kernel->taskmanager.getTask();
    Task *t2 = kernel->taskmanager.getTask();

    uint sp1 = 0x50000 - 16;
    uint sp2 = 0x80000 - 16;

    // __asm__ volatile ("move %0, $gp;" : "=r" (gp));
    // lcd_dprintf("%d %d %08x\n", t1->pid, t2->pid, gp);

    t1->tss.sp = sp1;
    //t1->tss.gp = gp;
    t1->tss.ra = (uint)process1;
    t1->tss.cp0_status = 1;
    t1->tss.cp0_epc = 0;
    t1->tss.cp0_cause = 0;
    t1->stack_start = sp1;
    t1->stack_end   = 0x20000 + 16;
    t1->brk         = 0x20000 + 16;

    lcd_dprintf("%08x \n", t1->tss.ra);

    t2->tss.sp = sp2;
    //t2->tss.gp = gp;
    t2->tss.ra = (uint)process2;
    t2->tss.cp0_status = 1;
    t2->tss.cp0_epc = 0;
    t2->tss.cp0_cause = 0;
    t2->stack_start = sp2;
    t2->stack_end   = sp1+32;
    t2->brk         = sp1+32;

    invalidate_icache();
    invalidate_dcache();

    lcd_dprintf("start!!!\n");
    //taskmanager.switchContext(current, t1);
}
