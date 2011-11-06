/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#define L_NAME "SimMips: Simple Computer Simulator of MIPS"
#define L_VER "Version 0.6.7 2010-07-31"

/**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>
#include <elf.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ttyc.h"

/**********************************************************************/
typedef unsigned int uint;
typedef signed long long llint;
typedef unsigned long long ullint;

typedef signed char        int008_t;
typedef signed short       int016_t;
typedef signed int         int032_t;
typedef signed long long   int064_t;
typedef unsigned char      uint008_t;
typedef unsigned short     uint016_t;
typedef unsigned int       uint032_t;
typedef unsigned long long uint064_t;

/**********************************************************************/
#include "simloader.h"
#define DELETE(obj) {delete obj; obj = NULL;}
#define DELETE_ARRAY(arr) {delete[] arr; arr = NULL;}

/**********************************************************************/
enum {
    CPU_STOP = 0,
    CPU_START = 1,
    CPU_ERROR = -1,

    CHIP_RUN = 0,
    CHIP_RUN_SILENT = 1,
    CHIP_HALT_CYCLE = -1,
    CHIP_HALT_MIPS = -2,
    CHIP_HALT_INT = -3,

    MAX_CYCLE_DEF = 0x7fffffffffffffffull,

    NREG = 32,
    MNEMONIC_BUF_SIZE = 128,
    INST_CODE_NUM = 106,

    MEM_SIZE_DEF = 0x08000000,  // 128MiB
    MM_PAGE_SIZE = 0x1000,      // 4KiB
    PAGE_SIZE    = 0x00001000,  //   4KiB
    KSEG0_MIN    = 0x80000000,
    KSEG1_MIN    = 0xa0000000,
    KSEG2_MIN    = 0xc0000000,
    UNMAP_MASK   = 0x1fffffff,
    NUM_MCINST = 4,
};

/* board.cc ***********************************************************/
class Chip;
class Mips;
class MainMemory;
class MemoryController;
class MemoryMap;
class MMDevice;

/**********************************************************************/
class Board {
 protected:
    ullint maxcycle, killcycle;
    char *binfile, *memfile, *workdir;
    SimLoader *ld;

    virtual int init();
    virtual void setupchip();
    virtual void usage();
    ullint atoi_postfix(const char *);
    void checkarg(char**);
    virtual int checkopt(char **, int *);
    int loadrawfile(char *, uint032_t);
    char *getlinehead(char *, FILE *);
    FILE *openmemfile();
    void closememfile(FILE *);
    void setdefaultmap();
    int setmemorymap();
    int setinitialdata();
    virtual int parsememfile(MemoryMap **, char *, char *, int);
    virtual MMDevice *setmmdevice(char *, uint032_t);
    void printresult();
    
 public:
    int debug_inst, debug_state, imix_mode;
    symtab_t *functable;
    int funcnum;
    Chip *chip;
    MemoryMap *mmap;

    Board();
    virtual ~Board();
    int siminit(int, char **);
    int siminit(char *);
    virtual void exec();
    ullint gettime();
    virtual void versioninfo();
    virtual void print();
    virtual void simerror();
};

/**********************************************************************/
class Chip {
 protected:
    Board *board;
    MemoryMap *mmap;

 public:
    ullint cycle, maxcycle, killcycle;
    int ready;

    Mips *mips;
    MemoryController *mc;

    Chip(Board *);
    virtual ~Chip();
    virtual void init();
    virtual void drive();
    virtual void update();
    virtual int getstate();
    virtual int issilent();
};

/* mipsinst.cc ********************************************************/
class MipsInst {
 protected:
    char mnemonic[MNEMONIC_BUF_SIZE];
    const char *instname;

 public:
    uint032_t ir, op, pc;
    uint attr;
    uint opcode, rs, rt, rd, shamt, funct, imm, addr;
    uint code_l, code_s, sel;

    uint032_t rrs, rrt, rrd, rhi, rlo, rslt32;
    uint032_t npc, vaddr;
    uint064_t rslt64, paddr;
    int cond;
    
    virtual ~MipsInst() {};
    virtual void clear();
    virtual void decode();
    virtual int execute();
    virtual const char *getinstname();
    virtual char *getmnemonic();
};

