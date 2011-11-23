/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define2.h"

#define OX(arg) ((arg) ? 'o' : 'x')
#define READBITS(reg, pos) ((readreg(reg) >> pos ## _SH) & pos ## _MASK)
#define MODIFYBITS(reg, pos, code) modifyreg(reg, (code) << pos ## _SH,\
                                             pos ## _MASK << pos ## _SH)

/**********************************************************************/
enum {
    EXC_HANDLE_BASE_NORM = 0x80000000,
    EXC_HANDLE_BASE_BEV  = 0xbfc00200,
    EXC_HANDLE_BASE      = 0x00000100,
    EXC_HANDLE_TLB       = 0x200,
    EXC_HANDLE_GENERAL   = 0x0,
    EXC_HANDLE_INT       = 0x300,


    COMPARE_CONNECTED = 7,
};

/**********************************************************************/
MipsTlbEntry::MipsTlbEntry()
{
    vpn2 = 0;
    asid = 0;
    pagemask = 0;
    pageshift = 0;
    global = 0;
    for (int i = 0; i < 2; i++) {
        pfn[i] = 0;
        valid[i] = 0;
        dirty[i] = 0;
        cache[i] = 0;
    }
}

/**********************************************************************/
void MipsTlbEntry::print()
{
    printf("%08x %02x %08x | ", vpn2 << TLB_VPAGE_SH, asid,
           (pagemask << TLB_VPAGE_SH) | TLB_VPAGE_LOWER);
    for (int i = 0; i < 2; i++) {
        printf("0x%08llx %c %c%c%c%s",
               (uint064_t) (pfn[i] & ~(pagemask >> 1)) << TLB_PPAGE_SH,
               OX(cache[i]), OX(valid[i]), OX(dirty[i]), OX(global),
               (i == 0) ? " | " : "\n");
    }
}

/**********************************************************************/
MipsCp0Lite::MipsCp0Lite(BoardMP *board)
{
    this->board = board;
    this->chip = static_cast<ChipMP *>(board->chip);
    for (int i = 0; i < NREG; i++) {
        r[i] = 0;
    }
    lastcount = 0;
    counter = 0;
}

/**********************************************************************/
int MipsCp0Lite::gettlbentry(uint032_t page)
{
    uint id = r[CP0_ENTRYHI_] & TLB_ASID_MASK;
    for (uint i = 0; i < TLB_ENTRY; i++) {
        uint032_t mask = ~tlb[i].pagemask;
        if (((tlb[i].vpn2 & mask) == (page & mask)) &&
            ((tlb[i].asid == id) || tlb[i].global))
            return i;
    }
    return -1;
}

/**********************************************************************/
void MipsCp0Lite::step()
{
    if (r[CP0_RANDOM__] <= r[CP0_WIRED___]) {
        r[CP0_RANDOM__] = TLB_ENTRY - 1;
    } else {
        r[CP0_RANDOM__]--;
    }

    //if ((++counter) < CP0_POLL_CYCLE)
    if ((++counter) < CP0_POLL_CYCLE*50)
        return;
    counter = 0;

    //r[CP0_COUNT___]++;
    r[CP0_COUNT___] = (uint032_t) (board->gettime() / 10) / 500;
    if (r[CP0_COUNT___] >= r[CP0_COMPARE_] && 
        lastcount < r[CP0_COMPARE_]) {
        lastcount = r[CP0_COUNT___];
        setinterrupt(COMPARE_CONNECTED);
    }
}

/**********************************************************************/
void MipsCp0Lite::tlbread()
{
    uint x = r[CP0_INDEX___];
    if (x >= TLB_ENTRY) {
        printf("!! TLB READ ERROR: index is too large: %d\n", x);
        board->simerror();
        return;
    }
    r[CP0_ENTRYHI_] = (tlb[x].vpn2 << TLB_VPAGE_SH) | tlb[x].asid;
    r[CP0_PAGEMASK] = tlb[x].pagemask << TLB_VPAGE_SH;
    for (int i = 0; i < 2; i++) {
        r[CP0_ENTRYLO0 + i] = (tlb[x].pfn[i]   << TLB_PFN_SH |
                               tlb[x].cache[i] << TLB_CACHE_SH |
                               tlb[x].dirty[i] << TLB_DIRTY_SH | 
                               tlb[x].valid[i] << TLB_VALID_SH |
                               tlb[x].global);
    }
}

