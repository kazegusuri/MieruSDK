########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "119.sub.s"
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
        sub.s   $f12, $f0, $f2
        sub.s   $f14, $f2, $f0
        sub.s   $f16, $f4, $f6
        sub.s   $f18, $f6, $f4
        sub.s   $f20, $f8, $f10
        sub.s   $f22, $f10,$f8
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
BENCH_DATA:
        .float 0.25, -8, 100000000, 15000, 0.0000000006, 0.00003
        
        # $f0  3e800000 0.25
        # $f2  c1000000 -8
        # $f4  4cbebc20 1e+08
        # $f6  466a6000 15000
        # $f8  3024ed3f 6e-10
        # $f10 37fba882 3e-05
        # $f12 41040000 8.25
        # $f14 c1040000 -8.25
        # $f16 4cbeb4cd 9.9985e+07
        # $f18 ccbeb4cd -9.9985e+07
        # $f20 b7fba738 -2.99994e-05
        # $f22 37fba738 2.99994e-05
    