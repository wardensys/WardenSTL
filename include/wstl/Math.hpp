// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s math utilities,
// and several custom functions have been added for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_MATH_HPP__
#define __WSTL_MATH_HPP__

#include "private/Platform.hpp"
#include "TypeTraits.hpp"
#include "Limits.hpp"
#include "ErrorHandler.hpp"
#include "StandardExceptions.hpp"

#include <stdint.h>
#include <float.h>

#ifndef __USE_C99_MATH
#define __USE_C99_MATH
#endif

#ifdef __WSTL_MATH_SUPPORT__
#include <math.h>
#include <stdlib.h>
#endif


// Defines introduced

/// @def __WSTL_MATH_SUPPORT__
/// @brief If defined, the library will use math.h and stdlib.h functions where necessary
/// @ingroup maths
#ifdef __DOXYGEN__
    #define __WSTL_MATH_SUPPORT__ 
#endif

namespace wstl {
    // Absolute

    namespace __private {
        /// @brief Non-constexpr function that is never called with valid input, but 
        /// if reached during constant evaluation, will cause a compile-time error. At runtime, it triggers the library's error handler
        template<typename T>
        inline T __SignedMinError() {
            __WSTL_THROW_RETURNVALUE__(WSTL_MAKE_EXCEPTION(OverflowError, "Absolute value of minimum signed integer is undefined"), T(0));
        }
    }

    /// @brief Computes an absolute value of integral type
    /// @param value The value to compute the absolute value of
    /// @return The absolute value of the input value
    /// @ingroup maths
    /// @see https://en.cppreference.com/w/c/numeric/math/abs
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
    inline typename EnableIf<IsSigned<T>::Value && IsIntegral<T>::Value, T>::Type Absolute(T value) {
        return (value == NumericLimits<T>::Min()) ? __private::__SignedMinError<T>() : static_cast<T>((value < T(0)) ? -value : value);
    }

    /// @brief Computes an absolute value of floating-point type
    /// @param value The value to compute the absolute value of
    /// @return The absolute value of the input value
    /// @ingroup maths
    /// @see https://en.cppreference.com/w/c/numeric/math/abs
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
    inline typename EnableIf<IsSigned<T>::Value && !IsIntegral<T>::Value, T>::Type Absolute(T value) __WSTL_NOEXCEPT__ {
        return (value < T(0)) ? -value : value;
    }

    /// @brief Computes an absolute value of integral type
    /// @param value The value to compute the absolute value of
    /// @return The absolute value of the input value
    /// @ingroup maths
    /// @see https://en.cppreference.com/w/c/numeric/math/abs
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
    inline typename EnableIf<IsUnsigned<T>::Value, T>::Type Absolute(T value) __WSTL_NOEXCEPT__ {
        return value;
    }

    // Absolute unsigned

    /// @brief Computes an absolute value of integral type and returns the result as unsigned type
    /// @param value The value to compute the absolute value of
    /// @return The absolute value of the input value as unsigned type
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsSigned<T>::Value, typename MakeUnsigned<T>::Type>::Type AbsoluteUnsigned(T value) __WSTL_NOEXCEPT__ {
        typedef typename MakeUnsigned<T>::Type Return;

        return (value == NumericLimits<T>::Min()) ? (NumericLimits<Return>::Max() / 2U) + 1U : 
            ((value < T(0)) ? Return(-value) : Return(value));
    }

    /// @brief Computes an absolute value of integral type and returns the result as unsigned type
    /// @param value The value to compute the absolute value of
    /// @return The absolute value of the input value as unsigned type
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<!IsSigned<T>::Value, T>::Type AbsoluteUnsigned(T value) __WSTL_NOEXCEPT__ {
        return value;
    }

    // Compile-time equivalent

    namespace __private {
        template<typename T, T N, bool = IsSigned<T>::Value, typename = typename EnableIf<IsIntegral<T>::Value>::Type>
        struct __Absolute;

        template<typename T, T N>
        struct __Absolute<T, N, false, void> : IntegralConstant<T, N> {};

