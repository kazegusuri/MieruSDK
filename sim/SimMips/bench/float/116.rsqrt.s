########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "116.rsqrt"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        la      $2, FLOAT_DATA
        la      $3, DOUBLE_DATA
        l.s     $f0, 0($2)
        l.s     $f2, 4($2)
        l.d     $f4, 0($3)
        l.d     $f6, 8($3)

        rsqrt.s  $f8,  $f0
        rsqrt.s  $f10, $f2

        rsqrt.d  $f12, $f4
        rsqrt.d  $f14, $f6
        

        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
FLOAT_DATA:
        .float  4, 1.96 
DOUBLE_DATA:
        .double 9e+10, 16e+100

        # $f0     4 (f00)
        # $f2     1.96 (f02)
        # $f4     9e+10 (d04)
        # $f6     1.6e+101 (d06)
        # $f8     0.5 (f08)
        # $f10    0.714286 (f10)
        # $f12    3.33333e-06 (d12)
        # $f14    2.5e-51 (d14)
        