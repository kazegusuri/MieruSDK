########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "114.recip"
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

        recip.s $f8,$f0
        recip.s $f10,$f2
        recip.d $f12,$f4
        recip.d $f14,$f6
    
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
FLOAT_DATA:
        .float 1.25, -0.625
DOUBLE_DATA:            
        .double 1.23e+50, 9.87e-50

        # $f0     1.25 (f00)
        # $f2     -0.625 (f02)
        # $f4     1.23e+50 (d04)
        # $f6     9.87e-50 (d06)
        # $f8     0.8 (f08)
        # $f10    -1.6 (f10)
        # $f12    8.13008e-51 (d12)
        # $f14    1.01317e+49 (d14)
        