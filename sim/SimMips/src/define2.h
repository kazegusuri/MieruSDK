/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"
#define L_NAME_MP "MieruPC Extension"
#define L_VER_MP  "2010-07-31"

/**********************************************************************/
enum {
    LCD_BUF_SIZE = 2048,
    MMC_BLOCK_SIZE = 512,
    MIERU_NUM_KEYBUF = 16,

    NCREG = 256,
    TLB_ENTRY = 16,

    CZ_PAGE_SIZE = 0x1000,  // 4KiB
    CP0_POLL_CYCLE = 100,
};

class BoardMP;
class ChipMP;

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
class MipsCp0Lite {
 private:
    Board *board;
    ChipMP *chip;
    MipsTlbEntry tlb[TLB_ENTRY];
    uint032_t r[NCREG];
    uint032_t lastcount;
    uint032_t counter;

    int gettlbentry(uint032_t);
    void regprint();
    void tlbprint();

 public:
    MipsCp0Lite(BoardMP *);
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

/* display.cc *********************************************************/
class DisplayTrans {
 public:
    virtual ~DisplayTrans() {};
    virtual int run(char *, char *, int);
};

/**********************************************************************/
class DisplayTransFromOld : public DisplayTrans {
 private:
    int currentpage, sourcepage;
    int cursorx, cursory;

 public:
    DisplayTransFromOld();    
    int run(char *, char *, int);
    int addcmd(const char *, char *, int *);
    int addcmd(char  , char *, int *);
    int addcursorposcmd(int, int, char *, int *);
};

/**********************************************************************/
class DisplayController : public MMDevice {
 private:
    char *cmdbuf, *tmpbuf;
    DisplayTrans *trans;
    int gui_pipe;
    
 public:
    ~DisplayController();
    void init(Board *);
    int write(const uint032_t, uint, uint064_t);
};  

/* device.cc **********************************************************/
class MultiMediaCard {
private:
    const char *filename;
public:
    uint032_t size;
    uint008_t *mem;
    
    MultiMediaCard(char *,uint032_t);
    ~MultiMediaCard();
    int readblock(uint032_t addr, uint008_t *data);
    int writeblock(uint032_t addr, uint008_t *data);
    int writeback();
};

/**********************************************************************/
class MMCController : public MMDevice {
 private:
    Board *board;
    MultiMediaCard *mmc;

    int ready;
    uint008_t cache[MMC_BLOCK_SIZE];
    uint032_t addr, lastaddr;
    
 public:
    void init(Board *);
    void update();
    void setaddr(uint032_t);
    uint032_t getstate();
    int read(const uint032_t, uint, void*);
    int write(const uint032_t, uint, uint064_t);
};

/**********************************************************************/
class MieruIO : public MMDevice {
 private:
    Board *board;
    MMCController *mmccon;

    uint032_t kbbuf[MIERU_NUM_KEYBUF];
    unsigned int counter;
    unsigned char keystate;
    int cntdiv;

    uint032_t mmc_control;
    void getasciicode();
    void read1b(const uint032_t, uint008_t *);
    void read4b(const uint032_t, uint032_t *);
    void write1b(const uint032_t, const uint008_t);
    void write4b(const uint032_t, const uint032_t);

public:
    DisplayController *dispcon;

    ~MieruIO();
    void init(Board *);
    void update();
    int read(const uint032_t, uint, void*);
    int write(const uint032_t, uint, uint064_t);
};

/* mieru.cc ***********************************************************/
class BoardMP : public Board {
 protected:
    ttyControl *ttyc;

    virtual int init();
    virtual void setupchip();
    virtual void usage();
    int checkopt(char **argv, int *index);
    virtual int parsememfile(MemoryMap **, char *, char *, int);
    virtual MMDevice *setmmdevice(char *, uint032_t);

 public:
    char *lcd_path;
    int lcd_type;
    MultiMediaCard *mmc;

    BoardMP();
    virtual ~BoardMP();
    virtual void versioninfo();
    virtual void print();
};

/**********************************************************************/
class ChipMP : public Chip {
 public:
    MipsCp0Lite *cp0;
    bool tlbmode;

    ChipMP(BoardMP *board) : Chip(board) {}
    virtual void init();
    virtual void drive();
    virtual void update();
};

/**********************************************************************/
class MipsMP : public Mips {
 protected:
    MipsCp0Lite *cp0;
    int exc_occur, exc_code;

    virtual void execute();
    virtual void writeback();
    virtual void setnpc();
    virtual void syscall();
    virtual void exception(int);
    virtual void decode();

 public:
    MipsMP(BoardMP *board, ChipMP *chip);
    virtual void drive();
};

/**********************************************************************/
enum {
    SYS_MMCSYNC = 5000,
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

    TLB_VPAGE_SH = 10,
    TLB_VPAGE_LOWER = 0x03ff,
    TLB_VPAGE_MASK = 0x7ffff,
    TLB_PPAGE_SH = 10,
    TLB_PFN_SH = 6,
    TLB_CACHE_SH = 3,
    TLB_DIRTY_SH = 2,
    TLB_VALID_SH = 1,
    TLB_CACHE_MASK = 0x7,
    TLB_DIRTY_MASK = 0x1,
    TLB_VALID_MASK = 0x1,
    TLB_GLOBAL_MASK = 0x1,
    TLB_ASID_MASK = 0xff,
    PAGESHIFT_MAX = 24,
    CONT_BADV_SH = 4,
    CONT_BADV_MASK = TLB_VPAGE_MASK,
};

/**********************************************************************/
