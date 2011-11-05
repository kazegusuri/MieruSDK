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
 *@since 2010/10/20
 *@date 2011/11/04
 *@version 0.2
 */

#pragma once

#define MIERUOS_VERSION "v0.5.6"
#ifndef MIERU_OS
#define MIERU_OS
#endif

#define _offsetof(type, member) ((size_t)&((type*)0)->member)

#include <mierulib.h>
#include <assert.h>
#include <file.h>
#include <syscall.h>
#include <task.h>
#include <fat.h>
#include <new.h>

void startKernel(void);

class Kernel{
    Kernel();
    Kernel(const Kernel &);
    Kernel &operator =(const Kernel &);

    
public:

    TaskManager taskmanager;
    Fat fat;

    FatFile *root;
    FatFile *current;
    FatFile files[MAX_FILE];
    int filepos;

    void init();
    void message();
    void start();

    static Kernel *_instance;
    static Kernel *getInstance();
};
