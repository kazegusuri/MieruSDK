/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

/**********************************************************************/
Mips::Mips(Board *board, Chip *chip)
{
    this->board = board;
    this->chip = chip;
    this->mc = chip->mc;
    state = CPU_STOP;
}

/**********************************************************************/
Mips::~Mips()
{
    DELETE(as);
    DELETE(ss);
    DELETE(inst);
}

/**********************************************************************/
void Mips::init()
{
    as = new MipsArchstate();
    ss = new MipsSimstate();
    inst = new MipsInst();
    inst->clear();
}

/**********************************************************************/
void Mips::drive()
{
    fetch();
    decode();
    regfetch();
    execute();
    memsend();
    memreceive();
}

/**********************************************************************/
void Mips::update()
{
    writeback();
    setnpc();
}

/**********************************************************************/
void Mips::fetch()
{
    ss->inst_count++;
    inst->clear();
    inst->pc = as->pc;

    if (board->debug_inst && !chip->issilent())
        for (int i = 0; i < board->funcnum; i++)
            if (board->functable[i].addr == inst->pc)
                printf("## %08x: <%s>\n",
                       inst->pc, board->functable[i].name);
}

/**********************************************************************/
void Mips::decode()
{
    if (mc->read(inst->pc, 4, &inst->ir)) {
        printf("!! fetch failure 0x%08x\n", inst->pc);
        board->simerror();
        return;
    }
    inst->decode();

    if (board->debug_inst && !chip->issilent())
        printf("[%10lld] %08x: %s\n",
               ss->inst_count, inst->pc, inst->getmnemonic());

    if (board->imix_mode && !chip->issilent())
        ss->imix[inst->op]++;
}

/**********************************************************************/
void Mips::regfetch()
{
    inst->rrs = as->r[inst->rs];
    inst->rrt = as->r[inst->rt];
    inst->rrd = as->r[inst->rd];
    inst->rhi = as->hi;
    inst->rlo = as->lo;

    if (board->debug_inst == 2 && !chip->issilent()) {
        printf("              ");
        if ((inst->attr & READ_RS) && (inst->rs))
            printf("$%s>%08x ", regname[inst->rs], inst->rrs);
        if ((inst->attr & READ_RT) && (inst->rt))
            printf("$%s>%08x ", regname[inst->rt], inst->rrt);
        if ((inst->attr & READ_RD) && (inst->rd))
            printf("$%s>%08x ", regname[inst->rd], inst->rrd);
        if (inst->attr & READ_HI)
            printf("$hi>%08x ", inst->rhi);
        if (inst->attr & READ_LO)
            printf("$lo>%08x ", inst->rlo);
    }
}

/**********************************************************************/
void Mips::execute()
{
    int ret = inst->execute();
    if (ret == -1) {
        fprintf(stderr, "!! UNKNOWN INSTRUCTION: %08x %08x\n",
                inst->pc, inst->ir);
        board->simerror();
    } else if (ret)
        syscall();

    inst->paddr = (uint064_t) inst->vaddr;
}

/**********************************************************************/
void Mips::memsend()
{
    int ret = 0;
    if (inst->attr & STORE_1B) {
        ret = mc->write(inst->paddr, 1, (uint008_t) inst->rslt32);
    } else if (inst->attr & STORE_2B) {
        ret = mc->write(inst->paddr, 2, (uint016_t) inst->rslt32);
    } else if (inst->attr & STORE_4B_ALIGN) {
        ret = mc->write(inst->paddr, 4, inst->rslt32);
    }
    if (ret) {
        fprintf(stderr, "!! MEMORY ACCESS ERROR: 0x%08llx\n", inst->paddr);
        board->simerror();
    }
}

/**********************************************************************/
void Mips::memreceive()
{
    int ret = 0;
    if (inst->attr & LOAD_1B) {
        uint008_t temp = 0;
        ret = mc->read(inst->paddr, 1, &temp);
        inst->rslt32 = ((inst->op == LBU______) ? temp : exts32(temp, 8));
    } else if (inst->attr & LOAD_2B) {
        uint016_t temp = 0;
        ret = mc->read(inst->paddr, 2, &temp);
        inst->rslt32 = ((inst->op == LHU______) ? temp : exts32(temp, 16));
    } else if (inst->attr & LOAD_4B_ALIGN) {
        ret = mc->read(inst->paddr, 4, &inst->rslt32);
    } else if (inst->attr & LOAD_4B_UNALIGN) {
        ret = mc->read(inst->paddr & ~0x3, 4, &inst->rslt32);
        if (inst->op == LWR______) {
            int shamt = (inst->paddr & 0x3) * 8;
            uint032_t mask = 0xffffffff >> shamt;
            inst->rslt32 = (((inst->rslt32 >> shamt) & mask) |
                            (inst->rrt & ~mask));
        } else {      // LWL______
            int shamt = 24 - (inst->paddr & 0x3) * 8;
            uint032_t mask = 0xffffffff << shamt;
            inst->rslt32 = (((inst->rslt32 << shamt) & mask) |
                            (inst->rrt & ~mask));
        }
    } else if (inst->attr & STORE_4B_UNALIGN) {
        uint032_t temp = 0;
        ret = mc->read(inst->paddr & ~0x3, 4, &temp);
        if (ret) {
        } else if (inst->op == SWR______) {
            int shamt = (inst->paddr & 0x3) * 8;
            uint032_t mask = 0xffffffff << shamt;
            temp = (((inst->rrt << shamt) & mask) | (temp & ~mask));
            ret = mc->write(inst->paddr & ~0x3, 4, temp);
        } else {     // SWL______
            int shamt = 24 - (inst->paddr & 0x3) * 8;
            uint032_t mask = 0xffffffff >> shamt;
            temp = (((inst->rrt >> shamt) & mask) | (temp & ~mask));
            ret = mc->write(inst->paddr & ~0x3, 4, temp);
        }
    }
    if (inst->op == SC_______)
        inst->rslt32 = 1;

    if (ret) {
        fprintf(stderr, "!! MEMORY ACCESS ERROR: 0x%08llx\n", inst->paddr);
        board->simerror();
    }
}

