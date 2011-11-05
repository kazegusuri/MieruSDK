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

/*! @file list.hpp
 *@brief list template
 *@author Masahiro Sano
 *@since 2011/11/01
 *@date 2011/11/05
 */

#pragma once

#include <types.h>
#include <new.h>
#include "iterator"
#include "initializer_list"
#include "algorithm"
#include "functional"

#include <test.h>

#define _LIBCPP_VISIBLE

namespace mpc {

    template<typename T> 
    class _List_node {
    public:
        T _data;
        _List_node* _next;
        _List_node* _prev;
    };


    template<typename T> 
    class _List_iterator {
    public:
        typedef _List_iterator<T>           _Self;
        typedef _List_node<T>               _Node;
        typedef T                           value_type;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef ptrdiff_t                   difference_type;
        typedef bidirectional_iterator_tag  iterator_category;

        _List_node<T> *_node;

        _List_iterator() : _node() {}
        _List_iterator(_List_node<T> *n) : _node(n) {}

        reference operator*() const {
            return _node->_data; 
        }

        pointer operator->() const {
            return &_node->_data;
        }

        _Self operator--(int) {
            _Self tmp = *this;
            _node = _node->_prev;
            return tmp;
        }

        _Self& operator--() {
            _node = _node->_prev;
            return *this;
        }

        _Self operator++(int) {
            _Self tmp = *this;
            _node = _node->_next;
            return tmp;
        }

        _Self& operator++() {
            _node = _node->_next;
            return *this;
        }

        bool operator==(const _Self& __x) const {
            return _node == __x._node;
        }

        bool operator!=(const _Self& __x) const {
            return _node != __x._node;
        }
    };

    template<typename T> 
    class _List_const_iterator {
    public:
        typedef _List_const_iterator<T>          _Self;
        typedef const _List_node<T>              _Node;
        typedef _List_iterator<T>                iterator;
        typedef T                                value_type;
        typedef const T*                         pointer;
        typedef const T&                         reference;
        typedef ptrdiff_t                        difference_type;
        typedef mpc::bidirectional_iterator_tag  iterator_category;

        const _List_node<T> *_node;

        _List_const_iterator() : _node() {}
        _List_const_iterator(const _List_node<T> *n) : _node(n) {}
        _List_const_iterator(const iterator &x) : _node(x._node) {}


        reference operator*() const {
            return _node->_data; 
        }

        pointer operator->() const {
            return &_node->_data;
        }

        _Self operator--(int) {
            _Self tmp = *this;
            _node = _node->_prev;
            return tmp;
        }

        _Self& operator--() {
            _node = _node->_prev;
            return *this;
        }

        _Self operator++(int) {
            _Self tmp = *this;
            _node = _node->_next;
            return tmp;
        }

        _Self& operator++() {
            _node = _node->_next;
            return *this;
        }

        bool operator==(const _Self& __x) const {
            return _node == __x._node;
        }

        bool operator!=(const _Self& __x) const {
            return _node != __x._node;
        }
    };

    template<typename T>
    class list {
        TEST_CLASS_REG(list);
    public:
        typedef T                                      value_type;
        typedef T*                                     pointer;
        typedef T&                                     reference;
        typedef const T*                               const_pointer;
        typedef const T&                               const_reference;
        typedef ptrdiff_t                              difference_type;
        typedef size_t                                 size_type;
        typedef _List_iterator<T>                      iterator;
        typedef _List_const_iterator<T>                const_iterator;
        typedef mpc::reverse_iterator<iterator>        reverse_iterator;
        typedef mpc::reverse_iterator<const_iterator>  const_reverse_iterator;
        typedef _List_node<T>                          _Node;

    private:
        _Node _node;
        size_t _size;

    public:
        _Node* _create_node(const value_type& __x) {
            _Node* __p = static_cast<_Node* >(::operator new(sizeof(_Node)));
            ::new((void *)&__p->_data) T(__x);
            ++_size;
            return __p;
        }

        void _destroy_node(_Node *node) {
            (&node->_data)->~T();
            delete(node);
            --_size;
        }

