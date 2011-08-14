########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "001.li"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li    $1,  100
        li    $2,  200
        li    $3,  300
        li    $4,  400
        li    $5,  500
        li    $6,  600
        li    $7,  700
        li    $8,  800
        li    $9,  900
        li    $10, 1000
        li    $11, 1100
        li    $12, 1200
        li    $13, 1300
        li    $14, 1400
        li    $15, 1500
        li    $16, 1600
        li    $17, 1700
        li    $18, 1800
        li    $19, 1900
        li    $20, 2000
        li    $21, 2100
        li    $22, 2200
        li    $23, 2300
        li    $24, 2400
        li    $25, 2500
        li    $26, 2600
        li    $27, 2700
        j       BENCH_END
        nop
        .end    BENCH_BODY
        