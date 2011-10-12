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
        "sw    $s0,  60($a0);"
        "sw    $s1,  64($a0);"
        "sw    $s2,  68($a0);"
        "sw    $s3,  72($a0);"
        "sw    $s4,  76($a0);"
        "sw    $s5,  80($a0);"
        "sw    $s6,  84($a0);"
        "sw    $s7,  88($a0);"
        "sw    $gp, 108($a0);"
        "sw    $sp, 112($a0);"
        "sw    $fp, 116($a0);"
        "sw    $ra, 120($a0);"
        
        "mfc0  $t0, $12;"      // load status reg
        "mfc0  $t1, $13;"      // load cause reg
        "mfc0  $t2, $14;"      // load epc reg
        "sw    $t0, 124($a0);" // store status
        "sw    $t1, 128($a0);" // store cause
        "sw    $t2, 132($a0);" // store epc

        // restore next process
        "lw    $s0, 60($a1);"
        "lw    $s1, 64($a1);"
        "lw    $s2, 68($a1);"
        "lw    $s3, 72($a1);"
        "lw    $s4, 76($a1);"
        "lw    $s5, 80($a1);"
        "lw    $s6, 84($a1);"
        "lw    $s7, 88($a1);"
        "lw    $gp, 108($a1);"
        "lw    $sp, 112($a1);"
        "lw    $fp, 116($a1);"
        "lw    $ra, 120($a1);"

        "lw    $t0, 132($a1);" // load epc
        "lw    $t1, 128($a1);" // load cause
        "lw    $t2, 124($a1);" // load status
        "mtc0  $t0, $14;"      // store epc
        "mtc0  $t1, $13;"      // store cause
        "mtc0  $t2, $12;"      // store status

        :
        : "r" (__a0), "r" (__a1)
        : "%t0", "%t1", "%t2"
        );
    return;
}

/******************************************************************************/
void print(const thread_struct &ts){
    lcd_dprintf("$zero     $at       $v0       $v1       $a0       $a1       $a2       $a3\n");
    lcd_dprintf("%08x  %08x  %08x  %08x  %08x  %08x  %08x  %08x\n",
                0, ts.at, ts.v0, ts.v1, ts.a0, ts.a1, ts.a2, ts.a3);
    lcd_dprintf("$t0       $t1       $t2       $t3       $t4       $t5       $t6       $t7\n");
    lcd_dprintf("%08x  %08x  %08x  %08x  %08x  %08x  %08x  %08x\n",
                ts.t0, ts.t1, ts.t2, ts.t3, ts.t4, ts.t5, ts.t6, ts.t7);
    lcd_dprintf("$s0       $s1       $s2       $s3       $s4       $s5       $s6       $s7\n");
    lcd_dprintf("%08x  %08x  %08x  %08x  %08x  %08x  %08x  %08x\n",
                ts.s0, ts.s1, ts.s2, ts.s3, ts.s4, ts.s5, ts.s6, ts.s7);
    lcd_dprintf("$t8       $t9       $k0       $k1       $gp       $sp       $fp       $ra\n");
    lcd_dprintf("%08x  %08x  %08x  %08x  %08x  %08x  %08x  %08x\n",
                ts.t8, ts.t9, ts.k0, ts.k1, ts.gp, ts.sp, ts.fp, ts.ra);

    lcd_dprintf("$status   $cause    $epc      ---       ---       ---       ---       ---\n");
    lcd_dprintf("%08x  %08x  %08x  %08x  %08x  %08x  %08x  %08x\n",
                ts.cp0_status, ts.cp0_cause, ts.cp0_epc, 0, 0, 0, 0, 0);
    
}

/******************************************************************************/

