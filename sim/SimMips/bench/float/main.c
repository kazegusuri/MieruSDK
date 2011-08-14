/**********************************************************************/
/* MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH */
/**********************************************************************/

#include <stdio.h>

typedef union {
    unsigned int i[2];
    float f[2];
    double d;
} freg_t;

unsigned int REG[32];
freg_t FREG[16];
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
    printf("\n\n[FPR]\n");
    for (i = 0; i < 17; i++) {
        printf("f%02d %08x %-12g | f%02d %08x %-12g | d%02d %-12g\n",
               i * 2,     FREG[i].i[0], FREG[i].f[0],
               i * 2 + 1, FREG[i].i[1], FREG[i].f[1],
               i * 2,     FREG[i].d);
    }

    waitkey();
    return 0;
}
