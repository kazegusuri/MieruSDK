##########################################################################
## SimMips: Simple Computer Simulator of MIPS    Arch Lab. TOKYO TECH   ##
##########################################################################
TARGET  = SimMips
HEADER  = define.h simloader.h
SOURCE  = main.cc board.cc memory.cc simloader.cc mips.cc mipsinst.cc \
          syscall.cc
OBJECT  = $(SOURCE:.cc=.o)
##########################################################################
include ../common/Makefile
##########################################################################
run:
	./$(TARGET) ../sample/qsort
##########################################################################