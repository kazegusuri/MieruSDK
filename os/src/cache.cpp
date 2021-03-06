/******************************************************************************/
/* MieruOS: cache v0.1                                                        */
/* written by Masahiro Sano Arch Lab. Tokyo Tech                   2010-09-17 */
/******************************************************************************/

/*
 * Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.
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

#include <kernel.h>
#include <mierulib.h>
#include <cache.h>

volatile uint invalid_op[ICACHE_SIZE/4];

void init_cache(void){
    int i;
    //create invalidate operation
    for(i=0;i<ICACHE_SIZE/4;i++)
        invalid_op[i] = 0x00000004; // 'sll $0, $0, $0'
    //memset((uint *)invalid_op, 0, ICACHE_SIZE);   // many nops
    invalid_op[ICACHE_SIZE/4 - 2] = 0x03e00008; // jr $ra

    volatile int *p, *end;
     unsigned int a = 0;
    p = (int *)0x20000;
    end = p + ICACHE_SIZE/4;
    while(p != end) 
        a += (int)*p++;
}

void invalidate_icache(void){
    void (*func)(void) = reinterpret_cast<void(*)(void)>(invalid_op);
    func(); 
}

int invalidate_dcache(void){
    int i;
    volatile uint s=0;
    for(i=0;i<ICACHE_SIZE/4;i++){
        s += invalid_op[i];
    }
    return s;
}

