########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
########################################################################

CC     = mipsel-linux-uclibc-gcc
OFLAG  = -static -mips32r2
SIM    = SimMips

BENCH  = 000.blank 001.li 002.add 003.and 004.beq 005.beql 006.bge \
         007.bgez 008.bgezal 009.bgezl 010.bgt 011.bgtz 012.bgtzl \
         013.ble 014.blez 015.blezl 016.blt 017.bltz 018.bltzal \
         019.bltzall 020.bltzl 021.bne 022.bnel 023.clo 024.clz 025.div \
         026.divu 027.jump 028.load_byte 029.load_half 030.load_word \
         031.load_word_l 032.load_word_r 033.lui_la 034.madd 035.maddu \
         036.mov 037.msub 038.msubu 039.mult 040.multu 041.nor 042.or \
         043.shift 044.slt 045.sltu 046.sub 047.store_word_lr 048.xor

TARGET = $(BENCH:=.elf)
ANSWER = $(BENCH:=.ans)
VERIFY = $(BENCH:=.ver)
MAIN   = main.c main.s

########################################################################
all:
	$(MAKE) $(TARGET)

clean:
	rm -f $(TARGET) verify.txt

answer: $(TARGET)
	$(MAKE) $(ANSWER)

verify: $(ANSWER)
	$(MAKE) $(TARGET)
	$(MAKE) $(VERIFY)
	echo -e "" > verify.txt
	cat $(VERIFY) >> verify.txt
	rm -f $(VERIFY)
	cat verify.txt

%.elf: %.s $(MAIN)
	$(CC) $(OFLAG) $< $(MAIN) -o $@

%.ans: %.elf
	$(SIM) $< | sed "/^##/d" > $@

%.ver: %.ans
	$(SIM) $*.elf | sed "/^##/d" > $*.log
	-diff -q $< $*.log > $@
	rm -f $*.log

########################################################################