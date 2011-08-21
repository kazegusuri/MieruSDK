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

#include <kernel.h>
#include <mierulib.h>
#include <task.h>

/******************************************************************************/
TaskManager::TaskManager(){
}

/******************************************************************************/
void TaskManager::init(){
    remain = MAX_TASK_NUM;
    current = getTask();
    previous = NULL;
}

/******************************************************************************/
Task *TaskManager::getTask(){
    Task *p;
    if(remain == 0)
        return NULL;

    for(int i=0;i<MAX_TASK_NUM;i++){
        p = &tasks[i];
        if(p->state != TASK_STATE_NOALLOC)
            continue;

        p->state = TASK_STATE_ALLOC;
        p->pid = i;
        
        remain--;
        return p;
    }
            
    return NULL;
}

/******************************************************************************/
Task *TaskManager::getCurrentTask(){
    return current;
}

/******************************************************************************/
Task *TaskManager::getPreviousTask(){
    return previous;
}

/******************************************************************************/
Task *TaskManager::getKernelTask(){
    return &tasks[0];
}

/******************************************************************************/
void TaskManager::freeTask(int pid){
    if(pid == 0) return;

    if(pid < 0 && pid >= MAX_TASK_NUM)
        return;

    Task *p = &tasks[pid];
    p->state = TASK_STATE_NOALLOC;
    remain++;
}

/******************************************************************************/
// doesn't work
void TaskManager::switchContext(Task *prev, Task *cur){
    current = cur;
    previous = prev;
    switch_to(&prev->tss, &cur->tss);
}

/******************************************************************************/
Task::Task() :
    state(TASK_STATE_NOALLOC),
    pid(-1)
{
}

/******************************************************************************/
void __attribute__((noinline)) TaskManager::switch_to(thread_struct *prev, thread_struct *cur){
    register unsigned long __a0 asm("$4") = (unsigned long) prev;
    register unsigned long __a1 asm("$5") = (unsigned long) cur;
    __asm__ volatile (
        // save prev process
        "sw    $s0,  56($a0);"
        "sw    $s1,  60($a0);"
        "sw    $s2,  64($a0);"
        "sw    $s3,  68($a0);"
        "sw    $s4,  72($a0);"
        "sw    $s5,  76($a0);"
        "sw    $s6,  80($a0);"
        "sw    $s7,  84($a0);"
        "sw    $gp,  96($a0);"
        "sw    $sp, 100($a0);"
        "sw    $fp, 104($a0);"
        "sw    $ra, 108($a0);"
        
//        "mfc0	$t0, $12;"      // load status reg
//        "nop;"
//        "mfc0  $t1, $14;"      // load epc reg
//        "nop;"
//        "sw    $t0, 116($a0);" // store status
//        "sw    $t1, 112($a0);" // store epc

        // restore next process
        "lw    $s0, 56($a1);"
        "lw    $s1, 60($a1);"
        "lw    $s2, 64($a1);"
        "lw    $s3, 68($a1);"
        "lw    $s4, 72($a1);"
        "lw    $s5, 76($a1);"
        "lw    $s6, 80($a1);"
        "lw    $s7, 84($a1);"
        "lw    $gp, 96($a1);"
        "lw    $sp, 100($a1);"
        "lw    $fp, 104($a1);"
        "lw    $ra, 108($a1);"

//        "lw    $t0, 112($a1);" // load epc
//        "lw    $t1, 116($a1);" // load status
//        "mtc0  $t0, $14;"      // store epc
//        "mtc0  $t1, $12;"      // store status

        :
        : "r" (__a0), "r" (__a1)
        );
}

/******************************************************************************/
