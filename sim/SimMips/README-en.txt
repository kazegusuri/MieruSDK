/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS    Arch Lab. TOKYO TECH *
 **********************************************************************/
COPYRIGHT

SimMips is free software.
See the file COPYING for copying permission.

/**********************************************************************/
/* 0. Index                                                           */
/**********************************************************************/
 1. About SimMips
 2. Building your SimMips binary  
 3. Let's run SimMips  
 4. Various modes of SimMips
 5. Command Line Options
 6. Machine Setting File
 7. SimMips History
 8. Project
 9. Reference

/**********************************************************************/
/* 1. About SimMips                                                   */
/**********************************************************************/
 There are increasing demands for system simulators as tools of computer
architecture or operating system. But well-known system simulators focus
on dealing with various platforms and high-speed simultation, rather
than simple and comprehensive source codes.

 We have developed a system simulator named SimMips, whose target
computer system includes a MIPS32 ISA processor, as a practical simulator
for embedded system education and research. Though there is a tradeoff
between readability and simulation speed, processor speedup now enable
simulators to satisfy high readability and sufficient speed at the same
time. SimMips is very useful in research and education of computer system
and operating system.

/**********************************************************************/
/* 2. Building your SimMips binary                                    */
/**********************************************************************/
 SimMips is written in clean C++. We confirmed operation on Intel Linux,
Intel Cygwin, etc (elf.h file may be needed). X Window System is needed
in the MieruPC-Mode.
 It doesn't work on big-endian platforms such as Cell/B.E. Linux so far.

 To make SimMips binary files you have to compile the source codes.
Download and expand the SimMips package. (You may have already done this
process because you are reading this file.)

 For example, if the file name of the package is SimMips-0.6.6.tgz, you
can expand it by the following command:

$ tar xvzf SimMips-0.6.6.tgz

 A directory named SimMips-0.6.6 is generated and the source codes and
the other files are extracted into it. Then go to the generated
directory and compile the source codes like below:

$ cd SimMips-0.6.6
$ make

 If the compilation finished successfully, some binary files are copied
to the 'bin' directory.

$ ls bin/
SimMieru  SimMips  SimMipsF  SimMipsM  SimMipsOS

/**********************************************************************/
/* 3. Let's run SimMips                                               */
/**********************************************************************/
 When you execute SimMips without specifing object file, it displays
usage.

| $ ./bin/SimMips
| ## SimMips: Simple Computer Simulator of MIPS Version 0.6.6 2010-07-02
| Usage: SimMips [-options] object_file_name
|  [Basic Options]
|   -e[num][kmg]: stop simulation after num cycles executed
|   -k[num][kmg]: skip first num cycles
|   -d1: debug mode 1: put the architectural state after simulation
|   -d2: debug mode 2: put all the instructions executed
|   -d3: debug mode 3: put the trace of the execution
|   -i: put the instruction mix after simulation
|   -M [filename]: specify machine setting file

 A quick sort program as a sample is located on test/qsort. So let's run
this program on SimMips.

| $ ./bin/SimMips test/qsort
| ## SimMips: Simple Computer Simulator of MIPS Version 0.6.6 2010-07-02
|
| Before:   65536 items quick sorting.
| a[    0] =        9086
| a[  200] =       28147
| a[  400] =        7617
| a[  600] =        9586
| a[  800] =       18764
| a[ 1000] =       28363
| a[ 1200] =       23597
| a[ 1400] =       26600
| a[ 1600] =        6196
| a[ 1800] =       12923
| 
| After:   65536 items quick sorting.
| a[    0] =           0
| a[  200] =          98
| a[  400] =         197
| a[  600] =         300
| a[  800] =         402
| a[ 1000] =         503
| a[ 1200] =         600
| a[ 1400] =         701
| a[ 1600] =         802
| a[ 1800] =         897
| 
| ## cpu stopped
| ## cycle count: 17663749
| ## inst count: 17663749
| ## simulation time:    1.431
| ## mips: 12.343

 You will see these output. Simulation time and million instruction per
second(mips) are varied with the host machine.

/**********************************************************************/
/* 4. Various modes of SimMips                                        */
/**********************************************************************/
 Descriptions between parentheses are ([directory of source code starting
from 'sim'] / [file name of binary which will generate]).

- App-Mode (core/SimMips)
 executes a statically linked MIPS application. This mode includes the
core components of SimMips and the other modes are based on this mode.

- MultiCycle-Mode (multicycle/SimMipsM)
 can execute the same applications as the App-Mode. The difference of
these two modes is execution model. This mode executes one instruction
by some cycles. This mode corresponds to the -m option of older versions
of SimMips.

- OS-Mode (osmode/SimMipsOS)
 can execute an operating system such as Linux by specifying an OS kernel
