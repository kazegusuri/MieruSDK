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

/*! @file shell.cpp
 *@brief shell implemetation
 *@author Masahiro Sano
 *@since 2010/06/14
 *@date 2011/11/04
 */

#include <kernel.h>
#include <mierulib.h>
#include <shell.h>
#include <syscall.h>
#include <fat.h>

/*****************************************************************/
Shell::Shell(){
    window.set(lcd_getwidth() / 8, lcd_getheight() / 16);
    cursor.set(0, 0);
    reset();
}

/*****************************************************************/
void Shell::run(){
    startup();
    prompt();
    for(;;){
        uchar key = static_cast<uchar>(getchar());
    
        if(mpc_oncontrol())
            control(key);
        else if(mpc_onalt())
            alt(key);
        else
            normal(key);
    }
}

/******************************************************************************/
// clear displayed characters
void Shell::clear(void){
    lcd_clear();
    move(0, 0);
    cmdpos = 0;
    curpos = 0;

    cmdbuf[0] = ' ';
    cmdbuf[1] = 0;
}

/******************************************************************************/
// get back to the settings before stating application
void Shell::reset(void){
    // move last postion
    move( lcd_getcurx(), lcd_getcury());

    lcd_settransparent(0);
    lcd_setscrollmode(1);
    lcd_setcursor(1);

    lcd_setforeground(COL_WHITE);
    lcd_setbackground(COL_BLACK);
    lcd_setshowpage(0);
    lcd_setdrawpage(0);
    
    cmdbuf[0] = ' ';
    cmdbuf[1] = 0;
    cmdpos = curpos = 0;

}

/******************************************************************************/
// get back to the settings before stating application
void Shell::prompt(void){
    char buf[32];
    sys_getcwd(buf, 32);
    lcd_printf("%s>", buf);
    move(lcd_getcurx(), lcd_getcury());
}


/******************************************************************************/
int Shell::move(int x, int y){
    while(1){
        if(x >= window.x){
            x -= window.x;
            y++;
        }else if(x < 0){
            x += window.x;
            y--;
        }else{
            break;
        }
    }

    if(y >= window.y){
        y = window.y - 1;
    }else if(y < 0){
        y = 0;
    }

    cursor.set(x, y);
    lcd_locate(x, y);
    return 0;
}

/******************************************************************************/
void Shell::startup(){
    int fd;
    fd = sys_open(const_cast<char *>("/init"), O_RDONLY);
    if(fd < 0) return;
    sys_close(fd);
    parse(const_cast<char *>("/init"));
}

/******************************************************************************/
void Shell::normal(uchar key){
    if(mpc_isprint(key)){
        if(cmdpos < MAX_CMD_BUF_SIZE - 2)
            append(key);
        
    }else{
        switch(key){
        case MKS_Enter:
            enter();
            break;
                
        case MKS_BackSpace:
        case MKS_Delete:
            //case 0x7f: /* MIERU_KEY_DELETE */
            backspace();
            break;
        case MKS_Left:
            left();
            break;
        case MKS_Right:
            right();
            break;
        }
    }
}

/******************************************************************************/
void Shell::control(uchar key){
    switch(key){
    case 'a':
        first();
        break;
    case 'b':
        left();
        break;
    case 'd':
        del();
        break;
    case 'e':
        end();
        break;
    case 'f':
        right();
        break;
    case 'h':
        backspace();
        break;
    case 'k':
        killline();
        break;
    case 'l':
        clear();
        prompt();
        break;
    }
}

/******************************************************************************/
void Shell::alt(uchar key){
    __UNUSED_VARIABLE(key);
}

/******************************************************************************/
void Shell::nextline(){
    lcd_puts("\n");
    move(0, cursor.y+1);
}

/******************************************************************************/
void Shell::right(){
    if(curpos != cmdpos){
        lcd_print((char *)&cmdbuf[curpos]);
        curpos++;
        move(cursor.x+1, cursor.y);
    }
}

/******************************************************************************/
void Shell::left(){
    if(curpos > 0){
        lcd_print((char *)&cmdbuf[curpos]);
        curpos--;
        move(cursor.x-1, cursor.y);
    }
}

/******************************************************************************/
void Shell::first(){
    lcd_print((char *)&cmdbuf[curpos]);
    move( cursor.x - curpos, cursor.y);
    curpos = 0;
}

