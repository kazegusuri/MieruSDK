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

.macro	INIT_REG
		.set 	noat
        move    $at, $0
		.set	 at
        move    $v0, $0
        move    $v1, $0
        move    $a0, $0
        move    $a1, $0
        move    $a2, $0
        move    $a3, $0
        move    $t0, $0
        move    $t1, $0
        move    $t2, $0
        move    $t3, $0
        move    $t4, $0
        move    $t5, $0
        move    $t6, $0
        move    $t7, $0
        move    $t8, $0
        move    $t9, $0
        move    $s0, $0
        move    $s1, $0
        move    $s2, $0
        move    $s3, $0
        move    $s4, $0
        move    $s5, $0
        move    $s6, $0
        move    $s7, $0
        move    $k0, $0
        move    $k1, $0
        move    $gp, $0
        move    $fp, $0
        move    $ra, $0
.endm

		.data
		.extern main
		.abicalls
		.text
        .globl _start
        .ent _start
_start:
        .set noreorder
		j		.INIT
        nop
		j		.ERROR
		nop

.INIT:
		INIT_REG
		sw		$zero, 0($zero)
		li		$sp, 0x1fff0		# 128KB
		j		main
		nop

.ERROR:
		j		.ERROR
		nop
        .end _start
