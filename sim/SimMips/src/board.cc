/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
#include "define.h"

#define MEM_HEADER "SimMips_Machine_Setting"

enum {
    HEAD_SIZE = 128,
};

/**********************************************************************/
volatile sig_atomic_t receive_int = 0;

void sigint_handler(int signum)
{
    receive_int = 1;
}

/**********************************************************************/
Board::Board()
{
    debug_inst = debug_state = imix_mode = 0;
    funcnum = 0;
    maxcycle = MAX_CYCLE_DEF;
    binfile = memfile = NULL;
    mmap = NULL;
}

/**********************************************************************/
Board::~Board()
{
    DELETE(chip);
    DELETE(mmap);
    for (int i = 0; i < funcnum; i++)
        DELETE_ARRAY(functable[i].name);
    DELETE_ARRAY(functable);
}

/**********************************************************************/
int Board::init()
{
    // set registers' default value
    chip->mips->as->pc = ld->entry;
    chip->mips->as->r[REG_SP] = ((ld->stackptr) ? ld->stackptr :
                                 MEM_SIZE_DEF - 0x100);
    chip->mips->as->r[REG_T9] = chip->mips->as->pc;
    for (int i = 0; i < ld->symtabnum; i++)
        if (strcmp(ld->symtab[i].name, "_gp") == 0) {
            chip->mips->as->r[REG_GP] = ld->symtab[i].addr;
            break;
        }
    return 0;
}
/**********************************************************************/
void Board::setupchip()
{
    chip = new Chip(this);
    chip->init();
}

/**********************************************************************/
int Board::siminit(int argc, char **argv)
{
    struct sigaction sa;

    checkarg(argv);
    if (!binfile)
        return 1;
    if (setmemorymap())
        return 1;

    // load ELF image
    ld = new SimLoader();
    if (ld->loadfile(binfile))
        return 1;
    if ((ld->archtype != EM_MIPS) ||
        (ld->filetype != ET_EXEC)) { // || (ld->dynamic)) {
        printf("## ERROR: inproper binary: %s\n", binfile);
        return 1;
    }

    // board and elements initialize
    setupchip();
    if (this->init())
        return 1;

    for (MemoryMap *temp = mmap; temp != NULL; temp = temp->next)
        temp->dev->init(this);
    if (setinitialdata())
        return 1;

    // write ELF image to memory
    for (int i = 0; i < ld->memtabnum; i++) {
        chip->mc->write(ld->memtab[i].addr, 1, ld->memtab[i].data);
        chip->mc->update();
    }

    // write function table
    for (int i = 0; i < ld->symtabnum; i++)
        if (ld->symtab[i].type == ST_FUNC && 
            strncmp(ld->symtab[i].name, "__GI_", 5))
            funcnum++;

    functable = new symtab_t[funcnum];
    funcnum = 0;
    for (int i = 0; i < ld->symtabnum; i++)
        if (ld->symtab[i].type == ST_FUNC && 
            strncmp(ld->symtab[i].name, "__GI_", 5)) {
            functable[funcnum].addr = ld->symtab[i].addr;
            int len = strlen(ld->symtab[i].name);
            functable[funcnum].name = new char[len + 1];
            strcpy(functable[funcnum].name, ld->symtab[i].name);
            funcnum++;
        }
    DELETE(ld);

    // set up console and signal
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // now, the chip is ready for execution
    chip->maxcycle = maxcycle;
    chip->killcycle = killcycle;
    chip->mips->state = CPU_START;
    chip->ready = 1;
    return 0;
}

/**********************************************************************/
void Board::exec()
{
    gettime();
    while (chip->getstate() >= CHIP_RUN) {
        chip->drive();
        chip->update();
    }
    printresult();
}

/**********************************************************************/
void Board::usage()
{
    char usagemessage[] = "\
 [Basic Options]\n\
  -e[num][kmg]: stop simulation after num cycles executed\n\
  -k[num][kmg]: skip first num cycles\n\
  -d1: debug mode 1: put the architectural state after simulation\n\
  -d2: debug mode 2: put all the instructions executed\n\
  -d3: debug mode 3: put the trace of the execution\n\
  -i: put the instruction mix after simulation\n\
  -M [filename]: specify machine setting file\n\
\n";

    printf("Usage: SimMips [-options] object_file_name\n");
    printf("%s", usagemessage);
}

/**********************************************************************/
ullint Board::atoi_postfix(const char *nptr)
{
    char *endptr;
    ullint result = strtoll(nptr, &endptr, 10);
    if ((*endptr == 'k') || (*endptr == 'K'))
        result *= 1000;
    else if ((*endptr == 'm') || (*endptr == 'M'))
        result *= 1000000;
    else if ((*endptr == 'g') || (*endptr == 'G'))
        result *= 1000000000;
    return result;
}

