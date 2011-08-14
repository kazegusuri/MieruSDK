########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "022.bnel"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
        .set    noreorder
BENCH_BODY:
        li      $2, -2     # fffffffe
        li      $3, -1     # ffffffff
        li      $4, 1      # 00000001
        li      $5, 2      # 00000002
        
        bnel    $2, $2, b1
        addi    $6, $6, 1
        addi    $7, $7, 1
b1:
        bnel    $2, $3, b2
        addi    $6, $6, 1
        addi    $8, $8, 1
b2:
        bnel    $2, $0, b3
        addi    $6, $6, 1
        addi    $9, $9, 1
b3:
        bnel    $2, $4, b4
        addi    $6, $6, 1
        addi    $10, $10, 1
b4:
        bnel    $3, $2, b5
        addi    $6, $6, 1
        addi    $11, $11, 1
b5:
        bnel    $3, $3, b6
        addi    $6, $6, 1
        addi    $12, $12, 1
b6:
        bnel    $3, $0, b7
        addi    $6, $6, 1
        addi    $13, $13, 1
b7:
        bnel    $3, $4, b8
        addi    $6, $6, 1
        addi    $14, $14, 1
b8:
        bnel    $0, $2, b9
        addi    $6, $6, 1
        addi    $15, $15, 1
b9:
        bnel    $0, $0, b10
        addi    $6, $6, 1
        addi    $16, $16, 1
b10:
        bnel    $0, $4, b11
        addi    $6, $6, 1
        addi    $17, $17, 1
b11:
        bnel    $4, $3, b12
        addi    $6, $6, 1
        addi    $18, $18, 1
b12:
        bnel    $4, $0, b13
        addi    $6, $6, 1
        addi    $19, $19, 1
b13:
        bnel    $4, $3, b14
        addi    $6, $6, 1
        addi    $20, $20, 1
b14:
        bnel    $4, $4, b15
        addi    $6, $6, 1
        addi    $21, $21, 1
b15:
        bnel    $5, $3, b16
        addi    $6, $6, 1
        addi    $22, $22, 1
b16:
        bnel    $5, $0, b17
        addi    $6, $6, 1
        addi    $23, $23, 1
b17:
        bnel    $5, $4, b18
        addi    $6, $6, 1
        addi    $24, $24, 1
b18:
        bnel    $5, $5, b19
        addi    $6, $6, 1
        addi    $25, $25, 1
b19:

        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $2 fffffffe
        # $3 ffffffff
        # $4 00000001
        
        # $5 00000002
        # $6 0000000e
        # $7 00000001
        # $8 00000000
        # $9 00000000
        
        # $10 00000000
        # $11 00000000
        # $12 00000001
        # $13 00000000
        # $14 00000000

        # $15 00000000
        # $16 00000001
        # $17 00000000
        # $18 00000000
        # $19 00000000
    
        # $20 00000000
        # $21 00000001
        # $22 00000000
        # $23 00000000
        # $24 00000000

        # $25 00000001
        