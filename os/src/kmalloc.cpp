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

/*! @file kmalloc.cpp
 *@brief memory allocator for kernel.
 *@author Masahiro Sano
 *@since 2011/10/12
 *@date 2011/11/04
 */

#include <mierulib.h>
#include <kernel.h>
#include <kmalloc.h>

char memory_allocator_buffer[sizeof(MemoryAllocator)];
MemoryAllocator *MemoryAllocator::_instance = NULL;

/**********************************************************************/
MemoryAllocator::MemoryAllocator(){
    extern uint __stack_end[];   // defined by linker
    extern uint __stack_start[]; // defined by linker
    lcd_dprintf("stack start:%08x end:%08x\n", __stack_start, __stack_end);

    this->stack_start = (uint)__stack_start;
    this->stack_end = (uint)__stack_end;
    this->brk = this->stack_end;
    this->head = NULL;
}

/**********************************************************************/
MemoryAllocator *MemoryAllocator::getInstance(){
    if (_instance == NULL) {
        _instance = new(memory_allocator_buffer) MemoryAllocator();
    }
    return _instance;
}

/**********************************************************************/
bool MemoryAllocator::is_valid_range(uint addr) const{
    uint lower = this->stack_end;
    uint upper = this->stack_start - MemoryAllocator::MAX_STACK_SIZE;
    return (lower < addr && addr < upper);
}

/**********************************************************************/
MemoryAllocator::MemoryControlBlock *MemoryAllocator::getBlock(const void *pointer) {
    return (MemoryAllocator::MemoryControlBlock *)((uint)pointer - sizeof(MemoryAllocator::MemoryControlBlock));    
}

/**********************************************************************/
void *MemoryAllocator::sbrk(const uint req_size) {
    if (req_size == 0) {
        return (void *)this->brk;
    }

    uint next = this->brk + req_size;
    if (!is_valid_range(next)){
        return NULL;
    }

    void *ret = (void *)this->brk;
    this->brk = next;
    this->stack_end = next;
    return ret;
}

/**********************************************************************/
void *MemoryAllocator::alloc(uint req_size){
    MemoryAllocator::MemoryControlBlock *current; // used to calculate address
    MemoryAllocator::MemoryControlBlock *prev = NULL;
    uint alloc_size;

    // 4Byte align
    alloc_size = (req_size + sizeof(size_t) -1) & (~0x3);

    // add the size of MemoryControlBlock
    alloc_size += sizeof(MemoryAllocator::MemoryControlBlock);

    // search empty location and simply use it
    for(current = this->head;
        current != NULL;
        prev = current, current = current->next()) {

        if(!current->isAvailable() || current->getSize() < alloc_size)
            continue;

        current->unsetAvailable();
            
        // split mergin space if the space is large
        uint min_size = sizeof(MemoryAllocator::MemoryControlBlock) + sizeof(uint);
        if (current->getSize() > alloc_size + min_size) {
            current->split(alloc_size);
        }

        return current->data();
    }

    /* couldn't find enough space in existing mcbs  */
    /* so aquire new memory space                   */
    MemoryAllocator::MemoryControlBlock *next;
    next = (MemoryAllocator::MemoryControlBlock *)this->sbrk(alloc_size);
    //lcd_dprintf("next %08x\n",next);
    if (next == NULL) return NULL;

    /*initialize memory_control_block*/
    next->reset();
    next->setSize(alloc_size);
    next->unsetAvailable();

    if (head == NULL) {
        this->head = next;
        next->connect(NULL);
    } else {
        prev->connect(next);
        next->connect(NULL);
    }

    return next->data();
}

/**********************************************************************/
void MemoryAllocator::free(const void *pointer) {
    MemoryAllocator::MemoryControlBlock *mcb, *prev, *next;

    if(pointer == NULL) return;

    /* put the pointer back to find the memory_control_block */
    mcb = getBlock(pointer);

    // set available bit
    mcb->setAvailable();

    next = mcb->next();
    if(next != NULL && next->isAvailable()){
        mcb->merge(next);
    }

    prev = mcb->prev();
    if(prev != NULL && prev->isAvailable()){
        prev->merge(mcb);
    }
}
/**********************************************************************/
