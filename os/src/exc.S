################################################################################
##  MieruPC Project: Exception Handler v0.2                                    #
##	written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-07-16 #
################################################################################

##
## Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.
## All rights reserved. 
## 
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions
## are met:
##
## 1. Redistributions of source code must retain the above copyright notice,
##    this list of conditions and the following disclaimer.
## 2. Redistributions in binary form must reproduce the above copyright notice,
##    this list of conditions and the following disclaimer in the documentation
##    and/or other materials provided with the distribution.
## 3. The name of the author may not be used to endorse or promote products
##    derived from this software without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
## IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
## OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
## IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
## INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
## NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
## DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
## THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
## (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
## THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##

#define __ASSEMBLY__
#include <exception.h>
.extern __TIMER_HANDLER_ADDR

.abicalls
.macro SAVE_ALL
        addiu   $sp, $sp, -136
        sw      $sp,  0($sp)
        sw      $fp,  4($sp)
        sw      $ra,  8($sp)
        sw      $gp, 12($sp)
        sw      $t0, 16($sp)
        sw      $t1, 20($sp)
        sw      $t2, 24($sp)
        sw      $t3, 28($sp)
        sw      $t4, 32($sp)
        sw      $t5, 36($sp)
        sw      $t6, 40($sp)
        sw      $t7, 44($sp)
        sw      $t8, 48($sp)
        sw      $t9, 52($sp)
        sw      $s0, 56($sp)
        sw      $s1, 60($sp)
        sw      $s2, 64($sp)
        sw      $s3, 68($sp)
        sw      $s4, 72($sp)
        sw      $s5, 76($sp)
        sw      $s6, 80($sp)
        sw      $s7, 84($sp)
        .set    noat
        sw      $at, 88($sp)
        .set    at
        sw      $v0, 92($sp)
        sw      $v1, 96($sp)
        sw      $a0, 100($sp)
        sw      $a1, 104($sp)
        sw      $a2, 108($sp)
        sw      $a3, 112($sp)
        
        sw      $k0, 116($sp)
        sw      $k1, 120($sp)
        mfc0    $k0, $12      ## load sr register
        sw      $k0, 124($sp)
        mfc0    $k0, $13      ## load cause register
        sw      $k0, 128($sp)
        mfc0    $k0, $14      ## load epc register
        sw      $k0, 132($sp)
        addiu   $sp, $sp, -16 ## for arguments stack ($a0-$a4)
.endm

.macro RESTORE_ALL
        addiu   $sp, $sp, 16 ## for arguments stack ($a0-$a4)
        lw      $sp,  0($sp)
        lw      $fp,  4($sp)
        lw      $ra,  8($sp)
        lw      $gp, 12($sp)
        lw      $t0, 16($sp)
        lw      $t1, 20($sp)
        lw      $t2, 24($sp)
        lw      $t3, 28($sp)
        lw      $t4, 32($sp)
        lw      $t5, 36($sp)
        lw      $t6, 40($sp)
        lw      $t7, 44($sp)
        lw      $t8, 48($sp)
        lw      $t9, 52($sp)
        lw      $s0, 56($sp)
        lw      $s1, 60($sp)
        lw      $s2, 64($sp)
        lw      $s3, 68($sp)
        lw      $s4, 72($sp)
        lw      $s5, 76($sp)
        lw      $s6, 80($sp)
        lw      $s7, 84($sp)
        .set    noat
        lw      $at, 88($sp)
        .set    at
        lw      $v0, 92($sp)
        lw      $v1, 96($sp)
        lw      $a0, 100($sp)
        lw      $a1, 104($sp)
        lw      $a2, 108($sp)
        lw      $a3, 112($sp)


        lw      $k0, 124($sp)
        mtc0    $k0, $12      ## store sr register
        lw      $k0, 128($sp)
        mtc0    $k0, $13      ## store cause register
        lw      $k0, 132($sp)
        mtc0    $k0, $14      ## store epc register

        lw      $k0, 116($sp)
        lw      $k1, 120($sp)
        
        addiu   $sp, $sp, 136
