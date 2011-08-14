export SDKROOT
export SIMMIPS  = $(SDKROOT)/sim/SimMips/bin/SimMieru
export LCD      = $(SDKROOT)/sim/lcd/lcd
export MEMGEN   = $(SDKROOT)/etc/memgen/memgen

export LIBDIR   = $(SDKROOT)/lib
export OSDIR    = $(SDKROOT)/os
export FATOPDIR = $(SDKROOT)/etc/fatop
export MMCROOT  = $(SDKROOT)/etc/fatop/mmcroot

## You should change $CMDPREF at first
CMDPREF = 
MIPSCC  ?= $(CMDPREF)mipsel-linux-gcc
MIPSCXX ?= $(CMDPREF)mipsel-linux-g++
export COMPILE ?= $(MIPSCC)
export MIPSLD    ?= $(CMDPREF)mipsel-linux-ld
export MIPSAS     ?= $(CMDPREF)mipsel-linux-as
OBJDUMP        ?= $(CMDPREF)mipsel-linux-objdump

ifeq "$(strip $(TYPE))" "STANDALONE"
MEMTXT   = $(SDKROOT)/etc/config/mem.txt
SCRIPT   = $(SDKROOT)/etc/config/std.ld.script
MACROOPT = -DMIERUPC_STANDALONE
MAKETYPE = std
endif
ifeq "$(strip $(TYPE))" "APPLICATION"
MEMTXT   = $(SDKROOT)/etc/config/mem.txt
SCRIPT   = $(SDKROOT)/etc/config/app.ld.script
MACROOPT = -DMIERUPC_APPLICATION
MAKETYPE = app
endif

export CFLAGS  ?= -Wall -O2 
CFLAGS += -ffreestanding -msoft-float -I$(LIBDIR) $(MACROOPT)
export SFLAGS  = 
export LFLAGS  = -T $(SCRIPT) -L$(LIBDIR) --allow-multiple-definition
MEMSIZE = 512
SIMDEFARGS = -t$(strip $(LCDTYPE)) -M $(MEMTXT) $(TARGET)
LCDPIPE = $(SDKROOT)/sim/lcd/lcd_pipe


.SUFFIXES: .cpp .c .S

$(TARGET): $(OBJS) 
	@echo Linking $@
	@$(MIPSLD) $(LFLAGS) $(OBJS) -o $@
	@$(MAKE) -s -C $(LIBDIR) clean

.cpp.o:
	@echo Compiling $(@D)/$(<F)
	@$(COMPILE) $(CFLAGS) -c $(@D)/$(<F) -o $(@D)/$(@F)

.c.o:
	@echo Compiling $(@D)/$(<F)
	@$(COMPILE) $(CFLAGS) -c $(@D)/$(<F) -o $(@D)/$(@F)

.S.o:
	@echo Compiling $(@D)/$(<F)
	@$(COMPILE) $(CFLAGS) $(SFLAGS) -c $(@D)/$(<F) -o $(@D)/$(@F)

app_all:
	@cat $(SDKROOT)/VERSION
	@$(MAKE) $(TARGET)
	@cp $(TARGET) $(MMCROOT)/
std_all:
	@cat $(SDKROOT)/VERSION
	@$(MAKE) $(TARGET)

app_run:
	@$(MAKE) -C $(OSDIR) run APP=$(TARGET)
std_run:
	$(SIMMIPS) $(SIMARGS) -l $(LCDPIPE) $(SIMDEFARGS)

os:
	@$(MAKE) -C $(OSDIR) run

%_image:
	$(MEMGEN) -b $(TARGET) $(MEMSIZE) > $(TARGET).bin

%_clean:
	$(MAKE) -C $(LIBDIR) clean
	rm -f *.o *~ log.txt $(TARGET) $(TARGET).bin 


app_debug:
	@$(MAKE) -C $(OSDIR) debug APP=$(TARGET)
std_debug:
	$(SIMMIPS) $(SIMARGS) -d3 -l $(LCDPIPE) $(TARGET)

%_dump:
	$(OBJDUMP) -S $(TARGET)

%_read:
	readelf -a $(TARGET)

lcd:
	$(LCD) -s $(LCDPIPE) > /dev/null&

lcd_debug:
	$(LCD) -s $(LCDPIPE) -d &


