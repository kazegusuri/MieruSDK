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

/*! @file register.cpp
 *@brief processor register
 *@author Masahiro Sano
 *@since 2011/11/22
 *@date 2011/11/22
 *@version 0.1
 */

#include <register.h>
namespace Register { 
    namespace CP0 {

//==============================================================================
        uint EntryLo0::_reg = 0;
        uint EntryLo0::pfn() {
            return (_reg >> EntryLo0::PFN_SHIFT) & EntryLo0::PFN_MASK;
        }
        void EntryLo0::pfn(uint _v) {
            _reg = 
                (_reg & ~(EntryLo0::PFN_MASK << EntryLo0::PFN_SHIFT)) | 
                (_v << EntryLo0::PFN_SHIFT);
        }
        uint EntryLo0::cache() {
            return (_reg >> EntryLo0::CACHE_SHIFT) & EntryLo0::CACHE_MASK;
        }
        void EntryLo0::cache(uint _v) {
            _reg = 
                (_reg & ~(EntryLo0::CACHE_MASK << EntryLo0::CACHE_SHIFT)) | 
                (_v << EntryLo0::CACHE_SHIFT);
        }
        uint EntryLo0::dirty() {
            return (_reg >> EntryLo0::DIRTY_SHIFT) & EntryLo0::DIRTY_MASK;
        }
        void EntryLo0::dirty(uint _v) {
            _reg = 
                (_reg & ~((EntryLo0::DIRTY_MASK << EntryLo0::DIRTY_SHIFT))) |
                (_v << EntryLo0::DIRTY_SHIFT);
        }
        uint EntryLo0::valid() {
            return (_reg >> EntryLo0::VALID_SHIFT) & EntryLo0::VALID_MASK;
        }
        void EntryLo0::valid(uint _v) {
            _reg = 
                (_reg & ~(EntryLo0::VALID_MASK << EntryLo0::VALID_SHIFT)) | 
                (_v << EntryLo0::VALID_SHIFT);
        }
        uint EntryLo0::global() {
            return (_reg >> EntryLo0::GLOBAL_SHIFT) & EntryLo0::GLOBAL_MASK;
        }
        void EntryLo0::global(uint _v) {
            _reg = 
                (_reg & ~(EntryLo0::GLOBAL_MASK << EntryLo0::GLOBAL_SHIFT)) | 
                (_v << EntryLo0::GLOBAL_SHIFT);
        }

        void EntryLo0::load() {
            asm("mfc0 %0, $2" : "=r" (_reg));
        }
        void EntryLo0::store() {
            asm("mtc0 %0, $2" : : "r" (_reg));
        }
        void EntryLo0::set(uint v) {
            _reg = v;
        }
        uint EntryLo0::get() {
            return _reg;
        }

//==============================================================================
        uint EntryHi::_reg = 0;
        uint EntryHi::vpn2() {
            return (_reg >> EntryHi::VPN2_SHIFT) & EntryHi::VPN2_MASK;
        }
        void EntryHi::vpn2(uint _v) {
            _reg = 
                (_reg & ~(EntryHi::VPN2_MASK << EntryHi::VPN2_SHIFT)) | 
                (_v << EntryHi::VPN2_SHIFT);
        }
        uint EntryHi::asid() {
            return (_reg >> EntryHi::ASID_SHIFT) & EntryHi::ASID_MASK;
        }
        void EntryHi::asid(uint _v) {
            _reg = 
                (_reg & ~(EntryHi::ASID_MASK << EntryHi::ASID_SHIFT)) | 
                (_v << EntryHi::ASID_SHIFT);
        }

        void EntryHi::load() {
            asm("mfc0 %0, $10" : "=r" (_reg));
        }
        void EntryHi::store() {
            asm("mtc0 %0, $10" : : "r" (_reg));
        }
        void EntryHi::set(uint v) {
            _reg = v;
        }
        uint EntryHi::get() {
            return _reg;
        }

//==============================================================================
        uint Status::_reg = 0;
        uint Status::im() {
            return (_reg >> Status::IM_SHIFT) & Status::IM_MASK;
        }
        void Status::im(uint _v) {
            _reg = 
                (_reg & ~(Status::IM_MASK << Status::IM_SHIFT)) | 
                (_v << Status::IM_SHIFT);
        }
        uint Status::ksu() {
            return (_reg >> Status::KSU_SHIFT) & Status::KSU_MASK;
        }
        void Status::ksu(uint _v) {
            _reg = 
                (_reg & ~(Status::KSU_MASK << Status::KSU_SHIFT)) | 
                (_v << Status::KSU_SHIFT);
        }
        uint Status::erl() {
            return (_reg >> Status::ERL_SHIFT) & Status::ERL_MASK;
        }
        void Status::erl(uint _v) {
            _reg = 
                (_reg & ~(Status::ERL_MASK << Status::ERL_SHIFT)) | 
                (_v << Status::ERL_SHIFT);
        }
        uint Status::exl() {
            return (_reg >> Status::EXL_SHIFT) & Status::EXL_MASK;
        }
        void Status::exl(uint _v) {
            _reg = 
                (_reg & ~(Status::EXL_MASK << Status::EXL_SHIFT)) | 
                (_v << Status::EXL_SHIFT);
        }
        uint Status::ie() {
            return (_reg >> Status::IE_SHIFT) & Status::IE_MASK;
        }
        void Status::ie(uint _v) {
            _reg = 
                (_reg & ~(Status::IE_MASK << Status::IE_SHIFT)) | 
                (_v << Status::IE_SHIFT);
        }
        void Status::load() {
            asm("mfc0 %0, $12" : "=r" (_reg));
        }
        void Status::store() {
            asm("mtc0 %0, $12" : : "r" (_reg));
        }
        void Status::set(uint v) {
            _reg = v;
        }
        uint Status::get() {
            return _reg;
        }

    };
};
