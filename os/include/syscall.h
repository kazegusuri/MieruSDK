/******************************************************************************/
/* MieruOS: Syscall v0.1                                                      */
/* written by Masahiro Sano Arch Lab. Tokyo Tech                   2010-06-14 */
/******************************************************************************/

/*
 * Copyright (c) 2010 Arch Lab. Tokyo Institute of Technology.
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

#pragma once

#include <file.h>
#include <mierulib.h>


#define SYSCALL_NULL      0      
#define SYSCALL_INIT      1
#define SYSCALL_FIN       2
#define SYSCALL_EXIT      3
#define SYSCALL_READ      4
#define SYSCALL_WRITE     5
#define SYSCALL_OPEN      6
#define SYSCALL_CLOSE     7
#define SYSCALL_STAT      8
#define SYSCALL_CREAT     9
#define SYSCALL_RENAME    10
#define SYSCALL_REMOVE    11
#define SYSCALL_EXECVE    12
#define SYSCALL_CHDIR     13
#define SYSCALL_GETCWD    14
#define SYSCALL_LSEEK     15
#define SYSCALL_BRK       16
#define SYSCALL_MKDIR     17

#define SEEK_SET	0	/* seek relative to beginning of file */
#define SEEK_CUR	1	/* seek relative to current file position */
#define SEEK_END	2	/* seek relative to end of file */

struct task_report{
    display_t disp;
};

void init_syscall(void);
void set_syscall_table(unsigned int syscall_nr, void *addr);
uint syscall_handler(uint a0, uint a1, uint a2, uint a3, uint syscall_nr);

int sys_init(struct task_report *repo);
int sys_fin(const struct task_report *repo);
int sys_exit(int status);
int sys_open(const char *pathname, int flags, int mode = 0);
int sys_creat(const char *pathname, int mode);
int sys_close(int fd);
int sys_stat(const char *pathname, Stat *st);
int sys_brk(unsigned int brk);
int sys_read(int fd, void *buf, unsigned int count);
int sys_write(int fd, const void *buf, unsigned int count);
int sys_execve(const char *filename, char *const argv[], char *const envp[]);
int sys_mkdir(const char *pathname, int mode);
int sys_chdir(const char *path);
int sys_rename(const char *oldpath, const char *newpath);
int sys_remove(const char *pathname);
int sys_lseek(int fd, int offset, int whence);
int sys_getcwd(char *buf, unsigned int size);

int sys_null();

/* namespace system{ */
/*     int _syscall(unsigned int syscall_nr, */
/*                  unsigned int arg1, */
/*                  unsigned int arg2, */
/*                  unsigned int arg3, */
/*                  unsigned int arg4); */
/* } */
