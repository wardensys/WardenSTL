// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_STANDARDEXCEPTIONS_HPP__
#define __WSTL_STANDARDEXCEPTIONS_HPP__

#include "private/Platform.hpp"
#include "Exception.hpp"


/// @defgroup standard_exceptions Standard Exceptions
/// @brief A set of standard exceptions that can be thrown by the library
/// @ingroup exception


namespace wstl {
    /// @brief Exception thrown when a logic error occurs
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/logic_error
    class LogicError : public Exception {
    public:
        #ifdef __WSTL_EXCEPTION_LOCATION__
        /// @brief Constructor
        /// @param file File where the exception occurred
        /// @param line Line number where the exception occurred
        /// @param message Message describing the exception, default is `Logic error`
        __WSTL_CONSTEXPR20__ LogicError(StringType file, NumericType line, StringType message = "Logic error") __WSTL_NOEXCEPT__ : Exception(file, line, message) {}
        #else
        /// @brief Constructor
        /// @param message Message describing the exception, default is `Logic error`
        __WSTL_CONSTEXPR20__ LogicError(StringType message = "Logic error") __WSTL_NOEXCEPT__ : Exception(message) {}
        #endif

        /// @copydoc Exception::Name()
        __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
            return "LogicError";
        }
    };

    /// @brief Exception thrown when exceeding the length of a container
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/length_error
    class LengthError : public LogicError {
    public:
        #ifdef __WSTL_EXCEPTION_LOCATION__
        /// @brief Constructor
        /// @param file File where the exception occurred
        /// @param line Line number where the exception occurred
        /// @param message Message describing the exception, default is `Length error`
        __WSTL_CONSTEXPR20__ LengthError(StringType file, NumericType line, StringType message = "Length error") __WSTL_NOEXCEPT__ : LogicError(file, line, message) {}
        #else
        /// @brief Constructor with message
        /// @param message Message describing the exception, default is `Length error`
        __WSTL_CONSTEXPR20__ LengthError(StringType message = "Length error") __WSTL_NOEXCEPT__ : LogicError(message) {}
        #endif

        /// @copydoc Exception::Name()
        __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
            return "LengthError";
        }
    };

    /// @brief Exception thrown when accessed element is out of range
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/out_of_range
    class OutOfRange __WSTL_FINAL__ : public LogicError {
    public:
        #ifdef __WSTL_EXCEPTION_LOCATION__
        /// @brief Constructor
        /// @param file File where the exception occurred
        /// @param line Line number where the exception occurred
        /// @param message Message describing the exception, default is `Out of range`
        __WSTL_CONSTEXPR20__ OutOfRange(StringType file, NumericType line, StringType message = "Out of range") __WSTL_NOEXCEPT__ : LogicError(file, line, message) {}
        #else
        /// @brief Constructor
        /// @param message Message describing the exception, default is `Out of range`
        __WSTL_CONSTEXPR20__ OutOfRange(StringType message = "Out of range") __WSTL_NOEXCEPT__ : LogicError(message) {}
        #endif

        /// @copydoc Exception::Name()
        __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
            return "OutOfRange";
        }
    };

    /// @brief Exception thrown when a bad cast is attempted
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/out_of_range
    class BadCast : public Exception {
    public:
        #ifdef __WSTL_EXCEPTION_LOCATION__
        /// @brief Constructor
        /// @param file File where the exception occurred
        /// @param line Line number where the exception occurred
        /// @param message Message describing the exception, default is `Bad cast`
        __WSTL_CONSTEXPR20__ BadCast(StringType file, NumericType line, StringType message = "Bad cast") __WSTL_NOEXCEPT__ : Exception(file, line, message) {}
        #else
        /// @brief Constructor with a message
        /// @param message Message describing the exception, default is `Bad cast`
        __WSTL_CONSTEXPR20__ BadCast(StringType message = "Bad cast") __WSTL_NOEXCEPT__ : Exception(message) {}
        #endif

        /// @copydoc Exception::Name()
        __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
            return "BadCast";
        }
    };

    /// @brief Exception thrown when a runtime error occurs
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/runtime_error
    class RuntimeError : public Exception {
    public:
        #ifdef __WSTL_EXCEPTION_LOCATION__
        /// @brief Constructor
        /// @param file File where the exception occurred
        /// @param line Line number where the exception occurred
        /// @param message Message describing the exception, default is `Runtime error`
        __WSTL_CONSTEXPR20__ RuntimeError(StringType file, NumericType line, StringType message = "Runtime error") __WSTL_NOEXCEPT__ : Exception(file, line, message) {}
        #else
        /// @brief Constructor with a message
        /// @param message Message describing the exception, default is `Runtime error`
        __WSTL_CONSTEXPR20__ RuntimeError(StringType message = "Runtime error") __WSTL_NOEXCEPT__ : Exception(message) {}
        #endif

        /// @copydoc Exception::Name()
        __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
            return "RuntimeError";
        }
    };

    /// @brief Exception thrown when an arithmetic overflow occurs
    /// @ingroup standard_exceptions
    /// @see https://en.cppreference.com/w/cpp/error/overflow_error
    class OverflowError : public RuntimeError {
    public:
        #ifdef __WSTL_EXCEPTION_LOCATION__
        /// @brief Constructor
        /// @param file File where the exception occurred
        /// @param line Line number where the exception occurred
        /// @param message Message describing the exception, default is `Runtime error`
        __WSTL_CONSTEXPR20__ OverflowError(StringType file, NumericType line, StringType message = "Overflow error") __WSTL_NOEXCEPT__ : RuntimeError(file, line, message) {}
        #else
        /// @brief Constructor with a message
        /// @param message Message describing the exception, default is `Runtime error`
        __WSTL_CONSTEXPR20__ OverflowError(StringType message = "Overflow error") __WSTL_NOEXCEPT__ : RuntimeError(message) {}
        #endif

        /// @copydoc Exception::Name()
        __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
            return "OverflowError";
        }
    };
}

#endif