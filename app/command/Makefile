
SDKROOT := ../..
MMCROOT = $(SDKROOT)/etc/fatop/mmcroot

clean:
	rm -f *~
	@for dir in */; do\
		echo Clean $$dir;\
		make clean -C $$dir --no-print-directory;\
	done

install:
	@if [ ! -e $(MMCROOT)/bin ]; then mkdir $(MMCROOT)/bin; fi;
	@for dir in */; do\
		echo Install $$dir;\
		make install -C $$dir --no-print-directory;\
	done

