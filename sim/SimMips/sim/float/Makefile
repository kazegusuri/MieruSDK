##########################################################################
## SimMips: Simple Computer Simulator of MIPS    Arch Lab. TOKYO TECH   ##
##########################################################################
CORE    = ../core

TARGET  = SimMipsF
HEADER  = define.h ${CORE}/define.h ${CORE}/simloader.h
SOURCE  = main.cc float.cc fpu.cc fpuinst.cc \
          ${CORE}/board.cc ${CORE}/memory.cc ${CORE}/simloader.cc \
          ${CORE}/mips.cc ${CORE}/mipsinst.cc $(CORE)/syscall.cc
OBJECT  = $(SOURCE:.cc=.o)
##########################################################################
include ../common/Makefile
##########################################################################
run:
	./$(TARGET) ../sample/fft_double
##########################################################################