########################################################################
#  MIPS Instruction Test Benchmark fxor MieruPC   Arch Lab. TOKYO TECH #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "003.and"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
	   li      $1, 0x55555555
        li      $2, 0xaaaaaaaa
        and     $3, $1, $2
        li      $4, -1
        andi    $5, $4, 0xf
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  55555555
        # $2  aaaaaaaa
        # $3  00000000
        # $4  ffffffff
        # $5  0000000f
        