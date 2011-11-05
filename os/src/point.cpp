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

/*! @file point.cpp
 *@brief point implemetantion
 *@author Masahiro Sano
 *@since 2010/06/14
 *@date 2011/11/04
 */

#include <point.h>

/*****************************************************************/
Point::Point(){
    x = y = 0;
}

/*****************************************************************/
Point::Point(const Point &p){
    x = p.x;
    y = p.y;
}

/*****************************************************************/
Point::Point(int _x, int _y){
    x = _x;
    y = _y;
}

/*****************************************************************/
bool Point::operator==(const Point &p){
    return (x == p.x) && (y == p.y);
}

/*****************************************************************/
Point &Point::operator=(const Point &p){
    x = p.x;
    y = p.y;
    return *this;
}

/*****************************************************************/
int Point::getx(){
    return x;
}

/*****************************************************************/
int Point::gety(){
    return y;
}

/*****************************************************************/
void Point::setx(int _x){
    x = _x;
}

/*****************************************************************/
void Point::sety(int _y){
    y = _y;
}

/*****************************************************************/
void Point::set(int _x, int _y){
    x = _x;
    y = _y;
}

/*****************************************************************/