        void _link_node(_Node &p, _Node &n) {
            p._prev->_next = &n;
            n._prev = p._prev;
            p._prev = &n;
            n._next = &p;
        }

        void _link_node(_Node &p, _Node &f, _Node &l) {
            p._prev->_next = &f;
            f._prev = p._prev;
            p._prev = &l;
            l._next = &p;
        }

        void _unlink_node(_Node &n) {
            n._prev->_next = n._next;
            n._next->_prev = n._prev;
        }

        void _unlink_node(_Node &f, _Node &l) {
            f._prev->_next = l._next;
            l._next->_prev = f._prev;
        }

        void _init() {
            this->_node._next = &this->_node;
            this->_node._prev = &this->_node;
            _size = 0;
        }

        void _clear() {
            erase(begin(), end());
        }

    public:
        list() {
            _init();
        }

        list(size_type n) {
            _init();
            for (; n > 0; --n) {
                push_back(value_type());
            }
        }
        list(size_type n, const value_type &x) {
            _init();
            for (; n > 0; --n) {
                push_back(x);
            }
        }
        list(const list& x) {
            _init();
            const_iterator it, e;
            for (it = x.begin(), e = x.end(); it != e; ++it) {
                push_back(*it);
            }
        }

        list(std::initializer_list<value_type> x) {
            _init();
            for (auto it = x.begin(); it != x.end(); ++it) {
                push_back(*it);
            }
        }

        template <class _InpIter>
        list(_InpIter first,
             _InpIter last,
             typename enable_if<__is_input_iterator<_InpIter>::value>::type* = 0) {
            _init();
            for (; first != last; ++first) {
                push_back(*first);
            }
        }

        list& operator=(const list& __x) {
            if (this == &__x) return *this;
            iterator __first1 = begin();
            iterator __last1 = end();
            const_iterator __first2 = __x.begin();
            const_iterator __last2 = __x.end();
            for (; __first1 != __last1 && __first2 != __last2;
                 ++__first1, ++__first2)
                *__first1 = *__first2;
            if (__first2 == __last2)
                erase(__first1, __last1);
            else
                insert(__last1, __first2, __last2);
        }

        void assign(size_type n, const value_type &x) {
            iterator it = begin();
            iterator e = end();
            for (; n > 0 && it != e; --n, ++it) {
                *it = x;
            }
            if (n > 0) {
                for (; n > 0; --n) {
                    push_back(x);
                }
            } else {
                erase(it, e);
            }
        }

        template<typename _InpIter>
        void assign(_InpIter first, _InpIter last,
                    typename enable_if<__is_input_iterator<_InpIter>::value>::type* = 0) {

            iterator it = begin();
            iterator e = end();
            for (; first != last && it != e; ++first, ++it) {
                *it = *first;
            }
            if (it == e) {
                insert(e, first, last);
            } else {
                erase(it, e);
            }
        }

        iterator begin() {
            return iterator(this->_node._next);
        }

        const_iterator begin() const {
            return const_iterator(this->_node._next);
        }

        iterator end() {
            return iterator(&this->_node);
        }

        const_iterator end() const {
            return const_iterator(&this->_node);
        }

        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }

        bool empty() const {
            return _size == 0;
        }

        size_type size() const {
            return _size;
        }

        size_type max_size() const {
            return size_t(-1) / sizeof(T);
        }

        void resize(size_type new_size, value_type x = value_type()) {
            iterator it = begin();
            iterator _end = end();
            for (; it != _end && new_size > 0; ++it, --new_size) ;
            if (new_size > 0) {
                for (; new_size > 0; --new_size) {
                    push_back(x);
                }
            } else {
                erase(it, _end);
            }
        }

        reference front() { return *begin(); }

        const_reference front() const { return *begin(); }

        reference back() { 
            return *(--end());
        }

        const_reference back() const { 
            return *(--end());
        }

        void push_front(const value_type& x) { 
            insert(begin(), x); 
        }
        void pop_front() { 
            erase(begin());
        }
        void push_back(const value_type& x) {
            insert(end(), x);
        }
        void pop_back() {
            erase(iterator(_node._prev));
        }

