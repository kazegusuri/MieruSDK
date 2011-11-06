// -*- C++ -*-
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

/*! @file test.h
 *@brief Header file for UnitTest
 *@author Masahiro Sano
 *@since 2011/11/04
 *@date 2011/11/04
 */

#pragma once

#ifdef TEST_MODE
#include <assert.h>
#include <mierulib.h>

void test_class_MemoryAllocator();
void test_class_list();

void _TEST_TRUE(bool x, const char *varx, const char *fname, int line);
void _TEST_FALSE(bool x, const char *varx, const char *fname, int line);

template <class T1, class T2>
inline void _TEST_EQ(const T1 &x, const T2 &y, const char *varx, const char *vary, const char *fname, int line) {
    if (!(x == y)) {
        lcd_dprintf("NG:%s:%d: %s neq %s\n", fname, line, varx, vary);
    }
}

template <class T>
inline void _TEST_EQ(const T &x, const T &y, const char *varx, const char *vary, const char *fname, int line) {
    if (!(x == y)) {
        lcd_dprintf("NG:%s:%d: %s neq %s\n", fname, line, varx, vary);
    }
}


template <class T1, class T2>
inline void _TEST_NEQ(const T1 &x, const T2 &y, const char *varx, const char *vary, const char *fname, int line) {
    if (!(x != y)) {
        lcd_dprintf("NG:%s:%d: %s eq %s\n", fname, line, varx, vary);
    }
}

template <class T>
inline void _TEST_IS_NULL(const T &x, const char *varx, const char *fname, int line) {
    if (!(x == NULL)) {
        lcd_dprintf("NG:%s:%d: %s is not NULL\n", fname, line, varx);
    }
}

template <class T>
inline void _TEST_NOT_NULL(const T &x, const char *varx, const char *fname, int line) {
    if (!(x != NULL)) {
        lcd_dprintf("NG:%s:%d: %s is NULL\n", fname, line, varx);
    }
}

#define TEST_CLASS_REG(x) friend void test_class_ ## x (void)
#define TEST_CLASS_CALL(x) test_class_ ## x ()

#define TEST_TRUE(x) _TEST_TRUE(x, #x, __FILE__, __LINE__)
#define TEST_FALSE(x) _TEST_FALSE(x, #x, __FILE__, __LINE__)
#define TEST_EQ(x, y) _TEST_EQ(x, y, #x, #y __FILE__, __LINE__)
#define TEST_NEQ(x, y) _TEST_NEQ(x, y, #x, #y __FILE__, __LINE__)
#define TEST_IS_NULL(x) _TEST_IS_NULL(x, #x, __FILE__, __LINE__)
#define TEST_NOT_NULL(x) _TEST_NOT_NULL(x, #x, __FILE__, __LINE__)
//#define aaTEST_NOT_NULL(x) _test_NOT_NULL<void *>(x, #x, __FILE__, __LINE__)


#else
#define TEST_CLASS_REG(x)
#define TEST_CLASS_CALL(x)
#endif


