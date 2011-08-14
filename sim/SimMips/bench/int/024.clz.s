########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "024.clz"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, 1
        li      $2, -1
        li      $3, 0x0000ffff

        clz     $4, $0
        clz     $5, $1
        clz     $6, $2
        clz     $7, $3
        
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  00000001
        # $2  ffffffff
        # $3  0000ffff
        # $4  00000020
        
        # $5  0000001f
        # $6  00000000
        # $7  00000010
        