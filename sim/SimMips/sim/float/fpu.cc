/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

/**********************************************************************/
void MipsFP::init()
{
    as = asfp = new MipsArchstateFP();
    ss = ssfp = new MipsSimstateFP();
    inst = instfp = new MipsInstFP();
    inst->clear();
}


/**********************************************************************/
void MipsFP::decode()
{
    Mips::decode();
    if (board->imix_mode && inst->op == FLOAT_OPS && !chip->issilent())
        ssfp->fimix[instfp->fop]++;
}

/**********************************************************************/
void MipsFP::regfetch()
{
    Mips::regfetch();
    if (inst->op != FLOAT_OPS)
        return;

    instfp->frd_w = asfp->fr.w[inst->rd];
    instfp->frd_l = asfp->fr.l[inst->rd / 2];
    instfp->frd_s = asfp->fr.s[inst->rd];
    instfp->frd_d = asfp->fr.d[inst->rd / 2];
    instfp->frt_w = asfp->fr.w[inst->rt];
    instfp->frt_l = asfp->fr.l[inst->rt / 2];
    instfp->frt_s = asfp->fr.s[inst->rt];
    instfp->frt_d = asfp->fr.d[inst->rt / 2];
    instfp->frs_s = asfp->fr.s[inst->rs];
    instfp->frs_d = asfp->fr.d[inst->rs / 2];
    instfp->fcsr  = asfp->fcsr;

    if (board->debug_inst == 2 && !chip->issilent()) {
        if (instfp->fattr & READ_FRD_S)
            printf("$f%.2d>%-12g ",   inst->rd, instfp->frd_s);
        if (instfp->fattr & READ_FRD_D)
            printf("$d%.2d>%-12g ",   inst->rd, instfp->frd_d);
        if (instfp->fattr & READ_FRD_W)
            printf("$f%.2d>%08x ",    inst->rd, instfp->frd_w);
        if (instfp->fattr & READ_FRD_L)
            printf("$d%.2d>%016llx ", inst->rd, instfp->frd_l);
        if (instfp->fattr & READ_FRT_S)
            printf("$f%.2d>%-12g ",   inst->rt, instfp->frt_s);
        if (instfp->fattr & READ_FRT_D)
            printf("$d%.2d>%-12g ",   inst->rt, instfp->frt_d);
        if (instfp->fattr & READ_FRT_W)
            printf("$f%.2d>%08x ",    inst->rt, instfp->frt_w);
        if (instfp->fattr & READ_FRT_L)
            printf("$d%.2d>%016llx ", inst->rt, instfp->frt_l);
        if (instfp->fattr & READ_FRS_S)
            printf("$f%.2d>%-12g ",   inst->rs, instfp->frs_s);
        if (instfp->fattr & READ_FRS_D)
            printf("$d%.2d>%-12g ",   inst->rs, instfp->frs_d);
        if (instfp->fattr & READ_FCSR)
            printf("FCSR>%08x ", instfp->fcsr);
    }
}

/**********************************************************************/
void MipsFP::memsend()
{
    Mips::memsend();
    if (instfp->fattr & STORE_8B)
        if (mc->write(inst->paddr, 8, inst->rslt64)) {
            fprintf(stderr, "!! MEMORY ACCESS ERROR: 0x%08llx\n",
                    inst->paddr);
            board->simerror();
        }
}

/**********************************************************************/
void MipsFP::memreceive()
{
    Mips::memreceive();
    if (instfp->fattr & LOAD_8B)
        if (mc->read(inst->paddr, 8, &inst->rslt64)) {
            fprintf(stderr, "!! MEMORY ACCESS ERROR: 0x%08llx\n",
                    inst->paddr);
            board->simerror();
        }
}

