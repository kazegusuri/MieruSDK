########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "148.c.ngt/cle/olt.d"
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

        c.ngt.d  $f0, $f0
        bc1t    next1
        addi    $2, $2, 1
        li      $3, 1
next1:  
        c.ngt.d  $f0, $f2
        bc1t    next2
        addi    $2, $2, 1
        li      $4, 1
next2:
        c.ngt.d  $f0, $f4
        bc1t    next3
        addi    $2, $2, 1
        li      $5, 1
next3:
        c.ngt.d  $f2, $f0
        bc1t    next4
        addi    $2, $2, 1
        li      $6, 1
next4:
        c.ngt.d  $f2, $f2
        bc1t    next5
        addi    $2, $2, 1
        li      $7, 1
next5:
        c.ngt.d  $f2, $f4
        bc1t    next6
        addi    $2, $2, 1
        li      $8, 1
next6:
        c.ngt.d  $f4, $f0
        bc1t    next7
        addi    $2, $2, 1
        li      $9, 1
next7:
        c.ngt.d  $f4, $f2
        bc1t    next8
        addi    $2, $2, 1
        li      $10, 1
next8:
        c.ngt.d  $f4, $f4
        bc1t    next9
        addi    $2, $2, 1
        li      $11, 1
        
next9:
        c.ole.d  $f0, $f0
        bc1t    next10
        addi    $2, $2, 1
        li      $12, 1
next10:  
        c.ole.d  $f0, $f2
        bc1t    next11
        addi    $2, $2, 1
        li      $13, 1
next11:
        c.ole.d  $f0, $f4
        bc1t    next12
        addi    $2, $2, 1
        li      $14, 1
next12:
        c.ole.d  $f2, $f0
        bc1t    next13
        addi    $2, $2, 1
        li      $15, 1
next13:
        c.ole.d  $f2, $f2
        bc1t    next14
        addi    $2, $2, 1
        li      $16, 1
next14:
        c.ole.d  $f2, $f4
        bc1t    next15
        addi    $2, $2, 1
        li      $17, 1
next15:
        c.ole.d  $f4, $f0
        bc1t    next16
        addi    $2, $2, 1
        li      $18, 1
next16:
        c.ole.d  $f4, $f2
        bc1t    next17
        addi    $2, $2, 1
        li      $19, 1
next17:
        c.ole.d  $f4, $f4
        bc1t    next18
        addi    $2, $2, 1
        li      $20, 1
        
next18:
        c.olt.d  $f0, $f0
        bc1t    next19
        addi    $2, $2, 1
        li      $21, 1
next19:  
        c.olt.d  $f0, $f2
        bc1t    next20
        addi    $2, $2, 1
        li      $22, 1
next20: 
        c.olt.d  $f0, $f4
        bc1t    next21
        addi    $2, $2, 1
        li      $23, 1
next21:
        c.olt.d  $f2, $f0
        bc1t    next22
        addi    $2, $2, 1
        li      $24, 1
next22:
        c.olt.d  $f2, $f2
        bc1t    next23
        addi    $2, $2, 1
        li      $25, 1
next23:
        c.olt.d  $f2, $f4
        bc1t    next24
        addi    $2, $2, 1
        li      $26, 1
next24:
        c.olt.d  $f4, $f0
        bc1t    next25
        addi    $2, $2, 1
        li      $27, 1
next25:
        c.olt.d  $f4, $f2
        bc1t    next26
        addi    $2, $2, 1
        addi   $27, 1
next26:
        c.olt.d  $f4, $f4
        bc1t    next27
        addi    $2, $2, 1
        addi    $27, 1
next27:
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data    
BENCH_DATA:
        .double 1,1e+30,-9.876e-30

        # $f0     1
        # $f2     1.234e+30
        # $f4     -9.876e-30

        # $2  0000001b
        # $3  0
        # $4  0
        
        # $5  1
        # $6  1
        # $7  0
        # $8  1
        # $9  0
        
        # $10 0
        # $11 0
        # $12 0
        # $13 0
        # $14 1
        
        # $15 1
        # $16 0
        # $17 1
        # $18 0
        # $19 0
        
        # $20 0
        # $21 1
        # $22 0
        # $23 1
        # $24 1
        
        # $25 1
        # $26 1
        # $27 1
