########################################################################
#   MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       # 
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "044.slt"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, -2
        li      $2, 0
        li      $3, 2
        slt     $4, $1, $2
        
        slt     $5, $2, $3
        slt     $6, $3, $1
        slt     $7, $1, $1
        slt     $8, $2, $2
        slt     $9, $3, $3
        
        slti    $10, $1, -3
        slti    $11, $1, -2
        slti    $12, $1, -1
        slti    $13, $1, 0
        slti    $14, $1, 1
        
        slti    $15, $2, -1
        slti    $16, $2, 0
        slti    $17, $2, 1
        
        slti    $18, $3, -1
        slti    $19, $3, 0
        slti    $20, $3, 1
        slti    $21, $3, 2
        slti    $22, $3, 3    

        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  fffffffe
        # $2  00000000
        # $3  00000002
        # $4  00000001
        
        # $5  00000001
        # $6  00000000
        # $7  00000000
        # $8  00000000
        # $9  00000000
        
        # $10 00000000
        # $11 00000000
        # $12 00000001
        # $13 00000001
        # $14 00000001
        
        # $15 00000000
        # $16 00000000
        # $17 00000001
        # $18 00000000
        # $19 00000000
        
        # $20 00000000
        # $21 00000000
        # $22 00000001
        