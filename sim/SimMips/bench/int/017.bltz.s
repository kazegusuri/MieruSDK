########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "017.bltz"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $2, -1
        li      $3, 1
        
        bltz    $2, b1
        nop
        li      $0, 1
b1:
        bltz    $0, b2
        nop
        li      $5, 1
b2:
        bltz    $3, b3
        nop
        li      $6, 1
b3:
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $2     ffffffff
        # $3     00000001
        # $4     00000000
        
        # $5     00000001
        # $6     00000001
        