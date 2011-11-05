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

/*! @file kmalloc.h
 *@brief memory allocator for kernel.
 *@author Masahiro Sano
 *@since 2011/10/12
 *@date 2011/11/04
 *@version 0.1
 */

#include <types.h>
#include <test.h>

class MemoryAllocator {
    TEST_CLASS_REG(MemoryAllocator);
public:

    class MemoryControlBlock {
    private:
        uint _size;      ///< ブロックサイズ
        void *_next;
        void *_prev;     ///< 前のブロックへのポインタ
        uint _data[0];  ///< データへのポインタ
    public:
        void reset() {
            _next = NULL;
            _prev = NULL;
        }

        void setSize(uint s) {
            _size = s | (int)this->isAvailable();
        }

        uint getSize() const {
            return _size & (~0x1);
        }

        void setAvailable() {
            _size |= (0x1);
        }

        void unsetAvailable() {
            _size &= (~0x1);
        }

        bool isAvailable() const {
            return (_size & 0x1);
        }

        void connect(MemoryControlBlock *__next) {
            if (__next != NULL) {
                __next->_next = this->_next;
                __next->_prev = this;
            }
            if(this->_next != NULL) {
                static_cast<MemoryControlBlock *>(this->_next)->_prev = __next;
            }
            this->_next = __next;
        }

        void split(uint s) {
            MemoryControlBlock *tmp;
            tmp = (MemoryControlBlock *)((uint)this + s);
            //lcd_dprintf("split: %p\n", tmp);
            tmp->setSize(this->getSize() - s);
            tmp->setAvailable();
            this->setSize(s);
            this->connect(tmp);
        }

        void merge(MemoryControlBlock *next) {
            this->setSize(this->getSize() + next->getSize());
            this->_next = next->_next;
            if(next->next() != NULL)
                next->next()->_prev = this;
        }


        MemoryControlBlock *next() const {
            //return (MemoryControlBlock *)((uint)this + this->getSize());
            return (MemoryControlBlock *)this->_next;
        }

        MemoryControlBlock *prev() const {
            return (MemoryControlBlock *)this->_prev;
        }

        void *data() const {
            //return reinterpret_cast<void *>(_data);
            return (void *)(_data);
        }
    };

    class ControlBlockIterator {
    private:
        MemoryControlBlock *current;
    public:
        typedef ControlBlockIterator _Self;
        typedef void*                value_type;
        typedef void*&               reference;
        typedef void**               pointer;
        
        /* reference operator*() const; */
        /* pointer operator->() const; */
        /* _Self& operator++() { */
        /*     current = (MemoryControlBlock *)((uint)current + current->size); */
        /*     return *this; */
        /* } */
        /* _Self& operator--() { */
        /*     current = current->prev; */
        /*     return *this; */
        /* } */
    };

private:
    MemoryAllocator();
    MemoryAllocator(const MemoryAllocator &);
    MemoryAllocator &operator =(const MemoryAllocator &);

    uint brk;
    uint stack_start;
    uint stack_end;
    const static uint MAX_STACK_SIZE = 4096;
    MemoryControlBlock *head;

    bool is_valid_range(uint addr) const;
    MemoryAllocator::MemoryControlBlock *getBlock(const void *pointer);

public:    
    static MemoryAllocator *_instance;
    static MemoryAllocator *getInstance();

    void *alloc(const uint req_size);
    void free(const void *pointer);
    void *sbrk(const uint req_size);
};
