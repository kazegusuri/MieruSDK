########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "041.nor"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, 0x55555555
        li      $2, 0xaaaaaaaa
        li      $3, 0x0000000a   
        nor     $4, $1, $2
        nor     $5, $1, $1
        nor     $6, $1, $3
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  55555555
        # $2  aaaaaaaa
        # $3  0000000a
        # $4  00000000
        # $5  aaaaaaaa
        # $6  aaaaaaa0
        