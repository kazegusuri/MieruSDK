########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .align  2
        .text
        .set    noat
        .globl  BENCH
        .ent    BENCH
BENCH:
        addi    $29, $29, -36 # save registers
        sw      $16,    0($29)
        sw      $17,    4($29)
        sw      $18,    8($29)
        sw      $19,   12($29)
        sw      $20,   16($29)
        sw      $21,   20($29)
        sw      $22,   24($29)
        sw      $23,   28($29)
        move    $1, $0         # clear registers
        move    $2, $0
        move    $3, $0
        move    $4, $0
        move    $5, $0
        move    $6, $0
        move    $7, $0
        move    $8, $0
        move    $9, $0
        move    $10, $0
        move    $11, $0
        move    $12, $0
        move    $13, $0
        move    $14, $0
        move    $15, $0
        move    $16, $0
        move    $17, $0
        move    $18, $0
        move    $19, $0
        move    $20, $0
        move    $21, $0
        move    $22, $0
        move    $23, $0
        move    $24, $0
        move    $25, $0
        move    $26, $0
        move    $27, $0
        j       BENCH_BODY     # call benchmark body
        nop
        .end    BENCH
        .globl  BENCH_END
        .ent    BENCH_END
BENCH_END:      
        sw      $1, 32($29)   # store registers to array
        la      $1, REG        
        sw      $0,    0($1)   
        sw      $2,    8($1)
        sw      $3,   12($1)
        sw      $4,   16($1)
        sw      $5,   20($1)
        sw      $6,   24($1)
        sw      $7,   28($1)
        sw      $8,   32($1)
        sw      $9,   36($1)
        sw      $10,  40($1)
        sw      $11,  44($1)
        sw      $12,  48($1)
        sw      $13,  52($1)
        sw      $14,  56($1)
        sw      $15,  60($1)
        sw      $16,  64($1)
        sw      $17,  68($1)
        sw      $18,  72($1)
        sw      $19,  76($1)
        sw      $20,  80($1)
        sw      $21,  84($1)
        sw      $22,  88($1)
        sw      $23,  92($1)
        sw      $24,  96($1)
        sw      $25, 100($1)
        sw      $26, 104($1)
        sw      $27, 108($1)
        move    $2, $1
        lw      $1, 32($29)
        sw      $1,    4($2)
        lw      $16,   0($29)  # restore registers
        lw      $17,   4($29)
        lw      $18,   8($29)
        lw      $19,  12($29)
        lw      $20,  16($29)
        lw      $21,  20($29)
        lw      $22,  24($29)
        lw      $23,  28($29)
        addi    $29, $29, 36
        jr      $31
        nop
        .end    BENCH_END
        