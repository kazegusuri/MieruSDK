########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "120.sub.d"
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
        l.d     $f8, 32($2)
        l.d     $f10, 40($2)
        l.d     $f12, 48($2)
        l.d     $f14, 56($2)
        sub.d   $f16, $f0, $f2
        sub.d   $f18, $f4, $f4
        sub.d   $f20, $f4, $f6
        sub.d   $f22, $f4, $f8
        sub.d   $f24, $f10, $f10
        sub.d   $f26, $f10, $f12
        sub.d   $f28, $f10, $f14
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
BENCH_DATA:
        .double 1.25, 0.625, 1.234e+100, -1.234e+100, 5.678e+100
        .double 6.543e-90, -6.543e-90, 9.876e-90

        # $f0     1.25
        # $f2     0.625
        # $f4     1.234e+100
        # $f6     -1.234e+100
        # $f8     5.678e+100
        # $f10    6.543e-90
        # $f12    -6.543e-90
        # $f14    9.876e-90
        # $f16    0.625
        # $f18    0
        # $f20    2.468e+100
        # $f22    -4.444e+100
        # $f24    0
        # $f26    1.3086e-89
        # $f28    -3.333e-90
    