/**********************************************************************/
void MipsCp0Lite::tlbwrite(int use_random)
{
    uint x = (uint) (use_random) ? r[CP0_RANDOM__] : r[CP0_INDEX___];
    if (x >= TLB_ENTRY) {
        printf("!! TLB WRITE ERROR: index is too large: %d\n", x);
        board->simerror();
        return;
    }
    tlb[x].vpn2 = r[CP0_ENTRYHI_] >> TLB_VPAGE_SH;
    tlb[x].asid = r[CP0_ENTRYHI_] & TLB_ASID_MASK;
    tlb[x].pagemask = 0;
    tlb[x].pageshift = TLB_PPAGE_SH;
    int pagemask = r[CP0_PAGEMASK] >> TLB_VPAGE_SH;
    for (uint i = 0; i <= PAGESHIFT_MAX - TLB_PPAGE_SH; i += 2) {
        if (pagemask == (1 << i) - 1) {
            tlb[x].pagemask = pagemask;
            tlb[x].pageshift += i;
            break;
        }
    }
    tlb[x].global = r[CP0_ENTRYLO0] & TLB_GLOBAL_MASK;
    for (int i = 0; i < 2; i++) {
        uint032_t entrylo = r[CP0_ENTRYLO0 + i];
        tlb[x].pfn[i] = entrylo >> TLB_PFN_SH;
        tlb[x].cache[i] = (entrylo >> TLB_CACHE_SH) & TLB_CACHE_MASK;
        tlb[x].dirty[i] = (entrylo >> TLB_DIRTY_SH) & TLB_DIRTY_MASK;
        tlb[x].valid[i] = (entrylo >> TLB_VALID_SH) & TLB_VALID_MASK;
    }    
    // if (board->debug_exctlb && !chip->issilent()) {
    //     printf("## TLB Wrote.\n");
    //     tlbprint();
    // }
}

/**********************************************************************/
void MipsCp0Lite::tlblookup()
{
    r[CP0_INDEX___] = gettlbentry(r[CP0_ENTRYHI_] >> TLB_VPAGE_SH);
}

/**********************************************************************/
uint032_t MipsCp0Lite::readreg(int x)
{
    return r[x];
}

/**********************************************************************/
void MipsCp0Lite::writereg(int x, uint032_t value)
{
    r[x] = value;
}

/**********************************************************************/
void MipsCp0Lite::modifyreg(int x, uint032_t value, uint032_t mask)
{
    writereg(x, (value & mask) | (readreg(x) & ~mask));
}

/**********************************************************************/
int MipsCp0Lite::getphaddr(uint032_t vaddr, uint064_t *paddr, int store)
{
    if (!chip->tlbmode) {
        *paddr = (uint064_t) vaddr;
        return 0;
    }

    int kernel_mode = (READBITS(CP0_SR______, SR_KSU)    == 0 ||
                       READBITS(CP0_SR______, SR_ERLEXL) != 0);
    if (kernel_mode) {
        *paddr = (uint064_t) vaddr;
        return 0;
    }

    int x = gettlbentry(vaddr >> TLB_VPAGE_SH);
    if (x < 0)
        return ((store) ? EXC_TLBS___ : EXC_TLBL___) | EXC_TLBREFL;

    // int odd = (vaddr >> tlb[x].pageshift) & 0x1;
    int odd = 0;
    uint064_t mask, tmp_addr;
    mask = (~((uint064_t) tlb[x].pagemask)) << TLB_PPAGE_SH;
    tmp_addr = ((((uint064_t) tlb[x].pfn[odd] << TLB_PPAGE_SH) & mask) | 
                ((uint064_t) vaddr & ~mask));
    if (!tlb[x].valid[odd])
        return (store) ? EXC_TLBS___ : EXC_TLBL___;
    if ((!tlb[x].dirty[odd]) && store)
        return EXC_MOD____;

    *paddr = tmp_addr;
    return 0;
}

