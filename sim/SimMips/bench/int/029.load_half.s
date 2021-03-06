########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "029.load halfword"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $2, 0x55555555   # 55555555
        li      $3, 0xaaaaaaaa   # aaaaaaaa
        la      $4, as_buf       # 00000010

        sb      $2, 4($4)
        sh      $2, 8($4)
        sw      $2, 12($4)
        sb      $3, 16($4)
        sh      $3, 20($4)
        sw      $3, 24($4)        
    
        lh      $5, 4($4)
        lhu     $6, 4($4)
        lh      $7, 8($4)
        lhu     $8, 8($4)
        lh      $9, 12($4)
        lhu     $10, 12($4)
        lh      $11, 16($4)
        lhu     $12, 16($4)
        lh      $13, 20($4)
        lhu     $14, 20($4)
        lh      $15, 24($4)
        lhu     $16, 24($4)            
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
as_buf:
        .space 256
    
        # $2 55555555
        # $3 aaaaaaaa
        # $4 address of as_buf
        
        # $5 00000055
        # $6 00000055
        # $7 00005555
        # $8 00005555
        # $9 00005555
        
        # $10 00005555
        # $11 000000aa
        # $12 000000aa
        # $13 ffffaaaa
        # $14 0000aaaa

        # $15 ffffaaaa
        # $16 0000aaaa
        