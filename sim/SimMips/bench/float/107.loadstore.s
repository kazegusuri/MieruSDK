########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "107.loadstore(fpu)"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        la      $2, FLOAT_DATA
        la      $3, DOUBLE_DATA
        la      $4, STORE_DATA
        l.s     $f0, 0($2)
        lwc1    $f2, 4($2)
        l.d     $f4, 0($3)
        ldc1    $f6, 8($3)

        swc1    $f0, 16($3)
        swc1    $f2, 24($3)
        sdc1    $f4, 32($3)
        sdc1    $f6, 40($3)

        l.s     $f8,  16($3)
        l.s     $f10, 24($3)
        l.d     $f12, 32($3)
        l.d     $f14, 40($3)

        swxc1   $f0, $0($4)
        lwxc1   $f16, $0($4)

        
        sdxc1   $f4, $0($4)
        ldxc1   $f18, $0($4)    
    
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
FLOAT_DATA:
        .float 1.25, -0.625
DOUBLE_DATA:            
        .double 1.23e+100, 9.87e-100
STORE_DATA:
        
        # $f0     1.25 (f00)
        # $f2     -0.625 (f02)
        # $f4     1.23e+100 (d04)
        # $f6     9.87e-100 (d06)
        # $f8     1.25 (f08)
        # $f10     -0.625 (f10)
        # $f12    1.23e+100 (d12)
        # $f14    9.87e-100 (d14)
        # $f16    1.25 (f16)
        # $f18    1.23e+100 (f18)
        