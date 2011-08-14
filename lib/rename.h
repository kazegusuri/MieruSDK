/******************************************************************************/
/* MieruPC Project: rename definition v0.1                                    */
/* written by Masahiro Sano, Arch Lab. Tokyo Tech                  2010-06-14 */
/******************************************************************************/
#pragma once

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

#define mpc_memcpy memcpy
#define mpc_memset memset

#ifdef MPC_MLIB_RENAME

#define mpc_snprintf snprintf
#define mpc_vsnprintf vsnprintf
#define mpc_strncpy strncpy
#define mpc_strcpy strcpy
#define mpc_strlen strlen
#define mpc_strncmp strncmp
#define mpc_strcmp strcmp
#define mpc_strchr strchr
#define mpc_strstr strstr
#define mpc_strcat strcat
#define mpc_atoi atoi
#define mpc_iscntrl iscntrl
#define mpc_isdigit isdigit
#define mpc_isprint isprint
#define mpc_tolower tolower
#define mpc_toupper toupper
#define mpc_islower islower
#define mpc_isupper isupper
#define mpc_memcmp mpc_memcmp

#define mpc_waitkey waitkey
#define mpc_gettime gettime
#define mpc_usleep usleep
#define mpc_getkey getkey
#define mpc_getchar getchar

#define mpc_malloc malloc
#define mpc_realloc realloc
#define mpc_free free



#endif
