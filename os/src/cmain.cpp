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

/*! @file cmain.cpp
 *@brief entry point in C
 *@author Masahiro Sano
 *@since 2010/06/14
 *@date 2010/06/14
 *@version 0.1
 */

#include <kernel.h>

extern unsigned int start_ctors, end_ctors, start_dtors, end_dtors;

/******************************************************************************/
extern "C" void main(void){

    // call all the static constructors
    for(unsigned int *constructor(&start_ctors); constructor < &end_ctors; ++constructor){
        //lcd_dprintf(">>%x\n", *constructor);
        ((void (*) (void)) (*constructor)) ();
    }

    startKernel();

    // call all the static destructors
    for(unsigned int *destructor(&start_dtors); destructor < &end_dtors; ++destructor)
        ((void (*) (void)) (*destructor)) ();

    for(;;);
}

/******************************************************************************/
extern "C" void __cxa_pure_virtual(){
    lcd_cls();
    lcd_dprintf("Inheritance Error\n");
    lcd_printf("Inheritance Error\n");
    lcd_printf("This is kernel error.\n");
    lcd_printf("Please report to a developer\n");
    for(;;);
}

/******************************************************************************/
int  _purecall(){
    // Do nothing or print an error message.
    return 0;
}

/******************************************************************************/

