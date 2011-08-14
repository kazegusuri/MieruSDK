########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "048.xor"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, 0x55555555
        li      $2, 0xaaaaaaaa
        xor     $3, $1, $2
        xor     $4, $1, $1
        xori    $5, $2, 0xf
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  55555555
        # $2  aaaaaaaa
        # $3  ffffffff
        # $4  00000000
    
        # $5  aaaaaaa5
        