        template<typename T, T N>
        struct __Absolute<T, N, true, void> : IntegralConstant<T, ((N < T(0)) ? -N : N)> {
            WSTL_STATIC_ASSERT(N != NumericLimits<T>::Min(), "Absolute value of minimum signed integer is undefined");
        };

        template<typename T, T N, bool = IsSigned<T>::Value, typename = typename EnableIf<IsIntegral<T>::Value>::Type>
        struct __AbsoluteUnsigned;

        template<typename T, T N>
        struct __AbsoluteUnsigned<T, N, true, void> {
            typedef typename MakeUnsigned<T>::Type Return;

            typedef IntegralConstant<Return, (N == NumericLimits<T>::Min()) ? 
                (NumericLimits<Return>::Max() / 2U) + 1U : ((N < T(0)) ? (Return(0) - Return(N)) : Return(N))> Type;
        };

        template<typename T, T N>
        struct __AbsoluteUnsigned<T, N, false, void> {
            typedef IntegralConstant<T, N> Type;
        };
    }

    namespace compile {
        /// @brief Computes an absolute value of integral type at compile time
        /// @tparam T Type of the input value
        /// @tparam N Value to compute the absolute value of
        /// @ingroup maths
        template<typename T, T N>
        struct Absolute : __private::__Absolute<T, N> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Absolute
        /// @since C++17
        template<typename T, T N>
        inline constexpr T AbsoluteValue = Absolute<T, N>::Value;
        #endif

        /// @brief Computes an absolute value of integral type at compile time and holds the result as unsigned type
        /// @tparam T Type of the input value
        /// @tparam N Value to compute the absolute value of
        /// @ingroup maths
        template<typename T, T N>
        struct AbsoluteUnsigned : __private::__AbsoluteUnsigned<T, N>::Type {};
        
        #ifdef __WSTL_CXX17__
        /// @copydoc AbsoluteUnsigned
        /// @since C++17
        template<typename T, T N>
        inline constexpr T AbsoluteUnsignedValue = AbsoluteUnsigned<T, N>::Value;
        #endif
    }
    
    // Divide

    /// @brief Result type of `Divide` function
    /// @ingroup maths
    /// @see https://en.cppreference.com/w/c/numeric/math/div
    template<typename T>
    struct DivisionType {
        T Quotient;
        T Remainder;
    };

    /// @brief Computes quotient and remainder of integral division
    /// @param x Divident
    /// @param y Divisor
    /// @return A `DivisionType` object that holds quotient and remainder
    /// @ingroup maths
    /// @see https://en.cppreference.com/w/c/numeric/math/div
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsIntegral<T>::Value, DivisionType<T> >::Type Divide(T x, T y) {
        return { x / y, x % y };
    }

    // Power

    namespace compile {
        /// @brief Raises a number of integral type to the given power
        /// @tparam T Type of the input value
        /// @tparam N Number to raise
        /// @tparam POWER Power to raise to
        /// @tparam R Type to use for storing the result (default is unsigned version of `T`)
        /// @ingroup maths
        template<typename T, T N, size_t POWER, typename R = typename MakeUnsigned<T>::Type>
        struct Power : IntegralConstant<R, N * Power<T, N, POWER - 1, R>::Value> {};

        /// @brief Raises a number of integral type to the given power - specialization for exponent 0
        /// @tparam T Type of the input value
        /// @tparam N Number to raise
        /// @tparam R Type to use for storing the result (default is unsigned version of `T`)
        /// @ingroup maths
        template<typename T, T N, typename R>
        struct Power<T, N, 0, R> : IntegralConstant<R, (N == 0) ? 0 : 1> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Power
        /// @ingroup maths
        template<typename T, T N, size_t POWER, typename R = typename MakeUnsigned<T>::Type>
        inline constexpr R PowerValue = Power<T, N, POWER, R>::Value;
        #endif
    }

    // Log

    namespace __private {
        template<typename T, T N, size_t BASE, bool = (N <= 1 || N < T(BASE))>
        struct __Log;

        template<typename T, T N, size_t BASE>
        struct __Log<T, N, BASE, false> : IntegralConstant<T, 1 + __Log<T, N / BASE, BASE>::Value> {};

