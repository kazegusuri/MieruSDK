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

#include <kernel.h>
#include <mierulib.h>
#include <syscall.h>
#include <fat.h>
#include <task.h>
#include <shell.h>
#include <cache.h>
#include <timer.h>
#include <exception.h>
#include <kmalloc.h>
#include <test.h>

Kernel *Kernel::_instance = NULL;
extern void sandbox();

/******************************************************************************/
void startKernel(void){
    TEST_CLASS_CALL(MemoryAllocator);
    Kernel *kernel = Kernel::getInstance();
    lcd_dprintf("Kernel:%p end:%x\n", kernel, kernel+1);
    kernel->init();
    kernel->start();
}

/******************************************************************************/
Kernel::Kernel(){
}

/******************************************************************************/
Kernel *Kernel::getInstance() {
    if (_instance == NULL) {
        _instance = new Kernel();
    }
    return _instance;
}

/******************************************************************************/
void Kernel::init(){
    int ret;

    lcd_reset();
    lcd_ttyopen(1);

    init_syscall();
    //init_timer();
    init_exception();
    clear_kernel_mode();
    enable_global_interupt();
    ret = fat.init();
    if(ret < 0){
        switch(-ret){
        case FAT_ERROR_MBR_INVALID:
            lcd_printf("Invalid MMC. mbr error.\n");
            break;
        case FAT_ERROR_FAT_TYPE_INVALID:
            lcd_printf("Invalid MMC. fat type error.\n");
            break;
        case FAT_ERROR_BPB_INVALID:
            lcd_printf("Invalid MMC. bpb error.\n");
            break;
        case FAT_ERROR_SECTOR_SIZE_INVALID:
            lcd_printf("Invalid MMC. sector size error.\n");
            break;
        }
        lcd_printf("Stop Kernel\n");
        for(;;);
    }

    taskmanager.init();
    init_cache();
}

/******************************************************************************/
void Kernel::message(){
    int i;
    lcd_printf("");
    lcd_printf("MieruOS EMB %s Arch Lab. Tokyo Tech ", MIERUOS_VERSION);
    for(i=0;i<10;i++){
        lcd_printf(".");
        mpc_usleep(200000);
    }
    lcd_printf("\n\n");
}


/******************************************************************************/
void Kernel::start(){
    TEST_CLASS_CALL(list);
    sandbox();

    Shell *sh = new Shell();
    message();
    sh->run();
    delete sh;
}

/******************************************************************************/
