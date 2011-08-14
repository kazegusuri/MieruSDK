########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "101.abs neg"
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

        abs.s   $f8, $f0
        abs.s   $f10,$f2
        abs.d   $f12,$f4
        abs.d   $f14,$f6
        
        neg.s   $f16,$f8 
        neg.s   $f18,$f10
        neg.d   $f20,$f12
        neg.d   $f22,$f14
        
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
FLOAT_DATA:
        .float 1.25, -0.625
DOUBLE_DATA:            
        .double 1.23e+100, -9.87e-100

        # $f0     1.25 (f02)
        # $f2     -0.625 (f04)
        # $f4     1.23e+100 (d06)
        # $f6     -9.87e-100 (d08)
        # $f8     1.25 (f08)
        # $f10    0.625 (f10)
        # $f12    1.23e+100 (d12)
        # $f14    9.87e-100 (d14)
        # $f16    -1.25 (f16)
        # $f18    -0.625 (f18)
        # $f20    -1.23e+100 (d20)
        # $f22    -9.87e-100 (d22)
        