/**********************************************************************/
void Board::checkarg(char **argv)
{
    int bin_index = -1;
    char *opt;

    for (int i = 1; (opt = argv[i]) != NULL; i++) {
        if (opt[0] != '-') {
            if (bin_index != -1) {
                fprintf(stderr, "## multiple binary files(%s, %s)\n",
                        argv[bin_index], opt);
                usage();
                return;
            }
            bin_index = i;
            continue;
        } else if (checkopt(argv, &i)) {
            return;
        }
    }
    if (bin_index == -1) {
        usage();
        return;
    }

    binfile = argv[bin_index];
}

/**********************************************************************/
int Board::checkopt(char **argv, int *index)
{
    int done = 1;
    char *opt = argv[*index];

    switch (opt[1]) {
    case 'd':
        if (opt[2] == '1') {
            debug_state = 1;
            printf("## debug mode 1: state.\n");
        } else if (opt[2] == '2') {
            debug_inst = 1;
            printf("## debug mode 2: instruction.\n");
        } else if (opt[2] == '3') {
            debug_inst = 2;
            printf("## debug mode 3: detailed instruction.\n");
        } else {
            done = 0;
        }
        break;
    case 'e':
        maxcycle = atoi_postfix(&opt[2]);
        if (!maxcycle)
            maxcycle = MAX_CYCLE_DEF;
        break;
    case 'k':
        killcycle = atoi_postfix(&opt[2]);
        break;
    case 'i':
        imix_mode = 1;
        break;
    case 'M':
        if (memfile) {
            fprintf(stderr, "## multiple -M options\n");
            return 1;
        }
        if ((memfile = argv[++*index]) == NULL) {
            fprintf(stderr, "## -M option: no file specified\n");
            return 1;
        }
        break;
    default:
        done = 0;
    }
    if (!done) {
        fprintf(stderr, "## %s: invalid option\n", opt);
        usage();
        return 1;
    }
    return 0;
}

/**********************************************************************/
int Board::loadrawfile(char *filename, uint032_t addr)
{
    FILE *fp;
    uint008_t temp;

    if ((fp = fopen(filename, "rb")) == NULL) {
        fprintf(stderr, "## can't open file: %s\n", filename);
        return 1;
    }
    while (fread(&temp, sizeof(temp), 1, fp)) {
        chip->mc->write(addr, 1, temp);
        chip->mc->update();
        addr++;
    }
    if (!feof(fp)) {
        fprintf(stderr, "## can't load file: %s\n", filename);
        return 1;
    }
    fclose(fp);
    return 0;
}

/**********************************************************************/
char* Board::getlinehead(char *dest, FILE *fp)
{
    memset(dest, '\0', HEAD_SIZE + 1);
    char *result = fgets(dest, HEAD_SIZE, fp);
    int len = strlen(dest);
    if (dest[len - 1] != '\n') {
        if (dest[len - 1] == '\r')
            dest[len - 1] = '\0';
        char skip[HEAD_SIZE];
        do
            if (fgets(skip, HEAD_SIZE, fp) == NULL)
                break;
        while (skip[strlen(skip) - 1] != '\n');
    } else {
        dest[len - 1] = '\0';
        if (dest[len - 2] == '\r')
            dest[len - 2] = '\0';
    }
    return result;
}

/**********************************************************************/
FILE *Board::openmemfile()
{
    FILE *fp;
    char line[HEAD_SIZE + 1];
    line[HEAD_SIZE] = '\0';

    if ((fp = fopen(memfile, "r")) == NULL) {
        fprintf(stderr, "## can't open file: %s\n", memfile);
        return NULL;
    }
    getlinehead(line, fp);
    if (strncmp(line, MEM_HEADER, strlen(MEM_HEADER)) != 0) {
        fprintf(stderr, "## not a machine setting file: %s\n", memfile);
        fclose(fp);
        return NULL;
    }

    workdir = new char[PATH_MAX + 1];
    if (getcwd(workdir, PATH_MAX) == NULL) {
        fclose(fp);
        return NULL;
    }
    char *temp = new char[strlen(memfile) + 1];
    strcpy(temp, memfile);
    for (int i = strlen(memfile) - 1; i >= 0 && temp[i] != '/'; i--)
        temp[i] = '\0';
    if (chdir(temp) < 0) {
        perror(NULL);
        exit(1);
    }
    DELETE_ARRAY(temp);

    return fp;
}