/******************************************************************************/
void Shell::end(){
    uchar off;
    lcd_print((char *)&cmdbuf[curpos]);
    off = static_cast<uchar>(cmdpos - curpos);
    move(cursor.x + off, cursor.y);
    curpos = cmdpos;
}

/******************************************************************************/
void Shell::append(char c){
    for(int i = cmdpos + 1; i >= curpos; i--){
        cmdbuf[i+1] = cmdbuf[i];
    }
    cmdbuf[curpos] = c;
    lcd_print((char *)&cmdbuf[curpos]);

    cmdpos++;
    curpos++;
    move(cursor.x+1,cursor.y);
}

/******************************************************************************/
void Shell::backspace(){
    uchar i;
    int x, y;
    
    if(curpos == 0)
        return;

    for(i=curpos; i<=cmdpos+1; i++){
        cmdbuf[i-1] = cmdbuf[i];
    }

    x = cursor.x-1; y = cursor.y;
    curpos--;
    cmdpos--;

    move(x, y);
    lcd_print((char *)&cmdbuf[curpos]);
    lcd_print(" ");
    move(x, y);
}

/******************************************************************************/
void Shell::del(){
    int i;
    if(curpos == cmdpos)
        return;
    
    for(i=curpos+1; i<=cmdpos+1; i++){
        cmdbuf[i-1] = cmdbuf[i];
    }

    lcd_print((char *)&cmdbuf[curpos]);
    lcd_print(" ");
    cmdpos--;
    move(cursor.x, cursor.y);    
}

/******************************************************************************/
void Shell::killline(){
    int i;
    for(i=curpos; i<cmdpos; i++)
        lcd_print(" ");

    cmdpos = curpos;
    cmdbuf[cmdpos] = ' ';
    cmdbuf[cmdpos + 1] = 0;
    move(cursor.x, cursor.y);
}

/******************************************************************************/
void Shell::enter(){
    end();
    nextline();
    parse((char *)cmdbuf);
    prompt();
}

/******************************************************************************/
void Shell::printc(char c){
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    lcd_print(buf);

    move(cursor.x + 1, cursor.y);
}

/******************************************************************************/
// make argc and argv
int Shell::parse(char *str){
    char *p;
    char *argv[MAX_ARGC_SIZE+1];
    int i, argc, ret = 0;

    for(i=0;i<MAX_ARGC_SIZE;i++)
        argv[i] = 0;

    argc = 0;
    p = str;

    while(1){
        while(*p == ' ') p++;
        if(*p == 0)
            break;

        argv[argc++] = p;
        if(argc == MAX_ARGC_SIZE)
            break;

        while(*p != ' ' && *p != 0) p++;
        if(*p == 0)
            break;

        *p = 0;
        p++;
        
    }

    argv[argc] = 0;

    if(argc > 0){
        ret = command(argc, argv);
        reset();
    }

    return ret;
}

/******************************************************************************/
// execute command 
int Shell::command(int argc, char *argv[]){
    int ret = 0;
    // inner kernel commands
    if(!strcmp(argv[0], "ls")){
        if(argc == 1)
            ret = emb_ls("./");
        else if(argc == 2)
            ret = emb_ls(argv[1]);

    }else if(!strcmp(argv[0], "cd")){
        if(argc == 1)
            ret = emb_cd("/");
        else if(argc == 2)
            ret = emb_cd(argv[1]);

    }else if(!strcmp(argv[0], "cls")){
        ret = emb_cls();

    }else if(!strcmp(argv[0], "pwd")){
        ret = emb_pwd();
        
    }else if(!strcmp(argv[0], "sh")){
        ret = emb_sh(argc, argv);

    }else if(!strcmp(argv[0], "help")){
        ret = emb_help();

    }else{ // other command
        ret = exec(argc, argv);
    }

    return ret;
}

