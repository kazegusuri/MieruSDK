########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "033.lui la"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        nop
BENCH2: 
        lui     $2, 0xaaaa
        lui     $3, 0xffff
        la      $4, BENCH_BODY
        la      $5, BENCH2
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $2 aaaa0000
        # $3 ffff0000
        # $4 address of BENCH_BODY
        # $5 $4+4
        