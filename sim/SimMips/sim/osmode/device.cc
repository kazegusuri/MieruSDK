/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"
#include <ncurses.h>

enum {
    PIC_RD_IRR = 0,
    PIC_RD_ISR = 1,
    PIC_RD_BUFIRR = 2,
    PIC_CONNECTED = 2,

    PIC_PRI_ADDR = 0x20,
    PIC_SEC_ADDR = 0xa0,
    PIC_ADDR_RANGE = 2,

    SIO_RBR = 0,
    SIO_IER = 1,
    SIO_IIR = 2,
    SIO_LCR = 3,
    SIO_MCR = 4,
    SIO_LSR = 5,
    SIO_MSR = 6,
    SIO_SCR = 7,

    SIO_IER_TX = 0x02,
    SIO_IER_RX = 0x01,
    SIO_IIR_FIFO = 0xc0,
    SIO_FIFO_SH = 5,
    SIO_IIR_TX = 0x02,
    SIO_IIR_RX = 0x04,
    SIO_IIR_NP = 0x01,
    SIO_FCR_FIFO = 0x06,
    SIO_LCR_DIV = 0x80,
    SIO_MCR_INT = 0x08,
    SIO_LSR_VALUE = 0x60,
    SIO_MSR_VALUE = 0x0a,
    SIO_CONNECTED = 4,

    SIO_PRI_ADDR = 0x3f8,
    SIO_ADDR_RANGE = 8,

    SIO_POLL_CYCLE = 0x100,
};

/* Interrupt Controller, integrating TWO 8259-like PIC                */
/**********************************************************************/
IntController::IntController(MipsCp0 *cp0)
{
    this->cp0 = cp0;
    for (int i = 0; i < 2; i++) {
        imr[i] = 0xff;
        irr[i] = 0;
        isr[i] = 0;
        tobe_read[i] = PIC_RD_BUFIRR;
        init_mode[i] = 0; 
    }
}

/**********************************************************************/
int IntController::checkaddr(uint032_t addr)
{
    uint032_t base = addr & ~0x1;
    if (base == PIC_PRI_ADDR)
        return 0;
    else if (base == PIC_SEC_ADDR)
        return 1;
    else
        return -1;
}
/**********************************************************************/
void IntController::recalcirq()
{
    // Secondary PIC(IRQ 8-15) is connected to IRQ2
    irr[0] = ((irr[0] & ~(1 << 2)) |
              (((irr[1] & ~imr[1]) != 0) << 2));

    for (int i = 0; i < 2; i++) {
        isr[i] = 0;
        for (int j = 0; j < 8; j++) {
            if (irr[i] & ~imr[i] & (1 << j)) {
                isr[i] = 1 << j;
                break;
            }
        }
    }
    if (isr[0])
        cp0->setinterrupt(PIC_CONNECTED);
    else
        cp0->clearinterrupt(PIC_CONNECTED);
}

/**********************************************************************/
int IntController::read(uint032_t addr, uint size, void *data)
{
    uint008_t *dat = (uint008_t *) data;
    if (size != 1)
        return 1;
    int ch = checkaddr(addr);
    if (ch < 0)
        return 1;

    if ((addr & 0x1) == 0) {
        if        (tobe_read[ch] == PIC_RD_IRR) {
            *dat = irr[ch];
        } else if (tobe_read[ch] == PIC_RD_ISR) {
            *dat = isr[ch];
        } else {                 // PIC_RD_BUFIRR
            *dat = 0x00;
            for (int i = 0; i < 8; i++) {
                if (irr[ch] & (1 << i)) {
                    *dat = 0x80 + i;
                    break;
                }
            }
        }
    } else {
        *dat = imr[ch];
    }
    return 0;
}

/**********************************************************************/
int IntController::write(uint032_t addr, uint size, uint064_t data)
{
    if (size != 1)
        return 1;
    int ch = checkaddr(addr);
    if (ch < 0)
        return 1;

    if ((addr & 0x1) == 0) {
        if (data == 0x0a) {
            tobe_read[ch] = PIC_RD_IRR;
        } else if (data == 0x0b) {
            tobe_read[ch] = PIC_RD_ISR;
        } else if (data == 0x0c) {
            tobe_read[ch] = PIC_RD_BUFIRR;
        } else if ((data >= 0x10) && (data <= 0x1f)) {
            init_mode[ch] = 2 + (data & 0x1);
        } else if ((data >= 0x20) && (data <= 0x27)) {
            irr[ch] &= ~isr[ch];
            isr[ch] = 0;
            recalcirq();
        } else if ((data >= 0x60) && (data <= 0x67)) {
            irr[ch] &= ~(1 << (data & 0xf));
            isr[ch] &= ~(1 << (data & 0xf));
            recalcirq();
        } else {
            printf("## IntController: undefined op: 0x%02llx\n", data);
        }
    } else {
        if (init_mode[ch]) {
            init_mode[ch]--;
        } else {
            imr[ch] = data;
            recalcirq();
        }
    }
    return 0;
}

/**********************************************************************/
void IntController::setinterrupt(int irq)
{
    irr[irq / 8] |= 1 << (irq % 8);
    recalcirq();
}

/**********************************************************************/
void IntController::clearinterrupt(int irq)
{
    irr[irq / 8] &= ~(1 << (irq % 8));
    recalcirq();
}

/* Serial I/O Controller, simplifying ns16550                         */
/**********************************************************************/
SerialIO::SerialIO(Chip *chip, IntController *pic)
{
    this->chip = chip;
    this->pic = pic;
    ier = 0;
    iir = SIO_IIR_NP;
    lcr = 0;
    mcr = 0;
    scr = 0;
    counter = 0;
    divisor = 12;
    currentchar = -1;
}

