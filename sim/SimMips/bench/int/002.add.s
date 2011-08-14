########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "002.add"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $1, 1
        li      $2, -1
        add     $3, $1, $2	
        srl     $4, $2, 1   
        
        add     $5, $4, $1  
        add     $6, $6, $1  
        addi    $7, $2, 1   
        addi    $8, $4, 1   
        addu    $9, $2, $1
        
        addu    $10, $4, $1  
        addu    $11, $11,$1  
        addiu   $12, $2, 1   
        addiu   $13, $4, 1   
        addiu   $14, $14,1   
        
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  00000001
        # $2  ffffffff
        # $3  00000000
        # $4  7fffffff
        # $5  80000000
        # $6  00000001
        # $7  00000000
        # $8  80000000
        # $9  00000000
        # $10 80000000
        # $11 00000001
        # $12 00000000
        # $13 80000000
        # $14 00000001
        