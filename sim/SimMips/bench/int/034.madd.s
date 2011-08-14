########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "034.madd"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, -1
        li      $2, 2
        li      $3, 3
    
        madd    $1, $2
        mfhi    $4
        mflo    $5
        madd    $2, $3
        mfhi    $6
        mflo    $7
        madd    $3, $1
        mfhi    $8
        mflo    $9
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  ffffffff
        # $2  00000002
        # $3  00000003
        # $4  00000001
        
        # $5  fffffffe
        # $6  00000002
        # $7  00000004
        # $8  00000005
        # $9  00000001
        