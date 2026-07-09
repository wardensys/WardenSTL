// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s numeric utilities,
// and has been extended with additional functionality for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_NUMERIC_HPP__
#define __WSTL_NUMERIC_HPP__

#include "private/Platform.hpp"
#include "Iterator.hpp"
#include "Math.hpp"


/// @defgroup numeric Numeric
/// @brief Numeric algorithms
/// @ingroup wstl

namespace wstl {
    // Iota

    /// @brief Fills a range with sequentially increasing values
    /// @param first Iterator to the initial position in the range
    /// @param last Iterator to the final position in the range
    /// @param value The value to start with
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/algorithm/iota
    template<typename ForwardIterator, typename T>
    __WSTL_CONSTEXPR14__ void Iota(ForwardIterator first, ForwardIterator last, T value) {
        for (; first != last; ++first, ++value) *first = value;
    }

    // Accumulate
    
    /// @brief Computes the sum of the given value and values in a range
    /// @param first Iterator to the initial position in the range
    /// @param last Iterator to the final position in the range
    /// @param initial Initial value to start with
    /// @return The sum of the values in the range
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/algorithm/accumulate
    template<typename InputIterator, typename T>
    __WSTL_CONSTEXPR14__ T Accumulate(InputIterator first, InputIterator last, T initial) {
        for(; first != last; ++first) initial = __WSTL_MOVE__(initial) + *first;
        return initial;
    }

    /// @brief Computes the result of applying a binary operation to the given value and values in a range
    /// @param first Iterator to the initial position in the range
    /// @param last Iterator to the final position in the range
    /// @param initial Initial value to start with
    /// @param operation Binary operation function object that will be applied
    /// @return The result of applying the binary operation to the values in the range
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/algorithm/accumulate
    template<typename InputIterator, typename T, typename BinaryOperation>
    __WSTL_CONSTEXPR14__ T Accumulate(InputIterator first, InputIterator last, T initial, BinaryOperation operation) {
        for(; first != last; ++first) initial = operation(__WSTL_MOVE__(initial), *first);
        return initial;
    }

    // Inner product

    /// @brief Computes the inner product (sum of products) of two ranges
    /// @param first1 Iterator to the initial position in the first range
    /// @param last1 Iterator to the final position in the first range
    /// @param first2 Iterator to the initial position in the second range
    /// @param initial Initial value to start with
    /// @return The inner product of the two ranges
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/algorithm/inner_product
    template<typename InputIterator1, typename InputIterator2, typename T>
    __WSTL_CONSTEXPR14__ T InnerProduct(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T initial) {
        for(; first1 != last1; ++first1, ++first2) initial = __WSTL_MOVE__(initial) + (*first1 * *first2);
        return initial;
    }

    /// @brief Computes the inner product (sum of products) of two ranges using binary operations
    /// @param first1 Iterator to the initial position in the first range
    /// @param last1 Iterator to the final position in the first range
    /// @param first2 Iterator to the initial position in the second range
    /// @param initial Initial value to start with
    /// @param operation1 Binary operation function object that will be applied to the result
    /// @param operation2 Binary operation function object that will be applied to the range elements
    /// @return The inner product of the two ranges
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/algorithm/inner_product
    template<typename InputIterator1, typename InputIterator2, typename T, typename BinaryOperation1, typename BinaryOperation2>
    __WSTL_CONSTEXPR14__ T InnerProduct(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T initial, 
    BinaryOperation1 operation1, BinaryOperation2 operation2) {
        for(; first1 != last1; ++first1, ++first2) initial = operation1(__WSTL_MOVE__(initial), operation2(*first1, *first2));
        return initial;
    }

    // Adjacent difference

    /// @brief Computes the difference between adjacent elements in a range
    /// @param first Iterator to the initial position in the range
    /// @param last Iterator to the final position in the range
    /// @param resultFirst Iterator to the initial position in the output range
    /// @return Iterator to the end of the output range
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/algorithm/adjacent_difference
    template<typename InputIterator, typename OutputIterator>
    __WSTL_CONSTEXPR14__ OutputIterator AdjacentDifference(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        if(first == last) return resultFirst;

        typedef typename IteratorTraits<InputIterator>::ValueType ValueType;
        ValueType accumulator = *first;
        *resultFirst++ = *first++;

        for(; first != last; ++first, ++resultFirst) {
            ValueType current = *first;
            *resultFirst = current - __WSTL_MOVE__(accumulator);
            accumulator = __WSTL_MOVE__(current);
        }

        return resultFirst;
    }

