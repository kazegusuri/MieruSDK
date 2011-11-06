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

/*! @file task.h
 *@brief Header file for task
 *@author Masahiro Sano
 *@since 2010/06/14
 *@date 2010/06/14
 *@version 0.1
 */

#pragma once
#include <syscall.h>
#include <mierulib.h>
#include <list.hpp>

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
    const static int TASK_STATE_NOALLOC   = 0x00;
    const static int TASK_STATE_ALLOC     = 0x01;
    const static int TASK_STATE_RUNNING   = 0x02;
    const static int TASK_STATE_WAITING   = 0x04;
    const static int TASK_STATE_STOP      = 0x20;
    
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
    Task *current;
    Task *previous;
    unsigned int max_pid;
    mpc::list<Task *> tasks;
    mpc::list<Task *> running;

    void switch_to(thread_struct *prev, thread_struct *cur);
public:

    TaskManager();
    void init();
    Task *getTask();
    bool freeTask(int pid);
    Task *findTask(int pid);
    bool startTask(Task *task);
    bool stopTask(Task *task);
    Task *getCurrentTask();
    Task *getPreviousTask();
    Task *getKernelTask();
    void switchContext();
    void switchContext(Task *prev, Task *cur);
};

void print(const thread_struct &ts);
void print(const Task &task);

#endif
