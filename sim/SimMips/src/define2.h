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
};

class BoardMP;
class ChipMP;

/* cp0.cc *************************************************************/
class MipsCp0Lite {
 private:
    Board *board;
    ChipMP *chip;
    uint032_t count, compare, sr, cause, epc;
    uint032_t lastcount;
    int timerint, counter;

 public:
    MipsCp0Lite(BoardMP *);
    void step();
    uint032_t readreg(int);
    void writereg(int, uint032_t);
    void modifyreg(int, uint032_t, uint032_t);
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

 public:
    MipsMP(BoardMP *board, ChipMP *chip);
    virtual void drive();
};

/**********************************************************************/
enum {
    SYS_MMCSYNC = 5000,

    EXC_INT____ = 0,  // Interupt
    EXC_SYSCALL = 8,  // System call

    CP0_COUNT___ = 9,
    CP0_COMPARE_ = 11,
    CP0_SR______ = 12,
    CP0_CAUSE___ = 13,
    CP0_EPC_____ = 14,

    SR_IE_SH = 0,
    SR_EXL_SH = 1,
    SR_IE_MASK = 0x1,
    SR_EXL_MASK = 0x1,

    CAUSE_EXC_SH = 2,
    CAUSE_IP7_SH = 15,
    CAUSE_BD_SH = 31,
    CAUSE_EXC_MASK = 0x1f,
    CAUSE_IP7_MASK = 0x1,
    CAUSE_BD_MASK = 0x1,
};

/**********************************************************************/