    /// @brief Computes the difference between adjacent elements in a range
    /// @param first Iterator to the initial position in the range
    /// @param last Iterator to the final position in the range
    /// @param resultFirst Iterator to the initial position in the output range
    /// @param operation Binary operation function object that will be applied
    /// @return Iterator to the end of the output range
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/algorithm/adjacent_difference
    template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
    __WSTL_CONSTEXPR14__ OutputIterator AdjacentDifference(InputIterator first, InputIterator last, OutputIterator resultFirst, 
    BinaryOperation operation) {
        if(first == last) return resultFirst;

        typedef typename IteratorTraits<InputIterator>::ValueType Value;
        Value accumulator = *first;
        *resultFirst++ = *first++;

        for(; first != last; ++first, ++resultFirst) {
            Value current = *first;
            *resultFirst = operation(current, __WSTL_MOVE__(accumulator));
            accumulator = __WSTL_MOVE__(current);
        }

        return resultFirst;
    }

    // Partial sum

    /// @brief Computes the partial sum of a range
    /// @param first Iterator to the initial position in the range
    /// @param last Iterator to the final position in the range
    /// @param resultFirst Iterator to the initial position in the output range
    /// @return Iterator to the end of the output range
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/algorithm/partial_sum
    template<typename InputIterator, typename OutputIterator>
    __WSTL_CONSTEXPR14__ OutputIterator PartialSum(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        if(first == last) return resultFirst;

        typename IteratorTraits<InputIterator>::ValueType sum = *first;
        *resultFirst++ = *first++;

        for(; first != last; ++first, ++resultFirst) {
            sum = __WSTL_MOVE__(sum) + *first;
            *resultFirst = sum;
        }

        return resultFirst;
    }

    /// @brief Computes the partial sum of a range
    /// @param first Iterator to the initial position in the range
    /// @param last Iterator to the final position in the range
    /// @param resultFirst Iterator to the initial position in the output range
    /// @param operation Binary operation function object that will be applied
    /// @return Iterator to the end of the output range
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/algorithm/partial_sum
    template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
    __WSTL_CONSTEXPR14__ OutputIterator PartialSum(InputIterator first, InputIterator last, OutputIterator resultFirst,
    BinaryOperation operation) {
        if(first == last) return resultFirst;

        typename IteratorTraits<InputIterator>::ValueType sum = *first;
        *resultFirst++ = *first++;

        for(; first != last; ++first, ++resultFirst) {
            sum = operation(__WSTL_MOVE__(sum), *first);
            *resultFirst = sum;
        }

        return resultFirst;
    }

    // Greatest common divisor

    /// @brief Computes the greatest common divisor of two numbers
    /// @param a First number
    /// @param b Second number
    /// @return The greatest common divisor of the two numbers
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/gcd
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    typename EnableIf<IsUnsigned<T>::Value, T>::Type GCD(T a, T b) __WSTL_NOEXCEPT__ {
        WSTL_STATIC_ASSERT(IsIntegral<T>::Value, "Integral type required!");

        if((a == 0) || (b == 0)) return (a + b);

        while(b != 0) {
            T temp = b;
            b = a % b;
            a = temp;
        }

        return a;
    }

    /// @brief Computes the greatest common divisor of two numbers
    /// @param a First number
    /// @param b Second number
    /// @return The greatest common divisor of the two numbers
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/gcd
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    typename EnableIf<IsSigned<T>::Value, T>::Type GCD(T a, T b) __WSTL_NOEXCEPT__ {
        WSTL_STATIC_ASSERT(IsIntegral<T>::Value, "Integral type required!");

        typedef typename MakeUnsigned<T>::Type U;
        U ua = AbsoluteUnsigned(a);
        U ub = AbsoluteUnsigned(b);

        return static_cast<T>(GCD(ua, ub));
    }

    #ifdef __WSTL_CXX11__
    #ifndef __WSTL_NO_INITIALIZERLIST__
    /// @brief Computes the greatest common divisor of a list of numbers
    /// @param first First number
    /// @param rest Remaining numbers
    /// @return The greatest common divisor of the numbers
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/gcd
    template<typename T, typename... Ts>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    T GCD(T first, Ts... rest) __WSTL_NOEXCEPT__ {
        T result = first;

        for(T value : {rest...}) {
            result = GCD(result, value);

            if(result == 1) return 1;
        }

        return result;
    }
    #else
    /// @brief Computes the greatest common divisor of a list of numbers
    /// @param a First number
    /// @param b Second number
    /// @param rest Remaining numbers
    /// @return The greatest common divisor of the numbers
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/gcd
    template<typename T, typename... Ts>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    T GCD(T a, T b, Ts... rest) __WSTL_NOEXCEPT__ {
        T result = GCD(a, b);
        return (result == 1) ? 1 : GCD(result, rest...);
    }
    #endif
    #endif

    // Lowest common multiple