/* mips.cc ************************************************************/
class MipsArchstate {
 public:
    uint032_t pc, delay_npc;
    uint032_t r[NREG];
    uint032_t hi, lo;

    MipsArchstate();
    virtual ~MipsArchstate() {}
    virtual void print();
};

/**********************************************************************/
class MipsSimstate {
 public:
    ullint inst_count;
    ullint imix[INST_CODE_NUM];

    MipsSimstate();
    virtual ~MipsSimstate() {}
    virtual void print();
};

/**********************************************************************/
class Mips {
 protected:
    Board *board;
    Chip *chip;
    MemoryController *mc;

    virtual void fetch();
    virtual void decode();
    virtual void regfetch();
    virtual void execute();
    virtual void memsend();
    virtual void memreceive();
    virtual void writeback();
    virtual void setnpc();
    virtual void syscall();

 public:
    MipsArchstate *as;
    MipsSimstate *ss;
    MipsInst *inst;
    int state;
    
    Mips(Board *, Chip *);
    virtual ~Mips();
    virtual void init();
    virtual void drive();
    virtual void update();
    virtual int running();
};

/* device.cc **********************************************************/
class MMDevice {
 public:
    virtual ~MMDevice() {}

    virtual void init(Board *) {}
    virtual void fini() {}
    virtual void drive() {}
    virtual void update() {}
    virtual int read(const uint032_t, uint, void*) {return 0;}
    virtual int write(const uint032_t, uint, uint064_t) {return 0;}
    virtual void print() {}
};

/* memory.cc **********************************************************/
class MainMemory : public MMDevice {
 private:
    uint032_t mem_size, npage;
    int *external;
    uint032_t *newpage(const uint032_t);
    uint032_t *getrealaddr(const uint032_t);
    
 public:
    uint032_t **pagetable;
    MainMemory(uint032_t);
    ~MainMemory();
    int read(const uint032_t, uint, void*);
    int write(const uint032_t, uint, uint064_t);
    void read1b(const uint032_t, uint008_t*);
    void read2b(const uint032_t, uint016_t*);
    void read4b(const uint032_t, uint032_t*);
    void read8b(const uint032_t, uint064_t*);
    void readnb(const uint032_t, int, uint008_t*);
    void write1b(const uint032_t, const uint008_t);
    void write2b(const uint032_t, const uint016_t);
    void write4b(const uint032_t, const uint032_t);
    void write8b(const uint032_t, const uint064_t);
    void writenb(const uint032_t, int, uint008_t*);
    uint032_t *setpageentry(const uint032_t, uint032_t*);
    void print();
};

/**********************************************************************/
class MemoryMap {
 public:
    uint032_t addr;
    uint032_t size;
    MMDevice *dev;
    MemoryMap *next;

    MemoryMap();
    ~MemoryMap(); 
};

/**********************************************************************/
class McInst {
 public:
    MMDevice *dev;
    uint032_t addr;
    uint      size;
    uint064_t data;

    McInst();
};

/**********************************************************************/
class MemoryController {
 private:
    MemoryMap *mmap;
    int mode;
    
 public:
    McInst inst[NUM_MCINST];

    MemoryController(MemoryMap *);
    int read(const uint064_t, uint, void*);
    int write(const uint064_t, uint, uint064_t);
    void drive() {}
    void update();
    void print();
};

/* Sign Extension ****************************************************/
/*********************************************************************/
inline uint032_t exts32(uint032_t x, int y)
{
    if (y == 32)
        return x;
    uint032_t temp = 0xffffffff << y;
    if (x & (1 << (y - 1)))
        return (temp | (x & ~temp));
    else
        return (x & ~temp);
}

/* MIPS Register Name Definition **************************************/
/**********************************************************************/
enum {
    REG_V0 = 2,
    REG_A0 = 4,
    REG_A1 = 5,
    REG_A2 = 6,
    REG_A3 = 7,
    REG_T9 = 25,
    REG_GP = 28,
    REG_SP = 29,
    REG_RA = 31,
};

