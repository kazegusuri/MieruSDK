########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "023.clo"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, 1
        li      $2, -1
        li      $3, 0xffff0000

        clo     $4, $0
        clo     $5, $1
        clo     $6, $2
        clo     $7, $3
        
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  00000001
        # $2  ffffffff
        # $3  ffff0000
        # $4  00000000
        
        # $5  00000000
        # $6  00000020
        # $7  00000010
        