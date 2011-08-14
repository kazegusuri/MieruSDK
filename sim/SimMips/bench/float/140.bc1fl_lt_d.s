########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "140.bc1fl lt double"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
        .set    noreorder
BENCH_BODY:        
        la      $2, BENCH_DATA
        l.d     $f0, 0($2)
        l.d     $f2, 8($2)
        l.d     $f4, 16($2)
        move    $2, $0

        c.lt.d  $f0, $f0
        bc1fl   next1
        addi    $2, $2, 1
        li      $3, 1
next1:  
        c.lt.d  $f0, $f2
        bc1fl   next2
        addi    $2, $2, 1
        li      $4, 1
next2:
        c.lt.d  $f0, $f4
        bc1fl   next3
        addi    $2, $2, 1
        li      $5, 1
next3:
        c.lt.d  $f2, $f0
        bc1fl   next4
        addi    $2, $2, 1
        li      $6, 1
next4:
        c.lt.d  $f2, $f2
        bc1fl   next5
        addi    $2, $2, 1
        li      $7, 1
next5:
        c.lt.d  $f2, $f4
        bc1fl   next6
        addi    $2, $2, 1
        li      $8, 1
next6:
        c.lt.d  $f4, $f0
        bc1fl   next7
        addi    $2, $2, 1
        li      $9, 1
next7:
        c.lt.d  $f4, $f2
        bc1fl   next8
        addi    $2, $2, 1
        li      $10, 1
next8:
        c.lt.d  $f4, $f4
        bc1fl   next9
        addi    $2, $2, 1
        li      $11, 1
next9:  
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
BENCH_DATA:
        .double 1,1e+100,-9.876e-100

        # $f0     1
        # $f2     1.234e+30
        # $f4     -9.876e-30

        # $2  00000006
        # $3  0
        # $4  1
        
        # $5  0
        # $6  0
        # $7  0
        # $8  0
        # $9  1
        
        # $10 1
        # $11 0