        template<typename T, T N, size_t BASE>
        struct __Log<T, N, BASE, true> : IntegralConstant<T, 0> {};
    }

    namespace compile {
        /// @brief Computes floor logarithm of a number of integral type
        /// @tparam T Type of the input value
        /// @tparam N Number to compute the logarithm of
        /// @tparam BASE Base of the logarithm
        /// @ingroup maths
        template<typename T, T N, size_t BASE>
        struct Log : __private::__Log<T, N, BASE> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Log
        /// @ingroup maths
        template<typename T, T N, size_t BASE>
        inline constexpr T LogValue = Log<T, N, BASE>::Value;
        #endif

        /// @brief Computes ceil logarithm of a number of integral type
        /// @tparam T Type of the input value
        /// @tparam N Number to compute the logarithm of
        /// @tparam BASE Base of the logarithm
        /// @ingroup maths
        template<typename T, T N, size_t BASE>
        struct LogCeil : IntegralConstant<T, ((N > 1) ? Log<T, N - 1, BASE>::Value + 1 : 0)> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc LogCeil
        /// @ingroup maths
        template<typename T, T N, size_t BASE>
        inline constexpr T LogCeilValue = LogCeil<T, N, BASE>::Value;
        #endif

        /// @brief Computes floor logarithm of a number of integral type with base 2
        /// @tparam T Type of the input value
        /// @tparam N Number to compute the logarithm of
        /// @ingroup maths
        template<typename T, T N>
        struct Log2 : Log<T, N, 2> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Log2
        /// @ingroup maths
        template<typename T, T N>
        inline constexpr T Log2Value = Log2<T, N>::Value;
        #endif

        /// @brief Computes ceil logarithm of a number of integral type with base 2
        /// @tparam T Type of the input value
        /// @tparam N Number to compute the logarithm of
        /// @ingroup maths
        template<typename T, T N>
        struct Log2Ceil : LogCeil<T, N, 2> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Log2Ceil
        /// @ingroup maths
        template<typename T, T N>
        inline constexpr T Log2CeilValue = Log2Ceil<T, N>::Value;
        #endif

        /// @brief Computes floor logarithm of a number of integral type with base 10
        /// @tparam T Type of the input value
        /// @tparam N Number to compute the logarithm of
        /// @ingroup maths
        template<typename T, T N>
        struct Log10 : Log<T, N, 10> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Log10
        /// @ingroup maths
        template<typename T, T N>
        inline constexpr T Log10Value = Log10<N>::Value;
        #endif

        /// @brief Computes ceil logarithm of a number of integral type with base 10
        /// @tparam T Type of the input value
        /// @tparam N Number to compute the logarithm of
        /// @ingroup maths
        template<typename T, T N>
        struct Log10Ceil : LogCeil<T, N, 10> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Log10Ceil
        /// @ingroup maths
        template<typename T, T N>
        inline constexpr T Log10CeilValue = Log10Ceil<N>::Value;
        #endif
    }

    // Nth root

    namespace __private {
        // Forward declaration of the main search struct

        template<typename T, T N, size_t ROOT, 
            typename U = typename MakeUnsigned<T>::Type, 
            U ABS_N = compile::AbsoluteUnsigned<T, N>::Value, 
            U LOW = 1, 
            U HIGH = 1 << (compile::Log2Ceil<U, ABS_N>::Value + 3) / ROOT, // optimization
            bool = (LOW > HIGH)
        >
        struct __NthRootSearch;

        // Recursive Branch (executed when LOW < HIGH)

        template<typename T, T N, size_t ROOT, typename U, U ABS_N, U LOW, U HIGH>
        struct __NthRootSearch<T, N, ROOT, U, ABS_N, LOW, HIGH, false> {
        private:
            static const __WSTL_CONSTEXPR__ U Mid = LOW + (HIGH - LOW) / 2;
            static const __WSTL_CONSTEXPR__ U MidPower = compile::Power<U, Mid, ROOT>::Value;

