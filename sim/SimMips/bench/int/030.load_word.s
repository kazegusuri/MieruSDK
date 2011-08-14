########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "030.load word"
        .align  2
        .text
        .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $2, 0x55555555
        li      $3, 0xaaaaaaaa
        la      $4, as_buf

        sb      $2, 4($4)
        sh      $2, 8($4)
        sw      $2, 12($4)
        sb      $3, 16($4)
        sh      $3, 20($4)
        sw      $3, 24($4)        
    
        lw      $5, 4($4)
        lw      $6, 8($4)
        lw      $7, 12($4)
        lw      $8, 16($4)
        lw      $9, 20($4)
        lw      $10,24($4)
        j       BENCH_END
        nop
        .end    BENCH_BODY
        .data
as_buf:
        .space 256

        # $2 55555555
        # $3 aaaaaaaa
        # $4 00416044
    
        # $5 00000055
        # $6 00005555
        # $7 55555555
        # $8 000000aa
        # $9 0000aaaa

        # $10 aaaaaaaa
                    