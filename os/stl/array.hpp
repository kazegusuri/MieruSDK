// -*- C++ -*-
//===-------------------------- iterator ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#pragma once

#include "type_traits"
#include "iterator"
//#include <utility>
//#include <algorithm>

namespace mpc {
    template <class _Tp, size_t _Size>
    struct _LIBCPP_VISIBLE array {
        typedef array __self;
        typedef _Tp                                   value_type;
        typedef value_type&                           reference;
        typedef const value_type&                     const_reference;
        typedef value_type*                           iterator;
        typedef const value_type*                     const_iterator;
        typedef value_type*                           pointer;
        typedef const value_type*                     const_pointer;
        typedef size_t                                size_type;
        typedef ptrdiff_t                             difference_type;
        typedef mpc::reverse_iterator<iterator>       reverse_iterator;
        typedef mpc::reverse_iterator<const_iterator> const_reverse_iterator;

        value_type __elems_[_Size > 0 ? _Size : 1];

        // No explicit construct/copy/destroy for aggregate type
        // void fill(const value_type& __u)
        // {mpc::fill_n(__elems_, _Size, __u);}
    
        // void swap(array& __a)
        // {mpc::swap_ranges(__elems_, __elems_ + _Size, __a.__elems_);}

        iterator begin()  {return iterator(__elems_);}
        const_iterator begin() const  {return const_iterator(__elems_);}
        iterator end()  {return iterator(__elems_ + _Size);}
        const_iterator end() const  {return const_iterator(__elems_ + _Size);}
    
        reverse_iterator rbegin()  {return reverse_iterator(end());}
        const_reverse_iterator rbegin() const  {return const_reverse_iterator(end());}
        reverse_iterator rend()  {return reverse_iterator(begin());}
        const_reverse_iterator rend() const  {return const_reverse_iterator(begin());}
    
        const_iterator cbegin() const  {return begin();}
        const_iterator cend() const  {return end();}
        const_reverse_iterator crbegin() const  {return rbegin();}
        const_reverse_iterator crend() const  {return rend();}

        size_type size() const  {return _Size;}
        size_type max_size() const  {return _Size;}
        bool empty() const  {return _Size == 0;}

        reference operator[](size_type __n)             {return __elems_[__n];}
        const_reference operator[](size_type __n) const {return __elems_[__n];}
        reference at(size_type __n);
        const_reference at(size_type __n) const;

        reference front()             {return __elems_[0];}
        const_reference front() const {return __elems_[0];}
        reference back()              {return __elems_[_Size > 0 ? _Size-1 : 0];}
        const_reference back() const  {return __elems_[_Size > 0 ? _Size-1 : 0];}

        value_type* data()  {return __elems_;}
        const value_type* data() const {return __elems_;}
    };

    template <class _Tp, size_t _Size>
    typename array<_Tp, _Size>::reference
    array<_Tp, _Size>::at(size_type __n) {
        if (__n >= _Size)
            assert(!"array::at out_of_range");
        return __elems_[__n];
    }

    template <class _Tp, size_t _Size>
    typename array<_Tp, _Size>::const_reference
    array<_Tp, _Size>::at(size_type __n) const {
        if (__n >= _Size)
            assert(!"array::at out_of_range");
        return __elems_[__n];
    }

    template <class _Tp, size_t _Size>
    inline bool operator==(const array<_Tp, _Size>& __x, const array<_Tp, _Size>& __y) {
        return mpc::equal(__x.__elems_, __x.__elems_ + _Size, __y.__elems_);
    }

    template <class _Tp, size_t _Size>
    inline bool operator!=(const array<_Tp, _Size>& __x, const array<_Tp, _Size>& __y) {
        return !(__x == __y);
    }

    template <class _Tp, size_t _Size>
    inline bool operator<(const array<_Tp, _Size>& __x, const array<_Tp, _Size>& __y) {
        return mpc::lexicographical_compare(__x.__elems_, __x.__elems_ + _Size, __y.__elems_, __y.__elems_ + _Size);
    }

    template <class _Tp, size_t _Size>
    inline bool operator>(const array<_Tp, _Size>& __x, const array<_Tp, _Size>& __y) {
        return __y < __x;
    }

    template <class _Tp, size_t _Size>
    inline bool operator<=(const array<_Tp, _Size>& __x, const array<_Tp, _Size>& __y) {
        return !(__y < __x);
    }

    template <class _Tp, size_t _Size>
    inline bool operator>=(const array<_Tp, _Size>& __x, const array<_Tp, _Size>& __y) {
        return !(__x < __y);
    }

    template <class _Tp, size_t _Size>
    inline typename enable_if <__is_swappable<_Tp>::value, void>::type
    swap(const array<_Tp, _Size>& __x, const array<_Tp, _Size>& __y) {
        __x.swap(__y);
    }


    // template <class _Tp, size_t _Size>
    // class tuple_size<array<_Tp, _Size> >
    //     : public integral_constant<size_t, _Size> {};

    // template <class _Tp, size_t _Size>
    // class tuple_size<const array<_Tp, _Size> >
    //     : public integral_constant<size_t, _Size> {};

    // template <size_t _Ip, class _Tp, size_t _Size>
    // class tuple_element<_Ip, array<_Tp, _Size> > {
    // public:
    //     typedef _Tp type;
    // };

    // template <size_t _Ip, class _Tp, size_t _Size>
    // class tuple_element<_Ip, const array<_Tp, _Size> > {
    // public:
    //     typedef const _Tp type;
    // };

    // template <size_t _Ip, class _Tp, size_t _Size>
    // inline _Tp& get(array<_Tp, _Size>& __a)  {
    //     return __a[_Ip];
    // }

    // template <size_t _Ip, class _Tp, size_t _Size>
    // inline const _Tp& get(const array<_Tp, _Size>& __a)  {
    //     return __a[_Ip];
    // }
};
