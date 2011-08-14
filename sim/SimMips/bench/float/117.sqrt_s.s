########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "117.sqrt.s"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        la      $2, BENCH_DATA
        l.s     $f0, 0($2)
        l.s     $f2, 4($2)
        l.s     $f4, 8($2)
        l.s     $f6, 12($2)

        sqrt.s  $f8, $f0
        sqrt.s  $f10, $f2
        sqrt.s  $f12,$f4
        sqrt.s  $f14,$f6
        
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
BENCH_DATA:
        .float 4, 3, 9e+20,16e+20

        # $f0     4
        # $f2     3
        # $f4     9e+20
        # $f6     1.6e+21
        # $f8     2
        # $f10    1.73205
        # $f12    3e+10
        # $f14    4e+10
