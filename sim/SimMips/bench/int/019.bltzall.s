########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "019.bltzall"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
        .set    noreorder
BENCH_BODY:
        move    $20, $31
        li      $2, -1
        li      $3, 1
        
        bltzall  $2, func1
        addi     $7, $7, 1
    
        bltzall  $0, func2
        addi     $7, $7, 1
    
        bltzall  $3, func3
        addi     $7, $7, 1
        j       end
        nop
    
func1:
        li      $4, 1
        jr      $31
        nop
func2:
        li      $5, 1
        jr      $31
        nop
func3:  
        li      $6, 1
        jr      $31
        nop
    
end:
        move    $31, $20
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $2     ffffffff
        # $3     00000001
        # $4     00000001
        
        # $5     00000001
        # $6     00000000
        # $7     00000001
    