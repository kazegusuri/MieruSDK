
clean:
	rm -f *.o *~

doc:
	doxygen

.PHONY: check-syntax
check-syntax:
	mipsel-linux-gcc -Wall -I. -fsyntax-only  $(CHK_SOURCES) -DMIERUPC_APPLICATION
