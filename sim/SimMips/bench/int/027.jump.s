########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "027.jump"
        .align  2
        .text
        # .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        la      $20, f2
        j       next
        nop
        li      $2, 1
    
next:
        move    $10, $31
        jal     f1
        nop
        jalr    $20
        nop
        j       end 
        nop
        
f1:
        li      $3, 1
        jr      $31
        nop
    
f2:
        li      $4, 1
        jr      $31
        nop

end:        
        move    $31, $10
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $1  00000000
        # $2  00000000
        # $3  00000001
        # $4  00000001
        