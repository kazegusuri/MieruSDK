/**********************************************************************
 * SimMips: Simple Computer Simulator of MIPS                         *
 * Copyright (C) 2008-2010 Arch Lab. Tokyo Institute of Technology    *
 *  Dual licensed under New BSD or GPL v2 licenses.                   *
 *  See COPYING file on the base directory for more information.      *
 **********************************************************************/
typedef struct {
    uint032_t addr;
    uint008_t data;
} memtab_t;

/**********************************************************************/
typedef enum {
    ST_NOTYPE,
    ST_FUNC,
    ST_OBJECT,
} symtype_t;

/**********************************************************************/
typedef struct {
    uint032_t addr;
    symtype_t type;
    char *name;
} symtab_t;

/**********************************************************************/
class SimLoader {
 public:
    memtab_t *memtab;
    symtab_t *symtab;
    
    int fileident;
    int filetype;
    int endian;
    int archtype;
    int dynamic;
    uint032_t entry;
    uint032_t stackptr;
    int memtabnum;
    int symtabnum;
    
    SimLoader();
    ~SimLoader();
    int loadfile(char*);
    int checkfile(const char *);
    int loadelf32(const char *);
};

/**********************************************************************/
