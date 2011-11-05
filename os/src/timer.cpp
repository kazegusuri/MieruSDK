/*
 * Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.
 * Copyright (c) 2011 sabottenda.
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

/*! @file timer.cpp
 *@brief Timer handler.
 *@author Masahiro Sano
 *@date 2011/09/23
 *@date 2011/11/04
 */

#include <mierulib.h>
#include <kernel.h>
#include <timer.h>
#include <exception.h>

inline void update_timer(int n){
    __asm__ (
        "mfc0    $t0, $11\n"
        "addu    $t0, $t0, %0 \n" 
        "mtc0    $t0, $11\n" 
        :  : "r" (n) : "%t0"
        );
}

void init_timer(){
    update_timer(0x100);
}

void timer_handler(){
    disable_global_interupt();
    update_timer(0x100);
    int pid = Kernel::getInstance()->taskmanager.getCurrentTask()->pid;

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

    enable_global_interupt();
}
