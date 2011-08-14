##########################################################################
## SimMips: Simple Computer Simulator of MIPS    Arch Lab. TOKYO TECH   ##
##########################################################################
CORE    = ../core
TARGET  = SimMieru
HEADER  = define.h ${CORE}/define.h ${CORE}/simloader.h
SOURCE  = main.cc mieru.cc cp0.cc device.cc display.cc ../common/ttyc.cc \
          ${CORE}/board.cc ${CORE}/memory.cc $(CORE)/syscall.cc \
          ${CORE}/simloader.cc ${CORE}/mips.cc ${CORE}/mipsinst.cc
OBJECT  = $(SOURCE:.cc=.o)
##########################################################################
include ../common/Makefile
##########################################################################
run:
	./$(TARGET) -M ../sample/mem_mpc2010.txt 
##########################################################################