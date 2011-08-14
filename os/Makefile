
TARGET   = mieruos

export SDKROOT = ../
OSDIR   = $(SDKROOT)/os
SRCDIR  = $(OSDIR)/src
INCDIR  = $(OSDIR)/include
LIBDIR  = $(SDKROOT)/lib

MEMTXT   = conf/mem.txt
MACROOPT = -DMIERUPC_STANDALONE -DMPC_MLIB_RENAME
SCRIPT   = conf/os.ld.script
MAKETYPE = std

COMPILE = $(MIPSCXX)
WARNINGOPT = -Wall -Wextra -Wformat=2 -Wcast-qual -Wwrite-strings -Wconversion -Wfloat-equal -Wpointer-arith -Winit-self -Weffc++ -Wredundant-decls
CFLAGS  ?= $(WARNINGOPT) -ffreestanding -msoft-float -Os -nostdlib -nostdinc -fno-builtin -fno-exceptions -fno-rtti -fno-strict-aliasing  -nostartfiles -nodefaultlibs -fno-stack-protector -fcheck-new -I$(INCDIR) -I$(LIBDIR) $(MACROOPT)


OBJS = $(SRCDIR)/cmain.o $(SRCDIR)/kernel.o $(SRCDIR)/boot.o $(SRCDIR)/task.o \
	$(LIBDIR)/mierulib.o $(LIBDIR)/itcfunc.o $(SRCDIR)/new.o \
	$(SRCDIR)/exc.o $(SRCDIR)/syscall.o \
	$(SRCDIR)/fatfile.o $(SRCDIR)/fat.o $(SRCDIR)/elf.o \
	$(SRCDIR)/shell.o $(SRCDIR)/point.o $(SRCDIR)/cache.o


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
