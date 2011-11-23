/*
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

/*! @file register.h
 *@brief processor register
 *@author Masahiro Sano
 *@since 2011/11/22
 *@date 2011/11/22
 *@version 0.1
 */

#include <types.h>

namespace Register {
    namespace General {
        const static int ZERO = 0;
        const static int AT   = 1;
        const static int V0   = 2;
        const static int V1   = 3;
        const static int A0   = 4;
        const static int A1   = 5;
        const static int A2   = 6;
        const static int A3   = 7;
        const static int T0   = 8;
        const static int T1   = 9;
        const static int T2   = 10;
        const static int T3   = 11;
        const static int T4   = 12;
        const static int T5   = 13;
        const static int T6   = 14;
        const static int T7   = 15;
        const static int S0   = 16;
        const static int S1   = 17;
        const static int S2   = 18;
        const static int S3   = 19;
        const static int S4   = 20;
        const static int S5   = 21;
        const static int S6   = 22;
        const static int S7   = 23;
        const static int T8   = 24;
        const static int T9   = 25;
        const static int K0   = 26;
        const static int K1   = 27;
        const static int GP   = 28;
        const static int SP   = 29;
        const static int S8   = 30;
        const static int FP   = 30;
        const static int RA   = 31;
    };

    namespace CP0 {
        const static int INDEX    = 0;
        const static int RANDOM   = 1;
        const static int ENTRYLO0 = 2;
        const static int ENTRYLO1 = 3;
        const static int CONTEXT  = 4;
        const static int PAGEMASK = 5;
        const static int WIRED    = 6;
        const static int BADVADDR = 8;
        const static int COUNT    = 9;
        const static int ENTRYHI  = 10;
        const static int COMPARE  = 11;
        const static int SR       = 12;
        const static int CAUSE    = 13;
        const static int EPC      = 14;
        const static int CACHEERR = 27;
        const static int ECC      = 26;
        const static int ERROREPC = 30;

        class EntryLo0{
        private:
            static uint _reg;
            EntryLo0();
        public:
            const static int PFN_SHIFT = 6;
            const static int PFN_MASK = 0x3ffffff;
            const static int CACHE_SHIFT = 3;
            const static int CACHE_MASK = 0x7;
            const static int DIRTY_SHIFT = 2;
            const static int DIRTY_MASK = 0x1;
            const static int VALID_SHIFT = 1;
            const static int VALID_MASK = 0x1;
            const static int GLOBAL_SHIFT = 0;
            const static int GLOBAL_MASK = 0x1;

            static uint pfn();
            static void pfn(uint _v);
            static uint cache();
            static void cache(uint _v);
            static uint dirty();
            static void dirty(uint _v);
            static uint valid();
            static void valid(uint _v);
            static uint global();
            static void global(uint _v);

            static void load();
            static void store();
            static void set(uint v);
            static uint get();
        };

        class Status{
        private:
            static uint _reg;
            Status();
        public:
            const static int IM_SHIFT  = 8;
            const static int IM_MASK   = 0xff;
            const static int KSU_SHIFT = 3;
            const static int KSU_MASK  = 0x3;
            const static int ERL_SHIFT = 2;
            const static int ERL_MASK  = 0x1;
            const static int EXL_SHIFT = 1;
            const static int EXL_MASK  = 0x1;
            const static int IE_SHIFT  = 1;
            const static int IE_MASK   = 0x1;

            static uint im();
            static void im(uint _v);
            static uint ksu();
            static void ksu(uint _v);
            static uint erl();
            static void erl(uint _v);
            static uint exl();
            static void exl(uint _v);
            static uint ie();
            static void ie(uint _v);

            static void load();
            static void store();
            static void set(uint v);
            static uint get();
        };

        class EntryHi{
        private:
            static uint _reg;
            EntryHi();
        public:
            const static int VPN2_SHIFT  = 10;
            const static int VPN2_MASK   = 0x3fffff;
            const static int ASID_SHIFT  = 0;
            const static int ASID_MASK   = 0xff;
            
            static uint vpn2();
            static void vpn2(uint _v);
            static uint asid();
            static void asid(uint _v);

            static void load();
            static void store();
            static void set(uint v);
            static uint get();
        };

    };
};
