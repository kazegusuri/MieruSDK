########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "039.mult"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $2, -1
        mult    $3, $2
        mfhi    $4
        mflo    $5
        mult    $2, $3
        mfhi    $6
        mflo    $7
        
        li      $3, 0x10000
        mult    $3, $2
        mfhi    $8
        mflo    $9
        mult    $2, $3
        mfhi    $10
        mflo    $11
    
        li      $3, -1
        mult    $3, $2
        mfhi    $12
        mflo    $13
        mult    $2, $3
        mfhi    $14
        mflo    $15       
        j       BENCH_END
        nop
        .end    BENCH_BODY


        # $2  ffffffff
        # $3  ffffffff
        # $4  00000000
    
        # $5  00000000
        # $6  00000000
        # $7  00000000
        # $8  0000ffff
        # $9  ffff0000
    
        # $10 0000ffff
        # $11 ffff0000
        # $12 fffffffe
        # $13 00000001
        # $14 fffffffe
        # $15 00000001
    