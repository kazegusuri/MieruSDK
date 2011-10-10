/*
 * Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.
 * All rights reserved. 
 * Copyright (c) 2011 sabottenda.
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

/*! @file timer.cpp
 *@brief Timer handler.
 *@author sabottenda
 *@date 2011/09/23
 */

#include <mierulib.h>
#include <kernel.h>
#include <syscall.h>
#include <timer.h>
#include <exception.h>

void disable_global_interupt(){
    __asm__ volatile (
        "mfc0    $t0, $12\n"
        "addiu   $t1, $zero, -2\n"
        "and     $t0, $t0, $1\n"
        "mtc0    $t0, $12\n"
        );
}

void enable_global_interupt(){
    __asm__ volatile (
        "mfc0    $t0, $12\n"
        "ori     $t0, $t0, 1\n"
        "mtc0    $t0, $12\n"
        );
}

void init_timer(){
    uint *timer_handler= (uint *)TIMER_HANDLER_ADDR;
    lcd_dprintf("timer %08x\n", timer_handler);
    *timer_handler = (uint)timer_exception;
    //void (*timer_handler)() = (void (*)())TIMER_HANDLER_ADDR;

    lcd_dprintf("timer %08x %08x\n", *timer_handler, timer_exception);
    
    //*timer_handler = 100;
    //*timer_handler = timer_exception;
    //void **table = (void **)syscall_table;

}

extern "C" void timer_exception(){
    disable_global_interupt();
    int pid = Kernel::getInstance()->taskmanager.getCurrentTask()->pid;
    //lcd_dprintf("timer start pid %d\n", pid);
    uint epc;
    // __asm__ volatile (
    //     "mfc0 $v1, $14 \n"
    //     "move %0, $v1 \n"
    //     : "=&r" (epc)
    //     );
    //lcd_dprintf("epc %08x\n", epc);

    if(pid == 0){
        Kernel::getInstance()->taskmanager.switchContext(
            Kernel::getInstance()->taskmanager.getCurrentTask(), 
            &Kernel::getInstance()->taskmanager.tasks[1]);

    }else if(pid == 1){
        Kernel::getInstance()->taskmanager.switchContext(
            Kernel::getInstance()->taskmanager.getCurrentTask(), 
            &Kernel::getInstance()->taskmanager.tasks[2]);

    }else if(pid == 2){
        Kernel::getInstance()->taskmanager.switchContext(
            Kernel::getInstance()->taskmanager.getCurrentTask(), 
            &Kernel::getInstance()->taskmanager.tasks[1]);
    }

    //lcd_dprintf("timer end pid %d\n", pid);
    enable_global_interupt();
}