/**********************************************************************/
void Mips::writeback()
{
    if (inst->attr & WRITE_RS)
        as->r[inst->rs] = inst->rslt32;
    if (inst->attr & WRITE_RT)
        as->r[inst->rt] = inst->rslt32;
    if (inst->attr & WRITE_RD)
        as->r[inst->rd] = inst->rslt32;
    if (inst->attr & WRITE_HI)
        as->hi = (uint032_t) (inst->rslt64 >> 32);
    if (inst->attr & WRITE_LO)
        as->lo = (uint032_t) inst->rslt64;
    if ((inst->attr & WRITE_RD_COND) && (inst->cond))
        as->r[inst->rd] = inst->rslt32;
    if (inst->attr & WRITE_RRA)
        as->r[REG_RA] = inst->rslt32;

    if (board->debug_inst == 2 && !chip->issilent()) {
        if ((inst->attr & WRITE_RS) && (inst->rs))
            printf("$%s<%08x ", regname[inst->rs], inst->rslt32);
        if ((inst->attr & WRITE_RT) && (inst->rt))
            printf("$%s<%08x ", regname[inst->rt], inst->rslt32);
        if ((inst->attr & WRITE_RD) && (inst->rd))
            printf("$%s<%08x ", regname[inst->rd], inst->rslt32);
        if (inst->attr & WRITE_HI)
            printf("$hi<%08x ", as->hi);
        if (inst->attr & WRITE_LO)
            printf("$lo<%08x ", as->lo);
        if ((inst->attr & WRITE_RD_COND) && (inst->rd) && (inst->cond))
            printf("$%s<%08x ", regname[inst->rd], inst->rslt32);
        if (inst->attr & WRITE_RRA)
            printf("$ra<%08x ", inst->rslt32);
        printf("\n");
    }

    as->r[0] = 0; // yes, register $zero is always zero
}

/**********************************************************************/
void Mips::setnpc()
{
    if (as->delay_npc) {
        as->pc = as->delay_npc;
        as->delay_npc = 0;
    } else if (((inst->attr & BRANCH) || (inst->attr & BRANCH_LIKELY))
               && (inst->cond)) {
        as->pc += 4;
        as->delay_npc = inst->npc;
        if (!as->delay_npc) {
            printf("!! Branch to zero. stop.\n");
            board->simerror();
        }
    } else if ((inst->attr & BRANCH_NODELAY) && (inst->cond)) {
        as->pc = inst->npc;
        if (!as->pc) {
            printf("!! Branch to zero. stop.\n");
            board->simerror();
        }
    } else if ((inst->attr & BRANCH_LIKELY) && (!inst->cond)) {
        as->pc += 8;
    } else {
        as->pc += 4;
    }
}

/**********************************************************************/
int Mips::running()
{
    return (state > 0);
}

/**********************************************************************/
MipsArchstate::MipsArchstate()
{
    for (int i = 0; i < NREG; i++)
        r[i] = 0;
    hi = lo = 0;
    pc = delay_npc = 0;
}

/**********************************************************************/
void MipsArchstate::print()
{
    for (int i = 0; i < NREG / 8; i++) {
        for (int j = 0; j < 8; j++)
            printf("$%s       ", regname[i * 8 + j]);
        printf("\n");
        for (int j = 0; j < 8; j++)
            printf("%08x  ", r[i * 8 + j]);
        printf("\n");
    }
    printf("pc        hi        lo\n");
    printf("%08x  %08x  %08x\n\n", pc, hi, lo);
}

/*********************************************************************/
MipsSimstate::MipsSimstate()
{
    inst_count = 0;
    for (int i = 0; i < INST_CODE_NUM; i++)
        imix[i] = 0;
}

/*********************************************************************/
void MipsSimstate::print()
{
    ullint temp[INST_CODE_NUM];
    ullint max, lastmax;
    MipsInst *inst = new MipsInst();
    int num = 0, index = 0;

    memcpy(temp, imix, sizeof(imix));
    printf("[[Instruction Statistics]]\n");
    lastmax = 0;
    for (int i = 0; i < INST_CODE_NUM; i++) {
        max = 0;
        for (int j = 0; j < INST_CODE_NUM; j++)
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
        inst->op = index;
        
        printf("[%3d] %-9s%11lld (%7.3f%%)\n",
               num, inst->getinstname(), max,
               (double) max / inst_count * 100.0);
    }
    printf("[---] Total    %11lld\n\n", inst_count);
}

/**********************************************************************/
