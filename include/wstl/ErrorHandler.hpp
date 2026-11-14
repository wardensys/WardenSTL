// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s error_handler.h,
// but it has been re-implemented with custom logic for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_ERRORHANDLER_HPP__
#define __WSTL_ERRORHANDLER_HPP__

#include "private/Platform.hpp"
#include "Exception.hpp"
#include "Functional.hpp"


/// @defgroup error_handler Error Handler
/// @brief Error handler for when throwing exceptions is not required
/// @ingroup wstl

// Defines introduced

/// @def __WSTL_HANDLE_ERRORS__
/// @brief If defined, the error handler functionality will be enabled with `ErrorHandler` class
/// @ingroup error_handler
#ifdef __DOXYGEN__
    #define __WSTL_HANDLE_ERRORS__ 
#endif

#ifdef __WSTL_HANDLE_ERRORS__
namespace wstl {
    /// @brief Error handler class
    /// @note Requires `__WSTL_HANDLE_ERRORS__` to be defined
    /// @ingroup error_handler
    class ErrorHandler {
    public:
        /// @brief Sets function that will be called when an exception occurs
        /// @param function Function interface class that takes `const Exception&` as a parameter
        /// @warning The function must be valid for the lifetime of the program, as it will be stored as a pointer
        static void SetCallback(const FunctionBase<void(const Exception&)>& function) {
            GetCallback() = &function;
        }

        /// @brief Clears the callback function
        static void ClearCallback() {
            GetCallback() = NullPointer;
        }

        /// @brief Calls the error handler function
        /// @param exception Exception that occurred
        static void Error(const Exception& exception) {
            const FunctionBase<void(const Exception&)>* function = GetCallback();
            if(function != NullPointer) (*function)(exception);
        }
    
    private:
        /// @brief Gets the pointer to the callback function
        static const FunctionBase<void(const Exception&)>*& GetCallback() {
            static const FunctionBase<void(const Exception&)>* function = NullPointer;
            return function;
        }
    };

    namespace __private {
        inline void __ErrorHandlerError(const Exception& exception) {
            ErrorHandler::Error(exception);
        }
    }
}
#endif
#endif
