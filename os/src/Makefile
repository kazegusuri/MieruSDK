

clean:
	rm -f *.o *~


## this is just a setting for flymake
COMPILE=mipsel-linux-g++
MACROOPT = -DMIERUPC_STANDALONE -DMPC_MLIB_RENAME
CFLAGS  = -Wall -ffreestanding -msoft-float -Os -nostdlib -nostdinc -fno-builtin -fno-exceptions -fno-rtti -fno-strict-aliasing  -nostartfiles -nodefaultlibs -fno-stack-protector -fcheck-new $(MACROOPT)

.PHONY: check-syntax
check-syntax:
	$(COMPILE) $(CFLAGS) -fsyntax-only -I../include -I../../lib  $(CHK_SOURCES) 