        public:
            static const __WSTL_CONSTEXPR__ U Value = wstl::Conditional<(MidPower == ABS_N), 
                IntegralConstant<U, Mid>, typename wstl::Conditional<
                    (MidPower > ABS_N),
                    __NthRootSearch<T, N, ROOT, U, ABS_N, LOW, Mid - 1>,
                    __NthRootSearch<T, N, ROOT, U, ABS_N, Mid + 1, HIGH> 
                >::Type>::Type::Value;
        };

        template<typename T, T N, size_t ROOT, typename U, U ABS_N, U LOW, U HIGH>
        const __WSTL_CONSTEXPR__ U __NthRootSearch<T, N, ROOT, U, ABS_N, LOW, HIGH, false>::Value;

        // Recursive Branch (executed when LOW < HIGH)

        template<typename T, T N, size_t ROOT, typename U, U ABS_N, U LOW, U HIGH>
        struct __NthRootSearch<T, N, ROOT, U, ABS_N, LOW, HIGH, true> {
            static const __WSTL_CONSTEXPR__ U Value = HIGH;
        };

        template<typename T, T N, size_t ROOT, typename U, U ABS_N, U LOW, U HIGH>
        const __WSTL_CONSTEXPR__ U __NthRootSearch<T, N, ROOT, U, ABS_N, LOW, HIGH, true>::Value;

        template<typename T, T N, size_t ROOT, bool = (N == 0), bool = (N == 1)>
        struct __NthRoot;
        
        template<typename T, T N, size_t ROOT>
        struct __NthRoot<T, N, ROOT, false, false> {
        private:
            static const __WSTL_CONSTEXPR__ T SearchValue = static_cast<T>(__NthRootSearch<T, N, ROOT>::Value);

        public:
            static const T Value = (N < 0) ? -SearchValue : SearchValue;
        };

        template<typename T, T N, size_t ROOT>
        const __WSTL_CONSTEXPR__ T __NthRoot<T, N, ROOT, false, false>::Value;

        template<typename T, T N, size_t ROOT>
        struct __NthRoot<T, N, ROOT, true, false> {
            static const T Value = 0;
        };

        template<typename T, T N, size_t ROOT>
        const __WSTL_CONSTEXPR__ T __NthRoot<T, N, ROOT, true, false>::Value;

        template<typename T, T N, size_t ROOT>
        struct __NthRoot<T, N, ROOT, false, true> {
            static const T Value = 1;
        };

        template<typename T, T N, size_t ROOT>
        const __WSTL_CONSTEXPR__ T __NthRoot<T, N, ROOT, false, true>::Value;
    }

    namespace compile {
        /// @brief Computes nth root of a number of integral type
        /// @tparam T Type of the input value
        /// @tparam N Number to compute the root of
        /// @tparam ROOT Root to compute
        /// @note For even roots, `N` must be non-negative. `ROOT` must be at least 2.
        /// @ingroup maths
        template<typename T, T N, size_t ROOT, typename = typename EnableIf<(ROOT >= 2) && !(ROOT % 2 == 0 && N < 0)>::Type>
        struct NthRoot : IntegralConstant<T, __private::__NthRoot<T, N, ROOT>::Value> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc NthRoot
        /// @ingroup maths
        template<typename T, T N, size_t ROOT>
        inline constexpr T NthRootValue = NthRoot<T, N, ROOT>::Value;
        #endif
    }

    // Square root

    namespace compile {
        /// @brief Computes square root of a number of integral type
        /// @tparam T Type of the input value
        /// @tparam N Number to compute the square root of
        /// @ingroup maths
        template<typename T, T N>
        struct SquareRoot : NthRoot<T, N, 2> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc SquareRoot
        /// @ingroup maths
        template<typename T, T N>
        inline constexpr T SquareRootValue = SquareRoot<T, N>::Value;
        #endif
    }

    // Factorial

    namespace __private {
        template<typename T, T N, bool = (N == 0)>
        struct __Factorial;

        template<typename T, T N>
        struct __Factorial<T, N, false> : IntegralConstant<T, N * __Factorial<T, N - 1>::Value> {};

        template<typename T, T N>
        struct __Factorial<T, N, true> : IntegralConstant<T, 1> {};
    }