/**********************************************************************/
void SerialIO::update()
{
    if (++counter < SIO_POLL_CYCLE)
        return;

    counter = 0;
    if (charavail()) {
        iir |= SIO_IIR_RX;
        recalcirq();
    }
}

/**********************************************************************/
int SerialIO::charavail()
{
    char buf;
    int ret;

    if (currentchar != -1)
        return 1;
    ret = ::read(STDIN_FILENO, &buf, sizeof(buf));
    if (ret == 1) {
        currentchar = ((int) buf) & 0xff;
        return 1;
    }
    return 0;
}

/**********************************************************************/
void SerialIO::recalcirq()
{
    int int_pend = 0;
    int_pend = ((ier & SIO_IER_TX) && (iir & SIO_IIR_TX));
    int_pend = int_pend || ((ier & SIO_IER_RX) && (iir & SIO_IIR_RX));
    iir = (iir & ~SIO_IIR_NP) | ((int_pend) ? 0 : SIO_IIR_NP);
    int_pend = int_pend && (mcr & SIO_MCR_INT);
    if (int_pend)
        pic->setinterrupt(SIO_CONNECTED);
    else
        pic->clearinterrupt(SIO_CONNECTED);
}

/**********************************************************************/
int SerialIO::read(uint032_t addr, uint size, void *data)
{
    uint008_t *dat = (uint008_t *) data;
    if (size != 1)
        return 1;
    switch(addr) {
    case SIO_RBR:
        if (lcr & SIO_LCR_DIV) {
            *dat = divisor & 0xff;
        } else {
            *dat = (uint008_t) currentchar;
            currentchar = -1;
            iir &= ~SIO_IIR_RX;
            recalcirq();
        }
        break;
    case SIO_IER:
        *dat = (lcr & SIO_LCR_DIV) ? divisor >> 8 : ier;
        break;
    case SIO_IIR:
        *dat = iir;
        iir &= ~SIO_IIR_TX;
        recalcirq();
        break;
    case SIO_LCR:
        *dat = lcr;
        break;
    case SIO_MCR:
        *dat = mcr;
        break;
    case SIO_LSR:
        *dat = SIO_LSR_VALUE + ((iir & SIO_IIR_RX) != 0);
        break;
    case SIO_MSR:
        *dat = SIO_MSR_VALUE;
        break;
    case SIO_SCR:
        *dat = scr;
        break;
    }
    return 0;
}

/**********************************************************************/
int SerialIO::write(uint032_t addr, uint size, uint064_t data)
{
    if (size != 1)
        return 1;
    switch(addr) {
    case SIO_RBR:
        if (lcr & SIO_LCR_DIV) {
            divisor = (divisor & 0xff00) | data;
        } else {
            if (!chip->issilent()) {
                putchar((int) data & 0xff);
                fflush(stdout);
            }
            iir |= SIO_IIR_TX;
            recalcirq();
        }
        break;
    case SIO_IER:
        if (lcr & SIO_LCR_DIV) {
            divisor = (divisor & 0x00ff) | ((uint) data << 8);
        } else {
            if (((ier & SIO_IER_TX) == 0) && ((data & SIO_IER_TX) != 0))
                iir |= SIO_IIR_TX;
            ier = data;
            recalcirq();
        }
        break;
    case SIO_IIR:
        iir = ((iir & ~SIO_IIR_FIFO) | 
               ((data & SIO_FCR_FIFO) << SIO_FIFO_SH));
        break;
    case SIO_LCR:
        lcr = data;
        break;
    case SIO_MCR:
        mcr = data;
        recalcirq();
        break;
    case SIO_LSR:
    case SIO_MSR:
        // lsr, msr are read-only
        break;
    case SIO_SCR:
        scr = data;
        break;
    }
    return 0;
}

/* ISA Bus I/O                                                        */
/**********************************************************************/
IsaIO::IsaIO()
{
    pic = NULL;
    sio = NULL;
}

/**********************************************************************/
IsaIO::~IsaIO()
{
    DELETE(pic);
    DELETE(sio);
}

/**********************************************************************/
void IsaIO::init(Board *board)
{
    pic = new IntController(static_cast<ChipCZ *>(board->chip)->cp0);
    sio = new SerialIO(board->chip, pic);
}

/**********************************************************************/
void IsaIO::update()
{
    sio->update();
}

/**********************************************************************/
int IsaIO::read(uint032_t addr, uint size, void *data)
{
    if ((addr - PIC_PRI_ADDR < PIC_ADDR_RANGE) ||
        (addr - PIC_SEC_ADDR < PIC_ADDR_RANGE))
        return pic->read(addr, size, data);
    else if (addr - SIO_PRI_ADDR < SIO_ADDR_RANGE)
        return sio->read(addr - SIO_PRI_ADDR, size, data);
    else if (size == 1)
        *(uint008_t *)data = 0x00;
    return 0;
}

/**********************************************************************/
int IsaIO::write(uint032_t addr, uint size, uint064_t data)
{
    if ((addr - PIC_PRI_ADDR < PIC_ADDR_RANGE) ||
        (addr - PIC_SEC_ADDR < PIC_ADDR_RANGE))
        return pic->write(addr, size, data);
    else if (addr - SIO_PRI_ADDR < SIO_ADDR_RANGE)
        return sio->write(addr - SIO_PRI_ADDR, size, data);
    return 0;
}

/**********************************************************************/
