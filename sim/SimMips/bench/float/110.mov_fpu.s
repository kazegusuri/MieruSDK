########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "110.mov(fpu)"
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

        mov.s   $f8,  $f0
        mov.s   $f10, $f2
        mov.d   $f12, $f4
        mov.d   $f14, $f6

        c.eq.s  $f0,  $f0
        li      $a2,  1
        movz.s  $f16, $f0, $a2
        movn.s  $f18, $f0, $a2
        movz.d  $f20, $f4, $a0
        movn.d  $f22, $f4, $a0
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
FLOAT_DATA:
        .float 1.25, -0.625
DOUBLE_DATA:            
        .double 1.23e+100, 9.87e-100

        # $f0     1.25 (f00)
        # $f2     -0.625 (f02)
        # $f4     1.23e+100 (d04)
        # $f6     9.87e-100 (d06)
        # $f8     1.25 (f08)
        # $f10    -0.625 (f10)
        # $f12    1.23e+100 (d12)
        # $f14    9.87e-100 (d14)
        # $f16    0
        # $f18    1.25 (f18)
        # $f20    1.23e+100 (d20)
        