/******************************************************************************/
/* MieruOS: Task v0.1                                                         */
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

#pragma once
#include <syscall.h>
#include <mierulib.h>

#define TASK_STATE_NOALLOC   0x00
#define TASK_STATE_ALLOC     0x01
#define TASK_STATE_RUNNING   0x02
#define TASK_STATE_WAITING   0x04
#define TASK_STATE_STOP      0x20


#ifndef __ASSEMBLY__

#define MAX_TASK_NUM 3


typedef struct _thread_struct{
    long at;
    long v0, v1;
    long a0, a1, a2, a3;
    long t0, t1, t2, t3, t4, t5, t6, t7;
    long s0, s1, s2, s3, s4, s5, s6, s7;
    long t8, t9;
    long k0, k1;
    long gp, sp, fp, ra;
    long cp0_status;
    long cp0_cause;
    long cp0_epc;
    long ksp;
}thread_struct;

class Task{
public:
    int state;
    int pid;
    int stack_start;
    int stack_end;

    struct task_report report;

    unsigned int brk;
    thread_struct tss;

    Task();
    
};


class TaskManager{
public:
    int remain;
    Task tasks[MAX_TASK_NUM];
    Task *current;
    Task *previous;

    TaskManager();
    void init();
    Task *getTask();
    Task *getCurrentTask();
    Task *getPreviousTask();
    Task *getKernelTask();
    void switchContext(Task *prev, Task *cur);
    void switch_to(thread_struct *prev, thread_struct *cur);
    void freeTask(int pid);
};

void print(const thread_struct &ts);

#endif
