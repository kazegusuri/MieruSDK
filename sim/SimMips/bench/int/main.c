/**********************************************************************/
/* MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH */
/**********************************************************************/

#include <stdio.h>

unsigned int REG[32];
char *BENCH_NAME;
void BENCH();

int waitkey() {}

int main(void)
{
    int i;
    
    BENCH();
    printf("Test Benchmark [%s]\n\n", &BENCH_NAME);
    printf("[GPR]");
    for (i = 0; i < 28; i++) {
        if (i % 5 == 0)
            printf("\nr%.2d ", i);
        printf("%08x ", REG[i]);
    }
    printf("\n");
    waitkey();
    return 0;
}
