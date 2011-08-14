##########################################################################
## SimMips: Simple Computer Simulator of MIPS    Arch Lab. TOKYO TECH   ##
##########################################################################
CORE  = ./sim/core
MC    = ./sim/multicycle
OS    = ./sim/osmode
FLOAT = ./sim/float
MIERU = ./sim/mieru
EXECUTABLES = bin/SimMips bin/SimMipsM bin/SimMipsOS bin/SimMipsF \
    bin/SimMieru

##########################################################################
all:
	${MAKE} -C ${CORE}
	${MAKE} -C ${MC}
	${MAKE} -C ${OS}
	${MAKE} -C ${FLOAT}
	${MAKE} -C ${MIERU}
	${MAKE} copy

copy:
	${MAKE} -C ${CORE} copy
	${MAKE} -C ${MC} copy
	${MAKE} -C ${OS} copy
	${MAKE} -C ${FLOAT} copy
	${MAKE} -C ${MIERU} copy

clean:
	${MAKE} -C ${CORE} clean
	${MAKE} -C ${MC} clean
	${MAKE} -C ${OS} clean
	${MAKE} -C ${FLOAT} clean
	${MAKE} -C ${MIERU} clean
	rm -f ${EXECUTABLES}

##########################################################################