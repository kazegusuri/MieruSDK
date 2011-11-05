/*
 * Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.
 * Copyright (c) 2011 Masahiro Sano.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*! @file elf.h
 *@brief elf header
 *@author Masahiro Sano
 *@since 2010/06/14
 *@date 2010/06/14
 *@version 0.1
 */

#include <types.h>

#define ELF_CLASS_32BIT  1
#define ELF_CLASS_64BIT  2
#define ELF_ENDIAN_LITTLE 1
#define ELF_ENDIAN_BIG    2
#define ELF_ELFVERSION_CURRENT 1
#define ELF_TYPE_RELOC    1
#define ELF_TYPE_EXEC     2
#define ELF_TYPE_DYNAMIC  3
#define ELF_TYPE_CORE     4
#define ELF_VERSION_CURRENT 1
#define ELF_MACHINE_M32     1
#define ELF_MACHINE_SPARC   2
#define ELF_MACHINE_386     3
#define ELF_MACHINE_68K     4
#define ELF_MACHINE_88K     5
#define ELF_MACHINE_486     6
#define ELF_MACHINE_860     7
#define ELF_MACHINE_MIPS    8

#define ELF_SECTION_TYPE_NULL      0
#define ELF_SECTION_TYPE_PROGBITS  1
#define ELF_SECTION_TYPE_SYMTAB    2
#define ELF_SECTION_TYPE_STRTAB    3
#define ELF_SECTION_TYPE_RELA      4
#define ELF_SECTION_TYPE_HASH      5
#define ELF_SECTION_TYPE_DYNAMIC   6
#define ELF_SECTION_TYPE_NOTE      7
#define ELF_SECTION_TYPE_NOBITS    8
#define ELF_SECTION_TYPE_REL       9
#define ELF_SECTION_TYPE_SHLIB     10
#define ELF_SECTION_TYPE_DYNSYM    11
#define ELF_SECTION_FLAG_WRITE     0x1
#define ELF_SECTION_FLAG_ALLOC     0x2
#define ELF_SECTION_FLAG_EXEC      0x4

typedef struct {
    uchar  sign[4];
    uchar  clazz;
    uchar  endian;
    uchar  elfversion;
    uchar  dummy[9];
    ushort type;
    ushort machine;
    uint   version;
    uint   entry;
    uint   phoffset;
    uint   shoffset;
    uint   flags;
    ushort size;
    ushort phentrysize;
    ushort phentrynum;
    ushort shentrysize;
    ushort shentrynum;
    ushort shstrtable;
} elf32_elf_hdr;

typedef struct {
    uint type;
    uint offset;
    uint vaddr;
    uint paddr;
    uint filesize;
    uint memsize;
    uint flags;
    uint align;
} elf32_program_hdr;

typedef struct {
    uint name;
    uint type;
    uint flags;
    uint addr;
    uint offset;
    uint size;
    uint link;
    uint info;
    uint align;
    uint entrysize;
} elf32_section_hdr;


int elf_load(const char *filename);
