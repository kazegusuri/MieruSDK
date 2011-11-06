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

/*! @file test_kmalloc.cpp
 *@brief test file for kmalloc.cpp
 *@author Masahiro Sano
 *@since 2011/11/04
 *@date 2011/11/04
 */

#include <kmalloc.h>
#include <test.h>

#ifdef TEST_MODE

void test_class_MemoryAllocator() {
    MemoryAllocator *_this = MemoryAllocator::getInstance();
    uint brk = _this->brk;
    void *p0, *p1, *p2;
    MemoryAllocator::MemoryControlBlock *current;
    MemoryAllocator::MemoryControlBlock *mcb0, *mcb1, *mcb2;
    uint mcb_size = sizeof(MemoryAllocator::MemoryControlBlock);
    TEST_TRUE((uint)_this->sbrk(0) == brk);

    // test #1: alloc first
    p0 = _this->alloc(64);
    TEST_NOT_NULL(p0);
    TEST_TRUE(((uint)p0 & 3) == 0);
    current = _this->head;
    TEST_NOT_NULL(current);
    TEST_TRUE(current->data() == p0);
    mcb0 = _this->getBlock(p0);
    TEST_IS_NULL(mcb0->next());
    TEST_IS_NULL(mcb0->prev());
    TEST_TRUE(mcb0->getSize() == 64 + mcb_size);
    TEST_FALSE(mcb0->isAvailable());

    // test #2: alloc continuous region
    p1 = _this->alloc(126);
    TEST_NOT_NULL(p1);
    TEST_TRUE(((uint)p1 & 3) == 0);
    current = _this->head;
    TEST_TRUE(current->data() == p0);
    current = current->next();
    TEST_NOT_NULL(current);
    TEST_TRUE(current->data() == p1);
    mcb1 = _this->getBlock(p1);
    TEST_IS_NULL(mcb1->next());
    TEST_TRUE(mcb1->prev() == mcb0);
    TEST_TRUE(mcb1->getSize() == 128 + mcb_size);
    TEST_FALSE(mcb1->isAvailable() );
    TEST_TRUE(mcb0->next() == mcb1);
    TEST_FALSE(mcb0->isAvailable());

    // test #3: just free
    mcb1 = _this->getBlock(p1);
    _this->free(p1);
    current = _this->head;
    TEST_TRUE(current->data() == p0);
    current = current->next();
    TEST_NOT_NULL(current);
    TEST_TRUE(current->data() == p1);
    TEST_IS_NULL(mcb1->next());
    TEST_TRUE(mcb1->prev() == mcb0);
    TEST_TRUE(mcb1->getSize() == 128  + mcb_size);
    TEST_TRUE(mcb1->isAvailable());
    TEST_TRUE(mcb0->next() == mcb1);
    TEST_FALSE(mcb0->isAvailable());

    // test #4: free and merge next
    mcb0 = _this->getBlock(p0);
    _this->free(p0);
    current = _this->head;
    TEST_NOT_NULL(current);
    TEST_TRUE(current->data() == p0);
    TEST_TRUE(current == mcb0);
    TEST_IS_NULL(current->next());
    TEST_TRUE(current->getSize() == (64 + 128 + 2*mcb_size));
    TEST_TRUE(current->isAvailable());
    TEST_IS_NULL(mcb0->next());
    TEST_IS_NULL(mcb0->prev());
    TEST_TRUE(mcb0->getSize() == (64 + 128 + 2* + mcb_size));
    TEST_TRUE(mcb0->isAvailable());

    // test #5: alloc new region
    p1 = _this->alloc(256);
    TEST_NOT_NULL(p1);
    TEST_TRUE(((uint)p1 & 3) == 0);
    current = _this->head;
    TEST_NOT_NULL(current);
    current = current->next();
    TEST_NOT_NULL(current);
    TEST_TRUE(current->data() == p1);
    mcb1 = _this->getBlock(p1);
    TEST_IS_NULL(mcb1->next());
    TEST_TRUE(mcb1->prev() == mcb0);
    TEST_TRUE(mcb1->getSize() == 256 + mcb_size);
    TEST_FALSE(mcb1->isAvailable());
    TEST_TRUE(mcb0->next() == mcb1);

    // test #6: alloc continuous region and free it
    p2 = _this->alloc(256);
    TEST_NOT_NULL(p2);
    mcb2 = _this->getBlock(p2);
    TEST_IS_NULL(mcb2->next());
    TEST_TRUE(mcb2->prev() == mcb1);
    TEST_TRUE(mcb2->getSize() == 256 + mcb_size);
    TEST_FALSE(mcb2->isAvailable());
    TEST_TRUE(mcb1->next() == mcb2);
    _this->free(p2);
    TEST_IS_NULL(mcb2->next());
    TEST_TRUE(mcb2->prev() == mcb1);
    TEST_TRUE(mcb2->getSize() == 256 + mcb_size);
    TEST_TRUE(mcb2->isAvailable());
    TEST_TRUE(mcb1->next() == mcb2);    

    // test #7: merge both prev and next
    _this->free(p1);
    current = _this->head;
    TEST_NOT_NULL(current);
    TEST_IS_NULL(current->next());
    TEST_TRUE(current->getSize() == (64 + 128 + 256 + 256 + 4*mcb_size));
    TEST_TRUE(current->isAvailable());

    // test #8: split existing region
    p0 = _this->alloc(256);
    TEST_NOT_NULL(p0);
    TEST_TRUE(((uint)p0 & 3) == 0);
    current = _this->head;
    TEST_TRUE(current->data() == p0);
    current = current->next();
    TEST_NOT_NULL(current);
    mcb0 = _this->getBlock(p0);
    TEST_TRUE(mcb0->next() == current);
    TEST_IS_NULL(mcb0->prev());
    TEST_TRUE(mcb0->getSize() == 256 + mcb_size);
    TEST_FALSE(mcb0->isAvailable());
    TEST_TRUE(current->getSize() == (192 + 256 + 3*mcb_size));
    TEST_IS_NULL(current->next());
    TEST_TRUE(current->prev() == mcb0);
    TEST_TRUE(current->isAvailable());
    // lcd_dprintf("mcb0   :%p size:%d use:%d next:%p prev:%p\n", 
    //             mcb0, mcb0->getSize(), mcb0->isAvailable(),
    //             mcb0->next(), mcb0->prev());
    // lcd_dprintf("current:%p size:%d use:%d next:%p prev:%p\n", 
    //             current, current->getSize(), current->isAvailable(),
    //             current->next(), current->prev());
}

#endif
