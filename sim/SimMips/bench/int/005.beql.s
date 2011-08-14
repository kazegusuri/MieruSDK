########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "005.beql"
        .align  2
        .text
        .globl  BENCH_BODY
        .ent    BENCH_BODY
        .set    noreorder
BENCH_BODY:
        li      $2, -2        # fffffffe
        li      $3, -1        # ffffffff
        li      $4,	1         # 00000001
        li      $5, 2         # 00000002
        
        beql    $2, $2, b1
        addi    $6, $6, 1
        addi    $7, $7, 1     # skip
b1:
        beql    $2, $3, b2
        addi    $6, $6, 1
        addi    $8, $8, 1     # 00000001
b2:
        beql    $2, $0, b3
        addi    $6, $6, 1
        addi    $9, $9, 1     # 00000001
b3:
        beql    $2, $4, b4
        addi    $6, $6, 1
        addi    $10, $10, 1   # 00000001
b4:
        beql    $3, $2, b5
        addi    $6, $6, 1
        addi    $11, $11, 1   # 00000001
b5:
        beql    $3, $3, b6
        addi    $6, $6, 1
        addi    $12, $12, 1   # skip
b6:
        beql    $3, $0, b7
        addi    $6, $6, 1
        addi    $13, $13, 1   # 00000001
b7:
        beql    $3, $4, b8
        addi    $6, $6, 1
        addi    $14, $14, 1   # 00000001
b8:

    
        beql    $0, $2, b9
        addi    $6, $6, 1
        addi    $15, $15, 1   # 00000001
b9:
        beql    $0, $0, b10
        addi    $6, $6, 1
        addi    $16, $16, 1   # skip
b10:
        beql    $0, $4, b11
        addi    $6, $6, 1
        addi    $17, $17, 1   # 00000001
b11:

    
        beql    $4, $3, b12
        addi    $6, $6, 1
        addi    $18, $18, 1   # 00000001
b12:
        beql    $4, $0, b13
        addi    $6, $6, 1
        addi    $19, $19, 1   # 00000001
b13:
        beql    $4, $3, b14
        addi    $6, $6, 1
        addi    $20, $20, 1   # 00000001
b14:
        beql    $4, $4, b15
        addi    $6, $6, 1
        addi    $21, $21, 1   # skip
b15:

        beql    $5, $3, b16
        addi    $6, $6, 1
        addi    $22, $22, 1   # 00000001
b16:
        beql    $5, $0, b17
        addi    $6, $6, 1
        addi    $23, $23, 1   # 00000001
b17:
        beql    $5, $3, b18
        addi    $6, $6, 1
        addi    $24, $24, 1   # 00000001
b18:
        beql    $5, $4, b19
        addi    $6, $6, 1
        addi    $25, $25, 1   # 00000001
b19:
        j BENCH_END
        nop
        .end    BENCH_BODY
        
        # $2 fffffffe
        # $3 ffffffff
        # $4 00000001
        
        # $5 00000002
        # $6 00000004
        # $7 00000000
        # $8 00000001
        # $9 00000001
          
        # $10 00000001
        # $11 00000001
        # $12 00000000
        # $13 00000001
        # $14 00000001
          
        # $15 00000001
        # $16 00000000
        # $17 00000001
        # $18 00000001
        # $19 00000001
          
        # $20 00000001
        # $21 00000000
        # $22 00000001
        # $23 00000001
        # $24 00000001
          
        # $25 00000001
        