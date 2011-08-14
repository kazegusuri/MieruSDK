########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "016.blt"
        .align  2
        .text
        # .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
        .set    noreorder
BENCH_BODY:
        li      $2, -2
        li      $3, -1
        li      $4, 1
        li      $5, 2
        
        blt     $2, $2, b1
        nop
        addi    $6, $6, 1
b1:
        blt     $2, $3, b2
        nop
        addi    $7, $7, 1
b2:
        blt     $2, $0, b3
        nop
        addi    $8, $8, 1
b3:
        blt     $2, $4, b4
        nop
        addi    $9, $9, 1
b4:
        blt     $3, $2, b5
        nop
        addi    $10, $10, 1
b5:
        blt     $3, $3, b6
        nop
        addi    $11, $11, 1
b6:
        blt     $3, $0, b7
        nop
        addi    $12, $12, 1
b7:
        blt     $3, $4, b8
        nop
        addi    $13, $13, 1
b8:
        blt     $0, $3, b9
        nop
        addi    $14, $14, 1
b9:
        blt     $0, $0, b10
        nop
        addi    $15, $15, 1
b10:
        blt     $0, $4, b11
        nop
        addi    $16, $16, 1
b11:
        blt     $4, $3, b12
        nop
        addi    $17, $17, 1
b12:
        blt     $4, $0, b13
        nop
        addi    $18, $18, 1
b13:
        blt     $4, $3, b14
        nop
        addi    $19, $19, 1
b14:
        blt     $4, $4, b15
        nop
        addi    $20, $20, 1
b15:
        blt     $5, $3, b16
        nop
        addi    $21, $21, 1
b16:
        blt     $5, $0, b17
        nop
        addi    $22, $22, 1
b17:
        blt     $5, $4, b18
        nop
        addi    $23, $23, 1
b18:
        blt     $5, $5, b19
        nop
        addi    $24, $24, 1
b19:

        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $2 fffffffe
        # $3 ffffffff
        # $4 00000001
        
        # $5 00000002
        # $6 00000001
        # $7 00000000
        # $8 00000000
        # $9 00000000
        
        # $10 00000001
        # $11 00000001
        # $12 00000000
        # $13 00000000
        # $14 00000000

        # $15 00000001
        # $16 00000000
        # $17 00000001
        # $18 00000001
        # $19 00000001
    
        # $20 00000001
        # $21 00000001
        # $22 00000001
        # $23 00000001
        # $24 00000001
        