    /// @brief Computes the lowest common multiple of two numbers
    /// @param a First number
    /// @param b Second number
    /// @return The lowest common multiple of the two numbers
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/lcm
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    typename EnableIf<IsUnsigned<T>::Value, T>::Type LCM(T a, T b) __WSTL_NOEXCEPT__ {
        WSTL_STATIC_ASSERT(IsIntegral<T>::Value, "Integral type required!");

        if((a == 0) || (b == 0)) return 0;
        else return a * (b / GCD(a, b));
    }

    /// @brief Computes the lowest common multiple of two numbers
    /// @param a First number
    /// @param b Second number
    /// @return The lowest common multiple of the two numbers
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/lcm
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
    typename EnableIf<IsSigned<T>::Value, T>::Type LCM(T a, T b) __WSTL_NOEXCEPT__ {
        WSTL_STATIC_ASSERT(IsIntegral<T>::Value, "Integral type required!");

        typedef typename MakeUnsigned<T>::Type U;
        U ua = AbsoluteUnsigned(a);
        U ub = AbsoluteUnsigned(b);

        return static_cast<T>(LCM(ua, ub));
    }

    #ifdef __WSTL_CXX11__
    #ifndef __WSTL_NO_INITIALIZERLIST__
    /// @brief Computes the lowest common multiple of a list of numbers
    /// @param first First number
    /// @param rest Remaining numbers
    /// @return The lowest common multiple of the numbers
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/lcm
    template<typename T, typename... Ts>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    T LCM(T first, Ts... rest) __WSTL_NOEXCEPT__ {
        T result = first;

        for(T value : {rest...}) {
            result = LCM(result, value);

            if(result == 0) return 0;
        }

        return result;
    }
    #else
    /// @brief Computes the lowest common multiple of a list of numbers
    /// @param first First number
    /// @param second Second number
    /// @param rest Remaining numbers
    /// @return The lowest common multiple of the numbers
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/lcm
    template<typename T, typename... Ts>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    T LCM(T a, T b, Ts... rest) __WSTL_NOEXCEPT__ {
        T result = LCM(a, b);

        if(result == 0) return 0;
        else return LCM(result, rest...);
    }
    #endif
    #endif

    // Midpoint

    /// @brief Computes the midpoint of two floating-point numbers, handling overflow correctly
    /// @param a First number
    /// @param b Second number
    /// @return The midpoint of the two numbers
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/midpoint
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    typename EnableIf<IsFloatingPoint<T>::Value, T>::Type Midpoint(T a, T b) __WSTL_NOEXCEPT__ {
        T low = NumericLimits<T>::Min() * 2;
        T high = NumericLimits<T>::Max() / 2;

        return (Absolute(a) <= high) && (Absolute(b) <= high) ? // typical case
            (a + b) / 2 :                                       // always correctly rounded
            Absolute(a) < low ? a + (b / 2) :                   // not safe to halve b
            Absolute(b) < low ? (a / 2) + b :                   // not safe to have a
            a / 2 + b / 2;                                      // otherwise correctly rounded
    }

    /// @brief Computes the midpoint of two integral numbers (signed or unsigned), handling overflow correctly
    /// @param a First number
    /// @param b Second number
    /// @return The midpoint of the two numbers, biased towards first number if the distance is odd
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/midpoint
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    typename EnableIf<IsIntegral<T>::Value && !IsNullPointer<T>::Value, T>::Type Midpoint(T a, T b) __WSTL_NOEXCEPT__ {
        typedef typename MakeUnsigned<T>::Type U;

        if(a > b) return a - T(U(U(a) - U(b)) >> 1);
        else return a + T((U(b) - U(a)) >> 1);
    }
    
    /// @brief Computes the midpoint of two pointers, handling overflow correctly
    /// @param a Pointer to the first element
    /// @param b Pointer to the second element
    /// @return Pointer to the midpoint of the two pointers, biased towards first pointer if the distance is odd
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/midpoint
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    typename EnableIf<IsSame<typename IteratorTraits<T>::IteratorCategory, RandomAccessIteratorTag>::Value, T>::Type Midpoint(T a, T b) __WSTL_NOEXCEPT__ {
        return a + Midpoint(ptrdiff_t(0), b - a);
    }

    /// @brief Computes the midpoint of two iterators, handling overflow correctly
    /// @param a First iterator
    /// @param b Second iterator
    /// @return The midpoint of the two iterators, biased towards first iterator if the distance is odd
    /// @note Only bidirectional and forward iterators are supported, and first iterator must be before or equal to the second iterator
    /// @ingroup numeric
    /// @see https://en.cppreference.com/w/cpp/numeric/midpoint
    template<typename T>
    __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__
    typename EnableIf<IsSame<typename IteratorTraits<T>::IteratorCategory, ForwardIteratorTag>::Value || 
    IsSame<typename IteratorTraits<T>::IteratorCategory, BidirectionalIteratorTag>::Value, T>::Type Midpoint(T a, T b) __WSTL_NOEXCEPT__ {
        return Next(a, Distance(a, b) / 2);
    }

