/*
 * Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.
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

/*! @file exception.h
 *@brief Header file for exception.
 *@author Masahiro Sano
 *@since 2011/10/09
 *@date 2011/10/09
 *@version 0.1
 */

#pragma once

#define EXCEPTION_ENTRY_ADDR 0x00f8

#define EXCCODE_INT       0
#define EXCCODE_MOD       1
#define EXCCODE_TLBL      2
#define EXCCODE_TLBS      3
#define EXCCODE_ADEL      4
#define EXCCODE_ADES      5
#define EXCCODE_IBE       6
#define EXCCODE_DBE       7
#define EXCCODE_SYSCALL   8
#define EXCCODE_BP        9
#define EXCCODE_RI        10
#define EXCCODE_CPU       11
#define EXCCODE_OV        12
#define EXCCODE_TRAP      13
#define EXCCODE_UNUSED0   14
#define EXCCODE_FPE       15
#define EXCCODE_CUSTOM0   16
#define EXCCODE_CUSTOM1   17
#define EXCCODE_C2E       18
#define EXCCODE_RESERVED0 19
#define EXCCODE_RESERVED1 20
#define EXCCODE_RESERVED2 21
#define EXCCODE_MDMX      22
#define EXCCODE_WATCH     23
#define EXCCODE_MCHECK    24
#define EXCCODE_THREAD    25
#define EXCCODE_DSP       26
#define EXCCODE_RESERVED3 27
#define EXCCODE_RESERVED4 28
#define EXCCODE_RESERVED5 29
#define EXCCODE_CACHERR   30
#define EXCCODE_UNUSED1   31

#ifndef __ASSEMBLY__
#include <task.h>

namespace Inst {
    extern "C" void eret(void);
    void tlbr(void);
    void tlbwr(void);
    void tlbwi(void);
    void tlbp(void);
    void ehb(void);
};

void clear_kernel_mode();
void set_kernel_mode();
void clear_exception();
void disable_global_interupt();
void enable_global_interupt();

void init_exception();
extern "C" void exception_handler(thread_struct *ts);
#endif
