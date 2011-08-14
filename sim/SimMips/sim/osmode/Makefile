##########################################################################
## SimMips: Simple Computer Simulator of MIPS    Arch Lab. TOKYO TECH   ##
##########################################################################
CORE    = ../core
TARGET  = SimMipsOS
HEADER  = define.h ${CORE}/define.h ${CORE}/simloader.h
SOURCE  = main.cc osmode.cc mips.cc cp0.cc device.cc ../common/ttyc.cc \
          ${CORE}/board.cc ${CORE}/memory.cc \
          ${CORE}/simloader.cc ${CORE}/mips.cc ${CORE}/mipsinst.cc
OBJECT  = $(SOURCE:.cc=.o)
##########################################################################
include ../common/Makefile
##########################################################################
run:
	./$(TARGET) -M ../sample/mem_qemu.txt ../sample/vmlinux-2.6.18-3-qemu
##########################################################################