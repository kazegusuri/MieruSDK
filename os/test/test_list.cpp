// -*- C++ -*-
/*
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

/*! @file test_list.cpp
 *@brief test for list template
 *@author Masahiro Sano
 *@since 2011/11/04
 *@date 2011/11/05
 */


#include <list.hpp>
#include <test.h>

#ifdef TEST_MODE

void test_class_list(void) {
    if(1){
        mpc::list<int>::iterator it;
        int i;

        mpc::list<int> a;
        TEST_TRUE(a.size() == 0);
        mpc::list<int> b(4);
        TEST_TRUE(b.size() == 4);
        for (it = b.begin(); it != b.end(); ++it) {
            TEST_TRUE(*it == 0);
        }
        mpc::list<int> c(3, 1);
        TEST_TRUE(c.size() == 3);
        for (it = c.begin(); it != c.end(); ++it) {
            TEST_TRUE(*it == 1);
        }
        mpc::list<int> d = {1,2,3,4,5};
        TEST_TRUE(d.size() == 5);
        for (it = d.begin(), i = 1; it != d.end(); ++it, ++i) {
            TEST_TRUE(*it == i);
        }

        mpc::list<int> e(d);
        TEST_TRUE(e.size() == 5);
        for (it = e.begin(), i = 1; it != e.end(); ++it, ++i) {
            TEST_TRUE(*it == i);
        }
        mpc::list<int>::iterator it0 = e.begin();
        mpc::list<int>::iterator it1 = e.end();
        ++it0;
        --it1;
        TEST_TRUE(*it0 == 2);
        TEST_TRUE(*it1 == 5);
        mpc::list<int> f(it0, it1);
        TEST_TRUE(f.size() == 3);
        for (it = f.begin(), i = 2; it != f.end(); ++it, ++i) {
            TEST_TRUE(*it == i);
        }
        mpc::list<int> g = {2,3,4};
        TEST_TRUE(f == g);
    }

    if(1){
        mpc::list<int> expect = {1, 2, 3};
        mpc::list<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        TEST_TRUE(expect == l);
    }

    if(1){
        mpc::list<int> expect1 = {3, 2, 1};
        mpc::list<int> expect2 = {3, 4, 2, 1};
        mpc::list<int> expect3 = {5, 5, 5, 3, 4, 2, 1};
        mpc::list<int> l;

        l.insert(l.begin(), 1);
        l.insert(l.begin(), 2);
        l.insert(l.begin(), 3);
        TEST_TRUE(expect1 == l);
        l.insert(++l.begin(), 4);
        TEST_TRUE(expect2 == l);
        l.insert(l.begin(), 3, 5);
        TEST_TRUE(expect3 == l);
    }

    if(1){
        mpc::list<int> expect = {0, 3, 4, 5, 1};
        mpc::list<int> l1 = {3, 4, 5};
        mpc::list<int> l2 = {0, 1};
        l2.insert(++l2.begin(), l1.begin(), l1.end());
        TEST_TRUE(expect == l2);
    }

    if(1){
        mpc::list<int>::iterator iter, ret;
        mpc::list<int> expect1 = {2, 3, 4, 5};
        mpc::list<int> expect2 = {2, 3, 5};
        mpc::list<int> expect3 = {2, 3};
        mpc::list<int> expect4 = {2};
        mpc::list<int> l = {1, 2, 3, 4, 5};
        iter = l.begin();
        ret = l.erase(l.begin());
        ++iter;
        TEST_TRUE(expect1 == l);
        TEST_TRUE(iter == ret);

        iter = ++++l.begin();
        ret = l.erase(iter);
        ++iter;
        TEST_TRUE(expect2 == l);
        TEST_TRUE(iter == ret);

        iter = ++++l.begin();
        ret = l.erase(iter);
        ++iter;        
        TEST_TRUE(expect3 == l);
        TEST_TRUE(iter == ret);

        iter = --l.end();
        ret = l.erase(iter);
        ++iter;
        TEST_TRUE(expect4 == l);
        TEST_TRUE(iter == ret);
    }

    if(1){
        mpc::list<int> expect1 = {3, 3, 3, 3, 3, 3, 3};
        mpc::list<int> expect2 = {6, 6, 6, 6};
        mpc::list<int> expect3 = {9};
        mpc::list<int> expect4 = {9, 8, 7, 6, 5, 4};
        mpc::list<int> l = {1, 2, 3};
        mpc::list<int> l2 = {9, 8, 7, 6, 5, 4};
        l.assign(7, 3);
        TEST_TRUE(expect1 == l);

        l.assign(4, 6);
        TEST_TRUE(expect2 == l);

        l.assign(l2.begin(), ++l2.begin());
        TEST_TRUE(expect3 == l);

        l.assign(l2.begin(), l2.end());
        TEST_TRUE(expect4 == l);
    }

    if(1){
        mpc::list<int> expect1 = {0, 0, 0, 0, 5, 5, 5};
        mpc::list<int> l;
        TEST_TRUE(l.empty());

        l.resize(4);
        TEST_FALSE(l.empty());
        TEST_TRUE(l.size() == 4);

        l.resize(7, 5);
        TEST_TRUE(l.size() == 7);
        TEST_TRUE(expect1 == l);

        l.clear();
        TEST_TRUE(l.empty());
    }

    if(1){
        mpc::list<int> expect1 = {4, 5, 6, 1, 2, 3};
        mpc::list<int> expect2 = {};
        mpc::list<int> expect3 = {9, 4, 5, 6, 1, 2, 3};
        mpc::list<int> expect4 = {7, 8, 10};
        mpc::list<int> expect5 = {8, 10, 9, 4, 5, 6, 1, 2, 3};
        mpc::list<int> expect6 = {7};
        mpc::list<int> l1 = {1, 2, 3};
        mpc::list<int> l2 = {4, 5, 6};
        mpc::list<int> l3 = {7, 8, 9, 10};

        l1.splice(l1.begin(), l2);
        TEST_TRUE(expect1 == l1);
        TEST_TRUE(expect2 == l2);

        l1.splice(l1.begin(), l3, ++++l3.begin());
        TEST_TRUE(expect3 == l1);
        TEST_TRUE(expect4 == l3);

        l1.splice(l1.begin(), l3, ++l3.begin(), l3.end());
        TEST_TRUE(expect5 == l1);
        TEST_TRUE(expect6 == l3);
    }

    if(1){
        class TestPred {
        public:
            bool operator()(const int &n) { return n > 1; };
        };

        mpc::list<int> expect1 = {2, 3, 3, 2};
        mpc::list<int> expect2 = {1, 1, 1};
        mpc::list<int> l1 = {1, 2, 3, 3, 1, 2, 1};
        mpc::list<int> l2 = {1, 2, 3, 3, 1, 2, 1};
        l1.remove(1);
        TEST_TRUE(expect1 == l1);

        TestPred _pred;
        l2.remove_if(_pred);
        TEST_TRUE(expect2 == l2);
    }
    
    if(1){
        mpc::list<int> expect1 = {3, 4, 5, 6, 3, 7};
        mpc::list<int> expect2 = {1, 2, 3, 3, 3, 4, 5, 5, 5, 6, 3, 7, 8, 9};
        mpc::list<int> expect3 = {0, 2, 3, 4, 4, 5, 5, 6, 7, 8, 9};
        mpc::list<int> expect4 = {9, 8, 7, 6, 5, 5, 4, 4, 3, 2, 0};
        mpc::list<int> l1 = {3, 4, 4, 4, 5, 6, 6, 3, 7, 7};
        mpc::list<int> l2 = {1, 2, 3, 3, 5, 5, 8, 9};
        mpc::list<int> l3 = {9, 4, 5, 6, 7, 8, 3, 4, 0, 5, 2};
        l1.unique();
        TEST_TRUE(expect1 == l1);

        l1.merge(l2);
        TEST_TRUE(expect2 == l1);

        l3.sort();
        TEST_TRUE(expect3 == l3);

        l3.reverse();
        TEST_TRUE(expect4 == l3);
    }

    if(1){
        mpc::list<int> expect1 = {1, 2, 3, 4};
        mpc::list<int> expect2 = {5, 4, 3, 2};
        mpc::list<int> expect3;
        mpc::list<int> l1 = {5, 4, 3, 2};
        mpc::list<int> l2 = {1, 2, 3, 4};
        l1.swap(l2);
        TEST_TRUE(expect1 == l1);
        TEST_TRUE(expect2 == l2);

        l1.clear();
        l1.swap(l2);
        TEST_TRUE(expect2 == l1);
        TEST_TRUE(expect3 == l2);

        l1.swap(l2);
        TEST_TRUE(expect3 == l1);
        TEST_TRUE(expect2 == l2);
    }
}

#endif
