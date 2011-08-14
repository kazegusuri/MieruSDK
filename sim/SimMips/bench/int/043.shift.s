########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "043.shift"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, -1       # ffffffff
        sll     $2, $1, 1    # fffffffe
        sll     $3, $1, 31   # 80000000
        sll     $4, $3, 1    # 00000000
                             
        srl     $5, $1, 1    # 7fffffff
        srl     $6, $1, 31   # 00000001
        srl     $7, $6, 1    # 00000000
        sra     $8, $3, 15   # ffff0000
        sra     $9, $5, 15   # 0000ffff
                             
        li      $10, 4       # 00000004
        sllv    $11, $1, $10 # fffffff0
        srlv    $12, $1, $10 # 0fffffff
        srav    $13, $3, $10 # f8000000
        srav    $14, $5, $10 # 07ffffff
        
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  ffffffff
        # $2  fffffffe
        # $3  80000000
        # $4  00000000
        
        # $5  7fffffff
        # $6  00000001
        # $7  00000000
        # $8  ffff0000
        # $9  0000ffff
        
        # $10 00000004
        # $11 fffffff0
        # $12 0fffffff
        # $13 f8000000
        # $14 07ffffff
        