########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "109.mfc1"
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

        mfc1    $2,  $f0
        mfc1    $3,  $f2
        mfc1    $4,  $f4
        mfc1    $5,  $f5
        mfc1    $6,  $f6
        mfc1    $7,  $f7

        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
FLOAT_DATA:
        .float 1.25, -0.625
DOUBLE_DATA:            
        .double 1.23e+100, 9.87e-100

        # $2     3fa00000
        # $3     bf200000
        # $4     40250f2c
        
        # $5     54b67e76
        # $6     91265429
        # $7     2b614541
    
        # $f0     1.25 (f00)
        # $f2     -0.625 (f02)
        # $f4     1.23e+100 (d04)
        # $f6     9.87e-100 (d06)
        