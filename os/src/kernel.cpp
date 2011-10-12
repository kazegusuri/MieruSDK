/******************************************************************************/
/* MieruOS: Kernel v0.1                                                       */
/* written by Masahiro Sano Arch Lab. Tokyo Tech                   2010-06-14 */
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
#include <syscall.h>
#include <fat.h>
#include <task.h>
#include <shell.h>
#include <cache.h>
#include <timer.h>
#include <exception.h>

extern void sh_mainloop();
char kernel_buffer[sizeof(Kernel)];
Kernel *Kernel::_instance = NULL;

void process1();
void process2();

/******************************************************************************/
extern "C" void __cxa_pure_virtual(){
    lcd_cls();
    lcd_dprintf("Inheritance Error\n");
    lcd_printf("Inheritance Error\n");
    lcd_printf("This is kernel error.\n");
    lcd_printf("Please report to a developer\n");
    for(;;);
}

/******************************************************************************/
int  _purecall(){
    // Do nothing or print an error message.
    return 0;
}

/******************************************************************************/
void startKernel(void){
    Kernel *kernel = Kernel::getInstance();
    kernel->init();
    kernel->start();
}

/******************************************************************************/
Kernel::Kernel(){
    // lcd_dprintf("hoge----------------------\n");
}

/******************************************************************************/
Kernel *Kernel::getInstance() {
    if (_instance == NULL) {
        _instance = new(kernel_buffer) Kernel();
    }
    return _instance;
}

/******************************************************************************/
void Kernel::init(){
    int ret;

    lcd_reset();
    lcd_ttyopen(1);

    init_syscall();
    init_timer();
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

    extern uint __stack_end[]; // defined by linker
    extern uint __stack_start[]; // defined by linker
    lcd_dprintf("stack_end:%08x\n", __stack_end);
    lcd_dprintf("stack_start:%08x\n", __stack_start);
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
    Task *current = taskmanager.getCurrentTask();
    unsigned int gp;
    
    //ret = system::_syscall(0,104,111,1200,0xffff);
    //lcd_dprintf("ret %d\n", ret);
    //for(;;);

    Task *t1 = taskmanager.getTask();
    Task *t2 = taskmanager.getTask();

    uint sp1 = 0x50000 - 16;
    uint sp2 = 0x80000 - 16;

    // __asm__ volatile ("move %0, $gp;" : "=r" (gp));
    // lcd_dprintf("%d %d %08x\n", t1->pid, t2->pid, gp);

    t1->tss.sp = sp1;
    //t1->tss.gp = gp;
    t1->tss.ra = (uint)process1;
    t1->tss.cp0_status = 1;
    t1->tss.cp0_epc = 0;
    t1->tss.cp0_cause = 0;
    t1->stack_start = sp1;
    t1->stack_end   = 0x20000 + 16;
    t1->brk         = 0x20000 + 16;

    lcd_dprintf("%08x \n", t1->tss.ra);

    t2->tss.sp = sp2;
    //t2->tss.gp = gp;
    t2->tss.ra = (uint)process2;
    t2->tss.cp0_status = 1;
    t2->tss.cp0_epc = 0;
    t2->tss.cp0_cause = 0;
    t2->stack_start = sp2;
    t2->stack_end   = sp1+32;
    t2->brk         = sp1+32;

    invalidate_icache();
    invalidate_dcache();

    lcd_dprintf("start!!!\n");
    //taskmanager.switchContext(current, t1);


    for(;;);
    // Shell sh;
    // message();
    // sh.run();
}

/******************************************************************************/

void process1(){
    for(;;){
        lcd_dprintf("process1\n");
        usleep(100000);
    }

    Kernel::getInstance()->taskmanager.switchContext(
        Kernel::getInstance()->taskmanager.getCurrentTask(), 
        &Kernel::getInstance()->taskmanager.tasks[2]);
    lcd_dprintf("process1\n");
    for(;;);
}

void process2(){
    for(;;){
        lcd_dprintf("process2\n");
        usleep(100000);
    }
    lcd_dprintf("process2\n");
    //usleep(1000000);
    Kernel::getInstance()->taskmanager.switchContext(
        Kernel::getInstance()->taskmanager.getCurrentTask(), 
        &Kernel::getInstance()->taskmanager.tasks[1]);
    lcd_dprintf("process2\n");

    for(;;);
}
