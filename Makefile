
MEMGEN  =  ./etc/memgen
SIMMIPS =  ./sim/SimMips
LCD     =  ./sim/lcd
OS      =  ./os
FATOP   =  ./etc/fatop
CMD     =  ./app/command

all:
	make -C $(MEMGEN)
	make -C $(FATOP)
	make -C $(SIMMIPS)
	make -C $(LCD)
	make -C $(OS)
	make -C $(CMD) install

clean:
	make -C $(MEMGEN) clean
	make -C $(FATOP) clean
	make -C $(SIMMIPS) clean
	make -C $(LCD) clean
	make -C $(OS) clean
	make -C $(CMD) clean
