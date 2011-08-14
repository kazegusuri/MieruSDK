########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "115.round trunc ceil floor"
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
        round.w.s $f4, $f0
        trunc.w.s $f6, $f0
        ceil.w.s  $f8, $f0
        floor.w.s $f10, $f0

        round.w.d $f12, $f2
        trunc.w.d $f14, $f2
        ceil.w.d  $f16, $f2
        floor.w.d $f18, $f2
        
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data

FLOAT_DATA:
        .float  10.25
DOUBLE_DATA:            
        .double 1.23456e+3
        
        # $f0     10.25 (f00)
        # $f2     1234.56 (d02)
        # $f4     0000000a (f04)
        # $f6     0000000a (f06)
        # $f8     0000000b (f08)
        # $f10    0000000a (f10)
        # $f12    000004d3 (f12)
        # $f14    000004d2 (f14)
        # $f16    000004d3 (f16)
        # $f18    000004d2 (f18)
        