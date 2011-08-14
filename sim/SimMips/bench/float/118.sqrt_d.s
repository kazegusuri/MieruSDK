########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "sqrt double"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        la      $2, BENCH_DATA
        l.d     $f0, 0($2)
        l.d     $f2, 8($2)
        l.d     $f4, 16($2)
        l.d     $f6, 24($2)

        sqrt.d  $f8, $f0
        sqrt.d  $f10, $f2
        sqrt.d  $f12,$f4
        sqrt.d  $f14,$f6
        
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
BENCH_DATA:
        .double 4, 3, 9e+20,16e+100

        # $f0     4
        # $f2     3
        # $f4     9e+20
        # $f6     1.6e+101
        # $f8     2
        # $f10    1.73205
        # $f12    3e+10
        # $f14    4e+50
