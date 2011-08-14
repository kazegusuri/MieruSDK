/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "../core/define.h"
#include <math.h>
#define L_NAME_FP "Floating Point Extension"
#define L_VER_FP  "2010-06-10"

/**********************************************************************/
enum {
    NFREG = 32,
    FIR_VALUE = 0x01538001,
    FP_CODE_NUM = 99,
};

/**********************************************************************/
#define FR_W(num) asfp->fr.w[num]
#define FR_L(num) asfp->fr.l[(num) / 2]
#define FR_S(num) asfp->fr.s[num]
#define FR_D(num) asfp->fr.d[(num) / 2]

/**********************************************************************/
class BoardFP : public Board {
 protected:
    virtual void setupchip();
 public:
    virtual void versioninfo();
};

/**********************************************************************/
class ChipFP : public Chip {
 public:
    ChipFP(BoardFP *board) : Chip(board) {}
    virtual void init();
};

/**********************************************************************/
class MipsInstFP : public MipsInst {
 public:
    uint032_t fop;
    uint fattr;
    uint032_t frd_w, frt_w, fcsr;
    uint064_t frd_l, frt_l;
    float     frd_s, frt_s, frs_s, rslt_s;
    double    frd_d, frt_d, frs_d, rslt_d;

    virtual void clear();
    virtual void decode();
    virtual int execute();
    virtual const char *getinstname();
    virtual char *getmnemonic();
};

/**********************************************************************/
typedef union {
    uint032_t w[NFREG];
    uint064_t l[NFREG / 2];
    float     s[NFREG];
    double    d[NFREG / 2];
} fpureg_t;

/**********************************************************************/
class MipsArchstateFP : public MipsArchstate {
 public:
    fpureg_t  fr;
    uint032_t fcsr;

    MipsArchstateFP();
    virtual void print();
};

/**********************************************************************/
class MipsSimstateFP : public MipsSimstate {
 public:
    ullint fimix[FP_CODE_NUM];

    MipsSimstateFP();
    virtual void print();
};

/**********************************************************************/
class MipsFP : public Mips {
 protected:
    virtual void decode();
    virtual void regfetch();
    virtual void memsend();
    virtual void memreceive();
    virtual void writeback();

 public:
    MipsArchstateFP *asfp;
    MipsSimstateFP *ssfp;
    MipsInstFP *instfp;

    MipsFP(BoardFP *board, ChipFP *chip) : Mips(board, chip) {}
    virtual void init();
};

/* FPU Control Register Definition ************************************/
/**********************************************************************/
enum {
    FPU_FIR_ = 0,
    FPU_FCCR = 25,
    FPU_FEXR = 26,
    FPU_FENR = 28,
    FPU_FCSR = 31,
};

/* FPU Instruction Definition *****************************************/
/**********************************************************************/
enum {
    MOVF_____ = 0,
    MOVT_____,
    MFC1_____,
    CFC1_____,
    MTC1_____,
    CTC1_____,
    BC1F_____,
    BC1T_____,
    BC1FL____,
    BC1TL____,
    ADD_S____,
    ADD_D____,
    SUB_S____,
    SUB_D____,
    MUL_S____,
    MUL_D____,
    DIV_S____,
    DIV_D____,
    SQRT_S___,
    SQRT_D___,
    ABS_S____,
    ABS_D____,
    MOV_S____,
    MOV_D____,
    NEG_S____,
    NEG_D____,
    ROUND_W_S,
    ROUND_W_D,
    TRUNC_W_S,
    TRUNC_W_D,
    CEIL_W_S_,
    CEIL_W_D_,
    FLOOR_W_S,
    FLOOR_W_D,
    MOVF_S___,
    MOVF_D___,
    MOVT_S___,
    MOVT_D___,
    MOVZ_S___,
    MOVZ_D___,
    MOVN_S___,
    MOVN_D___,
    RECIP_S__,
    RECIP_D__,
    RSQRT_S__,
    RSQRT_D__,
    CVT_S_D__,
    CVT_S_W__,
    CVT_D_S__,
    CVT_D_W__,
    CVT_W_S__,
    CVT_W_D__,
    C_F_S____,
    C_F_D____,
    C_UN_S___,
    C_UN_D___,
    C_EQ_S___,
    C_EQ_D___,
    C_UEQ_S__,
    C_UEQ_D__,
    C_OLT_S__,
    C_OLT_D__,
    C_ULT_S__,
    C_ULT_D__,
    C_OLE_S__,
    C_OLE_D__,
    C_ULE_S__,
    C_ULE_D__,
    C_SF_S___,
    C_SF_D___,
    C_SEQ_S__,
    C_SEQ_D__,
    C_NGL_S__,
    C_NGL_D__,
    C_LT_S___,
    C_LT_D___,
    C_NGE_S__,
    C_NGE_D__,
    C_LE_S___,
    C_LE_D___,
    C_NGT_S__,
    C_NGT_D__,
    LWXC1____,
    LDXC1____,
    SWXC1____,
    SDXC1____,
    MADD_S___,
    MADD_D___,
    MSUB_S___,
    MSUB_D___,
    NMADD_S__,
    NMADD_D__,
    NMSUB_S__,
    NMSUB_D__,
    L_S______,
    L_D______,
    S_S______,
    S_D______,
    UNDEF_FPU,
};

/**********************************************************************/
enum {
    READ_FRD_S = 0x1,
    READ_FRD_D = 0x2,
    READ_FRD_W = 0x4,
    READ_FRD_L = 0x8,
    READ_FRT_S = 0x10,
    READ_FRT_D = 0x20,
    READ_FRT_W = 0x40,
    READ_FRT_L = 0x80,
    READ_FRS_S = 0x100,
    READ_FRS_D = 0x200,
    READ_FCSR  = 0x400,
    WRITE_FRD_W = 0x1000,
    WRITE_FRT_W = 0x2000,
    WRITE_FRT_L = 0x4000,
    WRITE_FSH_S = 0x8000,
    WRITE_FSH_D = 0x10000,
    WRITE_FSH_W = 0x20000,
    WRITE_FSH_L = 0x40000,
    WRITE_FSH_S_COND = 0x80000,
    WRITE_FSH_D_COND = 0x100000,
    WRITE_FCSR = 0x200000,
    LOAD_8B = 0x1000000,
    STORE_8B = 0x2000000,
    LOADSTORE_FP = 0x3000000,
};

/**********************************************************************/
