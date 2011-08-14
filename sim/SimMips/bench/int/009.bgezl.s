########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "009.bgezl"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
        .set    noreorder
BENCH_BODY:
        li      $2, -1
        li      $3, 1
        
        bgezl   $2, b1
        addi    $7, $7, 1
        li      $4, 1
b1:
        bgezl   $0, b2
        addi    $7, $7, 1
        li      $5, 1
b2:
        bgezl   $3, b3
        addi    $7, $7, 1
        li      $6, 1
b3:
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $2     ffffffff
        # $3     00000001
        # $4     00000001
        
        # $5     00000000
        # $6     00000000
        # $7     00000002
        