/**********************************************************************/
const char regname[NREG + 1][3] =
    {"zr", "at", "v0", "v1", "a0", "a1", "a2", "a3", 
     "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", 
     "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", 
     "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra", "pc"};

/* Instruction Definition *********************************************/
/**********************************************************************/
enum {
    NOP______,
    SSNOP____,
    SLL______,
    SRL______,
    SRA______,
    SLLV_____,
    SRLV_____,
    SRAV_____,
    JR_______,
    JR_HB____,
    JALR_____,
    JALR_HB__,
    MOVZ_____,
    MOVN_____,
    SYSCALL__,
    BREAK____,
    SYNC_____,
    MFHI_____,
    MTHI_____,
    MFLO_____,
    MTLO_____,
    MULT_____,
    MULTU____,
    DIV______,
    DIVU_____,
    ADD______,
    ADDU_____,
    SUB______,
    SUBU_____,
    AND______,
    OR_______,
    XOR______,
    NOR______,
    SLT______,
    SLTU_____,
    TGE______,
    TGEU_____,
    TLT______,
    TLTU_____,
    TEQ______,
    TNE______,
    BLTZ_____,
    BGEZ_____,
    BLTZL____,
    BGEZL____,
    TGEI_____,
    TGEIU____,
    TLTI_____,
    TLTIU____,
    TEQI_____,
    TNEI_____,
    BLTZAL___,
    BGEZAL___,
    BLTZALL__,
    BGEZALL__,
    J________,
    JAL______,
    BEQ______,
    BNE______,
    BLEZ_____,
    BGTZ_____,
    ADDI_____,
    ADDIU____,
    SLTI_____,
    SLTIU____,
    ANDI_____,
    ORI______,
    XORI_____,
    LUI______,
    MFC0_____,
    CFC0_____,
    MTC0_____,
    TLBR_____,
    TLBWI____,
    TLBWR____,
    TLBP_____,
    ERET_____,
    WAIT_____,
    BEQL_____,
    BNEL_____,
    BLEZL____,
    BGTZL____,
    MADD_____,
    MADDU____,
    MUL______,
    MSUB_____,
    MSUBU____,
    CLZ______,
    CLO______,
    LB_______,
    LH_______,
    LWL______,
    LW_______,
    LBU______,
    LHU______,
    LWR______,
    SB_______,
    SH_______,
    SWL______,
    SW_______,
    SWR______,
    CACHE____,
    LL_______,
    PREF_____,
    SC_______,
    FLOAT_OPS,
    UNDEFINED,
};

/**********************************************************************/
enum {
    READ_NONE            = 0x0,
    READ_RS              = 0x1,
    READ_RT              = 0x2,
    READ_RD              = 0x4,
    READ_HI              = 0x8,
    READ_LO              = 0x10,
    READ_HILO            = 0x18,
    WRITE_NONE           = 0x0,
    WRITE_RS             = 0x100,
    WRITE_RT             = 0x200,
    WRITE_RD             = 0x400,
    WRITE_HI             = 0x800,
    WRITE_LO             = 0x1000,
    WRITE_HILO           = 0x1800,
    WRITE_RD_COND        = 0x4000,
    WRITE_RRA            = 0x8000,
    LOAD_1B              = 0x10000,
    LOAD_2B              = 0x20000,
    LOAD_4B_ALIGN        = 0x40000,
    LOAD_4B_UNALIGN      = 0x80000,
    LOAD_4B              = 0xc0000,
    LOAD_ANY             = 0xf0000,
    STORE_1B             = 0x100000,
    STORE_2B             = 0x200000,
    STORE_4B_ALIGN       = 0x400000,
    STORE_4B_UNALIGN     = 0x800000,
    STORE_4B             = 0xc00000,
    STORE_ANY            = 0xf00000,
    LOADSTORE            = 0xff0000,
    BRANCH               = 0x1000000,
    BRANCH_LIKELY        = 0x2000000,
    BRANCH_NODELAY       = 0x4000000,
};

/* System Call Definition *********************************************/
/**********************************************************************/
enum {
    SYS_EXIT  = 4001,
    SYS_WRITE = 4004,
    SYS_IOCTL = 4054,
};

/**********************************************************************/
