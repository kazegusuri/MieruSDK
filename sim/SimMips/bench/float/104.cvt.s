########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "104.cvt"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        la      $2, FLOAT_DATA
        la      $3, DOUBLE_DATA
        l.s     $f0, 0($2)
        l.d     $f2, 0($3)

        cvt.s.d $f4, $f2
        cvt.d.s $f6, $f0

        cvt.s.w $f8, $f0
        cvt.w.s $f10,$f0

        cvt.d.w $f12, $f0
        cvt.w.d $f14, $f2
        
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data

FLOAT_DATA:
        .float 10.25
DOUBLE_DATA:            
        .double 1.234e+5
        
        # $f0     10.25 (f00)
        # $f2     123400 (d02)
        # $f4     123400 (f04)
        # $f6     10.25 (d06)
        # $f8     6.50764e-315 (d08)
        # $f10    0000000a (10)
        # $f12    26.0356 (f13)
        # $f14    1.7292e-40 (f14)
        