.endm

        
.macro SAVE_SYS
        addiu   $sp, $sp, -60
        sw      $sp,  0($sp)
        sw      $fp,  4($sp)
        sw      $ra,  8($sp)
        sw      $gp, 12($sp)
        sw      $t0, 16($sp)
        sw      $t1, 20($sp)
        sw      $t2, 24($sp)
        sw      $t3, 28($sp)
        sw      $t4, 32($sp)
        sw      $t5, 36($sp)
        sw      $t6, 40($sp)
        sw      $t7, 44($sp)
        sw      $t8, 48($sp)
        sw      $t9, 52($sp)
#        mfc0    $k0, $14      ## load from epc register
#        sw      $k0, 56($sp)
        addiu   $sp, $sp, -16 ## for arguments stack ($a0-$a4)
.endm

.macro RESTORE_SYS
        addiu   $sp, $sp, 16 ## for arguments stack ($a0-$a4)
        lw      $sp,  0($sp)
        lw      $fp,  4($sp)
        lw      $ra,  8($sp)
        lw      $gp, 12($sp)
        lw      $t0, 16($sp)
        lw      $t1, 20($sp)
        lw      $t2, 24($sp)
        lw      $t3, 28($sp)
        lw      $t4, 32($sp)
        lw      $t5, 36($sp)
        lw      $t6, 40($sp)
        lw      $t7, 44($sp)
        lw      $t8, 48($sp)
        lw      $t9, 52($sp)
#        lw      $k0, 56($sp)
#        mtc0    $k0, $14      ## store to epc register
        addiu   $sp, $sp, 60
.endm

        .text
        .globl exc_entry
        .ent exc_entry
        .extern syscall_table
exc_entry:
        mfc0    $k0, $13       ## get cause reg
        li      $k1, 8
        srl     $k0, $k0, 2    ##
        andi    $k0, $k0, 0x1f ##
        beq     $k0, $k1, .SYSCALL
        nop
        li      $k1, 0
        beq     $k0, $k1, .TIMER
        nop
        b       -4
        nop

.TIMER:
        mfc0    $k0, $11
        addiu   $k0, $k0, 0x100
        mtc0    $k0, $11
        nop
        SAVE_ALL
#        la      $k0, timer_exception
        li      $k0, TIMER_HANDLER_ADDR
        lw      $k0, 0($k0)
        nop
        jalr     $k0
        nop
        RESTORE_ALL
        nop
        eret
        nop

.SYSCALL:
        SAVE_SYS
        li      $k0, 1
        mtc0    $k0, $12       ## clear status reg
        
        li      $t0, 4000      ## check syscall_nr
        ble     $t0, $v0, $end_sys
        nop
        li      $t0, SYSCALL_TABLE_ADDR     ## syscall table
        lw      $t0, 0($t0)
#        li      $t0, 0x600     ## syscall table
#        la      $t0, __syscall_table     ## syscall table
#        la      $t0, __TIMER_HANDLER_ADDR
        sll     $v0, $v0, 2
        addu    $t0, $t0, $v0  ## table[syscall_nr]
        lw      $t0, 0($t0)
        nop
        jalr    $t0
        nop
$end_sys:
        RESTORE_SYS
        mfc0    $k0, $14       ## load epc
        addi    $k0, $k0, 4    ## increment epc
        mtc0    $k0, $14       ## for pointing next instruction
        eret                   
        nop

.INT:
        eret
        nop


.SYSCALL_JMP:
        SAVE_SYS
        li      $t0, 4000      ## check syscall_nr
        ble     $t0, $v0, $end_sys_jmp
        nop
        li      $t0, 0x600     ## syscall table
        sll     $v0, $v0, 2
        addu    $t0, $t0, $v0  ## table[syscall_nr]
        lw      $t0, 0($t0)
        nop
        jalr    $t0
        nop
$end_sys_jmp:
        RESTORE_SYS
        jr      $ra
        nop

        .end exc_entry
