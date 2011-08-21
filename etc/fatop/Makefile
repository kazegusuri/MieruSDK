TARGET  = fatop
MASTER  = master.dat
MMCROOT = mmcroot/
OUTDIR  = outdir
MMC     = out.dat
OUTMMC  = out.dat_
FILE    = 

all:
	mkdir -p mmcroot
	gcc  -o $(TARGET) fatop.c

run: in

in:
	./$(TARGET) in $(MASTER) $(MMCROOT) $(MMC) $(FILE)

out:
	./$(TARGET) out $(OUTMMC) $(OUTDIR)

cat:
	./$(TARGET) cat $(MMC) $(FILE)

info:
	./$(TARGET) info $(MMC)

clean:
	rm -f *.o *~ $(TARGET) $(MMC)
	rm -rf $(OUTDIR)/*
	rm -rf $(MMCROOT)/*
