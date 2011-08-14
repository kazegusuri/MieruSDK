########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "113.mul.d"
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
        mul.d   $f16, $f0, $f2
        mul.d   $f18, $f4, $f4
        mul.d   $f20, $f4, $f6
        mul.d   $f22, $f4, $f8
        mul.d   $f24, $f10, $f10
        mul.d   $f26, $f10, $f12
        mul.d   $f28, $f10, $f14
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
BENCH_DATA:
        .double 3.14, 2, 1.5e+100,-1.5e+100, 2.0e+100,3.3e-90,-3.3e-90, 6.7e-90

        # $f0     3.14
        # $f2     2
        # $f4     1.5e+100
        # $f6     -1.5e+100
        # $f8     2e+100
        # $f10    3.3e-90
        # $f12    -3.3e-90
        # $f14    6.7e-90
        # $f16    6.28
        # $f18    2.25e+200
        # $f20    -2.25e+200
        # $f22    3e+200
        # $f24    1.089e-179
        # $f26    -1.089e-179
        # $f28    2.211e-179
