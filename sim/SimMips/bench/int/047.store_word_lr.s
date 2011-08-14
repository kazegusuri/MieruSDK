########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "047.swl swr"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $2, 0x12345678
        la      $3, as_buf

        swl     $2, 0($3)
        swr     $2, 1($3)
        swl     $2, 5($3)
        swr     $2, 6($3)
        swl     $2, 10($3)
        swr     $2, 11($3)

        lw      $4, 0($3)
        lw      $5, 4($3)
        lw      $6, 8($3)
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
as_buf:
        .space 256

        
        # $2 12345678
        # $3 address of as_buf
        # $4 34567812
        
        # $5 56781234
        # $6 78123456    
        