    namespace compile {
        /// @brief Computes factorial of a number of integral type
        /// @tparam T Type of the input value
        /// @tparam N Number to compute the factorial of
        /// @ingroup maths
        template<typename T, T N>
        struct Factorial : __private::__Factorial<T, N> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Factorial
        /// @ingroup maths
        template<typename T, T N>
        inline constexpr T FactorialValue = Factorial<N>::Value;
        #endif
    }

    // Fibonacci

    namespace __private {
        template<typename T, T N, bool = (N <= 1)>
        struct __Fibonacci;

        template<typename T, T N>
        struct __Fibonacci<T, N, false> : IntegralConstant<T, __Fibonacci<T, N - 1>::Value + __Fibonacci<T, N - 2>::Value> {};

        template<typename T, T N>
        struct __Fibonacci<T, N, true> : IntegralConstant<T, N> {};
    }

    namespace compile {
        /// @brief Computes Fibonacci sequence for nth number
        /// @tparam T Type of the input value
        /// @tparam N Nth number in the Fibonacci sequence
        /// @ingroup maths
        template<typename T, T N>
        struct Fibonacci : __private::__Fibonacci<T, N> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Fibonacci
        /// @ingroup maths
        template<typename T, T N>
        inline constexpr T FibonacciValue = Fibonacci<T, N>::Value;
        #endif
    }

    // Permutations

    namespace __private {
        template<typename T, T N, T R, bool = (R == 0)>
        struct __Permutations;

        template<typename T, T N, T R>
        struct __Permutations<T, N, R, false> : IntegralConstant<T, N * __Permutations<T, N - 1, R - 1>::Value> {};

        template<typename T, T N, T R>
        struct __Permutations<T, N, R, true> : IntegralConstant<T, 1> {};
    }

    namespace compile {
        /// @brief Computes permutations of `N` taken `R` at a time
        /// @tparam T Type of the input values
        /// @tparam N Total number of elements
        /// @tparam R Number of elements selected
        /// @note `R` must be less than or equal to `N`, and both `N` and `R` must be non-negative.
        /// @ingroup maths
        template<typename T, T N, T R, typename = typename EnableIf<(R <= N) && (R >= 0 && N >= 0)>::Type>
        struct Permutations : __private::__Permutations<T, N, R> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Permutations
        /// @ingroup maths
        template<typename T, T N, T R>
        inline constexpr T PermutationsValue = Permutations<T, N, R>::Value;
        #endif
    }

    // Combinations

    namespace compile {
        /// @brief Computes combinations of `N` taken `R` at a time
        /// @tparam T Type of the input values
        /// @tparam N Total number of elements
        /// @tparam R Number of elements selected
        /// @note `R` must be less than or equal to `N`, and both `N` and `R` must be non-negative.
        /// @ingroup maths
        template<typename T, T N, T R, typename = typename EnableIf<(R <= N) && (R >= 0 && N >= 0)>::Type>
        struct Combinations : IntegralConstant<T, Permutations<T, N, R>::Value / Factorial<T, R>::Value> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Combinations
        /// @ingroup maths
        template<typename T, T N, T R>
        inline constexpr T CombinationsValue = Combinations<T, N, R>::Value;
        #endif
    }

    // Divide floor

    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
    inline typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value, T>::Type DivideFloor(T a, T b) {
        return a / b;
    }

    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
    inline typename EnableIf<IsIntegral<T>::Value && IsSigned<T>::Value, T>::Type DivideFloor(T a, T b) {
        return (a / b) - ((a % b != 0) && (a < 0) != (b < 0));
    }

    // Divide ceil

    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
    inline typename EnableIf<IsIntegral<T>::Value && IsUnsigned<T>::Value, T>::Type DivideCeil(T a, T b) {
        return (a + b - 1) / b;
    }

    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
    inline typename EnableIf<IsIntegral<T>::Value && IsSigned<T>::Value, T>::Type DivideCeil(T a, T b) {
        return (a / b) + ((a % b != 0) && (a > 0) == (b > 0));
    }

    // Is NaN

