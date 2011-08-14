########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "046.sub"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, 1
        sub     $2, $2, $1
        li      $3, 1
        sub     $4, $3, $1
               
        subu    $5, $5, $1
        subu    $6, $3, $1
        
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  00000001
        # $2  ffffffff
        # $3  00000001
        # $4  00000000

        # $5  ffffffff
        # $6  00000000
        