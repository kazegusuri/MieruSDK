/******************************************************************************/
/* MieruPC Project: Memory Allocation Library v1.1                            */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-06-29 */
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
 *@brief メモリアロケーションライブラリ
 *
 *いわゆるmallocのライブラリ．
 *このライブラリはOS用のアプリケーションの時のみ使用可能である．
 */

#include <mierulib.h>

#ifdef MIERUPC_APPLICATION

static int has_initialized = 0;
static void *managed_memory_start;
static void *managed_memory_end;

/**********************************************************************/
void *mpc_sbrk(int req_size)
{
    static uint brk;
    uint tmp, last;
    if(req_size == 0) {
        brk = sys_brk(0);
        last = brk;
    } else {
        last = brk;
        tmp = brk + req_size;
        brk = sys_brk(tmp);
        
        if(brk != tmp)
            return (void *)-1;
    }

    /*return last_valid_address;*/
    return (void *)last;
}

/**********************************************************************/
void malloc_init()
{
    if(!has_initialized){
        /* according to mieruPC's specification */
        managed_memory_start = mpc_sbrk(0);
        managed_memory_end = managed_memory_start;
        has_initialized = 1;
    }
}

/**********************************************************************/
void mpc_free(void *pointer)
{
    struct memory_control_block *mcb, *prev, *next;

    if(pointer == NULL)
        return;

    /* put the pointer back to find the memory_control_block */
    mcb = (struct memory_control_block *)((uint)pointer - sizeof(struct memory_control_block));
    mcb->is_available = 1;

    next = mcb->next;
    if(next != NULL && next->is_available){
        mcb->size += next->size;
        mcb->next = next->next;
        if(next->next != NULL)
            next->next->prev = mcb;
    }

    prev = mcb->prev;
    if(prev != NULL && prev->is_available){
        prev->size += mcb->size;
        prev->next = mcb->next;
        if(mcb->next != NULL)
            mcb->next->prev = prev;
    }
}

/**********************************************************************/
void *mpc_malloc(long int req_size)
{
    struct memory_control_block *prev_location = NULL; // used to remember previous address
    //struct memory_control_block *next_location = NULL; // used to remember next address
    void *current_location;     // used to calculate address
    struct memory_control_block *current_location_mcb;
    void *mem_location_return = NULL; // return value
    struct memory_control_block *tmp;

    if (!has_initialized)
        malloc_init();

    /* the size of memory block should contain the 
     * control block which user shold not know.*/
	if(req_size%sizeof(size_t))
		req_size += sizeof(size_t) - req_size%sizeof(size_t); // align
    req_size = req_size + sizeof(struct memory_control_block);

    /* search empty location and simply use it */
    current_location = managed_memory_start;
    while(current_location != managed_memory_end) {
        current_location_mcb = (struct memory_control_block *) current_location;
        if(current_location_mcb->is_available &&
           current_location_mcb->size >= req_size) {
            current_location_mcb->is_available = 0;
            mem_location_return = current_location;
            
            /* split mergin space if the space is large*/
            if(current_location_mcb->size > req_size + 32){
                tmp = (struct memory_control_block *)((uint)current_location_mcb + req_size);
                tmp->is_available = 1;
                tmp->size = current_location_mcb->size - req_size;
                tmp->prev = current_location_mcb;
                tmp->next = current_location_mcb->next;

                current_location_mcb->next = tmp;
                current_location_mcb->size = req_size;

            }

            break;
        }

        prev_location = (struct memory_control_block *)current_location;
        current_location = (void *)((uint)current_location + current_location_mcb->size);
    }

    /*ask system for more memory if empty location not found*/
    if(mem_location_return == 0) {
        mem_location_return = mpc_sbrk(req_size);
        if((int)mem_location_return < 0)
            return 0;

        managed_memory_end = (void *)((uint)mem_location_return + req_size);

        /*initialize memory_control_block*/
        current_location_mcb = (struct memory_control_block *)mem_location_return;
        current_location_mcb->is_available = 0;
        current_location_mcb->size = req_size;
        current_location_mcb->prev = prev_location;
        current_location_mcb->next = 0;
        prev_location->next = current_location_mcb;
        //lcd_debugprintf("prev %x\n", prev_location);
    }

    mem_location_return = (void *)((uint)mem_location_return + sizeof(struct memory_control_block));
    return mem_location_return;
}

/**********************************************************************/
void *mpc_realloc(void *pointer, int size){
    struct memory_control_block *mcb;
    void *p;
    int s;

    if(pointer == NULL)
        return NULL;

    mcb = (struct memory_control_block *)((uint)pointer - sizeof(struct memory_control_block));

    p = mpc_malloc(size);
    if(p == NULL)
        return NULL;

    if( mcb->size > size)
        s = size;
    else
        s = mcb->size;

    mpc_memcpy(p, pointer, s);

    return p;
}


/**********************************************************************/
#endif
