########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "112.mul.s"
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
        l.s     $f8, 16($2)
        l.s     $f10, 20($2)
        mul.s   $f12, $f0, $f2
        mul.s   $f14, $f2, $f4
        mul.s   $f16, $f4, $f6
        mul.s   $f18, $f6, $f8       
        mul.s   $f20, $f8, $f10 
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
BENCH_DATA:
        .float 0.25, -8, 100000000, 15000, 0.0000006, 0.00003
        
        # $f0  3e800000 0.25
        # $f2  c1000000 -8
        # $f4  4cbebc20 1e+08
        # $f6  466a6000 15000
        # $f8  35210fb0 6e-07
        # $f10 37fba882 3e-05
        # $f12 c0000000 -2
        # $f14 ce3ebc20 -8e+08
        # $f16 53ae9f7c 1.5e+12
        # $f18 3c1374bd 0.009
        # $f20 2d9e5466 1.8e-11
    