/******************************************************************************/
int Shell::emb_ls(const char *arg){
    int fd, ret, len;
    FileEntry ent;

    if(!strcmp(arg, "help")){
        lcd_print("usage: ls [target_dir]\n");
        return 0;
    }

    fd  = sys_open(arg, O_DIRECTORY);
    if(fd < 0){
        lcd_printf("ls error\n");
        nextline();
        return -1;
    }

    for(;;){
        ret = sys_read(fd, &ent, 1 * sizeof(FileEntry));
        if(ret <= 0) break;
        
        len = strlen(ent.name);

        if(ent.mode & 0x10){
            lcd_printf(" %s/ ", ent.name);
            len += 2;
        }else{
            lcd_printf(" %s ", ent.name);
            len += 1;
        }
        move(cursor.x + len, cursor.y);
    }

    nextline();
    sys_close(fd);

    return ret;
}

/******************************************************************************/
int Shell::emb_cd(const char *arg){
    int ret;

    if(!strcmp(arg, "help")){
        lcd_print("usage: cd [target_dir]\n");
        return 0;
    }

    ret = sys_chdir(arg);
    if(ret < 0){
        switch(-ret){
        case FAT_ERROR_INVALID_FILE_NAME:
            lcd_print("invalid file name\n");
            break;
        case FAT_ERROR_NO_SUCH_FILE:
            lcd_print("file not found\n");
            break;
        case FAT_ERROR_NOT_DIRECTORY:
            lcd_printf("%s is not directory\n", arg);
            break;
        }
        nextline();
    }


    return ret;
}

/******************************************************************************/
int Shell::emb_sh(int argc, char **argv){
    int fd, ret;
    char *p;
    bool loop;

    if(argc != 2){
        lcd_print("usage: sh [script file]\n");
        return -1;
    }

    if(!strcmp(argv[1], "help")){
        lcd_print("usage: sh [script file]\n");
        return 0;
    }

    fd = sys_open(argv[1], O_RDONLY);
    if(fd < 0){
        lcd_printf("can't open %s.\n", argv[1]);
        return -1;
    }

    loop = true;
    while(loop){
        ret = sys_read(fd, cmdbuf, Shell::MAX_CMD_BUF_SIZE / 2);
        if(ret < 0){
            lcd_printf("file read error %s\n", argv[1]);
            break;
        }else if(ret <= 1){
            break;
        }

        cmdbuf[ret] = 0;
        for(p = (char *)cmdbuf; *p != '\n' && *p != 0; p++);
        if(*p == 0)
            loop = false;
        *p = 0;

        int len = p - (char *)cmdbuf;
        sys_lseek(fd, ret - len, SEEK_CUR);

        ret = parse((char *)cmdbuf);
        if(ret < 0)
            break;
    }

    sys_close(fd);
    return ret;
}

/******************************************************************************/
// all clear display settings
int Shell::emb_cls(void){
    lcd_reset();
    lcd_cls();
    return 0;
}

/******************************************************************************/
// all clear display settings
int Shell::emb_pwd(void){
    char buf[32];
    sys_getcwd(buf, 32);
    lcd_printf("%s\n", buf);
    return 0;
}

/******************************************************************************/
int Shell::emb_help(){
    clear();
    lcd_printf("MieruOS %s\n.", MIERUOS_VERSION);
    lcd_printf("built-in commands are ls, cd, cls, pwd, sh.\n");
    lcd_printf("For more information, please use help command.\n");
    return 0;
}

/******************************************************************************/
int Shell::exec(int argc, char *argv[]){
    __UNUSED_VARIABLE(argc);
    int ret;
    char path[128];
    snprintf(path, 128, "/bin/%s", argv[0]);

    lcd_setscrollmode(0);
    lcd_setcursor(0);
    lcd_settransparent(0);
    lcd_setforeground(COL_WHITE);
    lcd_setbackground(COL_BLACK);
    lcd_setpos(0, 0);
    lcd_setshowpage(0);
    lcd_setdrawpage(0);

    ret = sys_execve(path, argv, 0);
    if(ret == -1)
        ret = sys_execve(argv[0], argv, 0);

    reset();
    if(ret < 0){
        switch(-ret){
        case 1:
            lcd_print("command not found\n");
            break;
        case 2:
            lcd_print("kernel error. can't read file\n");
            break;
        case 3:
            lcd_print("file is not elf file\n");
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            lcd_print("improper elf file\n");
            break;
        case 8:
            lcd_print("entry point error\n");
            break;
        case 9:
        case 10:
        case 11:
        case 12:
            lcd_print("file is broken or kernel error\n");
            break;
        }
    }

    return ret;
}

/******************************************************************************/