    // Compile-time equivalents

    // GCD

    namespace __private {
        template<typename T, T A, T B, bool = (B == 0), typename = typename EnableIf<IsIntegral<T>::Value>::Type>
        struct __GCD;

        template<typename T, T A, T B>
        struct __GCD<T, A, B, true, void> : compile::Absolute<T, A> {};

        template<typename T, T A, T B>
        struct __GCD<T, A, B, false, void> : __GCD<T, B, A % B> {};
    }

    namespace compile {
        #ifdef __WSTL_CXX11__
        /// @brief Computes the greatest common divisor of a list of numbers at compile time
        /// @tparam T Type of the input values
        /// @tparam A First number
        /// @tparam B Second number
        /// @tparam ...Rest Remaining numbers
        /// @ingroup numeric
        template<typename T, T A, T B, T... Rest>
        struct GCD;

        template<typename T, T A, T B>
        struct GCD<T, A, B> : __private::__GCD<T, A, B> {};

        template<typename T, T A, T B, T... Rest>
        struct GCD : GCD<T, GCD<T, A, B>::Value, Rest...> {};
        #else
        /// @brief Computes the greatest common divisor of two of numbers at compile time
        /// @tparam T Type of the input values
        /// @tparam A First number
        /// @tparam B Second number
        /// @ingroup numeric
        template<typename T, T A, T B>
        struct GCD : __private::__GCD<T, A, B> {};
        #endif

        #ifdef __WSTL_CXX17__
        /// @copydoc GCD
        /// @since C++17
        template<typename T, T A, T B, T... Rest>
        inline constexpr T GCDValue = GCD<T, A, B, Rest...>::Value;
        #endif
    }

    // LCM

    namespace __private {
        template<typename T, T A, T B, bool = (A == 0 || B == 0), typename = typename EnableIf<IsIntegral<T>::Value>::Type>
        struct __LCM;

        template<typename T, T A, T B>
        struct __LCM<T, A, B, true, void> : IntegralConstant<T, 0> {};

        template<typename T, T A, T B>
        struct __LCM<T, A, B, false, void> : IntegralConstant<
            T, 
            compile::Absolute<T, B>::Value * (compile::Absolute<T, A>::Value / __GCD<T, A, B>::Value)
        > {};
    }

    namespace compile {
        #ifdef __WSTL_CXX11__
        /// @brief Computes the least common multiple of a list of numbers at compile time
        /// @tparam T Type of the input values
        /// @tparam A First number
        /// @tparam B Second number
        /// @tparam ...Rest Remaining numbers
        /// @ingroup numeric
        template<typename T, T A, T B, T... Rest>
        struct LCM;
        
        template<typename T, T A, T B>
        struct LCM<T, A, B> : __private::__LCM<T, A, B> {};

        template<typename T, T A, T B, T... Rest>
        struct LCM : LCM<T, LCM<T, A, B>::Value, Rest...> {};
        #else
        /// @brief Computes the least common multiple of two of numbers at compile time 
        /// @tparam T Type of the input values
        /// @tparam A First number
        /// @tparam B Second number
        template<typename T, T A, T B>
        struct LCM : __private::__LCM<T, A, B> {};
        #endif

        #ifdef __WSTL_CXX17__
        /// @copydoc LCM
        /// @since C++17
        template<typename T, T A, T B, T... Rest>
        inline constexpr T LCMValue = LCM<T, A, B, Rest...>::Value;
        #endif
    }

    namespace __private {
        template<typename T, T A, T B, bool = (A > B), typename = typename EnableIf<IsIntegral<T>::Value>::Type>
        struct __Midpoint;

        template<typename T, T A, T B>
        struct __Midpoint<T, A, B, true, void> : IntegralConstant<
            T, 
            A - T(typename MakeUnsigned<T>::Type(typename MakeUnsigned<T>::Type(A) - typename MakeUnsigned<T>::Type(B)) / 2)
        > {};

        template<typename T, T A, T B>
        struct __Midpoint<T, A, B, false, void> : IntegralConstant<
            T, 
            A + T((typename MakeUnsigned<T>::Type(B) - typename MakeUnsigned<T>::Type(A)) / 2)
        > {};
    }

    namespace compile {
        /// @brief Computes the midpoint of two integral numbers at compile time, handling overflow correctly
        /// @tparam T Type of the input values
        /// @tparam A First number
        /// @tparam B Second number
        /// @ingroup numeric
        template<typename T, T A, T B>
        struct Midpoint : __private::__Midpoint<T, A, B> {};

        #ifdef __WSTL_CXX17__
        /// @copydoc Midpoint
        /// @since C++17
        template<typename T, T A, T B>
        inline constexpr T MidpointValue = Midpoint<T, A, B>::Value;
        #endif
    }
}

#endif