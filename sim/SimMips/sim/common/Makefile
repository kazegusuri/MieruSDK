##########################################################################
## SimMips: Simple Computer Simulator of MIPS    Arch Lab. TOKYO TECH   ##
##########################################################################
CC      = g++
OFLAG   = -O3 -Wall
LFLAG   =
DEBUG   = -g

##########################################################################
all:
	$(MAKE) $(TARGET)
##########################################################################
$(TARGET): $(SOURCE) $(HEADER) Makefile
	$(CC) $(OFLAG) -o $@ $(SOURCE) $(LFLAG)
##########################################################################
debug: 
	$(CC) $(DEBUG) -o $(TARGET) $(SOURCE) $(LFLAG)
##########################################################################
copy:
	cp ${TARGET} ../../bin/
##########################################################################
.SUFFIXES :
.SUFFIXES : .o .cc

.cc.o: 
	$(CC) $(OFLAG) -c $<

$(OBJECT) : $(HEADER) Makefile
##########################################################################
wc:
	wc -l $(HEADER) $(SOURCE)

indent:
	indent -kr -ts4 -nut main.cc

text:
	cats Makefile > code.txt
	echo -e "\nFile Organization by [wc *.h *.cc]" >> code.txt
	echo -e "  lines words bytes" >> code.txt
	wc $(HEADER) $(SOURCE) >> code.txt      
	echo -e "\f" >> code.txt
	cats -f $(HEADER) $(SOURCE) >> code.txt
	a2ps --medium=a4 -f 6.5 code.txt -o  code.ps
	ps2pdf13 -sPAPERSIZE=a4 code.ps
	rm -f code.txt code.ps

cflow:
	cflow *.cc

clean:
	rm -f *.o *.*~ *.exe $(TARGET) code.cc code.ps code.pdf
##########################################################################