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

#include <kernel.h>
#include <mierulib.h>
#include <syscall.h>
#include <fat.h>
#include <task.h>
#include <elf.h>
#include <cache.h>

#define MAX_STACK_SIZE 4048
#define syscall_table 0x300


/******************************************************************************/
File *preopen(const char *name){
    File *f1, *f2;
    char path[128], *p, *prev;
    bool loop;

    if(name == NULL)
        return NULL;
    
    strncpy(path, name, 127);
    path[127] = 0;

    if(path[0] == '/'){
        f1 = (File *)Kernel::getInstance()->root;
        p = path+1;
    }else{
        f1 = (FatFile *)Kernel::getInstance()->current;
        p = path;
    }

    f1 = f1->lookup(".");

    if(*p == 0)
        return f1;

    
    loop = true;
    while(loop){
        prev = p;

        // search next '/'
        p = mpc_strchr(prev, '/');
        if(p == NULL)
            loop = false;
        else
            *p++ = 0;

        // lookup the file
        if(*prev == 0)     // special case path is end of '/' such as "./hoge/" 
            f2 = f1->lookup(".");
        else
            f2 = f1->lookup((const char *)prev);

        // delete old File class
        delete f1;
        f1 = f2;

        // invalid filename
        if(f1 == NULL)
            return NULL;
    }

    return f1;
}

/******************************************************************************/
void init_syscall(void){
    //void **table = (void **)&_syscall_table;
    void **table = (void **)syscall_table;
    for(int i = 0; i < 64; i++)
        table[i] = (void *)sys_null;

    table[SYSCALL_INIT]   = (void *)sys_init;
    table[SYSCALL_FIN]    = (void *)sys_fin;
    table[SYSCALL_EXIT]   = (void *)sys_exit;
    table[SYSCALL_READ]   = (void *)sys_read;
    table[SYSCALL_WRITE]  = (void *)sys_write;
    table[SYSCALL_OPEN]   = (void *)sys_open;
    table[SYSCALL_CLOSE]  = (void *)sys_close;
    table[SYSCALL_STAT]   = (void *)sys_stat;
    table[SYSCALL_CREAT]  = (void *)sys_creat;
    table[SYSCALL_RENAME] = (void *)sys_rename;
    table[SYSCALL_REMOVE] = (void *)sys_remove;
    table[SYSCALL_EXECVE] = (void *)sys_execve;
    table[SYSCALL_CHDIR]  = (void *)sys_chdir;
    table[SYSCALL_GETCWD] = (void *)sys_getcwd;
    table[SYSCALL_LSEEK]  = (void *)sys_lseek;
    table[SYSCALL_BRK]    = (void *)sys_brk;
    table[SYSCALL_MKDIR]  = (void *)sys_mkdir;
}

/******************************************************************************/
int sys_null(){
    lcd_dprintf("OS message: sys_null was called\n");
    return 0;
}

/******************************************************************************/
int sys_init(struct task_report *repo){
    repo->disp = lcd_getdisplay();
    return 0;
}

/******************************************************************************/
int sys_fin(const struct task_report *repo){
    Kernel::getInstance()->taskmanager.current->report = *repo;
    lcd_setdisplay(repo->disp);
    return 0;
}

/******************************************************************************/
int sys_exit(int status){
    __UNUSED_VARIABLE(status);
    Task *prev_task = Kernel::getInstance()->taskmanager.getCurrentTask();
    Task *current = Kernel::getInstance()->taskmanager.getKernelTask();

    for(int i = 0; i < MAX_FILE; i++){
        if(Kernel::getInstance()->files[i].user == (uint)prev_task->pid && 
           Kernel::getInstance()->files[i].use  == 1                    ){
            lcd_dprintf("OS message: User Process has't closed file. (%d)\n", i);
            delete &Kernel::getInstance()->files[i];
        }
    }

    Kernel::getInstance()->taskmanager.freeTask(prev_task->pid);
    Kernel::getInstance()->taskmanager.switchContext(prev_task, current);
    return 0;
}

/******************************************************************************/
int sys_execve(const char *filename, char *const argv[], char *const envp[]){
    __UNUSED_VARIABLE(envp);
    int ret;
    unsigned int sp;
    unsigned int *tmp_sp;
    char *tmp_argv[6];
    unsigned int argv_pos;
    int argc, i, len;
    int stack_end;
    ret = elf_load(filename);
    if(ret < 0)
        return ret;
    stack_end = ret;

    Task *prev_task = Kernel::getInstance()->taskmanager.getCurrentTask();
    Task *current = Kernel::getInstance()->taskmanager.getTask();

    if(current == NULL){
        lcd_printf("Task Allocation Error\n");
        for(;;);
    }


    sp = 0x80000; // max memory size

    argc = 0;
    while(argv[argc] != 0)
        argc++; 

    for(i = 0; i < argc; i++){
        len = strlen(argv[i]) + 1;

        sp -= len;
        sp &= 0xfffffff0;         //16 byte alignment
        tmp_argv[i] = (char *)sp;

        memcpy((void *)sp, argv[i], len);

    }

    
    sp -= 4 * (argc + 1); // num of argv and argv[argc]
    argv_pos = sp;
    tmp_sp = (unsigned int *)sp;
    for(i = 0; i < argc; i++){
        *tmp_sp = (unsigned int)tmp_argv[i];
        tmp_sp++;
    }
    *tmp_sp = 0;

    sp -= 4*3; // argc, argv, envp
    tmp_sp = (unsigned int *)sp;
    *tmp_sp++ = argc;
    *tmp_sp++ = argv_pos; //argv
    *tmp_sp   = 0;        //envp
    
    current->tss.sp = sp;
    current->tss.ra = 0x20000;
    current->tss.cp0_status = 0;
    current->tss.cp0_epc = 0;
    current->tss.cp0_cause = 0;
    
    current->stack_start = sp;
    current->stack_end   = stack_end;
    current->brk         = stack_end;

    invalidate_icache();
    invalidate_dcache();
    Kernel::getInstance()->taskmanager.switchContext(prev_task, current);
    
    return 0;
}

