########################################################################
#  MIPS Instruction Test Benchmark for MieruPC   Arch Lab. TOKYO TECH  #
########################################################################

CC     = mipsel-linux-uclibc-gcc
OFLAG  = -static -mips32r2
SIM    = SimMipsF

BENCH  = 100.blank 101.abs_neg 102.add_s 103.add_d 104.cvt 105.div_s \
         106.div_d 107.loadstore 108.madd_nmadd 109.mfc1 110.mov_fpu \
         111.msub_nmsub 112.mul_s 113.mul_d 114.recip 115.to_int \
         116.rsqrt 117.sqrt_s 118.sqrt_d 119.sub_s 120.sub_d \
         121.bc1f_eq_s 122.bc1f_eq_d 123.bc1fl_eq_s 124.bc1fl_eq_d \
         125.bc1t_eq_s 126.bc1t_eq_d 127.bc1tl_eq_s 128.bc1tl_eq_d \
         129.bc1f_le_s 130.bc1f_le_d 131.bc1fl_le_s 132.bc1fl_le_d \
         133.bc1t_le_s 134.bc1t_le_d 135.bc1tl_le_s 136.bc1tl_le_d \
         137.bc1f_lt_s 138.bc1f_lt_d 139.bc1fl_lt_s 140.bc1fl_lt_d \
         141.bc1t_lt_s 142.bc1t_lt_d 143.bc1tl_lt_s 144.bc1tl_lt_d \
         145.f_nge_ngl_s 146.f_nge_ngl_d 147.ngt_cle_olt_s \
         148.ngt_cle_olt_d 149.seq_sf_ueq_s 150.seq_sf_ueq_d \
         151.ule_ult_un_s 152.ule_ult_un_d 153.ole_s 154.ole_d

TARGET = $(BENCH:=.elf)
ANSWER = $(BENCH:=.ans)
VERIFY = $(BENCH:=.ver)
MAIN   = main.c bench.s

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