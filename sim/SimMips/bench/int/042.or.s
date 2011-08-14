########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "042.or"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, 0x55555555   # 55555555
        li      $2, 0xaaaaaaaa   # aaaaaaaa
        or      $3, $1, $2       # ffffffff
        or      $4, $1, $1       # 55555555
        ori     $5, $2, 0xf      # aaaaaaaf
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  55555555
        # $2  aaaaaaaa
        # $3  ffffffff
        # $4  55555555
        # $5  aaaaaaaf
        