/**********************************************************************/
void MipsFP::writeback()
{
    if (instfp->fattr & WRITE_FRD_W)
        asfp->fr.w[inst->rd]        = inst->rslt32;
    if (instfp->fattr & WRITE_FRT_W)
        asfp->fr.w[inst->rt]        = inst->rslt32;
    if (instfp->fattr & WRITE_FRT_L)
        asfp->fr.l[inst->rt / 2]    = inst->rslt64;
    if (instfp->fattr & WRITE_FSH_S)
        asfp->fr.s[inst->shamt]     = instfp->rslt_s;
    if (instfp->fattr & WRITE_FSH_D)
        asfp->fr.d[inst->shamt / 2] = instfp->rslt_d;
    if (instfp->fattr & WRITE_FSH_W)
        asfp->fr.w[inst->shamt]     = inst->rslt32;
    if (instfp->fattr & WRITE_FSH_L)
        asfp->fr.l[inst->shamt / 2] = inst->rslt64;
    if (instfp->fattr & WRITE_FSH_S_COND && inst->cond)
        asfp->fr.s[inst->shamt]     = instfp->rslt_s;
    if (instfp->fattr & WRITE_FSH_D_COND && inst->cond)
        asfp->fr.d[inst->shamt / 2] = instfp->rslt_d;
    if (instfp->fattr & WRITE_FCSR)
        asfp->fcsr                  = inst->rslt32;

    if (board->debug_inst == 2 && !chip->issilent()) {
        if (instfp->fattr & WRITE_FRD_W)
            printf("$f%.2d<%08x ",    inst->rd,    inst->rslt32);
        if (instfp->fattr & WRITE_FRT_W)
            printf("$f%.2d<%08x ",    inst->rt,    inst->rslt32);
        if (instfp->fattr & WRITE_FRT_L)
            printf("$d%.2d<%016llx ", inst->rt,    inst->rslt64);
        if (instfp->fattr & WRITE_FSH_S)
            printf("$f%.2d<%-12g ",   inst->shamt, instfp->rslt_s);
        if (instfp->fattr & WRITE_FSH_D)
            printf("$d%.2d<%-12g ",   inst->shamt, instfp->rslt_d);
        if (instfp->fattr & WRITE_FSH_W)
            printf("$f%.2d<%08x ",    inst->shamt, inst->rslt32);
        if (instfp->fattr & WRITE_FSH_L)
            printf("$d%.2d<%016llx ", inst->shamt, inst->rslt64);
        if (instfp->fattr & WRITE_FSH_S_COND && inst->cond)
            printf("$f%.2d<%-12g ",   inst->shamt, instfp->rslt_s);
        if (instfp->fattr & WRITE_FSH_D_COND && inst->cond)
            printf("$d%.2d<%-12g ",   inst->shamt, instfp->rslt_d);
        if (instfp->fattr & WRITE_FCSR)
            printf("FCSR<%08x ",      inst->rslt32);
    }
    Mips::writeback();
}

/**********************************************************************/
MipsArchstateFP::MipsArchstateFP()
{
    for (int i = 0; i < NFREG; i++)
        fr.w[i] = 0;
    fcsr = 0;
}

/**********************************************************************/
void MipsArchstateFP::print()
{
    MipsArchstate::print();
    for (int i = 0; i < NFREG; i += 2) {
        printf("              f%.2d=%08x(%12g) "
               "f%.2d=%08x(%12g) (d%.2d=%12g)\n",
               i, fr.w[i], fr.s[i], i + 1,
               fr.w[i + 1], fr.s[i + 1], i, fr.d[i / 2]);
    }
    printf("\n");
}

/**********************************************************************/
MipsSimstateFP::MipsSimstateFP()
{
    for (int i = 0; i < FP_CODE_NUM; i++)
        fimix[i] = 0;
}

/**********************************************************************/
void MipsSimstateFP::print()
{
    ullint temp[FP_CODE_NUM];
    ullint max, lastmax, fp_inst_count;
    MipsInstFP *inst = new MipsInstFP();
    int num = 0, index = 0;

    MipsSimstate::print();
    fp_inst_count = imix[FLOAT_OPS];
    memcpy(temp, fimix, sizeof(fimix));
    printf("[[FP Instruction Statistics]]\n");
    lastmax = 0;
    for (int i = 0; i < FP_CODE_NUM; i++) {
        max = 0;
        for (int j = 0; j < FP_CODE_NUM; j++)
            if (max < temp[j]) {
                max = temp[j];
                index = j;
            }
        if (max == 0)
            break;
        temp[index] = 0;
        if (max != lastmax) {
            lastmax = max;
            num = i + 1;
        }
        inst->op  = FLOAT_OPS;
        inst->fop = index;
        
        printf("[%3d] %-9s%11lld (%7.3f%%)\n",
               num, inst->getinstname(), max,
               (double) max / fp_inst_count * 100.0);
    }
    printf("[---] Total    %11lld\n\n", fp_inst_count);
}

/**********************************************************************/