/**********************************************************************/
void Board::closememfile(FILE *fp)
{
    fclose(fp);
    if (chdir(workdir) < 0) {
        perror(NULL);
        exit(1);
    }
    DELETE_ARRAY(workdir);
}

/**********************************************************************/
void Board::setdefaultmap()
{
    mmap = new MemoryMap();
    mmap->addr = 0;
    mmap->size = MEM_SIZE_DEF;
    mmap->dev = new MainMemory(MEM_SIZE_DEF);
}

/**********************************************************************/
int Board::setmemorymap()
{
    FILE *fp;    
    char line[HEAD_SIZE + 1];
    int ret;
    int linecnt = 1;
    MemoryMap *now = NULL;
    line[HEAD_SIZE] = '\0';

    if (!memfile) {
        setdefaultmap();
        return 0;
    }
    if ((fp = openmemfile()) == NULL)
        return 1;

    while (getlinehead(line, fp) != NULL) {
        linecnt++;
        if ((ret = parsememfile(&now, memfile, line, linecnt)) != 0) {
            closememfile(fp);
            return ret;
        }
    }
    if (!now)
        setdefaultmap();
    closememfile(fp);
    return 0;
}

/**********************************************************************/
int Board::setinitialdata()
{
    FILE *fp;
    char line[HEAD_SIZE + 1];
    int ret;
    int linecnt = 1;
    line[HEAD_SIZE] = '\0';

    if (!memfile)
        return 0;

    if ((fp = openmemfile()) == NULL)
        return 1;

    while (getlinehead(line, fp) != NULL) {
        linecnt++;
        if ((ret = parsememfile(NULL, memfile, line, linecnt)) != 0) {
            closememfile(fp);
            return ret;
        }
    }
    closememfile(fp);
    return 0;
}

/**********************************************************************/
int Board::parsememfile(MemoryMap **now, char *memfile,
                        char *line, int linecnt)
{
    int regnum;
    uint032_t addr, size, regval;
    char *endptr;

    if (line[0] != '@')
        return 0;

        // ==== device setup command ====
    if (strncmp(&line[1], "map", 3) == 0) {
        if (now == NULL)
            return 0;
        addr = strtoul(&line[5], &endptr, 16);
        size = strtoul(&endptr[1], &endptr, 16);
        if (!size) {
            fprintf(stderr, "## %s:%d: invalid syntax. skip.\n",
                    memfile, linecnt);
            return 0;
        }
        if (!*now)
            *now = (mmap = new MemoryMap());
        else
            *now = ((*now)->next = new MemoryMap());
        (*now)->addr = addr;
        (*now)->size = size;
        (*now)->dev = setmmdevice(&endptr[1], size);
        // ==== set default value commands ====
    } else if (strncmp(&line[1], "reg", 3) == 0) {
        if (now != NULL)
            return 0;
        regnum = strtol(&line[6], &endptr, 10);
        if (!regnum)
            for (int i = 1; i < NREG + 1; i++)
                if (strncmp(&line[6], regname[i], 
                            strlen(regname[i])) == 0) {
                    regnum = i;
                    endptr = &line[6 + strlen(regname[i])];
                    break;
                }
        if (endptr[0] != '=') {
            fprintf(stderr, "## %s:%d: invalid syntax. skip.\n",
                    memfile, linecnt);
            return 0;
        } else {
            regval = strtoul(&endptr[1], NULL, 0);
        }   
        if ((!regnum) && (regnum >= NREG + 1)) {
            fprintf(stderr, "## %s:%d: invalid register. skip.\n",
                    memfile, linecnt);
        } else if (regnum != NREG) {
            chip->mips->as->r[regnum] = regval;
        } else {
            chip->mips->as->pc = regval;
        }
    } else if (strncmp(&line[1], "mem", 3) == 0) {
        if (now != NULL)
            return 0;
        addr = strtoul(&line[5], &endptr, 16);
        loadrawfile(&endptr[1], addr);
    } else {
        fprintf(stderr, "## %s:%d: unknown command. skip.\n",
                memfile, linecnt);
    }
    return 0;
}

/**********************************************************************/
MMDevice *Board::setmmdevice(char *devname, uint032_t size)
{
    if (strcmp(devname, "MAIN_MEMORY") != 0)
        fprintf(stderr,
                "## %s: unknown device. mapped MAIN_MEMORY instead.",
                devname);
    return new MainMemory(size);
}

