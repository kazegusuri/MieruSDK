/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

#define BIT_FCSR(pos) ((fcsr << FCSR_ ## pos) & 0x1)
#define MOD_FCSR(pos, val) ((fcsr & ~(1 << FCSR_ ## pos)) |\
                            ((val) << FCSR_ ## pos))

/**********************************************************************/
enum {
    FCSR_RM = 0,
    FCSR_FLG_I,
    FCSR_FLG_U,
    FCSR_FLG_O,
    FCSR_FLG_Z,
    FCSR_FLG_V,
    FCSR_ENB_I,
    FCSR_ENB_U,
    FCSR_ENB_O,
    FCSR_ENB_Z,
    FCSR_ENB_V,
    FCSR_COZ_I,
    FCSR_COZ_U,
    FCSR_COZ_O,
    FCSR_COZ_Z,
    FCSR_COZ_V,
    FCSR_E = 17,
    /* Three Reserved Bits */
    FCSR_FN = 21,
    FCSR_FO,
    FCSR_FCC0,
    FCSR_FS,
    FCSR_FCC1,
    FCSR_FCC2,
    FCSR_FCC3,
    FCSR_FCC4,
    FCSR_FCC5,
    FCSR_FCC6,
    FCSR_FCC7 = 31,
    FCSR_FEXR_MASK = 0x0003f07f,
    FCSR_FENR_MASK = 0x00000f83,
    FCSR_FCC_MASK  = 0xfe000000,
    FENR_FS = 2,
    FCCR_FCC1 = 1,
};

/**********************************************************************/
void MipsInstFP::clear()
{
    MipsInst::clear();
    fop = UNDEF_FPU;
    fattr = 0;
}

/**********************************************************************/
void MipsInstFP::decode()
{
    MipsInst::decode();
    if (op != FLOAT_OPS)
        return;

    fop = UNDEF_FPU;
    fattr = 0;
    opcode = (ir >> 26) &      0x3f;
    rs     = (ir >> 21) &      0x1f;
    rt     = (ir >> 16) &      0x1f;
    funct  =  ir        &      0x3f;

    switch (opcode) {
    case 0:
        if (funct == 1) {
            fop = (rt & 0x1) ? MOVT_____ : MOVF_____;
            attr = READ_RS | WRITE_RD_COND;
            fattr = READ_FCSR;
        }
        break;
    case 17:
        if (rs == 8) {
            if (rt & 0x2) {
                fop = (rt & 0x1) ? BC1TL____ : BC1FL____;
                attr = BRANCH_LIKELY;
                fattr = READ_FCSR;
            } else {
                fop = (rt & 0x1) ? BC1T_____ : BC1F_____;
                attr = BRANCH;
                fattr = READ_FCSR;
            }
            break;
        }
        switch (funct) {
        case 0:
            switch (rs) {
            case 0:
                fop = MFC1_____;
                attr = WRITE_RT;
                fattr = READ_FRD_W;
                break;
            case 2:
                fop = CFC1_____;
                attr = WRITE_RT;
                fattr = READ_FCSR;
                break;
            case 4:
                fop = MTC1_____;
                attr = READ_RT;
                fattr = WRITE_FRD_W;
                break;
            case 6:
                fop = CTC1_____;
                attr = READ_RT;
                fattr = WRITE_FCSR;
                break;
            case 16:
            case 17:
                if (rs == 16) {
                    fop = ADD_S____;
                    fattr = READ_FRD_S | READ_FRT_S | WRITE_FSH_S;
                } else {
                    fop = ADD_D____;
                    fattr = READ_FRD_D | READ_FRT_D | WRITE_FSH_D;
                }
                break;
            }
            break;
        case 1:
            if (rs == 16) {
                fop = SUB_S____;
                fattr = READ_FRD_S | READ_FRT_S | WRITE_FSH_S;
            } else {
                fop = SUB_D____;
                fattr = READ_FRD_D | READ_FRT_D | WRITE_FSH_D;
            }
            break;
        case 2:
            if (rs == 16) {
                fop = MUL_S____;
                fattr = READ_FRD_S | READ_FRT_S | WRITE_FSH_S;
            } else {
                fop = MUL_D____;
                fattr = READ_FRD_D | READ_FRT_D | WRITE_FSH_D;
            }
            break;
        case 3:
            if (rs == 16) {
                fop = DIV_S____;
                fattr = READ_FRD_S | READ_FRT_S | WRITE_FSH_S;
            } else {
                fop = DIV_D____;
                fattr = READ_FRD_D | READ_FRT_D | WRITE_FSH_D;
            }
            break;
        case 4:
            if (rs == 16) {
                fop = SQRT_S___;
                fattr = READ_FRD_S | WRITE_FSH_S;
            } else {
                fop = SQRT_D___;
                fattr = READ_FRD_D | WRITE_FSH_D;
            }
            break;
        case 5:
            if (rs == 16) {
                fop = ABS_S____;
                fattr = READ_FRD_S | WRITE_FSH_S;
            } else {
                fop = ABS_D____;
                fattr = READ_FRD_D | WRITE_FSH_D;
            }
            break;
        case 6:
            if (rs == 16) {
                fop = MOV_S____;
                fattr = READ_FRD_S | WRITE_FSH_S;
            } else {
                fop = MOV_D____;
                fattr = READ_FRD_D | WRITE_FSH_D;
            }
            break;
        case 7:
            if (rs == 16) {
                fop = NEG_S____;
                fattr = READ_FRD_S | WRITE_FSH_S;
            } else {
                fop = NEG_D____;
                fattr = READ_FRD_D | WRITE_FSH_D;
            }
            break;
        case 12:
            if (rs == 16) {
                fop = ROUND_W_S;
                fattr = READ_FRD_S | WRITE_FSH_W;
            } else {
                fop = ROUND_W_D;
                fattr = READ_FRD_D | WRITE_FSH_W;
            }
            break;
        case 13:
            if (rs == 16) {
                fop = TRUNC_W_S;
                fattr = READ_FRD_S | WRITE_FSH_W;
            } else {
                fop = TRUNC_W_D;
                fattr = READ_FRD_D | WRITE_FSH_W;
            }
            break;
        case 14:
            if (rs == 16) {
                fop = CEIL_W_S_;
                fattr = READ_FRD_S | WRITE_FSH_W;
            } else {
                fop = CEIL_W_D_;
                fattr = READ_FRD_D | WRITE_FSH_W;
            }
            break;
        case 15:
            if (rs == 16) {
                fop = FLOOR_W_S;
                fattr = READ_FRD_S | WRITE_FSH_W;
            } else {
                fop = FLOOR_W_D;
                fattr = READ_FRD_D | WRITE_FSH_W;
            }
            break;
        case 17:
            if (rs == 16) {
                fop = (rt & 0x1) ? MOVT_S___ : MOVF_S___;
                fattr = READ_FRD_S | WRITE_FSH_S_COND;
            } else {
                fop = (rt & 0x1) ? MOVT_D___ : MOVF_D___;
                fattr = READ_FRD_D | WRITE_FSH_D_COND;
            }
            break;
        case 18:
            if (rs == 16) {
                fop = MOVZ_S___;
                fattr = READ_FRD_S | WRITE_FSH_S_COND;
            } else {
                fop = MOVZ_D___;
                fattr = READ_FRD_D | WRITE_FSH_D_COND;
            }
            attr = READ_RT | WRITE_NONE;
            break;
        case 19:
            if (rs == 16) {
                fop = MOVN_S___;
                fattr = READ_FRD_S | WRITE_FSH_S_COND;
            } else {
                fop = MOVN_D___;
                fattr = READ_FRD_D | WRITE_FSH_D_COND;
            }
            attr = READ_RT | WRITE_NONE;
            break;
        case 21:
            if (rs == 16) {
                fop = RECIP_S__;
                fattr = READ_FRD_S | WRITE_FSH_S;
            } else {
                fop = RECIP_D__;
                fattr = READ_FRD_D | WRITE_FSH_D;
            }
            break;
        case 22:
            if (rs == 16) {
                fop = RSQRT_S__;
                fattr = READ_FRD_S | WRITE_FSH_S;
            } else {
                fop = RSQRT_D__;
                fattr = READ_FRD_D | WRITE_FSH_D;
            }
            break;
        case 32:
            if (rs == 17) {
                fop = CVT_S_D__;
                fattr = READ_FRD_D | WRITE_FSH_S;
            } else {
                fop = CVT_S_W__;
                fattr = READ_FRD_W | WRITE_FSH_S;
            }
            break;
        case 33:
            if (rs == 16) {
                fop = CVT_D_S__;
                fattr = READ_FRD_S | WRITE_FSH_D;
            } else {
                fop = CVT_D_W__;
                fattr = READ_FRD_W | WRITE_FSH_D;
            }
            break;
        case 36:
            if (rs == 16) {
                fop = CVT_W_S__;
                fattr = READ_FRD_S | WRITE_FSH_W;
            } else {
                fop = CVT_W_D__;
                fattr = READ_FRD_D | WRITE_FSH_W;
            }
            break;
        case 48:
            if (rs == 16) {
                fop = C_F_S____;
                fattr = READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_F_D____;
                fattr = READ_FCSR | WRITE_FCSR;
            }
            break;
        case 49:
            if (rs == 16) {
                fop = C_UN_S___;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_UN_D___;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 50:
            if (rs == 16) {
                fop = C_EQ_S___;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_EQ_D___;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 51:
            if (rs == 16) {
                fop = C_UEQ_S__;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_UEQ_D__;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 52:
            if (rs == 16) {
                fop = C_OLT_S__;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_OLT_D__;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 53:
            if (rs == 16) {
                fop = C_ULT_S__;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_ULT_D__;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 54:
            if (rs == 16) {
                fop = C_OLE_S__;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_OLE_D__;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 55:
            if (rs == 16) {
                fop = C_ULE_S__;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_ULE_D__;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 56:
            if (rs == 16) {
                fop = C_SF_S___;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_SF_D___;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 58:
            if (rs == 16) {
                fop = C_SEQ_S__;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_SEQ_D__;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 59:
            if (rs == 16) {
                fop = C_NGL_S__;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_NGL_D__;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 60:
            if (rs == 16) {
                fop = C_LT_S___;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_LT_D___;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 61:
            if (rs == 16) {
                fop = C_NGE_S__;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_NGE_D__;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 62:
            if (rs == 16) {
                fop = C_LE_S___;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_LE_D___;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        case 63:
            if (rs == 16) {
                fop = C_NGT_S__;
                fattr = READ_FRD_S | READ_FRT_S | READ_FCSR | WRITE_FCSR;
            } else {
                fop = C_NGT_D__;
                fattr = READ_FRD_D | READ_FRT_D | READ_FCSR | WRITE_FCSR;
            }
            break;
        }
        break;
    case 19:
        switch (funct) {
        case 0:
            fop = LWXC1____;
            attr = READ_RS | READ_RT | LOAD_4B_ALIGN;
            fattr = WRITE_FSH_W;
            break;
        case 1:
            fop = LDXC1____;
            attr = READ_RS | READ_RT;
            fattr = LOAD_8B | WRITE_FSH_L;
            break;
        case 8:
            fop = SWXC1____;
            attr = READ_RS | READ_RT | STORE_4B_ALIGN;
            fattr = READ_FRD_W;
            break;
        case 9:
            fop = SDXC1____;
            attr = READ_RS | READ_RT;
            fattr = READ_FRD_L | STORE_8B;
            break;
        case 32:
            fop = MADD_S___;
            fattr = READ_FRD_S | READ_FRT_S | READ_FRS_S | WRITE_FSH_S;
            break;
        case 33:
            fop = MADD_D___;
            fattr = READ_FRD_D | READ_FRT_D | READ_FRS_D | WRITE_FSH_D;
            break;
        case 40:
            fop = MSUB_S___;
            fattr = READ_FRD_S | READ_FRT_S | READ_FRS_S | WRITE_FSH_S;
            break;
        case 41:
            fop = MSUB_D___;
            fattr = READ_FRD_D | READ_FRT_D | READ_FRS_D | WRITE_FSH_D;
            break;
        case 48:
            fop = NMADD_S__;
            fattr = READ_FRD_S | READ_FRT_S | READ_FRS_S | WRITE_FSH_S;
            break;
        case 49:
            fop = NMADD_D__;
            fattr = READ_FRD_D | READ_FRT_D | READ_FRS_D | WRITE_FSH_D;
            break;
        case 56:
            fop = NMSUB_S__;
            fattr = READ_FRD_S | READ_FRT_S | READ_FRS_S | WRITE_FSH_S;
            break;
        case 57:
            fop = NMSUB_D__;
            fattr = READ_FRD_D | READ_FRT_D | READ_FRS_D | WRITE_FSH_D;
            break;
        }
        break;
    case 49:
        fop = L_S______;
        attr = READ_RS | LOAD_4B_ALIGN;
        fattr = WRITE_FRT_W;
        break;
    case 53:
        fop = L_D______;
        attr = READ_RS;
        fattr = LOAD_8B | WRITE_FRT_L;
        break;
    case 57:
        fop = S_S______;
        attr = READ_RS | STORE_4B_ALIGN;
        fattr = READ_FRT_W;
        break;
    case 61:
        fop = S_D______;
        attr = READ_RS;
        fattr = READ_FRT_L | STORE_8B;
        break;
    }    
}

/**********************************************************************/
int MipsInstFP::execute()
{
    if (op != FLOAT_OPS)
        return MipsInst::execute();

    uint032_t fcc[8] = {1 << FCSR_FCC0, 1 << FCSR_FCC1, 
                        1 << FCSR_FCC2, 1 << FCSR_FCC3, 
                        1 << FCSR_FCC4, 1 << FCSR_FCC5, 
                        1 << FCSR_FCC6, 1 << FCSR_FCC7};
    uint c_bit = 0;
    
    switch (fop) {
    case MOVF_____:
    case MOVT_____:
        cond = ((fcsr & fcc[rt >> 2]) == 0) ^ (rt & 0x1);
        rslt32 = rrs;
        break;
    case MFC1_____:
        rslt32 = frd_w;
        break;
    case CFC1_____:
        if (rd == FPU_FIR_) {
            rslt32 = FIR_VALUE;
        } else if (rd == FPU_FCSR) {
            rslt32 = fcsr;
        } else if (rd == FPU_FCCR) {
            rslt32 = (fcsr >> (FCSR_FCC1 - FCCR_FCC1)) | BIT_FCSR(FCC0);
        } else if (rd == FPU_FEXR) {
            rslt32 = fcsr & FCSR_FEXR_MASK;
        } else if (rd == FPU_FENR) {
            rslt32 = (fcsr & FCSR_FENR_MASK) | (BIT_FCSR(FS) << FENR_FS);
        } else {
            rslt32 = 0;
        }
        break;
    case MTC1_____:
        rslt32 = rrt;
        break;
    case CTC1_____:
        if (rd == FPU_FCSR) {
            rslt32 = rrt;
        } else if (rd == FPU_FCCR) {
            rslt32 = MOD_FCSR(FCC0, rrt & 0x1);
            rslt32 = ((rslt32 & ~FCSR_FCC_MASK) | 
                    ((rrt << (FCSR_FCC1 - FCCR_FCC1)) & FCSR_FCC_MASK));
        } else if (rd == FPU_FEXR) {
            rslt32 = ((fcsr & ~FCSR_FEXR_MASK) | (rrt & FCSR_FEXR_MASK));
        } else if (rd == FPU_FENR) {
            rslt32 = MOD_FCSR(FS, (rrt >> FENR_FS) & 0x1);
            rslt32 = ((rslt32 & ~FCSR_FENR_MASK) | (rrt & FCSR_FENR_MASK));
        }
        break;
    case BC1F_____:
    case BC1FL____:
        cond = ((fcsr & fcc[rt >> 2]) == 0);
        npc = pc + (exts32(imm, 16) << 2) + 4;
        break;
    case BC1T_____:
    case BC1TL____:
        cond = ((fcsr & fcc[rt >> 2]) != 0);
        npc = pc + (exts32(imm, 16) << 2) + 4;
        break;
    case ADD_S____:
        rslt_s = frd_s + frt_s;
        break;
    case ADD_D____:
        rslt_d = frd_d + frt_d;
        break;
    case SUB_S____:
        rslt_s = frd_s - frt_s;
        break;
    case SUB_D____:
        rslt_d = frd_d - frt_d;
        break;
    case MUL_S____:
        rslt_s = frd_s * frt_s;
        break;
    case MUL_D____:
        rslt_d = frd_d * frt_d;
        break;
    case DIV_S____:
        rslt_s = frd_s / frt_s;
        break;
    case DIV_D____:
        rslt_d = frd_d / frt_d;
        break;
    case SQRT_S___:
        rslt_s = sqrtf(frd_s);
        break;
    case SQRT_D___:
        rslt_d = sqrt(frd_d);
        break;
    case ABS_S____:
        rslt_s = fabsf(frd_s);
        break;
    case ABS_D____:
        rslt_d = fabs(frd_d);
        break;
    case MOV_S____:
        rslt_s = frd_s;
        break;
    case MOV_D____:
        rslt_d = frd_d;
        break;
    case NEG_S____:
        rslt_s = -frd_s;
        break;
    case NEG_D____:
        rslt_d = -frd_d;
        break;
    case ROUND_W_S:
        rslt32 = (int032_t) roundf(frd_s);
        break;
    case ROUND_W_D:
        rslt32 = (int032_t) round(frd_d);
        break;
    case TRUNC_W_S:
        rslt32 = (int032_t) truncf(frd_s);
        break;
    case TRUNC_W_D:
        rslt32 = (int032_t) trunc(frd_d);
        break;
    case CEIL_W_S_:
        rslt32 = (int032_t) ceilf(frd_s);
        break;
    case CEIL_W_D_:
        rslt32 = (int032_t) ceil(frd_d);
        break;
    case FLOOR_W_S:
        rslt32 = (int032_t) floorf(frd_s);
        break;
    case FLOOR_W_D:
        rslt32 = (int032_t) floor(frd_d);
        break;
    case MOVF_S___:
        cond = ((fcsr & fcc[rt >> 2]) == 0);
        rslt_s = frd_s;
        break;
    case MOVF_D___:
        cond = ((fcsr & fcc[rt >> 2]) == 0);
        rslt_d = frd_d;
        break;
    case MOVT_S___:
        cond = ((fcsr & fcc[rt >> 2]) != 0);
        rslt_s = frd_s;
        break;
    case MOVT_D___:
        cond = ((fcsr & fcc[rt >> 2]) != 0);
        rslt_d = frd_d;
        break;
    case MOVZ_S___:
        cond = (rrt == 0);
        rslt_s = frd_s;
        break;
    case MOVZ_D___:
        cond = (rrt == 0);
        rslt_d = frd_d;
        break;
    case MOVN_S___:
        cond = (rrt != 0);
        rslt_s = frd_s;
        break;
    case MOVN_D___:
        cond = (rrt != 0);
        rslt_d = frd_d;
        break;
    case RECIP_S__:
        rslt_s = (float) 1.0 / frd_s;
        break;
    case RECIP_D__:
        rslt_d = (double) 1.0 / frd_d;
        break;
    case RSQRT_S__:
        rslt_s = sqrtf((float) 1.0 / frd_s);
        break;
    case RSQRT_D__:
        rslt_d = sqrt((double) 1.0 / frd_d);
        break;
    case CVT_S_D__:
        rslt_s = (float) frd_d;
        break;
    case CVT_S_W__:
        rslt_s = (float) (int032_t) frd_w;
        break;
    case CVT_D_S__:
        rslt_d = (double) frd_s;
        break;
    case CVT_D_W__:
        rslt_d = (double) (int032_t) frd_w;
        break;
    case CVT_W_S__:
        rslt32 = (int032_t) frd_s;
        break;
    case CVT_W_D__:
        rslt32 = (int032_t) frd_d;
        break;
    case C_F_S____:
    case C_SF_S___:
    case C_F_D____:
    case C_SF_D___:
        c_bit = 0;
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_UN_S___:
        c_bit = isnan(frd_s) || isnan(frt_s);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_UN_D___:
        c_bit = isnan(frd_d) || isnan(frt_d);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_EQ_S___:
    case C_SEQ_S__:
        c_bit = (frd_s == frt_s);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_EQ_D___:
    case C_SEQ_D__:
        c_bit = (frd_d == frt_d);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_UEQ_S__:
    case C_NGL_S__:
        c_bit = isnan(frd_s) || isnan(frt_s);
        c_bit = c_bit || (frd_s == frt_s);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_UEQ_D__:
    case C_NGL_D__:
        c_bit = isnan(frd_d) || isnan(frt_d);
        c_bit = c_bit || (frd_d == frt_d);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_OLT_S__:
    case C_LT_S___:
        c_bit = (frd_s < frt_s);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_OLT_D__:
    case C_LT_D___:
        c_bit = (frd_d < frt_d);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_ULT_S__:
    case C_NGE_S__:
        c_bit = isnan(frd_s) || isnan(frt_s);
        c_bit = c_bit || (frd_s < frt_s);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_ULT_D__:
    case C_NGE_D__:
        c_bit = isnan(frd_d) || isnan(frt_d);
        c_bit = c_bit || (frd_d < frt_d);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_OLE_S__:
    case C_LE_S___:
        c_bit = (frd_s == frt_s);
        c_bit = c_bit || (frd_s < frt_s);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_OLE_D__:
    case C_LE_D___:
        c_bit = (frd_d == frt_d);
        c_bit = c_bit || (frd_d < frt_d);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_ULE_S__:
    case C_NGT_S__:
        c_bit = isnan(frd_s) || isnan(frt_s);
        c_bit = c_bit || (frd_s == frt_s);
        c_bit = c_bit || (frd_s < frt_s);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case C_ULE_D__:
    case C_NGT_D__:
        c_bit = isnan(frd_d) || isnan(frt_d);
        c_bit = c_bit || (frd_d == frt_d);
        c_bit = c_bit || (frd_d < frt_d);
        rslt32 = ((fcsr & ~fcc[shamt >> 2]) | (c_bit * fcc[shamt >> 2]));
        break;
    case LWXC1____:
    case LDXC1____:
        vaddr = rrs + rrt;
        break;
    case SWXC1____:
        vaddr = rrs + rrt;
        rslt32 = frd_w;
        break;
    case SDXC1____:
        vaddr = rrs + rrt;
        rslt64 = frd_l;
        break;
    case MADD_S___:
        rslt_s =  frs_s + frd_s * frt_s;
        break;
    case MADD_D___:
        rslt_d =  frs_d + frd_d * frt_d;
        break;
    case MSUB_S___:
        rslt_s = -frs_s + frd_s * frt_s;
        break;
    case MSUB_D___:
        rslt_d = -frs_d + frd_d * frt_d;
        break;
    case NMADD_S__:
        rslt_s = -frs_s - frd_s * frt_s;
        break;
    case NMADD_D__:
        rslt_d = -frs_d - frd_d * frt_d;
        break;
    case NMSUB_S__:
        rslt_s =  frs_s - frd_s * frt_s;
        break;
    case NMSUB_D__:
        rslt_d =  frs_d - frd_d * frt_d;
        break;
    case L_S______:
        vaddr = rrs + exts32(imm, 16);
        break;
    case L_D______:
        vaddr = rrs + exts32(imm, 16);
        break;
    case S_S______:
        vaddr = rrs + exts32(imm, 16);
        rslt32 = frt_w;
        break;
    case S_D______:
        vaddr = rrs + exts32(imm, 16);
        rslt64 = frt_l;
        break;
    default:
        return -1;
    }
    return 0;
}

/**********************************************************************/
const char *MipsInstFP::getinstname()
{
    if (op != FLOAT_OPS)
        return MipsInst::getinstname();

    if (fop == MOVF_____)      instname = "movf";
    else if (fop == MOVT_____) instname = "movt";
    else if (fop == MFC1_____) instname = "mfc1";
    else if (fop == CFC1_____) instname = "cfc1";
    else if (fop == MTC1_____) instname = "mtc1";
    else if (fop == CTC1_____) instname = "ctc1";
    else if (fop == BC1F_____) instname = "bc1f";
    else if (fop == BC1T_____) instname = "bc1t";
    else if (fop == BC1FL____) instname = "bc1fl";
    else if (fop == BC1TL____) instname = "bc1tl";
    else if (fop == ADD_S____) instname = "add.s";
    else if (fop == ADD_D____) instname = "add.d";
    else if (fop == SUB_S____) instname = "sub.s";
    else if (fop == SUB_D____) instname = "sub.d";
    else if (fop == MUL_S____) instname = "mul.s";
    else if (fop == MUL_D____) instname = "mul.d";
    else if (fop == DIV_S____) instname = "div.s";
    else if (fop == DIV_D____) instname = "div.d";
    else if (fop == SQRT_S___) instname = "sqrt.s";
    else if (fop == SQRT_D___) instname = "sqrt.d";
    else if (fop == ABS_S____) instname = "abs.s";
    else if (fop == ABS_D____) instname = "abs.d";
    else if (fop == MOV_S____) instname = "mov.s";
    else if (fop == MOV_D____) instname = "mov.d";
    else if (fop == NEG_S____) instname = "neg.s";
    else if (fop == NEG_D____) instname = "neg.d";
    else if (fop == ROUND_W_S) instname = "round.w.s";
    else if (fop == ROUND_W_D) instname = "round.w.d";
    else if (fop == TRUNC_W_S) instname = "trunc.w.s";
    else if (fop == TRUNC_W_D) instname = "trunc.w.d";
    else if (fop == CEIL_W_S_) instname = "ceil.w.s";
    else if (fop == CEIL_W_D_) instname = "ceil.w.d";
    else if (fop == FLOOR_W_S) instname = "floor.w.s";
    else if (fop == FLOOR_W_D) instname = "floor.w.d";
    else if (fop == MOVF_S___) instname = "movf.s";
    else if (fop == MOVF_D___) instname = "movf.d";
    else if (fop == MOVT_S___) instname = "movt.s";
    else if (fop == MOVT_D___) instname = "movt.d";
    else if (fop == MOVZ_S___) instname = "movz.s";
    else if (fop == MOVZ_D___) instname = "movz.d";
    else if (fop == MOVN_S___) instname = "movn.s";
    else if (fop == MOVN_D___) instname = "movn.d";
    else if (fop == RECIP_S__) instname = "recip.s";
    else if (fop == RECIP_D__) instname = "recip.d";
    else if (fop == RSQRT_S__) instname = "rsqrt.s";
    else if (fop == RSQRT_D__) instname = "rsqrt.d";
    else if (fop == CVT_S_D__) instname = "cvt.s.d";
    else if (fop == CVT_S_W__) instname = "cvt.s.w";
    else if (fop == CVT_D_S__) instname = "cvt.d.s";
    else if (fop == CVT_D_W__) instname = "cvt.d.w";
    else if (fop == CVT_W_S__) instname = "cvt.w.s";
    else if (fop == CVT_W_D__) instname = "cvt.w.d";
    else if (fop == C_F_S____) instname = "c.f.s";
    else if (fop == C_F_D____) instname = "c.f.d";
    else if (fop == C_UN_S___) instname = "c.un.s";
    else if (fop == C_UN_D___) instname = "c.un.d";
    else if (fop == C_EQ_S___) instname = "c.eq.s";
    else if (fop == C_EQ_D___) instname = "c.eq.d";
    else if (fop == C_UEQ_S__) instname = "c.ueq.s";
    else if (fop == C_UEQ_D__) instname = "c.ueq.d";
    else if (fop == C_OLT_S__) instname = "c.olt.s";
    else if (fop == C_OLT_D__) instname = "c.olt.d";
    else if (fop == C_ULT_S__) instname = "c.ult.s";
    else if (fop == C_ULT_D__) instname = "c.ult.d";
    else if (fop == C_OLE_S__) instname = "c.ole.s";
    else if (fop == C_OLE_D__) instname = "c.ole.d";
    else if (fop == C_ULE_S__) instname = "c.ule.s";
    else if (fop == C_ULE_D__) instname = "c.ule.d";
    else if (fop == C_SF_S___) instname = "c.sf.s";
    else if (fop == C_SF_D___) instname = "c.sf.d";
    else if (fop == C_SEQ_S__) instname = "c.seq.s";
    else if (fop == C_SEQ_D__) instname = "c.seq.d";
    else if (fop == C_NGL_S__) instname = "c.ngl.s";
    else if (fop == C_NGL_D__) instname = "c.ngl.d";
    else if (fop == C_LT_S___) instname = "c.lt.s";
    else if (fop == C_LT_D___) instname = "c.lt.d";
    else if (fop == C_NGE_S__) instname = "c.nge.s";
    else if (fop == C_NGE_D__) instname = "c.nge.d";
    else if (fop == C_LE_S___) instname = "c.le.s";
    else if (fop == C_LE_D___) instname = "c.le.d";
    else if (fop == C_NGT_S__) instname = "c.ngt.s";
    else if (fop == C_NGT_D__) instname = "c.ngt.d";
    else if (fop == LWXC1____) instname = "lwxc1";
    else if (fop == LDXC1____) instname = "ldxc1";
    else if (fop == SWXC1____) instname = "swxc1";
    else if (fop == SDXC1____) instname = "sdxc1";
    else if (fop == MADD_S___) instname = "madd.s";
    else if (fop == MADD_D___) instname = "madd.d";
    else if (fop == MSUB_S___) instname = "msub.s";
    else if (fop == MSUB_D___) instname = "msub.d";
    else if (fop == NMADD_S__) instname = "nmadd.s";
    else if (fop == NMADD_D__) instname = "nmadd.d";
    else if (fop == NMSUB_S__) instname = "nmsub.s";
    else if (fop == NMSUB_D__) instname = "nmsub.d";
    else if (fop == L_S______) instname = "l.s";
    else if (fop == L_D______) instname = "l.d";
    else if (fop == S_S______) instname = "s.s";
    else if (fop == S_D______) instname = "s.d";
    else                       instname = "";
    
    return instname;
}

/**********************************************************************/
char *MipsInstFP::getmnemonic()
{
    if (op != FLOAT_OPS)
        return MipsInst::getmnemonic();

    char *arg;
    arg = new char[MNEMONIC_BUF_SIZE];
    arg[0] = '\0';
    int size = MNEMONIC_BUF_SIZE;

    getinstname();
    switch (fop) {
    case MOVF_____:
    case MOVT_____:
        snprintf(arg, size, "$%d, $%d, $fcc%d", rd, rs, rt >> 2); 
        break;
    case MFC1_____:
    case MTC1_____:
        snprintf(arg, size, "$%d, $f%d", rt, rd);
        break;
    case CFC1_____:
    case CTC1_____:
        snprintf(arg, size, "$%d, c%d", rt, rd);
        break;
    case BC1F_____:
    case BC1T_____:
    case BC1FL____:
    case BC1TL____:
        snprintf(arg, size, "$fcc%d, %08x", rt >> 2, 
                 pc + 4 + (exts32(imm, 16) << 2));
        break;
    case ADD_S____:
    case ADD_D____:
    case SUB_S____:
    case SUB_D____:
    case MUL_S____:
    case MUL_D____:
    case DIV_S____:
    case DIV_D____:
        snprintf(arg, size, "$f%d, $f%d, $f%d", shamt, rd, rt);
        break;
    case SQRT_S___:
    case SQRT_D___:
    case ABS_S____:
    case ABS_D____:
    case MOV_S____:
    case MOV_D____:
    case NEG_S____:
    case NEG_D____:
    case ROUND_W_S:
    case ROUND_W_D:
    case TRUNC_W_S:
    case TRUNC_W_D:
    case CEIL_W_S_:
    case CEIL_W_D_:
    case FLOOR_W_S:
    case FLOOR_W_D:
    case RECIP_S__:
    case RECIP_D__:
    case RSQRT_S__:
    case RSQRT_D__:
    case CVT_S_D__:
    case CVT_S_W__:
    case CVT_D_S__:
    case CVT_D_W__:
    case CVT_W_S__:
    case CVT_W_D__:
        snprintf(arg, size, "$f%d, $f%d", shamt, rd);
        break;
    case MOVF_S___:
    case MOVF_D___:
    case MOVT_S___:
    case MOVT_D___:
        snprintf(arg, size, "$f%d, $f%d, $fcc%d", shamt, rd, rt >> 2);
        break;
    case MOVZ_S___:
    case MOVZ_D___:
    case MOVN_S___:
    case MOVN_D___:
        snprintf(arg, size, "$f%d, $f%d, $%d", shamt, rd, rt);
        break;
    case C_F_S____:
    case C_F_D____:
    case C_UN_S___:
    case C_UN_D___:
    case C_EQ_S___:
    case C_EQ_D___:
    case C_UEQ_S__:
    case C_UEQ_D__:
    case C_OLT_S__:
    case C_OLT_D__:
    case C_ULT_S__:
    case C_ULT_D__:
    case C_OLE_S__:
    case C_OLE_D__:
    case C_ULE_S__:
    case C_ULE_D__:
    case C_SF_S___:
    case C_SF_D___:
    case C_SEQ_S__:
    case C_SEQ_D__:
    case C_NGL_S__:
    case C_NGL_D__:
    case C_LT_S___:
    case C_LT_D___:
    case C_NGE_S__:
    case C_NGE_D__:
    case C_LE_S___:
    case C_LE_D___:
    case C_NGT_S__:
    case C_NGT_D__:
        snprintf(arg, size, "$fcc%d, $f%d, $f%d", shamt >> 2, rd, rt);
        break;        
    case LWXC1____:
    case LDXC1____:
        snprintf(arg, size, "$f%d, $%d($%d)", shamt, rt, rs);
        break;
    case SWXC1____:
    case SDXC1____:
        snprintf(arg, size, "$f%d, $%d($%d)", rd, rt, rs);
        break;
    case MADD_S___:
    case MADD_D___:
    case MSUB_S___:
    case MSUB_D___:
    case NMADD_S__:
    case NMADD_D__:
    case NMSUB_S__:
    case NMSUB_D__:
        snprintf(arg, size, "$f%d, $f%d, $f%d, $f%d", shamt, rs, rd, rt);
        break;
    case L_S______:
    case L_D______:
    case S_S______:
    case S_D______:
        snprintf(arg, size, "$f%d, %d($%d)", rt, exts32(imm, 16), rs);
        break;
    }
    
    snprintf(mnemonic, size, "%-9s%s", instname, arg);
    DELETE_ARRAY(arg);
    return mnemonic;
}

/**********************************************************************/
