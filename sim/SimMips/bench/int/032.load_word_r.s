########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "032.lwr"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $2, 0x12345678
        la      $3, as_buf

        sw      $2, 0($3)        
    
        lwr     $4, 0($3)
        lwr     $5, 1($3)
        lwr     $6, 2($3)
        lwr     $7, 3($3)
        lwr     $8, 4($3)            
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
as_buf:
        .space 256
    
        # $2 12345678
        # $3 00416044
        # $4 12345678
        
        # $5 00123456
        # $6 00001234
        # $7 00000012
        # $8 00000000
        