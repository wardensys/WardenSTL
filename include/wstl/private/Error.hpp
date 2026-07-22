// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s error handling utilities,
// and some concepts and functions have been adapted for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_ERROR_HPP__
#define __WSTL_ERROR_HPP__

#include "Platform.hpp"
#include "../Exception.hpp"

#ifdef __WSTL_ASSERT_SUPPORT__
#include <assert.h>
#endif

// Defines introduced

/// @def __WSTL_ASSERT_SUPPORT__
/// @brief If defined, enables assert from `assert.h` in error handling macros
/// @details However, the library will prioritize usage of `ErrorHandler` functions if they are enabled
/// @ingroup error_handler
#ifdef __DOXYGEN__
    #define __WSTL_ASSERT_SUPPORT__ 
#endif

/// @def __WSTL_CHECK_PUSHPOP__
/// @brief If defined, push/pop operations in containers will be checked, and in case of error the exception will be thrown
/// @ingroup error_handler
#ifdef __DOXYGEN__
    #define __WSTL_CHECK_PUSHPOP__ 
#endif



#ifdef __WSTL_HANDLE_ERRORS__
namespace wstl {
    namespace __private {
        static void __ErrorHandlerError(const Exception&);
    }
}
#endif

#ifdef __WSTL_EXCEPTIONS__
    #ifdef __WSTL_HANDLE_ERRORS__
        #define __WSTL_ASSERT__(condition, exception) do {                  \
            if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ {    \
                wstl::__private::__ErrorHandlerError((exception));          \
                throw((exception));                                         \
            }                                                               \
        } while(false)

        #define __WSTL_ASSERT_RETURN__(condition, exception) do {           \
            if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ {    \
                wstl::__private::__ErrorHandlerError((exception));          \
                throw((exception));                                         \
                return;                                                     \
            }                                                               \
        } while(false)

        #define __WSTL_ASSERT_RETURNVALUE__(condition, exception, value) do {   \
            if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ {        \
                wstl::__private::__ErrorHandlerError((exception));              \
                throw((exception));                                             \
                return(value);                                                  \
            }                                                                   \
        } while(false)

        #define __WSTL_THROW__(exception) do {                  \
            wstl::__private::__ErrorHandlerError((exception));  \
            throw((exception));                                 \
        } while(false)

        #define __WSTL_THROW_RETURN__(exception) do {           \
            wstl::__private::__ErrorHandlerError((exception));  \
            throw((exception));                                 \
            return;                                             \
        } while(falses)

        #define __WSTL_THROW_RETURNVALUE__(exception, value) do {   \
            wstl::__private::__ErrorHandlerError((exception));      \
            throw((exception));                                     \
            return (value);                                         \
        } while(false)
    #else
        #define __WSTL_ASSERT__(condition, exception) do {                                  \
            if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ throw((exception));  \
        } while(false)

        #define __WSTL_ASSERT_RETURN__(condition, exception) do {           \
            if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ {    \
                throw((exception));                                         \
            }                                                               \
        } while(false)

        #define __WSTL_ASSERT_RETURNVALUE__(condition, exception, value) do {   \
            if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ {        \
                throw((exception));                                             \
            }                                                                   \
        } while(false)

        #define __WSTL_THROW__(exception) do {  \
            throw((exception));                 \
        }

        #define __WSTL_THROW_RETURN__(exception) do {   \
            throw((exception));                         \
        } while(false)

        #define __WSTL_THROW_RETURNVALUE__(exception, value) do {   \
            throw((exception));                                     \
        } while(false)
    #endif
#else
    #ifdef __WSTL_HANDLE_ERRORS__
        #define __WSTL_ASSERT__(condition, exception) do {              \
            if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__  \
                wstl::__private::__ErrorHandlerError((exception));      \
        } while(false)

        #define __WSTL_ASSERT_RETURN__(condition, exception) do {           \
            if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ {    \
                wstl::__private::__ErrorHandlerError((exception));          \
                return;                                                     \
            }                                                               \
        }

        #define __WSTL_ASSERT_RETURNVALUE__(condition, exception, value) do {   \
            if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ {        \
                wstl::__private::__ErrorHandlerError((exception));              \
                return (value);                                                 \
            }                                                                   \
        } while(false)

        #define __WSTL_THROW__(exception) do {                  \
            wstl::__private::__ErrorHandlerError((exception));  \
        } while(false)

        #define __WSTL_THROW_RETURN__(exception) do {           \
            wstl::__private::__ErrorHandlerError((exception));  \
            return;                                             \
        } while(false)

        #define __WSTL_THROW_RETURNVALUE__(exception, value) do {   \
            wstl::__private::__ErrorHandlerError((exception));      \
            return (value);                                         \
        } while(false)
    #else
        #ifdef __WSTL_ASSERT_SUPPORT__
            #define __WSTL_ASSERT__(condition, exception) assert((condition))

            #define __WSTL_ASSERT_RETURN__(condition, exception) do {           \
                if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ {    \
                    assert(false);                                              \
                    return;                                                     \
                }                                                               \
            } while(false)

            #define __WSTL_ASSERT_RETURNVALUE__(condition, exception, value) {  \
                if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ {    \
                    assert(false);                                              \
                    return (value);                                             \
                }                                                               \
            } while(false)

            #define __WSTL_THROW__(exception) do {  \
                assert(false);                      \
            } while(false)

            #define __WSTL_THROW_RETURN__(exception) do {   \
                assert(false);                              \
                return;                                     \
            } while(false)

            #define __WSTL_THROW_RETURNVALUE__(exception, value) do {   \
                assert(false);                                          \
                return (value);                                         \
            } while(false)
        #else
            #define __WSTL_ASSERT__(condition, exception)

            #define __WSTL_ASSERT_RETURN__(condition, exception) do {               \
                if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ return;  \
            } while(false)

            #define __WSTL_ASSERT_RETURNVALUE__(condition, exception, value) do {           \
                if(__WSTL_UNLIKELY_EXPR__(!(condition))) __WSTL_UNLIKELY__ return (value);  \
            } while(false)

            #define __WSTL_THROW__(exception)

            #define __WSTL_THROW_RETURN__(exception) do {   \
                return;                                     \
            } while(false)

            #define __WSTL_THROW_RETURNVALUE__(exception, value) do {   \
                return (value);                                         \
            } while(false)
        #endif
        
    #endif
#endif

// Additional defines

#ifdef __WSTL_CHECK_PUSHPOP__
    #define __WSTL_ASSERT_PUSHPOP_RETURN__(condition, exception) __WSTL_ASSERT_RETURN__(condition, exception)
#else
    #define __WSTL_ASSERT_PUSHPOP_RETURN__(condition, exception)
#endif

#ifdef __WSTL_CHECK_ITERATOR__
    #define __WSTL_ASSERT_ITERATOR_RETURNVALUE__(condition, exception, value) __WSTL_ASSERT_RETURNVALUE__(condition, exception, value)
#else
    #define __WSTL_ASSERT_ITERATOR_RETURNVALUE__(condition, exception, value)
#endif

#ifdef __WSTL_CHECK_INDEXOPERATOR__
    #define __WSTL_ASSERT_INDEXOPERATOR__(condition, exception) __WSTL_ASSERT__(condition, exception)
#else
    #define __WSTL_ASSERT_INDEXOPERATOR__(condition, exception)
#endif

#ifdef __WSTL_CHECK_EXTRA__
    #define __WSTL_ASSERT_EXTRA__(condition, exception) __WSTL_ASSERT__(condition, exception)
#else
    #define __WSTL_ASSERT_EXTRA__(condition, exception)
#endif


#endif