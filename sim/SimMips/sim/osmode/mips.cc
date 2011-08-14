/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

#define SGN(x) ((x) & 0x80000000)

enum {
    LOADSTORE_2B       = LOAD_2B | STORE_2B,
    LOADSTORE_4B_ALIGN = LOAD_4B_ALIGN | STORE_4B_ALIGN,

    TLB_OP_READ         = 1,
    TLB_OP_WRITE_INDEX  = 2,
    TLB_OP_WRITE_RANDOM = 3,
    TLB_OP_LOOKUP       = 4,
};

void Mips::syscall() {}

/**********************************************************************/
MipsCZ::MipsCZ(BoardCZ *board, ChipCZ *chip) : Mips(board, chip)
{
    this->cp0 = chip->cp0;
}

/**********************************************************************/
void MipsCZ::init()
{
    as = new MipsArchstate();
    ss = new MipsSimstate();
    inst = new MipsInstCZ();
}

/**********************************************************************/
void MipsCZ::drive()
{
    if (cp0->checkinterrupt()) {
        exception(EXC_INT____);
        return;
    }
    if (state == CPU_WAIT)
        return;
    Mips::drive();
}

/**********************************************************************/
void MipsCZ::update()
{
    if (state == CPU_WAIT)
        return;
    Mips::update();
}

