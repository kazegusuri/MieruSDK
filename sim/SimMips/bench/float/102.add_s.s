########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "102.add.s"
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
        add.s   $f8, $f0, $f2
        add.s   $f10, $f4, $f6
        add.s   $f12, $f8, $f10        
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
BENCH_DATA:
        .float 1.25, -0.625, 3.125, -12.3125

        # $f0  1.25
        # $f2  -0.625
        # $f4  3.125
        # $f6  -12.3125
        # $f8  0.625
        # $f10 -9.1875
        # $f12 -8.5625
    