    #if defined(__WSTL_CXX11__) && defined(__WSTL_MATH_SUPPORT__)
    /// @brief Checks if a number is Not a Number
    /// @param value Value to check
    /// @return `true` if the value is NaN, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsNaN(T value) {
        return fpclassify(value) == FP_NAN;
    }
    #else
    /// @brief Checks if a number is Not a Number
    /// @param value Value to check
    /// @return `true` if the value is NaN, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsNaN(T value) {
        return value != value;
    }
    #endif

    /// @brief Checks if a number is Not a Number
    /// @param value Value to check
    /// @return `true` if the value is NaN, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<!IsFloatingPoint<T>::Value, bool>::Type IsNaN(T) {
        return false;
    }

    // Is infinity

    #if defined(__WSTL_CXX11__) && defined(__WSTL_MATH_SUPPORT__)
    /// @brief Checks whether a floating point value is infinity
    /// @param value Value to check
    /// @return `true` if the value is infinity, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsInfinity(T value) {
        return fpclassify(value) == FP_INFINITE;
    }
    #else
    /// @brief Checks whether a floating point value is infinity
    /// @param value Value to check
    /// @return `true` if the value is infinity, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsInfinity(T value) {
        return (value == NumericLimits<T>::Infinity()) || (value == -NumericLimits<T>::Infinity());
    }
    #endif

    /// @brief Checks whether a floating point value is infinity
    /// @param value Value to check
    /// @return `true` if the value is infinity, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<!IsFloatingPoint<T>::Value, bool>::Type IsInfinity(T) {
        return false;
    }

    // Is zero

    #if defined(__WSTL_CXX11__) && defined(__WSTL_MATH_SUPPORT__)
    /// @brief Checks whether a floating point value is zero
    /// @param value Value to check
    /// @return `true` if the value is zero, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsZero(T value) {
        return fpclassify(value) == FP_ZERO;
    }
    #else
    /// @brief Checks whether a floating point value is zero
    /// @param value Value to check
    /// @return `true` if the value is zero, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<IsFloatingPoint<T>::Value, bool>::Type IsZero(T value) {
        return value == T(0);
    }
    #endif

    /// @brief Checks whether a floating point value is zero
    /// @param value Value to check
    /// @return `true` if the value is zero, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline typename EnableIf<!IsFloatingPoint<T>::Value, bool>::Type IsZero(T value) {
        return value == T(0);
    }

    // Is exactly equal

    /// @brief Checks whether two values are exactly equal
    /// @param x First value
    /// @param y Second value
    /// @return `true` if the values are exactly equal, `false` otherwise
    /// @ingroup maths
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR__
    inline bool IsExactlyEqual(T x, T y) {
        return x == y;
    }

    // Math constants

    namespace __private {
        template<typename T = void>
        struct __MathConstants {
            static const __WSTL_CONSTEXPR__ double PI = 3.14159265358979;
            static const __WSTL_CONSTEXPR__ double PI_RECIRPOCAL = 0.31830988618379;
            static const __WSTL_CONSTEXPR__ double PI_SQUARED = 9.86960440108936;
            static const __WSTL_CONSTEXPR__ double E = 2.71828182845905;
            static const __WSTL_CONSTEXPR__ double E_RECIPROCAL = 0.36787944117144;
            static const __WSTL_CONSTEXPR__ double E_SQUARED = 7.38905609893065;
            static const __WSTL_CONSTEXPR__ double ROOT2 = 1.41421356237310;
            static const __WSTL_CONSTEXPR__ double ROOT2_RECIPROCAL = 0.70710678118655;
            static const __WSTL_CONSTEXPR__ double EULER = 0.57721566490153;
            static const __WSTL_CONSTEXPR__ double GOLDEN_RATIO = 1.61803398874989;
        };

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::PI;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::PI_RECIRPOCAL;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::PI_SQUARED;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::E;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::E_RECIPROCAL;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::E_SQUARED;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::ROOT2;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::ROOT2_RECIPROCAL;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::EULER;

        template<typename T>
        const __WSTL_CONSTEXPR__ double __MathConstants<T>::GOLDEN_RATIO;
    }

    /// @brief Class that defines various mathematical constants
    /// @ingroup maths
    struct MathConstants : __private::__MathConstants<> {};
}   

#endif