/**********************************************************************/
void MipsCZ::decode()
{
    uint064_t phaddr = (uint064_t) inst->pc;
    int ret;

    if ((ret = cp0->getphaddr(inst->pc, &phaddr, 0)) != 0) {
        inst->vaddr = inst->pc;
        exception(ret);
        return;
    }
    
    if (mc->read(phaddr, 4, &inst->ir)) {
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
void MipsCZ::regfetch()
{
    Mips::regfetch();
    if (inst->op == MFC0_____) {
        uint032_t rc0 = cp0->readreg(inst->rd + inst->sel * 32);
        static_cast<MipsInstCZ *>(inst)->rc0 = rc0;
        if (board->debug_inst == 2 && !chip->issilent())
            printf("%d.%d>%08x ", inst->rd, inst->sel, rc0);
    } else if (inst->op == ERET_____) {
        uint032_t rc0 = cp0->readreg(CP0_EPC_____);
        static_cast<MipsInstCZ *>(inst)->rc0 = rc0;
        if (board->debug_inst == 2 && !chip->issilent())
            printf("%d.0>%08x ", CP0_EPC_____, rc0);        
    }
}

/**********************************************************************/
void MipsCZ::execute()
{
    int ret = inst->execute();
    // address translation
    if (inst->attr & LOADSTORE) {
        if ((inst->attr & (LOADSTORE_2B      ) && inst->vaddr & 0x1) ||
            (inst->attr & (LOADSTORE_4B_ALIGN) && inst->vaddr & 0x3) ) {
            ret = EXC_ADEL___;
        } else {
            ret = cp0->getphaddr(inst->vaddr, &inst->paddr, 
                                 ((inst->attr & STORE_ANY) != 0));
        }
    }
    if (ret == -1) {
        fprintf(stderr, "!! UNKNOWN INSTRUCTION: %08x %08x\n",
                inst->pc, inst->ir);
        board->simerror();
    } else if (ret)
        exception(ret);
}

/**********************************************************************/
void MipsCZ::writeback()
{
    if (inst->op == MTC0_____) {
        cp0->writereg(inst->rd + inst->sel * 32, inst->rslt32);
        if (board->debug_inst == 2 && !chip->issilent())
            printf("%d.%d>%08x ", inst->rd, inst->sel, inst->rslt32);
    }
    Mips::writeback();

    int tlbop = static_cast<MipsInstCZ *>(inst)->tlbop;
    if (tlbop == TLB_OP_READ)
        cp0->tlbread();
    else if  (tlbop == TLB_OP_WRITE_INDEX){
        cp0->tlbwrite(0);}
    else if (tlbop == TLB_OP_WRITE_RANDOM)
        cp0->tlbwrite(1);
    else if (tlbop == TLB_OP_LOOKUP)
        cp0->tlblookup();
}

/**********************************************************************/
void MipsCZ::setnpc()
{
    if (exc_occur) {
        as->pc = cp0->doexception(exc_code, as->pc, 
                                  inst->vaddr, (as->delay_npc) ? 1 : 0);
        as->delay_npc = 0;
        exc_occur = 0;
    } else {
        Mips::setnpc();
    }
    if (inst->op == ERET_____)
        cp0->modifyreg(CP0_SR______, 0, 0x2);
    if (state != CPU_ERROR && inst->op == WAIT_____)
        state = CPU_WAIT;
}

/**********************************************************************/
void MipsCZ::exception(int code)
{
    if (exc_occur)
        return;
    exc_occur = 1;
    exc_code = code;
    if (state == CPU_WAIT)
        state = CPU_START;
    if (static_cast<BoardCZ *>(board)->debug_exctlb && !chip->issilent())
        printf("## exception #%d (PC=0x%08x vaddr=0x%08x)\n",
               code, as->pc, inst->vaddr);
    inst->clear();
}

/**********************************************************************/
void MipsInstCZ::clear()
{
    MipsInst::clear();
    rc0 = 0;
    tlbop = 0;
}

/**********************************************************************/
int MipsInstCZ::execute()
{
    int ret =  MipsInst::execute();
    switch (op) {
    // === syscall, breakpoint ===
    case SYSCALL__:
        ret = EXC_SYSCALL;
        break;
    case BREAK____:
        ret = EXC_BP_____;
        break;
    // === overflow ===
    case ADD______:
        if ((SGN(rrs) == SGN(rrt)) && (SGN(rrs) != SGN(rslt32)))
            ret = EXC_OV_____;
        break;
    case SUB______:
        if ((SGN(rrs) != SGN(rrt)) && (SGN(rrs) != SGN(rslt32)))
            ret = EXC_OV_____;
        break;
    case ADDI_____:
        if ((SGN(rrs)    == SGN(exts32(imm, 16))) &&
            (SGN(rslt32) != SGN(exts32(imm, 16))))
            ret = EXC_OV_____;
        break;
    // === trap ===
    case TGE______:
        if ((int032_t) rrs >= (int032_t) rrt)
            ret = EXC_TRAP___;
        break;
    case TGEU_____:
        if (rrs >= rrt)
            ret = EXC_TRAP___;
        break;
    case TLT______:
        if ((int032_t) rrs < (int032_t) rrt)
            ret = EXC_TRAP___;
        break;
    case TLTU_____:
        if (rrs < rrt)
            ret = EXC_TRAP___;
        break;
    case TEQ______:
        if (rrs == rrt)
            ret = EXC_TRAP___;        
        break;
    case TNE______:
        if (rrs != rrt)
            ret = EXC_TRAP___;        
        break;
    case TGEI_____:
        if ((int032_t) rrs >= (int032_t) exts32(imm, 16))
            ret = EXC_TRAP___;
        break;
    case TGEIU____:
        if (rrs >= exts32(imm, 16))
            ret = EXC_TRAP___;
        break;
    case TLTI_____:
        if ((int032_t) rrs < (int032_t) exts32(imm, 16))
            ret = EXC_TRAP___;
        break;
    case TLTIU____:
        if (rrs < exts32(imm, 16))
            ret = EXC_TRAP___;
        break;
    case TEQI_____:
        if (rrs == exts32(imm, 16))
            ret = EXC_TRAP___;        
        break;
    case TNEI_____:
        if (rrs != exts32(imm, 16))
            ret = EXC_TRAP___;        
        break;
    // === cp0 control and others ===
    case MFC0_____:
        rslt32 = rc0;
        break;
    case TLBR_____:
        tlbop = TLB_OP_READ;
        break;
    case TLBWI____:
        tlbop = TLB_OP_WRITE_INDEX;
        break;
    case TLBWR____:
        tlbop = TLB_OP_WRITE_RANDOM;
        break;
    case TLBP_____:
        tlbop = TLB_OP_LOOKUP;
        break;
    case ERET_____:
        npc = rc0;
        cond = 1;
        break;
    case FLOAT_OPS:
        ret = EXC_CPU____ | EXC_CPU1___;
        break;
    }
    return ret;
}

/**********************************************************************/
