
TARGET   = mieruos

export SDKROOT = ../
OSDIR   = $(SDKROOT)/os
SRCDIR  = $(OSDIR)/src
INCDIR  = $(OSDIR)/include
LIBDIR  = $(SDKROOT)/lib
STLDIR  = $(OSDIR)/stl
TESTDIR = $(OSDIR)/test

MEMTXT   = conf/mem.txt
SCRIPT   = conf/os.ld.script
MAKETYPE = std

COMPILE = $(MIPSCXX)
DEBUGOPT = -g 
INCLUDEOPT = -I$(INCDIR) -I$(LIBDIR) -I$(STLDIR) -I$(TESTDIR)
MACROOPT = -DMIERUPC_STANDALONE -DMPC_MLIB_RENAME -DMIERU_OS
WARNINGOPT = -Wall -Wextra -Wformat=2 -Wcast-qual -Wwrite-strings -Wconversion -Wfloat-equal -Wpointer-arith -Winit-self -Wredundant-decls -finline -finline-functions -fno-implement-inlines -Winline -fkeep-inline-functions
STANDALONEOPT = -ffreestanding -msoft-float -nostdlib -nostdinc -fno-builtin -fno-exceptions -fno-rtti -nostartfiles -nodefaultlibs 
MISCOPT = -std=c++0x -mips32 -O2 -fno-strict-aliasing -fno-stack-protector -fcheck-new
CFLAGS  ?= $(DEBUGOPT) $(WARNINGOPT) $(STANDALONEOPT) $(MISCOPT) $(MACROOPT) $(INCLUDEOPT)
SFLAGS  ?= -mips32r2

OBJS = $(SRCDIR)/cmain.o $(SRCDIR)/kernel.o $(SRCDIR)/boot.o $(SRCDIR)/task.o \
	$(LIBDIR)/mierulib.o $(LIBDIR)/itcfunc.o $(LIBDIR)/syscall.o $(SRCDIR)/new.o \
	$(SRCDIR)/exception.o \
	$(SRCDIR)/entry.o $(SRCDIR)/syscall.o $(SRCDIR)/timer.o \
	$(SRCDIR)/fatfile.o $(SRCDIR)/fat.o $(SRCDIR)/elf.o \
	$(SRCDIR)/shell.o $(SRCDIR)/point.o $(SRCDIR)/cache.o \
	$(SRCDIR)/kmalloc.o $(SRCDIR)/assert.o $(SRCDIR)/register.o \
	$(SRCDIR)/sandbox.o \
	$(TESTDIR)/test.o \
	$(TESTDIR)/test_kmalloc.o $(TESTDIR)/test_list.o\


all:
	@cat $(SDKROOT)/VERSION
	@$(MAKE) $(TARGET)
	@$(MAKE) image
	@cp $(TARGET).bin $(MMCROOT)/

include $(SDKROOT)/etc/config/base.mk

run: mmc
	@cat $(SDKROOT)/VERSION
	@$(MAKE) $(MAKETYPE)_$@
debug: mmc
	@$(MAKE) $(MAKETYPE)_$@

clean:
	make -C $(LIBDIR) clean
	make -C $(SRCDIR) clean
	make -C $(INCDIR) clean
	make -C $(TESTDIR) clean
	rm -f *.o *~ $(TARGET) $(TARGET).bin log.txt

.PHONY: mmc
mmc:
	@if [ $(strip $(APP)) ]; then echo $(APP) > $(MMCROOT)/startup;\
	 else rm -f $(MMCROOT)/startup; fi;
	@$(MAKE) -C $(FATOPDIR) in FILE=$(TARGET).bin > /dev/null

.PHONY: image
image:
	@echo Create image file. $(TARGET).bin
	@$(MEMGEN) -b $(TARGET) 128 > $(TARGET).bin


## This is just for flymake(emacs)
.PHONY: check-syntax
check-syntax:
	$(COMPILE) $(CFLAGS) -fsyntax-only  $(CHK_SOURCES)
