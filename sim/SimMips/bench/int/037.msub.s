########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "037.msub"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, -1
        li      $2, 2
        li      $3, 3
    
        msub    $1, $2
        mfhi    $4
        mflo    $5
        msub    $2, $3
        mfhi    $6
        mflo    $7
        msub    $3, $1
        mfhi    $8
        mflo    $9
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  ffffffff
        # $2  00000002
        # $3  00000003
        # $4  fffffffe
        # $5  00000002
        # $6  fffffffd
        # $7  fffffffc
        # $8  fffffffa
        # $9  ffffffff
        