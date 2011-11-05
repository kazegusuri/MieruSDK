/*
 * Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.
 * Copyright (c) 2011 Masahiro Sano
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

/*! @file exception.cpp
 *@brief exceptions handler
 *@author Masahiro Sano
 *@since 2010/10/20
 *@date 2011/11/04
 *@version 0.1
 */

#include <mierulib.h>
#include <exception.h>
#include <syscall.h>
#include <timer.h>

void clear_kernel_mode(){
    __asm__ volatile (
        "mfc0    $t0, $12\n"
        "addiu   $t1, $zero, -3\n"
        "and     $t0, $t0, $t1\n"
        "mtc0    $t0, $12\n"
        : : : "%t0", "%t1"
        );
}

void disable_global_interupt(){
    __asm__ volatile (
        "mfc0    $t0, $12\n"
        "addiu   $t1, $zero, -2\n"
        "and     $t0, $t0, $t1\n"
        "mtc0    $t0, $12\n"
        : : : "%t0", "%t1"
        );
}

void enable_global_interupt(){
    __asm__ volatile (
        "mfc0    $t0, $12\n"
        "ori     $t0, $t0, 1\n"
        "mtc0    $t0, $12\n"
        : : : "%t0"
        );
}

void init_exception(){

    extern uint __exception_entry_addr[]; // defined by linker

    if (EXCEPTION_ENTRY_ADDR != (unsigned int)__exception_entry_addr){
        lcd_dprintf("Configuration Error: exception entry address doesn't match\n");
        lcd_dprintf("  define:%08x var:%08x\n", EXCEPTION_ENTRY_ADDR, __exception_entry_addr);
    }

    uint *exception_handler_addr = (uint *)EXCEPTION_ENTRY_ADDR;
    *exception_handler_addr = (uint)exception_handler;
    lcd_dprintf("exception addr:%08x\n", __exception_entry_addr);
    lcd_dprintf("exception handler:%08x\n", exception_handler_addr);
}

extern "C" void exception_handler(thread_struct *ts){
    int exccode = (ts->cp0_cause >> 2) & 0x1f;
    lcd_dprintf("exccode %08x\n", exccode);
    //_print_ts(*ts);

    switch(exccode){
    case EXCCODE_INT:
        timer_handler();
        break;
    case EXCCODE_SYSCALL:
        if(ts->v0 < 4000) {
            ts->v0 = syscall_handler(ts->a0, ts->a1, ts->a2, ts->a3, ts->v0);
        }
        ts->cp0_epc += 4;
        break;
    default:
        lcd_dprintf("not implemented yet(%d)\n", exccode);
    }
}