        iterator insert(iterator pos, const value_type &x) {
            _Node* tmp = _create_node(x);
            _link_node(*pos._node, *tmp);
            return iterator(tmp);
        }
        void insert(iterator pos, size_type n, const value_type &x) {  
            for (; n > 0; --n) {
                insert(pos, x);
            }
        }

        template<typename _InpIter>
        void insert(iterator pos, _InpIter first, _InpIter last,
                    typename enable_if<__is_input_iterator<_InpIter>::value>::type* = 0) {
            for (; first != last; ++first) {
                insert(pos, *first);
            }
        }

        iterator erase(iterator pos) {
            iterator ret = iterator(pos._node->_next);
            _unlink_node(*pos._node);
            _destroy_node(pos._node);
            return ret;
        }
        iterator erase(iterator first, iterator last) {
            while (first != last) {
                first = erase(first);
            }
            return last;
        }

        void swap(list &x) {
            if (size() == 0) {
                splice(begin(), x);
            } else {
                _Node &next = *this->_node._next;
                _unlink_node(this->_node);
                _link_node(*x._node._next, this->_node);
                _unlink_node(x._node);
                _link_node(next, x._node);
                size_t tmp = this->_size;
                this->_size = x._size;
                x._size = tmp;
            }
        }

        void clear() {
            _clear();
            _init();
        }

        void splice(iterator pos, list &x) {
            if (!x.empty()) {
                _Node &first = *x._node._next;
                _Node &last  = *x._node._prev;
                _unlink_node(first, last);
                _link_node(*pos._node, first, last);
                this->_size += x.size();
                x._size = 0;
            }
        }

        void splice(iterator pos, list &x, iterator iter) {
            if (pos._node == iter._node || pos._node == iter._node->_next)
                return;
            _unlink_node(*iter._node);
            _link_node(*pos._node, *iter._node);
            this->_size++;
            x._size--;
        }

        void splice(iterator pos, list &x, iterator first, iterator last) {
            if (first == last) return;

            if (this != &x) {
                size_type size = mpc::distance(first, last);
                this->_size += size;
                x._size -= size;
            }

            last--;
            _unlink_node(*first._node, *last._node);
            _link_node(*pos._node, *first._node, *last._node);
        }

        void remove(const T &x) {
            for (iterator it = begin(); it != end();) {
                if (*it == x) {
                    it = erase(it);
                } else {
                    ++it;
                }
            }
        }

        template<typename _Predicate>
        void remove_if(_Predicate _pred) {
            for (iterator it = begin(); it != end();) {
                if (_pred(*it)) {
                    it = erase(it);
                } else {
                    ++it;
                }
            }
        }

        void unique() {
            iterator it = begin();
            iterator _end = end();
            while (it != _end) {
                iterator next = it;
                for (++next; next != _end && *next == *it; ++next) ;
                ++it;
                if (it != next) {
                    it = erase(it, next);
                }
            }
        }

        template<typename _BinaryPredicate>
        void unique(_BinaryPredicate _binary_pred) {
            iterator it = begin();
            iterator _end = end();
            while (it != _end) {
                iterator next = it;
                for (++next; next != _end && _binary_pred(*next, *it); ++next) ;
                ++it;
                if (it != next) {
                    it = erase(it, next);
                }
            }
        }

        void merge(list& x) {
            merge(x, mpc::less<value_type>());
        }

        template<typename _Comp> 
        void merge(list &x, _Comp _comp) {
            if (this == &x) return;

            iterator __f1 = begin();
            iterator __e1 = end();
            iterator __f2 = x.begin();
            iterator __e2 = x.end();
            while (__f1 != __e1 && __f2 != __e2) {
                if (_comp(*__f2, *__f1)) {
                    size_type __ds = 1;
                    iterator __m2 = __f2;
                    ++__m2;
                    for (; __m2 != __e2 && _comp(*__m2, *__f1); ++__m2, ++__ds) ;
                    this->_size += __ds;
                    x._size -= __ds;
                    _Node& __f = *__f2._node;
                    _Node& __l = *__m2._node->_prev;
                    __f2 = __m2;
                    _unlink_node(__f, __l);
                    __m2 = __f1;
                    ++__m2;
                    _link_node(*__f1._node, __f, __l);
                    __f1 = __m2;
                } else {
                    ++__f1;
                }
            }
            splice(__e1, x);
        }