/******************************************************************************/
int sys_open(const char *pathname, int flags, int mode){
    int ret;
    File *f;

    f = preopen(pathname);
    if(f == NULL)
        return -FAT_ERROR_INVALID_FILE_NAME;

    ret  = f->open(flags, mode);
    if(ret < 0){
        delete f;
        return ret;
    }

    f->user = Kernel::getInstance()->taskmanager.getCurrentTask()->pid;
    return f->num;
}

/******************************************************************************/
int sys_creat(const char *pathname, int mode){
    return sys_open(pathname, O_WRONLY | O_CREAT | O_TRUNK, mode);
}

/******************************************************************************/
int sys_rename(const char *oldpath, const char *newpath){
    int ret;
    File *f1, *f2;
    f1 = preopen(oldpath);
    if(f1 == NULL)
        return -FAT_ERROR_INVALID_FILE_NAME;
    
    f2 = preopen(newpath);
    if(f2 == NULL){
        delete f1;
        return -FAT_ERROR_INVALID_FILE_NAME;
    }

    ret = f1->rename(f2);
    delete f1;
    delete f2;

    return ret;
}

/******************************************************************************/
int sys_remove(const char *pathname){
    int ret;
    File *f;
    f = preopen(pathname);
    if(f == NULL)
        return -FAT_ERROR_INVALID_FILE_NAME;
    
    ret = f->remove();
    delete f;

    return ret;
}

/******************************************************************************/
int sys_close(int fd){
    int ret;
    File *f;
    if(fd < 0)
        return -1;

    f = &Kernel::getInstance()->files[fd];
    ret = f->close();
    delete f;
    return ret;
}

/******************************************************************************/
int sys_stat(const char *pathname, Stat *st){
    int ret;
    File *f;
    f = preopen(pathname);
    if(f == NULL)
        return -FAT_ERROR_INVALID_FILE_NAME;
    
    ret = f->stat(st);
    delete f;

    return ret;
}

/******************************************************************************/
int sys_brk(unsigned int brk){
    Task *current = Kernel::getInstance()->taskmanager.getCurrentTask();
    if( brk > (uint)current->stack_end && 
        brk < (uint)current->stack_start - MAX_STACK_SIZE){

        current->brk = brk;
    }
    return current->brk;
}

/******************************************************************************/
int sys_read(int fd, void *buf, unsigned int count){
    int ret=0;
    File *f;
    if(fd < 0) return -1;

    f = &Kernel::getInstance()->files[fd];
    ret = f->read(buf, count);
    return ret;
}

/******************************************************************************/
int sys_write(int fd, const void *buf, unsigned int count){
    int ret=0;
    File *f;
    if(fd < 0)
        return -1;

    f = &Kernel::getInstance()->files[fd];
    ret = f->write(buf, count);
    return ret;
}

/******************************************************************************/
int sys_mkdir(const char *pathname, int mode){
    int ret;
    File *f;
    f = preopen(pathname);
    if(f == NULL)
        return -FAT_ERROR_INVALID_FILE_NAME;

    ret  = f->create(mode, 1);
    delete f;
    if(ret < 0)
        return ret;
    return ret;
}

/******************************************************************************/
int sys_chdir(const char *path){
    File *f;
    f = preopen(path);
    if(f == NULL)
        return -FAT_ERROR_INVALID_FILE_NAME;

    if(!f->isdir){
        delete f;
        return -FAT_ERROR_NOT_DIRECTORY;
    }

    if(!f->exist){
        delete f;
        return -FAT_ERROR_NO_SUCH_FILE;
    }

    delete Kernel::getInstance()->current;
    Kernel::getInstance()->current = (FatFile *)f;
    return 0;
}

/******************************************************************************/
int sys_getcwd(char *buf, unsigned int size){
    char *p, *end;
    char tmp[128];
    File *f, *next;
    Stat st;
    int i, len;
    if(size > 127)
        size = 127;

    p = tmp;
    end = p + size;

    f = preopen(".");
    for(;;){
        f->stat(&st);
        if(st.name[0] == 0 || st.name[0] == '/')
            break;

        len = strlen(st.name);
        for(i = len - 1; i >= 0; i--)
            if(p != end)
                *p++ = st.name[i];
        
        if(p != end)
            *p++ = '/';

        next = f->lookup("..");
        delete f;
        f = next;
    }
    *p = 0;

    len = strlen(tmp);
    for(i = 0; i < len; i++)
        buf[i] = tmp[len - i - 1];

    if(len == 0)
        buf[i++] = '/';
    buf[i] = 0;
    
    delete f;
    return len;
}


/******************************************************************************/
int sys_lseek(int fd, int offset, int whence){
    int ret=0;
    File *f;
    if(fd < 0)
        return -1;

    f = &Kernel::getInstance()->files[fd];
    ret = f->seek(offset, whence);
    return ret;
}

/******************************************************************************/
