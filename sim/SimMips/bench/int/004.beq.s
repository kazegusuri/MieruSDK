########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "004.beq"
        .align  2
        .text
        .globl  BENCH_BODY
        .ent    BENCH_BODY
        .set    noreorder
BENCH_BODY:
	   li      $2, -2      # fffffffe
        li      $3, -1      # ffffffff
        li      $4,	1       # 00000001
        li      $5, 2       # 00000002
        
        beq     $2, $2, b1
        nop
        addi    $6, $6, 1   # skip
b1:
        beq     $2, $3, b2
        nop
        addi    $7, $7, 1   # 00000001
b2:
        beq     $2, $0, b3
        nop
        addi    $8, $8, 1   # 00000001
b3:
        beq     $2, $4, b4
        nop
        addi    $9, $9, 1   # 00000001
b4:
        beq     $3, $2, b5
        nop
        addi    $10, $10, 1 # 00000001
b5:
        beq     $3, $3, b6
        nop
        addi    $11, $11, 1 # skip
b6:
        beq     $3, $0, b7
        nop
        addi    $12, $12, 1 # 00000001
b7:
        beq     $3, $4, b8
        nop
        addi    $13, $13, 1 # 00000001
b8:
        beq     $0, $3, b9
        nop
        addi    $14, $14, 1 # 00000001
b9:
        beq     $0, $0, b10
        nop
        addi    $15, $15, 1 # skip
b10:
        beq     $0, $4, b11
        nop
        addi    $16, $16, 1 # 00000001
b11:
        beq     $4, $3, b12
        nop
        addi    $17, $17, 1 # 00000001
b12:
        beq     $4, $0, b13
        nop
        addi    $18, $18, 1 # 00000001
b13:
        beq     $4, $3, b14
        nop
        addi    $19, $19, 1 # 00000001
b14:
        beq     $4, $4, b15
        nop
        addi    $20, $20, 1 # skip
b15:
        beq     $5, $3, b16
        nop
        addi    $21, $21, 1 # 00000001
b16:
        beq     $5, $0, b17
        nop
        addi    $22, $22, 1 # 00000001
b17:
        beq     $5, $4, b18
        nop
        addi    $23, $23, 1 # 00000001
b18:
        beq     $5, $5, b19
        nop
        addi    $24, $24, 1 # skip
b19:
        
        j BENCH_END
        nop
        .end    BENCH_BODY

        # $2 fffffffe
        # $3 ffffffff
        # $4 00000001
        
        # $5 00000002
        # $6 00000000
        # $7 00000001
        # $8 00000001
        # $9 00000001
        
        # $10 00000001
        # $11 00000000
        # $12 00000001
        # $13 00000001
        # $14 00000001

        # $15 00000000
        # $16 00000001
        # $17 00000001
        # $18 00000001
        # $19 00000001
        
        # $20 00000000
        # $21 00000001
        # $22 00000001
        # $23 00000001
        # $24 00000000
        