        // sort
        void sort() {
            sort(mpc::less<value_type>());
        }
        template <class _Comp>
        void sort(_Comp _comp) {
            _sort(begin(), end(), size(), _comp);
        }

        // reverse
        void reverse() {
            if (size() == 0) return;
            iterator _end = end();
            for (iterator iter = begin(); iter._node != _end._node;) {
                mpc::swap(iter._node->_prev, iter._node->_next);
                iter._node = iter._node->_prev;
            }
            mpc::swap(_end._node->_prev, _end._node->_next);
        }

    private:
        template <class _Comp> 
        iterator _sort(iterator __f1, iterator __e2, size_type __n, _Comp& _comp) {
            switch (__n) {
            case 0:
            case 1:
                return __f1;
            case 2:
                if (_comp(*--__e2, *__f1))
                {
                    _Node& __f = *__e2._node;
                    _unlink_node(__f);
                    _link_node(*__f1._node, __f);
                    return __e2;
                }
                return __f1;
            }
            size_type __n2 = __n / 2;
            iterator __e1 = mpc::next(__f1, __n2);
            iterator  __r = __f1 = _sort(__f1, __e1, __n2, _comp);
            iterator __f2 = __e1 = _sort(__e1, __e2, __n - __n2, _comp);
            if (_comp(*__f2, *__f1)) {
                iterator __m2 = mpc::next(__f2);
                for (; __m2 != __e2 && _comp(*__m2, *__f1); ++__m2)
                    ;
                _Node& __f = *__f2._node;
                _Node& __l = *__m2._node->_prev;
                __r = __f2;
                __e1 = __f2 = __m2;
                _unlink_node(__f, __l);
                __m2 = mpc::next(__f1);
                _link_node(*__f1._node, __f, __l);
                __f1 = __m2;
            } else {
                ++__f1;
            }
            while (__f1 != __e1 && __f2 != __e2) {
                if (_comp(*__f2, *__f1))
                {
                    iterator __m2 = mpc::next(__f2);
                    for (; __m2 != __e2 && _comp(*__m2, *__f1); ++__m2)
                        ;
                    _Node& __f = *__f2._node;
                    _Node& __l = *__m2._node->_prev;
                    if (__e1 == __f2)
                        __e1 = __m2;
                    __f2 = __m2;
                    _unlink_node(__f, __l);
                    __m2 = mpc::next(__f1);
                    _link_node(*__f1._node, __f, __l);
                    __f1 = __m2;
                }
                else
                    ++__f1;
            }
            return __r;
        }
    };

    template<class T>
    inline bool operator==(const list<T> &lhs, const list<T> &rhs) {
        return lhs.size() == rhs.size() && mpc::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template<class T>
    inline bool operator<(const list<T> &lhs, const list<T> &rhs) {
        return mpc::lexicographical_compare(lhs.begin(), lhs.end(),
                                            rhs.begin(), rhs.end());
    }
    template<class T>
    inline bool operator!=(const list<T> &lhs, const list<T> &rhs) {
        return !(lhs == rhs);
    }

    template<class T>
    inline bool operator>(const list<T> &lhs, const list<T> &rhs) {
        return rhs < lhs;
    }

    template<class T>
    inline bool operator<=(const list<T> &lhs, const list<T> &rhs) {
        return !(rhs < lhs);
    }

    template<class T>
    inline bool operator>=(const list<T> &lhs, const list<T> &rhs) {
        return !(lhs < rhs);
    }

    template<class T>
    inline void swap(list<T> &lhs, list<T> &rhs) {
        lhs.swap(rhs);
    }
}
