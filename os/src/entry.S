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

.macro SAVE_ALL
        addiu   $sp, $sp, -136
        ## the order depends on thread_sturct
        .set noat
        sw      $at,  0($sp)
        .set at
        sw      $v0,  4($sp)
        sw      $v1,  8($sp)
        sw      $a0, 12($sp)
        sw      $a1, 16($sp)
        sw      $a2, 20($sp)
        sw      $a3, 24($sp)
        sw      $t0, 28($sp)
        sw      $t1, 32($sp)
        sw      $t2, 36($sp)
        sw      $t3, 40($sp)
        sw      $t4, 44($sp)
        sw      $t5, 48($sp)
        sw      $t6, 52($sp)
        sw      $t7, 56($sp)
        sw      $s0, 60($sp)
        sw      $s1, 64($sp)
        sw      $s2, 68($sp)
        sw      $s3, 72($sp)
        sw      $s4, 76($sp)
        sw      $s5, 80($sp)
        sw      $s6, 84($sp)
        sw      $s7, 88($sp)
        sw      $t8, 92($sp)
        sw      $t9, 96($sp)
        sw      $k0, 100($sp)
        sw      $k1, 104($sp)
        sw      $gp, 108($sp)
        sw      $sp, 112($sp)
        sw      $fp, 116($sp)
        sw      $ra, 120($sp)
        
        mfc0    $k0, $12      ## load status register
        nop
        sw      $k0, 124($sp)
        mfc0    $k0, $13      ## load cause register
        nop
        sw      $k0, 128($sp)
        mfc0    $k0, $14      ## load epc register
        nop
        sw      $k0, 132($sp)
.endm

.macro RESTORE_ALL
        ## the order depends on thread_sturct
        lw      $k0, 124($sp)
        mtc0    $k0, $12      ## store status register
        lw      $k0, 128($sp)
        mtc0    $k0, $13      ## store cause register
        lw      $k0, 132($sp)
        mtc0    $k0, $14      ## store epc register

        .set noat
        lw      $at,  0($sp)
        .set at
        lw      $v0,  4($sp)
        lw      $v1,  8($sp)
        lw      $a0, 12($sp)
        lw      $a1, 16($sp)
        lw      $a2, 20($sp)
        lw      $a3, 24($sp)
        lw      $t0, 28($sp)
        lw      $t1, 32($sp)
        lw      $t2, 36($sp)
        lw      $t3, 40($sp)
        lw      $t4, 44($sp)
        lw      $t5, 48($sp)
        lw      $t6, 52($sp)
        lw      $t7, 56($sp)
        lw      $s0, 60($sp)
        lw      $s1, 64($sp)
        lw      $s2, 68($sp)
        lw      $s3, 72($sp)
        lw      $s4, 76($sp)
        lw      $s5, 80($sp)
        lw      $s6, 84($sp)
        lw      $s7, 88($sp)
        lw      $t8, 92($sp)
        lw      $t9, 96($sp)
        lw      $k0, 100($sp)
        lw      $k1, 104($sp)
        lw      $gp, 108($sp)
        lw      $sp, 112($sp)
        lw      $fp, 116($sp)
        lw      $ra, 120($sp)
        addiu   $sp, $sp, 136
.endm

        .abicalls
        .text
        .globl exc_entry
        .ent exc_entry
exc_entry:
        .set noreorder
        SAVE_ALL
        move    $a0, $sp       ## set a pointer to thread_struct
        addiu   $sp, $sp, -16  ## for arguments stack ($a0-$a4)
        li      $k1, EXCEPTION_ENTRY_ADDR
        lw      $k1, 0($k1)
        jalr    $k1
        nop
        addiu   $sp, $sp, 16   ## for arguments stack ($a0-$a4)
        RESTORE_ALL
        eret
        nop
        .end exc_entry

