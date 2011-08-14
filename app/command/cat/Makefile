
TARGET   = cat
SDKROOT = ../../..
OBJS = $(LIBDIR)/itcfunc.o $(LIBDIR)/mierulib.o $(LIBDIR)/syscall.o\
	   $(LIBDIR)/xmalloc.o $(LIBDIR)/bmp.o $(LIBDIR)/startup.o\
	   main.o

TYPE     = APPLICATION   # STANDALONE(without OS), APPLICATION(with OS)
LCDTYPE  = 1            # 0: OLDITC(MieruPC2008)  1: MieruLCD(MieruPC2010)

# If you want extra runtime options, add those here.
SIMARGS   = 

# Target name like "APP_all" is default,
# so you call it from all and use default behavior.
# But you can change the default behavior to change following targets.
all:
	@$(MAKE) $(MAKETYPE)_$@
run:
	@$(MAKE) $(MAKETYPE)_$@
image:
	@$(MAKE) $(MAKETYPE)_$@
clean:
	@$(MAKE) $(MAKETYPE)_$@
debug:
	@$(MAKE) $(MAKETYPE)_$@
dump:
	@$(MAKE) $(MAKETYPE)_$@
read:
	@$(MAKE) $(MAKETYPE)_$@

install: all
	@mv $(MMCROOT)/$(TARGET) $(MMCROOT)/bin

include $(SDKROOT)/etc/config/base.mk
