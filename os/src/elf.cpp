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

/*! @file elf.cpp
 *@brief elf file management
 *@author Masahiro Sano
 *@since 2010/06/14
 *@date 2011/11/23
 *@version 0.2
 */

#include <mierulib.h>
#include <elf.h>
#include <fat.h>
#include <syscall.h>

#define APPLICATION_ENTRY 0x20000

int elf_check(const elf32_elf_hdr *ehdr){
    // lcd_dprintf("%x %x %x %x %x %x %x %x %x\n",
    //             ehdr->phoffset,
    //             ehdr->shoffset,
    //             ehdr->flags,
    //             ehdr->size,
    //             ehdr->phentrysize,
    //             ehdr->phentrynum,
    //             ehdr->shentrysize,
    //             ehdr->shentrynum,
    //             ehdr->shstrtable
    //     );

    // check signature
    if(ehdr->sign[0] != 0x7f || ehdr->sign[1] != 'E' ||
       ehdr->sign[2] != 'L'  || ehdr->sign[3] != 'F'  ){
        return -3;
    }

    // check addressing mode
    switch(ehdr->clazz){
    case ELF_CLASS_32BIT:
        break;
    case ELF_CLASS_64BIT:
    default:
        return -4;
    }

    // check endian
    switch(ehdr->endian){
    case ELF_ENDIAN_LITTLE:
        break;
    case ELF_ENDIAN_BIG:
    default:
        return -5;
    }

    // check file type
    switch(ehdr->type){
    case ELF_TYPE_EXEC:
        break;
    default:
        return -6;
    }

    // check architecture
    switch(ehdr->machine){
    case ELF_MACHINE_MIPS:
        break;
    default:
        return -7;
    }

    //check entry point
    // if(ehdr->entry != APPLICATION_ENTRY){
    //     return -8;
    // }

    return 0;
}

/******************************************************************************/
int elf_load(const char *filename, uint offset){
    int fd;
    elf32_elf_hdr ehdr;
    elf32_section_hdr shdr;
    int i;
    int ret;
    int stack_end;
    
    fd = sys_open(filename, O_RDONLY);
    if(fd < 0)
        return -1;

    ret = sys_read(fd, &ehdr, sizeof(elf32_elf_hdr));
    if(ret < 0){
        sys_close(fd);
        return -2;
    }

    ret = elf_check(&ehdr);
    if(ret < 0){
        sys_close(fd);
        return ret;
    }

    for(i = 0; i < ehdr.shentrynum; i++){
        ret= sys_lseek(fd, ehdr.shoffset + i * ehdr.shentrysize, SEEK_SET);
        if(ret < 0){
            sys_close(fd);
            return -9;
        }
        
        ret=sys_read(fd, &shdr, ehdr.shentrysize);
        if(ret < 0){
            sys_close(fd);
            return -10;
        }

        if(shdr.flags & ELF_SECTION_FLAG_ALLOC){
            if(shdr.type & ELF_SECTION_TYPE_NOBITS){
                memset((void *)(shdr.addr + offset), 0, shdr.size);
            }else{
                ret = sys_lseek(fd, shdr.offset, SEEK_SET);
                if(ret < 0){
                    sys_close(fd);
                    return -11;
                }

                ret = sys_read(fd, (void *)(shdr.addr + offset), shdr.size);
                if(ret < 0){
                    sys_close(fd);
                    return -12;
                }
               
            }
        }
    }
    stack_end = shdr.offset + shdr.size + 0x10000 + offset;
    stack_end = (stack_end + 0x1f) & 0xffffffe0; // 32 Byte align

    sys_close(fd);

    return stack_end;
}

/******************************************************************************/
