/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "../core/define.h"
#include "../common/ttyc.h"
#define L_NAME_CZ "Coprocessor Zero Extension"
#define L_VER_CZ  "2010-06-02"

/**********************************************************************/
enum {
    CPU_WAIT = 100,

    NCREG = 256,
    TLB_ENTRY = 16,

    CZ_PAGE_SIZE = 0x1000,  // 4KiB
    KSEG0_MIN =  0x80000000,
    KSEG1_MIN =  0xa0000000,
    KSEG2_MIN =  0xc0000000,
    UNMAP_MASK = 0x1fffffff,
};

/* osmode.cc **********************************************************/
class MipsCp0;

class BoardCZ : public Board {
 protected:
    ttyControl *ttyc;

    virtual int init();
    virtual void setupchip();
    virtual void usage();
    int checkopt(char **argv, int *index);
    virtual MMDevice *setmmdevice(char *, uint032_t);    

 public:
    int debug_exctlb;

    BoardCZ();
    virtual ~BoardCZ();
    virtual void versioninfo();
    virtual void print();
};

/**********************************************************************/
class ChipCZ : public Chip {
 public:
    MipsCp0 *cp0;

    ChipCZ(BoardCZ *board) : Chip(board) {}
    virtual void init();
    virtual void drive();
    virtual void update();
};

/* mips.cc ************************************************************/
class MipsCZ : public Mips {
 private:
    MipsCp0 *cp0;
    int exc_occur, exc_code;
    
    virtual void decode();
    virtual void regfetch();
    virtual void execute();
    virtual void writeback();
    virtual void setnpc();
    virtual void exception(int);

 public:
    MipsCZ(BoardCZ *board, ChipCZ *chip);
    virtual void init();
    virtual void drive();
    virtual void update();
};

/**********************************************************************/
class MipsInstCZ : public MipsInst {
 public:
    uint032_t rc0;
    int tlbop;

    virtual void clear();
    virtual int execute();
};

/* cp0.cc *************************************************************/
class MipsTlbEntry {
 public:
    uint vpn2;
    uint asid;
    uint pagemask;
    uint pageshift;
    uint global;
    uint pfn[2];
    uint valid[2];
    uint dirty[2];
    uint cache[2];

    MipsTlbEntry();
    void print();
};

/**********************************************************************/
class MipsCp0 {
 private:
    BoardCZ *board;
    ChipCZ *chip;
    MipsTlbEntry tlb[TLB_ENTRY];
    uint032_t r[NCREG];
    int counter, divisor;
    int gettlbentry(uint032_t);
    void regprint();
    void tlbprint();

 public:
    MipsCp0(BoardCZ *, int);
    void step();
    void tlbread();
    void tlbwrite(int);
    void tlblookup();
    uint032_t readreg(int);
    void writereg(int, uint032_t);
    void modifyreg(int, uint032_t, uint032_t);
    int getphaddr(uint032_t, uint064_t *, int);
    uint032_t doexception(int, uint032_t, uint032_t, int);
    void setinterrupt(int);
    void clearinterrupt(int);
    int checkinterrupt();
    void print();
};

/* device.cc **********************************************************/
class IntController {
 private:
    MipsCp0 *cp0;
    uint008_t imr[2], irr[2], isr[2];
    int tobe_read[2], init_mode[2];

    int checkaddr(uint032_t);
    void recalcirq();

 public:
    IntController(MipsCp0 *);

    int read(const uint032_t, uint, void*);
    int write(const uint032_t, uint, uint064_t);
    void setinterrupt(int);
    void clearinterrupt(int);
};

/**********************************************************************/
class SerialIO {
 private:
    Chip *chip;
    IntController *pic;
    uint008_t ier, iir, lcr, mcr, scr;
    int currentchar;
    uint divisor;
    int counter;

    int charavail();
    void recalcirq();

 public:
    SerialIO(Chip *, IntController *);

    void update();
    int read(const uint032_t, uint, void*);
    int write(const uint032_t, uint, uint064_t);
};

/**********************************************************************/
class IsaIO : public MMDevice {
 private:
    IntController *pic;
    SerialIO *sio;
 public:
    IsaIO();
    ~IsaIO();
    
    void init(Board *);
    void update();
    int read(const uint032_t, uint, void*);
    int write(const uint032_t, uint, uint064_t);
};

/* Exception Code Definition ******************************************/
/**********************************************************************/
enum {
    EXC_INT____ = 0,  // Interupt
    EXC_MOD____ = 1,  // Modifying read-only page
    EXC_TLBL___ = 2,  // TLB miss load
    EXC_TLBS___ = 3,  // TLB miss store
    EXC_ADEL___ = 4,  // Address error load
    EXC_ADES___ = 5,  // Address error store
    EXC_IBE____ = 6,  // Bus error inst
    EXC_DBE____ = 7,  // Bus error data
    EXC_SYSCALL = 8,  // System call
    EXC_BP_____ = 9,  // Breakpoint
    EXC_RI_____ = 10, // Illegal instruction
    EXC_CPU____ = 11, // Coprocessor unavailable
    EXC_OV_____ = 12, // Overflow
    EXC_TRAP___ = 13, // Trap instruction
    // # Codes below are NOT architectually defined
    EXC_TLBREFL = 0x100, // TLB Refill flag (for TLBL, TLBS)
    EXC_CPU1___ = 0x200, // CP1 unavailable? (for CPU)
};

/* CP0 Register Definition ********************************************/
/**********************************************************************/
enum {
    CP0_INDEX___ = 0,
    CP0_RANDOM__ = 1,
    CP0_ENTRYLO0 = 2,
    CP0_ENTRYLO1 = 3,
    CP0_CONTEXT_ = 4,
    CP0_PAGEMASK = 5,
    CP0_WIRED___ = 6,
    CP0_BADVADDR = 8,
    CP0_COUNT___ = 9,
    CP0_ENTRYHI_ = 10,
    CP0_COMPARE_ = 11,
    CP0_SR______ = 12,
    CP0_CAUSE___ = 13,
    CP0_EPC_____ = 14,
    CP0_PRID____ = 15,
    CP0_CONFIG__ = 16,
    CP0_CONFIG1_ = 48,

    SR_IE_SH = 0,
    SR_EXL_SH = 1,
    SR_ERLEXL_SH = 1,
    SR_KSU_SH = 3,
    SR_BEV_SH = 22,
    SR_IE_MASK = 0x1,
    SR_EXL_MASK = 0x1,
    SR_ERLEXL_MASK = 0x3,
    SR_KSU_MASK = 0x3,
    SR_BEV_MASK = 0x1,

    CAUSE_EXC_SH = 2,
    CAUSE_IP_SH = 8,
    CAUSE_IV_SH = 23,
    CAUSE_CE_SH = 28,
    CAUSE_BD_SH = 31,
    CAUSE_EXC_MASK = 0x1f,
    CAUSE_IP_MASK = 0xff,
    CAUSE_IV_MASK = 0x1,
    CAUSE_CE_MASK = 0x3,
    CAUSE_BD_MASK = 0x1,

    TLB_VPAGE_SH = 13,
    TLB_VPAGE_LOWER = 0x1fff,
    TLB_VPAGE_MASK = 0x7ffff,
    TLB_PPAGE_SH = 12,
    CONT_BADV_SH = 4,
    CONT_BADV_MASK = TLB_VPAGE_MASK,
};

/**********************************************************************/
