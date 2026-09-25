// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s static assert utilities,
// with modifications made for improved support under C++98.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_STATICASSERT_HPP__
#define __WSTL_STATICASSERT_HPP__

#include "private/Platform.hpp"
#include "private/ns/private.hpp"


#ifdef __WSTL_CXX11__
/// @brief A macro to trigger a static assertion based on a boolean condition
/// @param condition The boolean condition to evaluate
/// @param ... Optional message to display on failure (ignored in C++98)
/// @note Do not put `false` as the condition, use `WSTL_STATIC_ASSERT_FALSE` instead for 
/// template contexts because of C++98 limitations
/// @ingroup utilities
#define WSTL_STATIC_ASSERT static_assert

/// @brief A macro to trigger a static assertion failure based on a template parameter
/// @param templateParam The template parameter to evaluate (should be a type, ignored in C++11 and higher)
/// @param ... Optional message to display on failure (ignored in C++98)
/// @ingroup utilities
#define WSTL_STATIC_ASSERT_FALSE(templateParam, ...) static_assert(false, __VA_ARGS__)
#else
namespace wstl {
    namespace __private {
        template<bool Condition>
        struct __StaticAssert;

        template<>
        struct __StaticAssert<true> {};

        template<typename T>
        struct __StaticAssertFalse {
            enum { Value = false };
        };
    }
}

#define __WSTL_STATIC_ASSERT_IMPL__(condition, line) \
    enum { WSTL_CONCATENATE(__StaticAssertion, line) = sizeof(wstl::__private::__StaticAssert<static_cast<bool>((condition))>) }

/// @brief A macro to trigger a static assertion based on a boolean condition
/// @param condition The boolean condition to evaluate
/// @param ... Optional message to display on failure (ignored in C++98)
/// @note Do not put `false` as the condition, use `WSTL_STATIC_ASSERT_FALSE` instead for 
/// template contexts because of C++98 limitations
/// @ingroup utilities
#define WSTL_STATIC_ASSERT(condition, ...) __WSTL_STATIC_ASSERT_IMPL__(condition, __LINE__)

/// @brief A macro to trigger a static assertion failure based on a template parameter
/// @param templateParam The template parameter to evaluate (should be a type, ignored in C++11 and higher)
/// @param ... Optional message to display on failure (ignored in C++98)
/// @ingroup utilities
#define WSTL_STATIC_ASSERT_FALSE(templateParam, ...) \
    __WSTL_STATIC_ASSERT_IMPL__(wstl::__private::__StaticAssertFalse<templateParam>::Value, __LINE__)

#endif
#endif
