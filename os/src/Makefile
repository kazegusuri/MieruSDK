
SDKROOT = ../..
OSDIR = $(SDKROOT)/os
LIBDIR = $(SDKROOT)/lib
INCDIR = $(OSDIR)/include
STLDIR = $(OSDIR)/stl
TESTDIR = $(OSDIR)/test

clean:
	rm -f *.o *~

## this is just a setting for flymake
DEBUGOPT = 
INCLUDEOPT = -I$(INCDIR) -I$(LIBDIR) -I$(STLDIR) -I$(TESTDIR)
MACROOPT = -DMIERUPC_STANDALONE -DMPC_MLIB_RENAME -DMIERU_OS
WARNINGOPT = -Wall -Wextra -Wformat=2 -Wcast-qual -Wwrite-strings -Wconversion -Wfloat-equal -Wpointer-arith -Winit-self -Wredundant-decls 
STANDALONEOPT = -ffreestanding -msoft-float -nostdlib -nostdinc -fno-builtin -fno-exceptions -fno-rtti -nostartfiles -nodefaultlibs 
MISCOPT = -std=c++0x -Os -fno-strict-aliasing -fno-stack-protector -fcheck-new
CFLAGS  ?= $(DEBUGOPT) $(WARNINGOPT) $(STANDALONEOPT) $(MISCOPT) $(MACROOPT) $(INCLUDEOPT)

COMPILE=mipsel-linux-g++

.PHONY: check-syntax
check-syntax:
	$(COMPILE) $(CFLAGS) -fsyntax-only $(CHK_SOURCES) 

