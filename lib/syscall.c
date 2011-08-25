/******************************************************************************/
/* MieruPC Project: System Call Library v0.3                                  */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-06-14 */
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

/*! @file
 *@brief システムコール用ライブラリ
 *
 *システムコールを使うために必要なライブラリ．
 *このライブラリはOS用のアプリケーションの時のみ使用可能である．
 */

#include <mierulib.h>

/**********************************************************************/
#if defined(MIERU_OS)
int __attribute__((noinline)) system::_syscall(
#else
int __attribute__((noinline)) _syscall(
#endif
    uint syscall_nr,
    uint arg1,
    uint arg2,
    uint arg3,
    uint arg4){
	register uint v0 asm("$2") = (uint) syscall_nr;
	register uint a0 asm("$4") = (uint) arg1;
	register uint a1 asm("$5") = (uint) arg2;
	register uint a2 asm("$6") = (uint) arg3;
	register uint a3 asm("$7") = (uint) arg4;

    __asm__ volatile (
        "syscall   \n"
        "move %0, $v0 \n"
        : "=&r" (v0)
        : "r" (v0) , "r" (a0), "r" (a1), "r" (a2), "r" (a3)
        );
    /* __asm__ volatile ( */
    /*     "li $k0, 0x100 \n" */
    /*     "addiu $sp, $sp, -20 \n" // $a0-$a3 + $sp */
    /*     "sw $ra, 16($sp) \n" */
    /*     "jalr $k0 \n" */
    /*     "nop \n" */
    /*     "lw $ra, 16($sp) \n" */
    /*     "addiu $sp, $sp, 20 \n" */
    /*     "move %0, $v0 \n" */
    /*     : "=&r" (v0) */
    /*     : "r" (v0) , "r" (a0), "r" (a1), "r" (a2), "r" (a3) */
    /*     ); */

	return v0;
}


#ifdef MIERUPC_APPLICATION
/**********************************************************************/
int sys_init(struct task_report *repo){
    return _syscall(SYSCALL_INIT, (uint)repo, 0,0,0);
}

/**********************************************************************/
int sys_fin(const struct task_report *repo){
    return _syscall(SYSCALL_FIN, (uint)repo, 0,0,0);
}

/**********************************************************************/
int sys_exit(int status){
    struct task_report report;
    report.disp = lcd_getdisplay();
    sys_fin(&report);

    return _syscall(SYSCALL_EXIT,status,0,0,0);
}

/**********************************************************************/
int sys_creat(const char *path, int mode){
    return _syscall(SYSCALL_CREAT, (uint)path, mode, 0, 0);
}

/**********************************************************************/
int sys_open(const char *path, int flags){
    return _syscall(SYSCALL_OPEN, (uint)path, flags, 0, 0);
}

/**********************************************************************/
int sys_read(int fd, char *str, int count){
    return _syscall(SYSCALL_READ, fd, (uint)str, count, 0);
}

/**********************************************************************/
int sys_write(int fd, const char *str, int count){
    return _syscall(SYSCALL_WRITE, fd, (uint)str, count, 0);
}

/**********************************************************************/
int sys_close(int fd){
    return _syscall(SYSCALL_CLOSE, fd, 0, 0, 0);
}

/**********************************************************************/
int sys_stat(const char *pathname, Stat *st){
    return _syscall(SYSCALL_STAT, (uint)pathname, (uint)st, 0, 0);
}

/**********************************************************************/
int sys_brk(unsigned int brk){
    return _syscall(SYSCALL_BRK, brk, 0, 0, 0);
}

/**********************************************************************/
int sys_chdir(const char *str){
    return _syscall(SYSCALL_CHDIR, (uint)str, 0, 0, 0);
}

/**********************************************************************/
int sys_rename(const char *oldpath, const char *newpath){
    return _syscall(SYSCALL_RENAME, (uint)oldpath, (uint)newpath, 0, 0);
}

/**********************************************************************/
int sys_remove(const char *pathname){
    return _syscall(SYSCALL_REMOVE, (uint)pathname, 0, 0, 0);
}

/**********************************************************************/
int sys_lseek(int fd, int offset, int whence){
    return _syscall(SYSCALL_LSEEK, fd, offset, whence, 0);
}

/**********************************************************************/
int sys_getcwd(char *buf, unsigned int size){
    return _syscall(SYSCALL_GETCWD, (uint )buf, size, 0, 0);
}

/**********************************************************************/
int sys_mkdir(const char *path, int mode){
    return _syscall(SYSCALL_MKDIR, (uint)path, mode, 0, 0);
}

/**********************************************************************/
#endif
