########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "028.load byte"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $2, 0x55555555   # 55555555
        li      $3, 0xaaaaaaaa   # aaaaaaaa
        la      $4, as_buf

        sb      $2, 4($4)
        sh      $2, 8($4)
        sw      $2, 12($4)
        sb      $3, 16($4)
        sh      $3, 20($4)
        sw      $3, 24($4)        
    
        lb      $5, 4($4)
        lbu     $6, 4($4)
        lb      $7, 8($4)
        lbu     $8, 8($4)
        lb      $9, 12($4)
        lbu     $10, 12($4)
        lb      $11, 16($4)
        lbu     $12, 16($4)
        lb      $13, 20($4)
        lbu     $14, 20($4)
        lb      $15, 24($4)
        lbu     $16, 24($4)            
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
        # $7 00000055
        # $8 00000055
        # $9 00000055
        
        # $10 00000055
        # $11 ffffffaa
        # $12 000000aa
        # $13 ffffffaa
        # $14 000000aa

        # $15 ffffffaa
        # $16 000000aa
        