/**********************************************************************/
int Board::siminit(char *arg)
{
    // this is a wrapper of siminit;
    // convert from char * to char ** and call siminit(char **)

    if (arg == NULL)
        return 1;
    if (arg[0] == '\0')
        return 1;

    int num, argc = 0;
    int arglen = strlen(arg);
    char *head = new char[arglen + 1];
    memset(head, 0, arglen + 1);
    char *temp = new char[arglen + 1];
    strcpy(temp, arg);
    char flag = ' ';

    for (int i = 0; i < arglen; i++) {
        if ((temp[i] == '"') || (temp[i] == '\'')) {
            if (flag == temp[i]) {
                temp[i] = '\0';
                flag = ' ';
            } else if ((flag != '"') && (flag != '\'')) {
                head[i + 1] = 1;
                argc++;
                flag = temp[i];
            }
        } else if ((temp[i] == ' ') && (flag == '\0')) {
            temp[i] = '\0';
            flag = ' ';
        } else if ((temp[i] != ' ') && (flag == ' ')) {
            head[i] = 1;
            argc++;
            flag = '\0';
        }
    }
    if ((flag == '"') || (flag == '\'')) {
        fprintf(stderr, "## checkarg(): syntax error\n");
        return 1;
    }
    char **argv = new char*[argc + 2];
    num = 1;
    for (int i = 0; i < arglen; i++) {
        if (head[i]) {
            argv[num] = &temp[i];
            num++;
        }
    }
    argv[0] = argv[argc + 1] = NULL;

    int ret = siminit(argc, argv);

    DELETE_ARRAY(head);
    DELETE_ARRAY(temp);
    DELETE_ARRAY(argv);
    return ret;
}

/**********************************************************************/
ullint Board::gettime()
{
    static ullint start = 0;
    ullint tm;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    tm = tv.tv_sec * 1000000ull + tv.tv_usec;
    if (start == 0)
        start = tm;
    return tm - start;
}

/**********************************************************************/
void Board::versioninfo()
{
    printf("## %s %s\n", L_NAME, L_VER);    
}

/**********************************************************************/
void Board::printresult()
{
    double simtime = (double) gettime() / 1000000.0;

    for (MemoryMap *temp = mmap; temp != NULL; temp = temp->next)
        temp->dev->fini();

    if      (chip->getstate() == CHIP_HALT_CYCLE)
        printf("\n## cycle count reaches the limit\n");
    else if (chip->getstate() == CHIP_HALT_MIPS)
        printf("\n## cpu stopped\n");
    else // (receive_int)
        printf("\n## interrupt\n");
    printf("## cycle count: %llu\n", chip->cycle);
    printf("## inst count: %llu\n", chip->mips->ss->inst_count);
    printf("## simulation time: %8.3f\n", simtime);
    printf("## mips: %4.3f\n\n", 
           chip->mips->ss->inst_count / (simtime * 1000000.0));
    print();
}

/**********************************************************************/
void Board::print()
{
    if (debug_state) {
        chip->mips->as->print();
        chip->mc->print();
    }
    if (imix_mode)
        chip->mips->ss->print();
}

/**********************************************************************/
void Board::simerror()
{
    fprintf(stderr, "ERROR: pc:%08x\n", chip->mips->inst->pc);
    chip->mips->inst->clear();
    chip->mips->state = CPU_ERROR;
}

/**********************************************************************/
Chip::Chip(Board *board)
{
    cycle = 0;
    ready = 0;

    this->board = board;
    this->mmap = board->mmap;
}

/**********************************************************************/
Chip::~Chip()
{
    DELETE(mips);
    DELETE(mc);
}

/**********************************************************************/
void Chip::init()
{
    mc = new MemoryController(mmap);
    mips = new Mips(board, this);
    mips->init();
}

/**********************************************************************/
void Chip::drive()
{
    mips->drive();
    for (MemoryMap *temp = mmap; temp != NULL; temp = temp->next)
        temp->dev->drive();
    mc->drive();
}

/**********************************************************************/
void Chip::update()
{
    mips->update();
    for (MemoryMap *temp = mmap; temp != NULL; temp = temp->next)
        temp->dev->update();
    mc->update();
    cycle++;
}

/**********************************************************************/
int Chip::getstate()
{
    if (cycle >= maxcycle)
        return CHIP_HALT_CYCLE;
    else if (!mips->running())
        return CHIP_HALT_MIPS;
    else if (receive_int)
        return CHIP_HALT_INT;
    else if (cycle < killcycle)
        return CHIP_RUN_SILENT;
    else
        return CHIP_RUN;
}

/**********************************************************************/
int Chip::issilent()
{
    return (getstate() == CHIP_RUN_SILENT);
}

/**********************************************************************/
