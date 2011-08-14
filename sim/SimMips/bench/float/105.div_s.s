########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "105.div.s"
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
        div.s   $f12, $f0, $f2
        div.s   $f14, $f2, $f0
        div.s   $f16, $f4, $f6
        div.s   $f18, $f6, $f4       
        div.s   $f20, $f8, $f10
        div.s   $f22, $f10, $f8 
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
BENCH_DATA:
        .float 0.25, -0.5, -0.75, 150, 0.6, 0.3
        
        # $f0  3e800000 0.25
        # $f2  bf000000 -0.5
        # $f4  bf400000 -0.75
        # $f6  43160000 150
        # $f8  3f19999a 0.6
        # $f10 3e99999a 0.3
        # $f12 bf000000 -0.5
        # $f14 c0000000 -2
        # $f16 bba3d70a -0.005
        # $f18 c3480000 -200
        # $f20 40000000 2
        # $f22 3f000000 0.5
    