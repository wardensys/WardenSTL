// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_PLACEMENTNEW_HPP__
#define __WSTL_PLACEMENTNEW_HPP__

#include "private/Platform.hpp"
#include <stddef.h>

#ifndef __WSTL_NO_PLACEMENTNEW__
    #ifdef __has_include
        #if __has_include(<new>)
            #define __WSTL_NO_PLACEMENTNEW__
        #endif
    #endif
#endif

#ifndef __WSTL_NO_PLACEMENTNEW__
inline void* operator new(size_t, void* p) __WSTL_NOEXCEPT__ { return p; }
inline void* operator new[](size_t, void* p) __WSTL_NOEXCEPT__ { return p; }

inline void operator delete(void*, void*) __WSTL_NOEXCEPT__ {}
inline void operator delete[](void*, void*) __WSTL_NOEXCEPT__ {}
#else
#include <new>
#endif

#endif