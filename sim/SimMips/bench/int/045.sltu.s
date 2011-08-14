########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "045.sltu"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, -2
        li      $2, 0
        li      $3, 2
        sltu    $4, $1, $2

        sltu    $5, $2, $3
        sltu    $6, $3, $1
        sltu    $7, $1, $1
        sltu    $8, $2, $2
        sltu    $9, $3, $3
        
        sltiu   $10, $1, -3
        sltiu   $11, $1, -2
        sltiu   $12, $1, -1
        sltiu   $13, $1, 0
        sltiu   $14, $1, 1
        
        sltiu   $15, $2, -1
        sltiu   $16, $2, 0
        sltiu   $17, $2, 1
        sltiu   $18, $3, -1
        sltiu   $19, $3, 0
        
        sltiu   $20, $3, 1
        sltiu   $21, $3, 2
        sltiu   $22, $3, 3

        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  fffffffe
        # $2  00000000
        # $3  00000002
        # $4  00000000
        
        # $5  00000001
        # $6  00000001
        # $7  00000000
        # $8  00000000
        # $9  00000000
        
        # $10 00000000
        # $11 00000000
        # $12 00000001
        # $13 00000000
        # $14 00000000

        # $15 00000001
        # $16 00000000
        # $17 00000001
        # $18 00000001
        # $19 00000000

        # $20 00000000
        # $21 00000000
        # $22 00000001
        