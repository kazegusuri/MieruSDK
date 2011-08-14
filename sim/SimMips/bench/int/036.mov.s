########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "036.mov"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $2, 0xffffffff
        li      $3, 0x00000001
        li      $4, 0x80000000
        move    $5, $0
        move    $6, $2
        move    $7, $3
        move    $8, $4
    
        mthi    $0
        mfhi    $9
        mthi    $2
        mfhi    $10
        mthi    $3
        mfhi    $11
        mthi    $4
        mfhi    $12

        mtlo    $0
        mflo    $13
        mtlo    $2
        mflo    $14
        mtlo    $3
        mflo    $15
        mtlo    $4
        mflo    $16

        movz    $17, $2, $0
        movz    $18, $2, $2
        movz    $19, $3, $0
        movz    $20, $3, $2

        movn    $21, $2, $0
        movn    $22, $2, $2
        movn    $23, $3, $0
        movn    $24, $3, $2

        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $2     0000000f
        # $3     0000000f
        # $4     00000000
        # $5     00000002
        # $6     00000002
        # $7     00000003
        # $8     00000003
        # $9     0000000f
        # $10    00000000
        # $11    00000000
        # $12    0000000f
        