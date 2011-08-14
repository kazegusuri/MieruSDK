########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
#   Dual licensed under New BSD or GPL v2.                             #
#   See COPYING file on the base directory for more information.       #
########################################################################

        .data
        .globl  BENCH_NAME
BENCH_NAME:
        .asciiz "026.divu"
        .align  2
        .text
        # .set    noat
        .globl  BENCH_BODY
        .ent    BENCH_BODY
BENCH_BODY:
        li      $2, 1       
        divu    $0, $3, $2  # 0/1
        mfhi    $4          
        mflo    $5          
        li      $6, 12      
        li      $7, 6

        divu    $0, $6, $7  # 12/6
        mfhi    $8          
        mflo    $9          
        divu    $0, $7, $6  # 6/12
        mfhi    $10         
        mflo    $11         
        divu    $0, $7, $7  # 6/6
        mfhi    $12         
        mflo    $13
        
        li      $6, -12
        divu    $0, $6, $7  # -12/6
        mfhi    $14         
        mflo    $15         
        divu    $0, $7, $6  # 6/(-12)
        mfhi    $16         
        mflo    $17         
        divu    $0, $6, $6  # 6/6
        mfhi    $18         
        mflo    $19
        
        li      $6, 12      
        li      $7, -6      
        divu    $0, $6, $7  # 12/(-6)
        mfhi    $20         
        mflo    $21         
        divu    $0, $7, $6  # -6/12
        mfhi    $22         
        mflo    $23         
        divu    $0, $7, $7  # (-6)/(-6)
        mfhi    $24         
        mflo    $25
        
        j       BENCH_END
        nop
        .end    BENCH_BODY

        # $2     00000001
        # $3     00000000
        # $4     00000000
        
        # $5     00000000
        # $6     0000000c
        # $7     fffffffa
        # $8     00000000
        # $9     00000002
        
        # $10    00000006
        # $11    00000000
        # $12    00000000
        # $13    00000001
        # $14    00000004
        
        # $15    2aaaaaa8
        # $16    00000006
        # $17    00000000
        # $18    00000000
        # $19    00000001
        
        # $20    0000000c
        # $21    00000000
        # $22    0000000a
        # $23    15555554
        # $24    00000000
        
        # $25    00000001
        