and disk image. This mode simulates a MIPS processor control(CP0), an
interrupt controller, serial controller, and so on. Downloading QEMU disk
image for mipsel, put them on 'image' directory, and set mem_qemu.txt as
a machine setting file and vmlinux-2.6.18-3-qemu as an object file,
and you will confirm operation of the OS-Mode.
  http://wiki.qemu.org/Download

- Float-Mode (float/SimMipsF)
 executes statically linked MIPS application which has floating point
instructions. An FPU is Added for this mode.

- MieruPC-Mode (mieru/SimMieru)
 executes an application or an OS which work on MieruPC-2008 or 2010.
To use this mode, an LCD simulator included by MieruSDK has to be ready.
This mode is an extension of a part of the App-Mode of older versions of
SimMips.
 MieruSDK is (will be) available on the Web site of MieruPC Project.
The URL is < http://www.arch.cs.titech.ac.jp/mieru/ >.

/**********************************************************************/
/* 5. Command Line Options                                            */
/**********************************************************************/
- -e option
 simulation will be terminated after specified cycle of execution.
k, K(1,000), m, M(million), g, or G(billion) are available as postfix.

| $ ./SimMips -e10k test/qsort
| ## SimMips: Simple Computer Simulator of MIPS Version 0.5.0 2008-11-05
| 
| ## cycle count reaches the limit
| ## cycle count: 10000
| ## inst count: 10000
| ## simulation time:    0.001
| ## mips: 11.455

- -k option
 suppresses the output before completion of specified cycle of
execution.

- -d option
 is debug mode. Three or four modes are available.
 -d1 mode shows the contents of register and main memory after
     simulation.
 -d2 mode shows the sequence of the instructions executed.
 -d3 mode shows the sequence and the value of register which is read or
     written of the instructions executed.
 -d4 mode shows the occurence of the exceptions and the TLB trace.
     (OS-Mode only)

- -i option
 shows the statistics of the instructions executed after simulation.

- -M option
 sets the initial state of registers and main memory, as instructed by
a machine setting file. For more information about machine setting files,
see section 6.

- -l option
 specifies the pipe of LCD simulator.(MieruPC-Mode only)

- -t option
 specifies which kind of LCD is assumed by the target application(/OS).
-t0 means MieruPC-2008 LCD is expected, and -t1 means MieruPC-2010 LCD
is expected.

/**********************************************************************/
/* 6. Machine Setting File                                            */
/**********************************************************************/
 When you specify a text file, written in the format below, as a machine
setting file, you can set the initial values of registers, the memory map,
and the initial values of main memory.

 The first line is the header. It must be `SimMips_Machine_Setting'.
The following lines beginning with `@' are commands. The other lines are
comments.

- @reg $REG=VALUE : initial value of register
 eg) @reg $a0=0x12345678
SimMips interprets either register number(such as $4) or register name
(such as $a0). Program counter has the register number $32 and the name
$pc for convenience.

- @map BASE SIZE NAME : physical memory map
 eg) @map 0x0 0x1000000 MAIN_MEMORY
If two or more @map commands has overlapped address, the first one
precedes.

The available `NAME's are as follows:
`NAME'      mode         description
MAIN_MEMORY All modes    Main Memory
ISA_IO      OS           ISA Bus IO (Interrupt and Serial Controller)
ISA_BUS     OS           ISA Bus (which Operates nothing so far)
MIERU_IO    MieruPC      MieruPC IO
MMC_IO      MieruPC      MultiMediaCard Bus

- @mem ADDR NAME : initial value of main memory
 eg) @mem 0x00010000 hoge.bin
 The contents of the file `NAME' is written to the area starting from
the virtual address `ADDR'.

- @mmc SIZE NAME : MultiMediaCard assumption (MieruPC-Mode only)
 eg) @mmc 1m image.dat
 SimMips assumes a MultiMediaCard whose capacity is `SIZE' bytes.
Postfixs k, m, and g are available. The initial data of the card is
read from the file `NAME'. If the `NAME' is not specified or the file
size is less than `SIZE', the unwritten area is filled by zero.

/**********************************************************************/
/* 7. SimMips History                                                 */
/**********************************************************************/
 See the Japanese Version

/**********************************************************************/
/* 8. Project                                                         */
/**********************************************************************/
 The development of SimMips is a part of MieruPC Project (The research
and education project to build the `Mieru' (visible) computer systems).
For more information about MieruPC Project, see the URL below:
  http://www.arch.cs.titech.ac.jp/mieru/
 The hardware of MieruPC is sold by MieruPC Inc. (International shipping
is not available as of August 2010)
Å@http://www.mierupc.com/

 Also, a part of the development of SimMips is supported by Core Research
for Evolutional Science and Technology (CREST), JST.

/**********************************************************************/
/* 9. Reference                                                       */
/**********************************************************************/
 See the Japanese Version

/**********************************************************************/