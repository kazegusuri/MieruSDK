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
        b       .SYSCALL_JMP # syscall tmp        
        nop

.SYSCALL_JMP:
        SAVE_SYS
        li      $t0, 4000      ## check syscall_nr
        ble     $t0, $v0, $end_sys_jmp
        nop
        li      $t0, 0x300     ## syscall table
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
