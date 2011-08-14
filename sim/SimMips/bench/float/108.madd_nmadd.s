########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "108.madd"
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
        l.s     $f4, 8($2)
        l.d     $f6, 0($3)
        l.d     $f8, 8($3)
        l.d     $f10,16($3)

        madd.s  $f12, $f0, $f2, $f4
        nmadd.s  $f14, $f0, $f2, $f4

        madd.d  $f16, $f6, $f8, $f10
        nmadd.d  $f18, $f6, $f8, $f10

        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
FLOAT_DATA:
        .float 4, 1.25,0.625
DOUBLE_DATA:    
        .double 1e-100, 5e-50,7e-50 

        # $f0     4 (f00)
        # $f2     1.25 (f02)
        # $f4     0.625 (f04)
        # $f6     1e-100 (d06)
        # $f8     5e-50 (d08)
        # $f10    7e-50 (d10)
        # $f12    4.78125 (f12)
        # $f14    -4.78125 (f14)
        # $f16    3.6e-99 (d16)
        # $f18    -3.6e-99 (d18)