/**********************************************************************/
uint032_t MipsCp0Lite::doexception(int code, uint032_t epc,
                               uint032_t vaddr, int delay)
{
    uint032_t base, restart_pc;
    int refill = code & EXC_TLBREFL;
    int cp1 = code & EXC_CPU1___;
    code &= CAUSE_EXC_MASK;

    if (code == EXC_INT____) {
        clearinterrupt(COMPARE_CONNECTED);
    }

    MODIFYBITS(CP0_CAUSE___, CAUSE_EXC, code);
    if (code == EXC_CPU____) {
        MODIFYBITS(CP0_CAUSE___, CAUSE_CE, (cp1) ? 1 : 0);
    }
    if ((code >= EXC_MOD____) && (code <= EXC_ADES___)) {
        writereg(CP0_BADVADDR, vaddr);
        MODIFYBITS(CP0_CONTEXT_, CONT_BADV, vaddr >> TLB_VPAGE_SH);
        MODIFYBITS(CP0_ENTRYHI_, TLB_VPAGE, vaddr >> TLB_VPAGE_SH);
    }
    if (!READBITS(CP0_SR______, SR_EXL)) {
        writereg(CP0_EPC_____, (delay) ? epc - 4 : epc);
        MODIFYBITS(CP0_CAUSE___, CAUSE_BD, (delay) ? 1 : 0);
    } else {
        board->simerror();
    }

    // jump to the address of interrupt handler
    base = EXC_HANDLE_BASE;
    
    if (((code == EXC_TLBL___) || (code == EXC_TLBS___))
        && refill && (!READBITS(CP0_SR______, SR_EXL))) {
        restart_pc = base + EXC_HANDLE_TLB;
    } else if ((code == EXC_INT____) && READBITS(CP0_CAUSE___, CAUSE_IV)) {
        restart_pc = base + EXC_HANDLE_INT;
    } else {
        restart_pc = base + EXC_HANDLE_GENERAL;
    }

    // set exception bit and restart
    MODIFYBITS(CP0_SR______, SR_EXL, 1);
    return restart_pc;
}

/**********************************************************************/
void MipsCp0Lite::setinterrupt(int num)
{
    r[CP0_CAUSE___] |= 1 << (CAUSE_IP_SH + num);
}

/**********************************************************************/
void MipsCp0Lite::clearinterrupt(int num)
{
    r[CP0_CAUSE___] &= ~(1 << (CAUSE_IP_SH + num));
}

/**********************************************************************/
int MipsCp0Lite::checkinterrupt()
{
    return (((r[CP0_SR______] & r[CP0_CAUSE___]) 
             >> CAUSE_IP_SH) & CAUSE_IP_MASK &&
            READBITS(CP0_SR______, SR_ERLEXL) == 0 &&
            READBITS(CP0_SR______, SR_IE) != 0);
}

/**********************************************************************/
void MipsCp0Lite::regprint()
{
    printf("[[CP0 Register]]\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++)
            printf("cr%.2d      ", (i % 4) * 8 + j);
        printf("\n");
        for (int j = 0; j < 8; j++)
            printf("%08x  ", r[i * 8 + j]);
        printf("\n");
    }
    printf("cr16.1\n%08x\n\n", r[CP0_CONFIG1_]);
}

/**********************************************************************/
void MipsCp0Lite::tlbprint()
{
    printf("[[TLB Entries]]\n");
    printf("     VPN AS     MASK | G |"
           "       PFN0 VDC |       PFN1 VDC\n");
    for (uint i = 0; i < TLB_ENTRY; i++)
        tlb[i].print();
    printf("\n");
}

/**********************************************************************/
void MipsCp0Lite::print()
{
    regprint();
    tlbprint();
}

/**********************************************************************/
