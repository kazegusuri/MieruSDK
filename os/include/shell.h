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

/*! @file shell.h
 *@brief Header file for shell class
 *@author Masahiro Sano
 *@since 2010/06/14
 *@date 2010/06/14
 *@version 0.1
 */

#pragma once
#include <types.h>
#include <point.h>

class Shell{
private:
    enum{
        MAX_CMD_BUF_SIZE = 128,
        MAX_ARGC_SIZE = 6,
    };

    Point window;
    Point cursor;
    uchar cmdbuf[MAX_CMD_BUF_SIZE];
    uchar cmdpos;
    uchar curpos;

public:
    Shell();

    void run();
    void startup();
    void normal(uchar key);
    void control(uchar key);
    void alt(uchar key);

    void clear();
    void reset();
    void prompt();
    void cls();
    int  move(int x, int y);
    void nextline();
    void right();
    void left();
    void first();
    void end();
    void append(char c);
    void backspace();
    void del();
    void killline();
    void enter();
    void printc(char c);

    int parse(char *str);
    int command(int argc, char *argv[]);
    int emb_ls(const char *arg);
    int emb_cd(const char *arg);
    int emb_cls();
    int emb_pwd();
    int emb_sh(int argc, char **argv);
    int emb_help();
    int exec(int argc, char *argv[]);
};
