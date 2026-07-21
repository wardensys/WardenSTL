// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_TYPETRAITS_HPP__
#define __WSTL_TYPETRAITS_HPP__

#include "private/Platform.hpp"
#include "NullPointer.hpp"
#include "StaticAssert.hpp"
#include <stddef.h>
#include <stdint.h>


/// @defgroup type_traits Type Traits
/// @brief A set of type trait definitions
/// @ingroup utilities

// Defines introduced

/// @def __WSTL_EXCEPTIONS__
/// @brief If defined, enables exception-related type traits and functionalities
/// @ingroup type_traits
#ifdef __DOXYGEN__
    #define __WSTL_EXCEPTIONS__ 
#endif

/// @def __WSTL_TYPETRAITS_NO_BUILTINS__
/// @brief If defined, disables the use of compiler intrinsics for type traits, 
/// forcing the use of heuristic implementations, some traits may always evaluate to `false`
/// @ingroup type_traits
#ifdef __DOXYGEN__
    #define __WSTL_TYPETRAITS_NO_BUILTINS__ 
#endif

namespace wstl {
    #ifdef __WSTL_CXX11__
    /// @brief Type alias for STL naming support
    template<typename...>
    using void_t = void;

    template<typename...>
    using VoidType = void;
    #endif

    // Integral constant
    
    /// @brief Wrapper for static constant
    /// @tparam T Type of value
    /// @tparam V Value to define
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/integral_constant
    template <typename T, T V>
    struct IntegralConstant {
        static const __WSTL_CONSTEXPR__ T Value = V;

        typedef T ValueType;
        typedef IntegralConstant<T, V> Type; 

        __WSTL_CONSTEXPR__ operator ValueType() const __WSTL_NOEXCEPT__ {
            return Value;
        }

        __WSTL_CONSTEXPR__ ValueType operator()() const __WSTL_NOEXCEPT__ {
            return Value;
        }
    };

    template<typename T, T V>
    const __WSTL_CONSTEXPR__ T IntegralConstant<T, V>::Value;

    #ifdef __WSTL_CXX17__
    /// @copydoc IntegralConstant
    /// @since C++17
    template<typename T, T V>
    inline constexpr T IntegralConstantValue = IntegralConstant<T, V>::Value;
    #endif

    // Bool constant
    
    #ifdef __WSTL_CXX11__
    /// @brief Helper alias template for integral constant with boolean type
    /// @tparam V Boolean value
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/integral_constant
    template<bool V>
    using BoolConstant = IntegralConstant<bool, V>;
    #else
    /// @brief Helper alias template for integral constant with boolean type
    /// @tparam V Boolean value
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/integral_constant
    template<bool V>
    struct BoolConstant : IntegralConstant<bool, V> {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc BoolConstant
    /// @since C++17
    template<bool V>
    inline constexpr bool BoolConstantValue = BoolConstant<V>::Value;
    #endif

    // Integral constant definitions

    /// @brief Static constant for false
    typedef BoolConstant<false> FalseType;
    /// @brief Static constant for true
    typedef BoolConstant<true> TrueType;

    // Remove reference
    
    /// @brief Removes reference from specified type
    /// @tparam T Type to remove reference from
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_reference
    template<typename T>
    struct RemoveReference;

    template<typename T>
    struct RemoveReference { typedef T Type; };

    template<typename T>
    struct RemoveReference<T&> { typedef T Type; };

    #ifdef __WSTL_CXX11__
    template<typename T>
    struct RemoveReference<T&&> { typedef T Type; };
    #endif

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveReference
    /// @since C++11
    template<typename T>
    using RemoveReferenceType = typename RemoveReference<T>::Type;
    #endif

    // Typedef test macro

    #define __WSTL_TYPEDEF_PARAMS__(...) WSTL_CONCATENATE(__WSTL_TYPEDEF, WSTL_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

    #define __WSTL_TYPEDEF1(A) \
        typename RemoveReference<typename T::A>::Type*

    #define __WSTL_TYPEDEF2(A, B) \
        typename RemoveReference<typename T::A>::Type*, \
        typename RemoveReference<typename T::B>::Type*

    #define __WSTL_TYPEDEF3(A, B, C) \
        typename RemoveReference<typename T::A>::Type*, \
        typename RemoveReference<typename T::B>::Type*, \
        typename RemoveReference<typename T::C>::Type*

    #define __WSTL_TYPEDEF4(A, B, C, D) \
        typename RemoveReference<typename T::A>::Type*, \
        typename RemoveReference<typename T::B>::Type*, \
        typename RemoveReference<typename T::C>::Type*, \
        typename RemoveReference<typename T::D>::Type*

    #define __WSTL_TYPEDEF5(A, B, C, D, E) \
        typename RemoveReference<typename T::A>::Type*, \
        typename RemoveReference<typename T::B>::Type*, \
        typename RemoveReference<typename T::C>::Type*, \
        typename RemoveReference<typename T::D>::Type*, \
        typename RemoveReference<typename T::E>::Type*

    #define __WSTL_NULLPTR_LIST__(N) WSTL_CONCATENATE(__WSTL_ZEROS, N)

    #define __WSTL_ZEROS0
    #define __WSTL_ZEROS1 0
    #define __WSTL_ZEROS2 0, 0
    #define __WSTL_ZEROS3 0, 0, 0
    #define __WSTL_ZEROS4 0, 0, 0, 0
    #define __WSTL_ZEROS5 0, 0, 0, 0, 0

    /// @brief A macro to declare a typedef test function with a specified name
    /// @details The test function will return `long` if all specified typedefs are present in the type `T`, 
    /// and `char` otherwise. This allows for compile-time checks of typedef presence
    /// @param function The name of the test function
    /// @param ... The typedefs to test, up to 5
    /// @ingroup utilities
    #define WSTL_DECLARE_TYPEDEF_TEST(function, ...) \
        template<typename T> \
        static long function(__WSTL_TYPEDEF_PARAMS__(__VA_ARGS__)); \
        \
        template<typename> \
        static char function(...);

    /// @brief A macro for the result of a typedef test function
    /// @param function The name of the test function
    /// @param type The type to test for present typedefs
    /// @param count The number of typedefs to test for, maximum of 5
    /// @ingroup utilities
    #define WSTL_TYPEDEF_TEST_RESULT(function, type, count) \
        sizeof(function<type>(__WSTL_NULLPTR_LIST__(count))) == sizeof(long)

    // Remove pointer

    /// @brief Removes pointer from specified type
    /// @tparam T Type to remove pointer from
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_pointer
    template<typename T>
    struct RemovePointer;

    template<typename T>
    struct RemovePointer { typedef T Type; };

    template<typename T>
    struct RemovePointer<T*> { typedef T Type; };

    template<typename T>
    struct RemovePointer<T* const> { typedef T Type; };

    template<typename T>
    struct RemovePointer<T* volatile> { typedef T Type; };

    template<typename T>
    struct RemovePointer<T* const volatile> { typedef T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemovePointer
    /// @since C++11
    template<typename T>
    using RemovePointerType = typename RemovePointer<T>::Type;
    #endif

    // Remove const

    /// @brief Removes constant from specified type
    /// @tparam T Type to remove constant from
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_cv
    template<typename T>
    struct RemoveConst;

    template<typename T>
    struct RemoveConst { typedef T Type; };

    template<typename T>
    struct RemoveConst<const T> { typedef T Type; };

    template<typename T>
    struct RemoveConst<volatile T> { typedef volatile T Type; };

    template<typename T>
    struct RemoveConst<const volatile T> { typedef volatile T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveConst
    /// @since C++11
    template<typename T>
    using RemoveConstType = typename RemoveConst<T>::Type;
    #endif

    // Remove volatile

    /// @brief Removes volatile specifier from specified type
    /// @tparam T Type to remove volatile from
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_cv
    template<typename T>
    struct RemoveVolatile;

    template<typename T>
    struct RemoveVolatile { typedef T Type; };

    template<typename T>
    struct RemoveVolatile<volatile T> { typedef T Type; };

    template<typename T>
    struct RemoveVolatile<const T> { typedef const T Type; };

    template<typename T>
    struct RemoveVolatile<const volatile T> { typedef const T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveVolatile
    /// @since C++11
    template<typename T>
    using RemoveVolatileType = typename RemoveVolatile<T>::Type;
    #endif

    // Remove const and volatile

    /// @brief Removes const and volatile specifiers from specified type
    /// @tparam T Type to remove const and volatile from
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_cv
    template<typename T>
    struct RemoveCV;

    template<typename T>
    struct RemoveCV { typedef T Type; };

    template<typename T>
    struct RemoveCV<const T> { typedef T Type; };

    template<typename T>
    struct RemoveCV<volatile T> { typedef T Type; };

    template<typename T>
    struct RemoveCV<const volatile T> { typedef T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveCV
    /// @since C++11
    template<typename T>
    using RemoveCVType = typename RemoveCV<T>::Type;
    #endif

    // Remove extent

    /// @brief Removes one dimension of array
    /// @tparam T Array type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_extent
    template<typename T>
    struct RemoveExtent;

    template<typename T>
    struct RemoveExtent { typedef T Type; };

    template<typename T>
    struct RemoveExtent<T[]> { typedef T Type; };

    template<typename T, size_t S>
    struct RemoveExtent<T[S]> { typedef T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveExtent
    /// @since C++11
    template<typename T>
    using RemoveExtentType = typename RemoveExtent<T>::Type;
    #endif

    // Remove all extents

    /// @brief Removes all dimensions in array
    /// @tparam T Array type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_all_extents
    template<typename T>
    struct RemoveAllExtents;

    template<typename T>
    struct RemoveAllExtents { typedef T Type; };

    template<typename T>
    struct RemoveAllExtents<T[]> { typedef typename RemoveAllExtents<T>::Type Type; };

    template<typename T, size_t S>
    struct RemoveAllExtents<T[S]> { typedef typename RemoveAllExtents<T>::Type Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveAllExtents
    /// @since C++11
    template<typename T>
    using RemoveAllExtentsType = typename RemoveAllExtents<T>::Type;
    #endif

    // Add pointer

    /// @brief Converts type into pointer
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_pointer
    template<typename T>
    struct AddPointer;

    template<typename T>
    struct AddPointer { typedef typename RemoveReference<T>::Type* Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc AddPointer
    /// @since C++11
    template<typename T>
    using AddPointerType = typename AddPointer<T>::Type;
    #endif

    // Add lvalue reference

    /// @brief Creates lvalue reference of specified type
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_reference
    template<typename T>
    struct AddLValueReference;

    template<typename T>
    struct AddLValueReference { typedef T& Type; };

    template<typename T>
    struct AddLValueReference<T&> { typedef T& Type; };

    template<>
    struct AddLValueReference<void> { typedef void Type; };

    template<>
    struct AddLValueReference<const void> { typedef const void Type; };

    template<>
    struct AddLValueReference<volatile void> { typedef volatile void Type; };

    template<>
    struct AddLValueReference<const volatile void> { typedef const volatile void Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc AddLValueReference
    /// @since C++11
    template<typename T>
    using AddLValueReferenceType = typename AddLValueReference<T>::Type;
    #endif

    // Add rvalue reference

    #ifdef __WSTL_CXX11__
    /// @brief Creates rvalue reference of specified type
    /// @tparam T Type to convert
    /// @since C++11
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_reference
    template<typename T>
    struct AddRValueReference;

    template<typename T>
    struct AddRValueReference { using Type = T&&; };

    template<typename T>
    struct AddRValueReference<T&> { using Type = T&; };

    template<>
    struct AddRValueReference<void> { using Type = void; };

    template<>
    struct AddRValueReference<const void> { using Type = const void; };

    template<>
    struct AddRValueReference<volatile void> { using Type = volatile void; };

    template<>
    struct AddRValueReference<const volatile void> { using Type = const volatile void; };
    
    /// @copydoc AddRValueReference
    /// @since C++11
    template<typename T>
    using AddRValueReferenceType = typename AddRValueReference<T>::Type;
    #endif

    // Add const

    /// @brief Adds const specifier to type
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_cv
    template<typename T>
    struct AddConst;

    template<typename T>
    struct AddConst { typedef const T Type; };  

    template<typename T>
    struct AddConst<const T> { typedef const T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc AddConst
    /// @since C++11
    template<typename T>
    using AddConstType = typename AddConst<T>::Type;
    #endif

    // Add volatile

    /// @brief Adds volatile specifier to type
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_cv
    template<typename T>
    struct AddVolatile;

    template<typename T>
    struct AddVolatile { typedef volatile T Type; };

    template<typename T>
    struct AddVolatile<volatile T> { typedef volatile T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc AddVolatile
    /// @since C++11
    template<typename T>
    using AddVolatileType = typename AddVolatile<T>::Type;
    #endif

    // Add const volatile

    /// @brief Adds const and volatile specifiers to type
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/add_cv
    template<typename T>
    struct AddCV;

    template<typename T>
    struct AddCV { typedef typename AddVolatile<typename AddConst<T>::Type>::Type Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc AddCV
    /// @since C++11
    template<typename T>
    using AddCVType = typename AddCV<T>::Type;
    #endif

    // Remove const volatile reference

    /// @brief Removes const volatile specifiers and reference from type
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/remove_cvref
    template<typename T>
    struct RemoveCVReference { typedef typename RemoveCV<typename RemoveReference<T>::Type>::Type Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc RemoveCVReference
    /// @since C++11
    template<typename T>
    using RemoveCVReferenceType = typename RemoveCVReference<T>::Type;
    #endif

    // Alignment of

    /// @brief Gets alignment requirements of type
    /// @tparam T Type to get alignment of
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/alignment_of
    template<typename T>
    struct AlignmentOf;

    #ifdef __WSTL_CXX11__
    template<typename T>
    struct AlignmentOf : IntegralConstant<size_t, alignof(T)> {};
    #elif defined(__WSTL_MSVC__)
    template<typename T>
    struct AlignmentOf : IntegralConstant<size_t, size_t(__alingof(T))> {};
    #else
    template<typename T>
    struct AlignmentOf : IntegralConstant<size_t, size_t(__alingof__(T))> {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc AlignmentOf
    /// @since C++17
    template<typename T>
    inline constexpr size_t AlignmentOfValue = AlignmentOf<T>::Value;
    #endif
    
    // Rank

    /// @brief Provides rank (depth) of an array type
    /// @tparam T Type to get rank of
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/rank
    template<typename T>
    struct Rank;

    template<typename T>
    struct Rank : IntegralConstant<size_t, 0> {};

    template<typename T>
    struct Rank<T[]> : IntegralConstant<size_t, Rank<T>::Value + 1> {};

    template<typename T, size_t S>
    struct Rank<T[S]> : IntegralConstant<size_t, Rank<T>::Value + 1> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc Rank
    /// @since C++17
    template<typename T>
    inline constexpr size_t RankValue = Rank<T>::Value;
    #endif

    // Extent

    /// @brief Gets size of dimension of array
    /// @tparam T Array type
    /// @tparam N Array dimemsion
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/extent
    template<typename T, unsigned N = 0>
    struct Extent;

    template<typename T, unsigned N>
    struct Extent : IntegralConstant<size_t, 0> {};

    template<typename T>
    struct Extent<T[], 0> : IntegralConstant<size_t, 0> {};

    template<typename T, unsigned N>
    struct Extent<T[], N> : IntegralConstant<size_t, Extent<T, N - 1>::Value> {};

    template<typename T, unsigned I>
    struct Extent<T[I], 0> : IntegralConstant<size_t, I> {};

    template<typename T, unsigned I, unsigned N>
    struct Extent<T[I], N> : IntegralConstant<size_t, Extent<T, N - 1>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc Extent
    /// @since C++17
    template<typename T, unsigned N = 0>
    inline constexpr size_t ExtentValue = Extent<T, N>::Value;
    #endif

    // Declare value

    #ifdef __WSTL_CXX11__
    template<typename T>
    /// @brief Declares a value of specified type
    /// @tparam T Type of value
    /// @ingroup type_traits
    /// @since C++11
    AddRValueReferenceType<T> DeclareValue() __WSTL_NOEXCEPT__;
    #endif

    // Enable if
    
    /// @brief Defines a type if condition is satisfied
    /// @tparam T Type to define
    /// @tparam B Condition to satisfy
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/enable_if
    template<bool B, typename T>
    struct EnableIf;

    template<bool B, typename T = void>
    struct EnableIf {};

    template<typename T>
    struct EnableIf<true, T> { typedef T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc EnableIf
    /// @since C++11
    template<bool B, typename T = void>
    using EnableIfType = typename EnableIf<B, T>::Type;
    #endif

    // Conditional

    /// @brief Defines type based on condition
    /// @tparam T Defined type if condition is true
    /// @tparam F Defined type if condition is false
    /// @tparam B Boolean condition
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/conditional
    template<bool B, typename T, typename F>
    struct Conditional;

    template<bool B, typename T, typename F>
    struct Conditional { typedef T Type; };

    template<typename T, typename F>
    struct Conditional<false, T, F> { typedef F Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc Conditional
    /// @since C++11
    template<bool B, typename T, typename F>
    using ConditionalType = typename Conditional<B, T, F>::Type;
    #endif

    // Result of

    namespace __private {
        WSTL_DECLARE_TYPEDEF_TEST(__TestResultType, ResultType)

        template<typename Functor, bool IsFunctor = WSTL_TYPEDEF_TEST_RESULT(__TestResultType, Functor, 1)>
        struct __ResultOfFunctor;

        template<typename Functor>
        struct __ResultOfFunctor<Functor, true> { typedef typename Functor::ResultType Type; };

        template<typename Functor>
        struct __ResultOf : __private::__ResultOfFunctor<Functor> {};

        template<typename Return, typename Arg1, typename Arg2, typename Arg3>
        struct __ResultOf<Return(Arg1, Arg2, Arg3)> { typedef Return Type; };

        template<typename Return, typename Arg1, typename Arg2>
        struct __ResultOf<Return(Arg1, Arg2)> { typedef Return Type; };

        template<typename Return, typename Arg>
        struct __ResultOf<Return(Arg)> { typedef Return Type; };

        template<typename Return>
        struct __ResultOf<Return()> { typedef Return Type; };

        template<typename Return, typename Arg1, typename Arg2, typename Arg3>
        struct __ResultOf<Return(*)(Arg1, Arg2, Arg3)> { typedef Return Type; };

        template<typename Return, typename Arg1, typename Arg2>
        struct __ResultOf<Return(*)(Arg1, Arg2)> { typedef Return Type; };

        template<typename Return, typename Arg>
        struct __ResultOf<Return(*)(Arg)> { typedef Return Type; };

        template<typename Return>
        struct __ResultOf<Return(*)()> { typedef Return Type; };

        template<typename Return, typename Arg1, typename Arg2, typename Arg3>
        struct __ResultOf<Return(&)(Arg1, Arg2, Arg3)> { typedef Return Type; };

        template<typename Return, typename Arg1, typename Arg2>
        struct __ResultOf<Return(&)(Arg1, Arg2)> { typedef Return Type; };

        template<typename Return, typename Arg>
        struct __ResultOf<Return(&)(Arg)> { typedef Return Type; };

        template<typename Return>
        struct __ResultOf<Return(&)()> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg1, typename Arg2, typename Arg3>
        struct __ResultOf<Return(Object::*)(Arg1, Arg2, Arg3)> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg1, typename Arg2>
        struct __ResultOf<Return(Object::*)(Arg1, Arg2)> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg>
        struct __ResultOf<Return(Object::*)(Arg)> { typedef Return Type; };

        template<typename Object, typename Return>
        struct __ResultOf<Return(Object::*)()> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg1, typename Arg2, typename Arg3>
        struct __ResultOf<Return(Object::*)(Arg1, Arg2, Arg3) const> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg1, typename Arg2>
        struct __ResultOf<Return(Object::*)(Arg1, Arg2) const> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg>
        struct __ResultOf<Return(Object::*)(Arg) const> { typedef Return Type; };

        template<typename Object, typename Return>
        struct __ResultOf<Return(Object::*)() const> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg1, typename Arg2, typename Arg3>
        struct __ResultOf<Return(Object::*)(Arg1, Arg2, Arg3) volatile> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg1, typename Arg2>
        struct __ResultOf<Return(Object::*)(Arg1, Arg2) volatile> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg>
        struct __ResultOf<Return(Object::*)(Arg) volatile> { typedef Return Type; };

        template<typename Object, typename Return>
        struct __ResultOf<Return(Object::*)() volatile> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg1, typename Arg2, typename Arg3>
        struct __ResultOf<Return(Object::*)(Arg1, Arg2, Arg3) const volatile> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg1, typename Arg2>
        struct __ResultOf<Return(Object::*)(Arg1, Arg2) const volatile> { typedef Return Type; };

        template<typename Object, typename Return, typename Arg>
        struct __ResultOf<Return(Object::*)(Arg) const volatile> { typedef Return Type; };

        template<typename Object, typename Return>
        struct __ResultOf<Return(Object::*)() const volatile> { typedef Return Type; };
    }

    /// @brief Deduces the result/return type of callable object
    /// @tparam Callable Type of callable object
    /// @ingroup type_traits
    /// @deprecated Recommended below C++11. Otherwise better use `wstl::InvokeResult`
    /// @note Maximum argument count is 3
    /// @see https://en.cppreference.com/w/cpp/types/result_of
    template<typename Callable>
    struct ResultOf : __private::__ResultOf<Callable> {};

    #ifdef __WSTL_CXX11__
    /// @copydoc ResultOf
    /// @since C++11
    template<typename Callable>
    using ResultOfType = typename ResultOf<Callable>::Type;
    #endif

    // Conjunction

    #ifdef __WSTL_CXX11__
    /// @brief Performs logical AND on the sequence of traits
    /// @tparam T Sequence of traits
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/conjunction
    template<typename... T>
    struct Conjunction;

    template<typename...>
    struct Conjunction : TrueType {};

    template<typename T>
    struct Conjunction<T> : T {};

    template<typename T1, typename... Tn>
    struct Conjunction<T1, Tn...> : Conditional<bool(T1::Value), Conjunction<Tn...>, T1>::Type {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc Conjunction
    /// @since C++17
    template<typename... T>
    inline constexpr bool ConjunctionValue = Conjunction<T...>::Value;
    #endif

    // Disjunction

    #ifdef __WSTL_CXX11__
    /// @brief Performs logical OR on the sequence of traits
    /// @tparam T Sequence of traits
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/disjunction
    template<typename... T>
    struct Disjunction;

    template<typename...>
    struct Disjunction : FalseType {};

    template<typename T>
    struct Disjunction<T> : T {};

    template<typename T1, typename... Tn>
    struct Disjunction<T1, Tn...> : Conditional<bool(T1::Value), T1, Disjunction<Tn...>>::Type {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc Disjunction
    /// @since C++17
    template<typename... T>
    inline constexpr bool DisjunctionValue = Disjunction<T...>::Value;
    #endif

    // Negation

    /// @brief Performs logical NOT on the specified type
    /// @tparam T Type to negate
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/negation
    template<typename T>
    struct Negation;

    template<typename T>
    struct Negation : BoolConstant<!bool(T::Value)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc Negation
    /// @since C++17
    template<typename T>
    inline constexpr bool NegationValue = Negation<T>::Value;
    #endif

    // Type identity

    /// @brief Provides the member typedef type
    /// @tparam T Type to provide
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/type_identity
    template<typename T>
    struct TypeIdentity;

    template<typename T>
    struct TypeIdentity { typedef T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc TypeIdentity
    /// @since C++11
    template<typename T>
    using TypeIdentityType = TypeIdentity<T>;
    #endif

    // Is const

    /// @brief Checks whether type is constant
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_const
    template<typename T>
    struct IsConst;

    template<typename T>
    struct IsConst : FalseType {};

    template<typename T>
    struct IsConst<const T> : TrueType {};

    template<typename T>
    struct IsConst<const volatile T> : TrueType {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsConst
    /// @since C++17
    template<typename T>
    inline constexpr bool IsConstValue = IsConst<T>::Value;
    #endif

    // Is volatile

    /// @brief Checks whether type is volatile
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_volatile
    template<typename T>
    struct IsVolatile;

    template<typename T>
    struct IsVolatile : FalseType {};

    template<typename T>
    struct IsVolatile<volatile T> : TrueType {};

    template<typename T>
    struct IsVolatile<const volatile T> : TrueType {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsVolatile
    /// @since C++17
    template<typename T>
    inline constexpr bool IsVolatileValue = IsVolatile<T>::Value;
    #endif

    // Is same

    /// @brief Checks whether two types are same
    /// @tparam T1 First type to check
    /// @tparam T2 Second type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_same
    template<typename T1, typename T2>
    struct IsSame;

    template<typename T1, typename T2>
    struct IsSame : FalseType {};

    template<typename T>
    struct IsSame<T, T> : TrueType {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsSame
    /// @since C++17
    template<typename T1, typename T2>
    inline constexpr bool IsSameValue = IsSame<T1, T2>::Value;
    #endif

    // Is null pointer

    /// @brief Checks whether pointer is null
    /// @tparam T Type of pointer to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_null_pointer
    template<typename T>
    struct IsNullPointer : IsSame<NullPointerType, typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNullPointer
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNullPointerValue = IsNullPointer<T>::Value;
    #endif

    // Is void

    /// @brief Checks whether type is void
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_void
    template<typename T>
    struct IsVoid : IsSame<void, typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsVoid
    /// @since C++17
    template<typename T>
    inline constexpr bool IsVoidValue = IsVoid<T>::Value;
    #endif

    // Is integral

    /// @brief Checks whether type is integral
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_integral
    template<typename T>
    struct IsIntegral;
    
    template<typename T>
    struct IsIntegral : FalseType {};

    template<> struct IsIntegral<char> : TrueType {};
    template<> struct IsIntegral<signed char> : TrueType {};
    template<> struct IsIntegral<bool> : TrueType {};
    template<> struct IsIntegral<short> : TrueType {};
    template<> struct IsIntegral<int> : TrueType {};
    template<> struct IsIntegral<long> : TrueType {};
    template<> struct IsIntegral<long long> : TrueType {};
    template<> struct IsIntegral<wchar_t> : TrueType {};

    template<> struct IsIntegral<unsigned char> : TrueType {};
    template<> struct IsIntegral<unsigned short> : TrueType {};
    template<> struct IsIntegral<unsigned int> : TrueType {};
    template<> struct IsIntegral<unsigned long> : TrueType {};
    template<> struct IsIntegral<unsigned long long> : TrueType {};

    #ifdef __WSTL_CXX20__
    template<> struct IsIntegral<char8_t> : TrueType {};
    #endif

    #ifdef __WSTL_CXX11__
    template<> struct IsIntegral<char16_t> : TrueType {};
    template<> struct IsIntegral<char32_t> : TrueType {};
    #endif
 
    template<typename T> struct IsIntegral<const T> : IsIntegral<T> {};
    template<typename T> struct IsIntegral<volatile T> : IsIntegral<T> {};
    template<typename T> struct IsIntegral<const volatile T> : IsIntegral<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsIntegral
    /// @since C++17
    template<typename T>
    inline constexpr bool IsIntegralValue = IsIntegral<T>::Value;
    #endif

    // Is floating point

    /// @brief Checks whether type is floating point (float, double)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_floating_point
    template<typename T>
    struct IsFloatingPoint;

    template<typename T>
    struct IsFloatingPoint : FalseType {};

    template<> struct IsFloatingPoint<float> : TrueType {};
    template<> struct IsFloatingPoint<double> : TrueType {};
    template<> struct IsFloatingPoint<long double> : TrueType {};

    template<typename T> struct IsFloatingPoint<const T> : IsFloatingPoint<T> {};
    template<typename T> struct IsFloatingPoint<volatile T> : IsFloatingPoint<T> {};
    template<typename T> struct IsFloatingPoint<const volatile T> : IsFloatingPoint<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsFloatingPoint
    /// @since C++17
    template<typename T>
    inline constexpr bool IsFloatingPointValue = IsFloatingPoint<T>::Value;
    #endif

    // Is arithmetic

    /// @brief Checks whether type if arithmetic (int, float, etc.)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_arithmetic
    template<typename T>
    struct IsArithmetic : BoolConstant<(IsIntegral<T>::Value || IsFloatingPoint<T>::Value)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsArithmetic
    /// @since C++17
    template<typename T>
    inline constexpr bool IsArithmeticValue = IsArithmetic<T>::Value;
    #endif

    // Is fundamental

    /// @brief Checks whether type is fundamental (arithmetic, void, nullptr_t)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_fundamental
    template<typename T>
    struct IsFundamental;

    template<typename T>
    struct IsFundamental : BoolConstant<(IsArithmetic<T>::Value || IsVoid<T>::Value ||
        IsNullPointer<T>::Value)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsFundamental
    /// @since C++17
    template<typename T>
    inline constexpr bool IsFundamentalValue = IsFundamental<T>::Value;
    #endif
    
    // Is compound

    /// @brief Checks whether type is compound
    /// (array, function, object pointer, function pointer, member object pointer, 
    /// member function pointer, reference, struct, union, or enumeration, 
    /// including any cv-qualified variants)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_compound
    template<typename T>
    struct IsCompound;

    template<typename T>
    struct IsCompound : BoolConstant<(!IsFundamental<T>::Value)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsCompound
    /// @since C++17
    template<typename T>
    inline constexpr bool IsCompoundValue = IsCompound<T>::Value;
    #endif

    // Is lvalue reference

    namespace __private {
        template<typename T>
        struct __IsLValueReference : FalseType {};

        template<typename T>
        struct __IsLValueReference<T&> : TrueType {};
    }   

    /// @brief Checks whether type is lvalue reference
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_lvalue_reference
    template<typename T>
    struct IsLValueReference : __private::__IsLValueReference<typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsLValueReference
    /// @since C++17
    template<typename T>
    inline constexpr bool IsLValueReferenceValue = IsLValueReference<T>::Value;
    #endif

    // Is rvalue reference

    #ifdef __WSTL_CXX11__
    namespace __private {
        template<typename T>
        struct __IsRValueReference : FalseType {};

        template<typename T>
        struct __IsRValueReference<T&&> : TrueType {};
    }

    /// @brief Checks whether type is rvalue reference
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_rvalue_reference
    template<typename T>
    struct IsRValueReference : __private::__IsRValueReference<typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsRValueReference
    /// @since C++17
    template<typename T>
    inline constexpr bool IsRValueReferenceValue = IsRValueReference<T>::Value;
    #endif
    #endif

    // Is reference (lvalue or rvalue)

    /// @brief Checks whether type is reference
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_reference
    template<typename T>
    struct IsReference : BoolConstant<(
        IsLValueReference<T>::Value
        #ifdef __WSTL_CXX11__
        || IsRValueReference<T>::Value
        #endif
    )> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsReference
    /// @since C++17
    template<typename T>
    inline constexpr bool IsReferenceValue = IsReference<T>::Value;
    #endif

    // Is function

    /// @brief Checks whether type is function
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_function
    template<typename T>
    struct IsFunction : BoolConstant<!(IsConst<const T>::Value || IsReference<T>::Value)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsFunction
    /// @since C++17 
    template<typename T>
    inline constexpr bool IsFunctionValue = IsFunction<T>::Value;
    #endif

    // Is array

    /// @brief Checks whether type is an array
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_array
    template<typename T>
    struct IsArray;

    template<typename T>
    struct IsArray : FalseType {};

    template<typename T>
    struct IsArray<T[]> : TrueType {};
    
    template<typename T, size_t S>
    struct IsArray<T[S]> : TrueType {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsArray
    /// @since C++17
    template<typename T>
    inline constexpr bool IsArrayValue = IsArray<T>::Value;
    #endif

    // Is union

    /// @brief Checks whether type is union
    /// @tparam T Type to check
    /// @note This trait may not work correctly on some compilers that do not support `__is_union` builtin. 
    /// In such cases, it will always return false.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_union
    template<typename T>
    struct IsUnion : BoolConstant<
    #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
    __is_union(T)
    #else
    false
    #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsUnion
    /// @since C++17
    template<typename T>
    inline constexpr bool IsUnionValue = IsUnion<T>::Value;
    #endif

    // Is member pointer

    namespace __private {
        template<typename T>
        struct __IsMemberPointer : FalseType {};

        template<typename T, typename U>
        struct __IsMemberPointer<T U::*> : TrueType {};
    }

    /// @brief Checks whether type is pointer to non-static member object or function
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_member_pointer
    template<typename T>
    struct IsMemberPointer : __private::__IsMemberPointer<typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsMemberPointer
    /// @since C++17
    template<typename T>
    inline constexpr bool IsMemberPointerValue = IsMemberPointer<T>::Value;
    #endif

    // Is member function pointer

    namespace __private {
        template<typename T>
        struct __IsMemberFunctionPointer : FalseType {};

        template<typename T, typename U>
        struct __IsMemberFunctionPointer<T U::*> : IsFunction<T> {};
    }
    
    /// @brief Checks whether type is pointer to non-static member function
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_member_function_pointer
    template<typename T>
    struct IsMemberFunctionPointer : 
        public __private::__IsMemberFunctionPointer<typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsMemberFunctionPointer
    /// @since C++17
    template<typename T>
    inline constexpr bool IsMemberFunctionPointerValue = IsMemberFunctionPointer<T>::Value;
    #endif

    // Is member object pointer

    /// @brief Checks whether type is pointer to non-static member object
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_member_object_pointer
    template<typename T>
    struct IsMemberObjectPointer : BoolConstant<IsMemberPointer<T>::Value && 
        !IsMemberFunctionPointer<T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsMemberObjectPointer
    /// @since C++17
    template<typename T>
    inline constexpr bool IsMemberObjectPointerValue = IsMemberObjectPointer<T>::Value;
    #endif

    // Is class

    namespace __private {
        template<typename T>
        static typename Conditional<!IsUnion<T>::Value, long, char>::Type __TestClass(int T::*);

        template<typename T> 
        static char __TestClass(...);
    }

    /// @brief Checks whether type is non-union class
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_class
    template<typename T>
    struct IsClass : BoolConstant<sizeof(__private::__TestClass<T>(0)) == sizeof(long)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsClass
    /// @since C++17
    template<typename T>
    inline constexpr bool IsClassValue = IsClass<T>::Value;
    #endif

    // Is base of

    /// @brief Checks whether type is base of another type
    /// @tparam Base Base type
    /// @tparam Derived Derived type
    /// @note This trait may not work correctly on some compilers that do not support `__is_base_of` builtin. 
    /// In such cases, it will always return false.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_base_of
    template<typename Base, typename Derived>
    struct IsBaseOf : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __is_base_of(Base, Derived)
        #else
        false
        #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsBaseOf
    /// @since C++17
    template<typename Base, typename Derived>
    inline constexpr bool IsBaseOfValue = IsBaseOf<Base, Derived>::Value;
    #endif

    // Is convertible

    namespace __private {
        template<typename T>
        static long __TestReturnable(T(*)());

        template<typename>
        static char __TestReturnable(...);

        #ifdef __WSTL_CXX11__
        template<typename From, typename To>
        static auto __TestImplicitlyConvertible(int) -> decltype(void(DeclareValue<void(&)(To)>()(DeclareValue<From>())), TrueType{});

        template<typename, typename>
        static auto __TestImplicitlyConvertible(...) -> FalseType;
        #else
        template<typename To>
        static long __TestImplicitlyConvertible(To);

        template<typename>
        static char __TestImplicitlyConvertible(...);

        template<typename From>
        static From& __TestConvertFrom();
        #endif
    }

    /// @brief Checks whether implicit conversion can be done between types
    /// @details In C++98 may not work well with explicit
    /// @tparam From Initial type
    /// @tparam To Type to convert to
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_convertible
    template<typename From, typename To>
    struct IsConvertible : BoolConstant<
        (sizeof(__private::__TestReturnable<To>(static_cast<To(*)()>(0))) == sizeof(long) && 
        #ifdef __WSTL_CXX11__
        decltype(__private::__TestImplicitlyConvertible<From, To>(0))::Value) ||
        #else
        sizeof(__private::__TestImplicitlyConvertible<To>(__private::__TestConvertFrom<From>())) == sizeof(long)) ||
        #endif
        (IsVoid<From>::Value && IsVoid<To>::Value)
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsConvertible
    /// @since C++17
    template<typename From, typename To>
    inline constexpr bool IsConvertibleValue = IsConvertible<From, To>::Value;
    #endif

    // Is enum

    /// @brief Checks whether type is enumeration
    /// @tparam T Type to check
    /// @note This trait may not work correctly on some compilers that do not support `__is_enum` builtin. 
    /// In such cases, it will use a fallback implementation that incorrectly identifies scoped enums and user-defined types with conversion to int.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_enum
    template<typename T>
    struct IsEnum : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __is_enum(T)
        #else
        !IsFundamental<T>::Value && !IsClass<T>::Value && 
        !IsUnion<T>::Value && IsConvertible<T, int>::Value
        #endif
    > {};
    
    #ifdef __WSTL_CXX17__
    /// @copydoc IsEnum
    /// @since C++17
    template<typename T>
    inline constexpr bool IsEnumValue = IsEnum<T>::Value;
    #endif

    // Is pointer

    namespace __private {
        template<typename T>
        struct __IsPointer : FalseType {};

        template<typename T>
        struct __IsPointer<T*> : TrueType {};
    }

    /// @brief Checks whether type is pointer
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_pointer
    template<typename T>
    struct IsPointer : __private::__IsPointer<typename RemoveCV<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsPointer
    /// @since C++17
    template<typename T>
    inline constexpr bool IsPointerValue = IsPointer<T>::Value;
    #endif

    // Is scalar

    /// @brief Checks whether type is scalar (arithmetic, enum, pointer, pointers to member)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_scalar
    template<typename T>
    struct IsScalar : BoolConstant<IsArithmetic<T>::Value || IsEnum<T>::Value 
        || IsPointer<T>::Value || IsMemberPointer<T>::Value || IsNullPointer<T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsScalar
    /// @since C++17
    template<typename T>
    inline constexpr bool IsScalarValue = IsScalar<T>::Value;
    #endif

    // Is trivial

    /// @brief Checks whether type is trivial (not user-defined)
    /// @tparam T Type to check
    /// @note This trait may not work correctly on some compilers that do not support `__is_trivial` builtin. 
    /// In such cases, it will always return false.
    /// @deprecated Use more precise traits instead, such as `wstl::IsTriviallyConstructible`
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_trivial
    template<typename T>
    struct IsTrivial : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __is_trivial(T)
        #else
        false
        #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTrivial
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTrivialValue = IsTrivial<T>::Value;
    #endif

    // Is POD

    /// @brief Checks whether type is plain-old data 
    /// @tparam T Type to check
    /// @note This trait may not work correctly on some compilers that do not support `__is_pod` builtin. 
    /// In such cases, it will always return false.
    /// @deprecated Use more precise traits instead, such as `wstl::IsStandardLayout`
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_pod
    template<typename T>
    struct IsPOD : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __is_pod(T)
        #else
        false
        #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsPOD
    /// @since C++17
    template<typename T>
    inline constexpr bool IsPODValue = IsPOD<T>::Value;
    #endif

    // Is standard layout

    /// @brief Checks whether type is standard layout
    /// @tparam T Type to check
    /// @note This trait may not work correctly on some compilers that do not support `__is_standard_layout` builtin. 
    /// In such cases, it will always return false.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_standard_layout
    template<typename T>
    struct IsStandardLayout : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __is_standard_layout(T)
        #else
        false
        #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsStandardLayout
    /// @since C++17
    template<typename T>
    inline constexpr bool IsStandardLayoutValue = IsStandardLayout<T>::Value;
    #endif

    // Is object

    /// @brief Checks whether type is object (not function, reference, void)
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_object
    template<typename T>
    struct IsObject : BoolConstant<IsScalar<T>::Value || IsArray<T>::Value || 
        IsArray<T>::Value || IsUnion<T>::Value || IsClass<T>::Value> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsObject
    /// @since C++17
    template<typename T>
    inline constexpr bool IsObjectValue = IsObject<T>::Value;
    #endif
    
    // Is constructible

    #ifdef __WSTL_CXX11__
    /// @brief Checks whether type is constructor
    /// @tparam T Constructor type
    /// @tparam Args Argument types
    /// @note This trait may not work correctly on some compilers that do not support `__is_constructible` builtin. 
    /// In such cases, it will always return false.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T, typename... Args>
    struct IsConstructible : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __is_constructible(T, Args...)
        #else
        false
        #endif
    > {};
    #else
    /// @brief Checks whether type is constructor
    /// @tparam T Constructor type
    /// @tparam Args Argument types
    /// @note This trait may not work correctly on some compilers that do not support `__is_constructible` builtin. 
    /// In such cases, it will always return false.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T, typename Args = void>
    struct IsConstructible : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __is_constructible(T, Args)
        #else
        false
        #endif
    > {};
    
    /// @brief Checks whether type is constructor
    /// @tparam T Constructor type
    /// @note This trait may not work correctly on some compilers that do not support `__is_constructible` builtin. 
    /// In such cases, it will always return false.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T>
    struct IsConstructible<T, void> : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __is_constructible(T)
        #else
        false
        #endif
    > {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc IsConstructible
    /// @since C++17
    template<typename T, typename... Args>
    inline constexpr bool IsConstructibleValue = IsConstructible<T, Args...>::Value;
    #endif

    // Is trivially constuctible

    #ifdef __WSTL_CXX11__
    /// @brief Checks whether type is trivially constructible (does not call not trivial operations)
    /// @tparam T Constructor type
    /// @tparam Args Argument types
    /// @note This trait may not work correctly on some compilers that do not support `__is_trivially_constructible` builtin. 
    /// In such cases, it will always return false. In C++98 arguments are not supported.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T, typename... Args>
    struct IsTriviallyConstructible : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __is_trivially_constructible(T, Args...)
        #else
        false
        #endif
    > {};
    #else
    /// @brief Checks whether type is trivially constructible (does not call not trivial operations)
    /// @tparam T Constructor type
    /// @note This trait may not work correctly on some compilers that do not support `__has_trivial_constructor` builtin. 
    /// In such cases, it will always return false. In C++98 arguments are not supported.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T>
    struct IsTriviallyConstructible : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __has_trivial_constructor(T)
        #else
        false 
        #endif
    > {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyConstructible
    /// @since C++17
    template<typename T, typename... Args>
    inline constexpr bool IsTriviallyConstructibleValue = IsTriviallyConstructible<T, Args...>::Value;
    #endif

    // Is nothrow constructible

    #ifdef __WSTL_EXCEPTIONS__
    #ifdef __WSTL_CXX11__
    namespace __private {
        template<typename T, typename... Args>
        static auto __TestNothrowConstructible(int) -> decltype(T(DeclareValue<Args>()...), BoolConstant<noexcept(T(DeclareValue<Args>()...))>{});

        template<typename, typename...>
        static FalseType __TestNothrowConstructible(...);
    }

    /// @brief Checks whether type is nothrow constructible (noexcept)
    /// @tparam T Constructor type
    /// @tparam ...Args Arguments type
    /// @note This trait mainly uses `__is_nothrow_constructible` builtin, but if it's not available, 
    /// it will use a fallback implementation that generally works but may not be 100% accurate in all cases. 
    /// Requires `__WSTL_EXCEPTIONS__` to be defined.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T, typename... Args>
    struct IsNothrowConstructible : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __is_nothrow_constructible(T, Args...)
        #else
        decltype(__private::__TestNothrowConstructible<T, Args...>(0))::Value && !IsVoid<T>::Value
        #endif
    > {};

    #else
    /// @brief Checks whether type is nothrow constructible (noexcept)
    /// @tparam T Constructor type
    /// @tparam Args Arguments type (does nothing, not supported in C++98)
    /// @note This trait may not work correctly on some compilers that do not support used builtin.
    /// In such cases, it will always return false. Requires `__WSTL_EXCEPTIONS__` to be defined.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T, typename Args = void>
    struct IsNothrowConstructible : BoolConstant<
        #ifdef __WSTL_TYPETRAITS_NO_BUILTINS__
        false
        #elif defined(__WSTL_GCC__) || defined(__WSTL_CLANG__) || defined(__WSTL_ICC__)
        __has_nothrow_constructor(T)
        #elif defined(__WSTL_MSVC__)
        _has_nothrow_constructor(T)
        #else
        false
        #endif
    > {};
    
    /// @brief Checks whether type is nothrow constructible (noexcept)
    /// @tparam T Constructor type
    /// @note This trait may not work correctly on some compilers that do not support used builtin.
    /// In such cases, it will always return false. Requires `__WSTL_EXCEPTIONS__` to be defined.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_constructible
    template<typename T>
    struct IsNothrowConstructible<T, void> : BoolConstant<
    #ifdef __WSTL_TYPETRAITS_NO_BUILTINS__
    false
    #elif defined(__WSTL_GCC__) || defined(__WSTL_CLANG__) || defined(__WSTL_ICC__)
    __has_nothrow_constructor(T)
    #elif defined(__WSTL_MSVC__)
    _has_nothrow_constructor(T)
    #else
    false
    #endif
    > {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowConstructible
    /// @since C++17
    template<typename T, typename... Args>
    inline constexpr bool IsNothrowConstructibleValue = IsNothrowConstructible<T, Args...>::Value;
    #endif
    #endif

    // Is default constructible

    /// @brief Checks whether type can be constructed without arguments (T, T())
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_default_constructible
    template<typename T>
    struct IsDefaultConstructible : IsConstructible<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsDefaultConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsDefaultConstructibleValue = IsDefaultConstructible<T>::Value;
    #endif

    // Is implicitly default constructible

    #ifdef __WSTL_CXX11__
    namespace __private {
        template<typename T>
        void __TestImplicit(T);

        template<typename T>
        static auto __TestImplicitConstructible(int) -> decltype(__TestImplicit<T const&>({}), IsDefaultConstructible<T>{});

        template<typename>
        static FalseType __TestImplicitConstructible(...);
    } 

    /// @brief Checks whether type can be implicitly constructed without arguments
    /// @tparam T Constructor type
    /// @ingroup type_traits
    template<typename T>
    struct IsImplicitlyDefaultConstructible : decltype(__private::__TestImplicitConstructible<T>(0)) {};
    #endif

    // Is trivially default constructible

    /// @brief Checks whether type can be trivially (not calling any not trivial operations) constructed without arguments (T, T())
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_default_constructible
    template<typename T>
    struct IsTriviallyDefaultConstructible : IsTriviallyConstructible<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyDefaultConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyDefaultConstructibleValue = IsTriviallyDefaultConstructible<T>::Value;
    #endif

    // Is nothrow default constructible

    #if defined(__WSTL_CXX11__) && defined(__WSTL_EXCEPTIONS__)
    /// @brief Checks whether type can be nothrow (noexcept) constructed without arguments (T, T())
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_default_constructible
    template<typename T>
    struct IsNothrowDefaultConstructible : IsNothrowConstructible<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowDefaultConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowDefaultConstructibleValue = IsNothrowDefaultConstructible<T>::Value;
    #endif
    #endif

    // Is copy constructible

    /// @brief Checks whether type can be copy-constructed
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_constructible
    template<typename T>
    struct IsCopyConstructible : IsConstructible<T, 
        typename AddLValueReference<typename AddConst<T>::Type>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsCopyConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsCopyConstructibleValue = IsCopyConstructible<T>::Value;
    #endif

    // Is trivially copy constructible

    /// @brief Checks whether type can be trivially copy-constructed (not calling any not trivial operations)
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_constructible
    template<typename T>
    struct IsTriviallyCopyConstructible : IsTriviallyConstructible<T, 
        typename AddLValueReference<typename AddConst<T>::Type>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyCopyConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyCopyConstructibleValue = IsTriviallyCopyConstructible<T>::Value;
    #endif

    // Is nothrow copy constructible

    #ifdef __WSTL_EXCEPTIONS__
    /// @brief Checks whether type can be nothrow (noexcept) copy-constructed
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_constructible
    template<typename T>
    struct IsNothrowCopyConstructible : IsNothrowConstructible<T, 
        typename AddLValueReference<typename AddConst<T>::Type>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowCopyConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowCopyConstructibleValue = IsNothrowCopyConstructible<T>::Value;
    #endif
    #endif

    // Is move constructible

    #ifdef __WSTL_CXX11__
    /// @brief Checks whether type can be move-constructed
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_move_constructible
    template<typename T>
    struct IsMoveConstructible : IsConstructible<T, typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsMoveConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsMoveConstructibleValue = IsMoveConstructible<T>::Value;
    #endif

    // Is trivially move constructible

    /// @brief Checks whether type can be trivially move-constructed (not calling any non-trivial operations)   
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_move_constructible
    template<typename T>
    struct IsTriviallyMoveConstructible : IsTriviallyConstructible<T, 
        typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyMoveConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyMoveConstructibleValue = IsTriviallyMoveConstructible<T>::Value;
    #endif

    // Is nothrow move constructible

    #ifdef __WSTL_EXCEPTIONS__
    /// @brief Checks whether type can be nothrow (noexcept) move-constructed
    /// @tparam T Constructor type
    /// @ingroup type_traits
    /// @since C++11 
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_move_constructible
    template<typename T>
    struct IsNothrowMoveConstructible : IsNothrowConstructible<T, 
        typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowCopyConstructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowMoveConstructibleValue = IsNothrowMoveConstructible<T>::Value;
    #endif
    #endif
    #endif

    // Is assignable

    namespace __private {
        #ifdef __WSTL_CXX11__
        template<typename T, typename U>
        static auto __TestAssignable(int) -> decltype(DeclareValue<T>() = DeclareValue<U>(), TrueType{});

        template<typename, typename>
        static FalseType __TestAssignable(...);  
        #else
        template<typename T, typename U>
        static long __TestAssignable(int, T* t = static_cast<T*>(0), U* u = static_cast<U*>(0), 
        char(*)[sizeof(*t = *u)] = 0);

        template<typename, typename>
        static char __TestAssignable(...);
        #endif
    }    

    /// @brief Checks whether type is assignable to another
    /// @tparam T Type to check
    /// @tparam U Type to check against
    /// @note This trait mainly uses `__is_assignable` builtin, but if it's not available,
    /// it will use a fallback implementation that generally works but may not be 100% accurate in all cases.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_assignable
    template<typename T, typename U>
    struct IsAssignable : BoolConstant<
    #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
    __is_assignable(T, U)
    #elif defined(__WSTL_CXX11__)
    decltype(__private::__TestAssignable<T, U>(0))::Value
    #else
    sizeof(__private::__TestAssignable<T, U>(0)) == sizeof(long)
    #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsAssignable
    /// @since C++17
    template<typename T, typename U>
    inline constexpr bool IsAssignableValue = IsAssignable<T, U>::Value;
    #endif

    // Is trivially assignable

    /// @brief Checks whether type is trivially assignable to another (not calling any non-trivial operations)
    /// @tparam T Type to check
    /// @tparam U Type to check against
    /// @note This trait may not work correctly on some compilers that do not support used builtins.
    /// In such cases, it will always return false.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_assignable
    template<typename T, typename U>
    struct IsTriviallyAssignable : BoolConstant<
        #ifdef __WSTL_TYPETRAITS_NO_BUILTINS__
        false
        #elif defined(__WSTL_SUPPORTED_COMPILER__) && defined(__WSTL_CXX11__)
        __is_trivially_assignable(T, U)
        #else
        IsAssignable<T, U>::Value && 
        #if defined(__WSTL_GCC__) || defined(__WSTL_ICC__) || defined(__WSTL_CLANG__)
        __has_trivial_assign(T) && __has_trivial_assign(U)
        #elif defined(__WSTL_MSVC__)
        __is_pod(T) && __is_pod(U)
        #else
        false
        #endif
        #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyAssignable
    /// @since C++17
    template<typename T, typename U>
    inline constexpr bool IsTriviallyAssignableValue = IsTriviallyAssignable<T, U>::Value;
    #endif

    // Is nothrow assignable

    #ifdef __WSTL_EXCEPTIONS__
    #ifdef __WSTL_CXX11__
    namespace __private {
        template<typename T, typename U>
        static auto __TestNothrowAssignable(int) -> decltype(BoolConstant<noexcept(DeclareValue<T>() = DeclareValue<U>())>{});

        template<typename, typename>
        static FalseType __TestNothrowAssignable(...);
    }

    /// @brief Checks whether type is nothrow (noexcept) assignable to another
    /// @tparam T Type to check
    /// @tparam U Type to check against
    /// @note This trait mainly uses `__is_nothrow_assignable` builtin, but if it's not available, 
    /// it will use a fallback implementation that generally works but may not be 100% accurate in all cases.
    /// Requires `__WSTL_EXCEPTIONS__` to be defined.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_assignable
    template<typename T, typename U>
    struct IsNothrowAssignable : BoolConstant<
        #if defined(__WSTL_SUPPORTED_COMPILER__) && !defined(__WSTL_TYPETRAITS_NO_BUILTINS__)
        __is_nothrow_assignable(T, U)
        #else
        IsAssignable<T, U>::Value && 
        decltype(__private::__TestNothrowAssignable<T, U>(0))::Value
        #endif
    > {};
    #else
    /// @brief Checks whether type is nothrow (noexcept) assignable to another
    /// @tparam T Type to check
    /// @tparam U Type to check against
    /// @note This trait may not work correctly on some compilers that do not support used builtin.
    /// In such cases, it will always return false. Requires `__WSTL_EXCEPTIONS__` to be defined.
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_assignable
    template<typename T, typename U>
    struct IsNothrowAssignable : BoolConstant<
    #ifdef __WSTL_TYPETRAITS_NO_BUILTINS__
    false
    #elif defined(__WSTL_GCC__) || defined(__WSTL_CLANG__) || defined(__WSTL_ICC__)
    __is_nothrow_assignable(T, U)
    #else
    false
    #endif
    > {};
    #endif

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowAssignable
    /// @since C++17
    template<typename T, typename U>
    inline constexpr bool IsNothrowAssignableValue = IsNothrowAssignable<T, U>::Value;
    #endif
    #endif

    // Is copy assignable

    /// @brief Checks whether type is copy-assignable to another
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_assignable
    template<typename T>
    struct IsCopyAssignable : IsAssignable<typename AddLValueReference<T>::Type, 
        typename AddLValueReference<const T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsCopyAssignable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsCopyAssignableValue = IsCopyAssignable<T>::Value;
    #endif
    
    // Is trivially copy assignable

    /// @brief Checks whether type is trivially (not calling any non-trivial operations) copy-assignable to another
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_assignable
    template<typename T>
    struct IsTriviallyCopyAssignable : IsTriviallyAssignable<typename AddLValueReference<T>::Type,
        typename AddLValueReference<const T>::Type> {};

    #ifdef __WSTL_CXX17__
    template<typename T>
    inline constexpr bool IsTriviallyCopyAssignableValue = IsTriviallyCopyAssignable<T>::Value;
    #endif

    // Is nothrow copy assignable
    
    #ifdef __WSTL_EXCEPTIONS__
    /// @brief Checks whether type is nothrow (noexcept) copy-assignable to another
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_copy_assignable
    template<typename T>
    struct IsNothrowCopyAssignable : IsNothrowAssignable<typename AddLValueReference<T>::Type,
        typename AddLValueReference<const T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowCopyAssignable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowCopyAssignableValue = IsNothrowCopyAssignable<T>::Value;
    #endif
    #endif

    // Is move assignable

    #ifdef __WSTL_CXX11__
    /// @brief Checks whether type is move-assignable to another
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_move_assignable
    template<typename T>
    struct IsMoveAssignable : IsAssignable<typename AddLValueReference<T>::Type, 
        typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsMoveAssignable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsMoveAssignableValue = IsMoveAssignable<T>::Value;
    #endif
    
    // Is trivially move assignable

    /// @brief Checks whether type is trivially (not calling any non-trivial operations) move-assignable to another
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_move_assignable
    template<typename T>
    struct IsTriviallyMoveAssignable : IsTriviallyAssignable<typename AddLValueReference<T>::Type,
        typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyMoveAssignable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyMoveAssignableValue = IsTriviallyMoveAssignable<T>::Value;
    #endif

    // Is nothrow move assignable
    
    #ifdef __WSTL_EXCEPTIONS__
    /// @brief Checks whether type is nothrow (noexcept) move-assignable to another
    /// @tparam T Type to check
    /// @tparam U Type to check against
    /// @ingroup type_traits
    /// @since C++11 
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_move_assignable
    template<typename T>
    struct IsNothrowMoveAssignable : IsNothrowAssignable<typename AddLValueReference<T>::Type,
        typename AddRValueReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowMoveAssignable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowMoveAssignableValue = IsNothrowMoveAssignable<T>::Value;
    #endif
    #endif
    #endif

    // Is destructible

    namespace __private {
        #ifdef __WSTL_CXX11__
        template<typename T>
        static auto __TestDestructible(int) -> decltype(DeclareValue<T&>().~T(), TrueType{});

        template<typename T>
        static FalseType __TestDestructible(...);
        #endif
    }

    /// @brief Checks whether type is destructible
    /// @tparam T Type to check
    /// @note This trait mainly uses `__is_destructible` or `__has_user_destructor` builtins, but if they're not available,
    /// it will use a fallback implementation that generally works but may not be 100% accurate in all cases.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_destructible
    template<typename T>
    struct IsDestructible : BoolConstant<
        #if !defined(__WSTL_TYPETRAITS_NO_BUILTINS__) && (__WSTL_HAS_BUILTIN__(__is_destructible) \
            || defined(__WSTL_ICC__) || defined(__WSTL_MSVC__))
            __is_destructible(T)
        #elif defined(__WSTL_CXX11__)
            decltype(__private::__TestDestructible<RemoveCVReferenceType<RemoveAllExtentsType<T>>>(0))::Value
        #else
            false
        #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsDestructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsDestructibleValue = IsDestructible<T>::Value;
    #endif

    // Is trivially destructible

    /// @brief Checks whether type is trivially destructible
    /// @tparam T Type to check
    /// @note This trait may not work correctly on some compilers that do not support used builtins.
    /// In such cases, it will always return false.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_destructible
    template<typename T>
    struct IsTriviallyDestructible : BoolConstant<
        #ifdef __WSTL_TYPETRAITS_NO_BUILTINS__
            false
        #elif __WSTL_HAS_BUILTIN__(__is_trivially_destructible) || defined(__WSTL_MSVC__) || defined(__WSTL_ICC__)
            __is_trivially_destructible(T)
        #elif __WSTL_HAS_BUILTIN__(__has_trivial_destructor)
            __has_trivial_destructor(T)
        #else
            false
        #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyDestructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyDestructibleValue = IsTriviallyDestructible<T>::Value;
    #endif

    // Is nothrow destructible

    #ifdef __WSTL_EXCEPTIONS__
    namespace __private {
        #ifdef __WSTL_CXX11__
        template<typename T>
        static auto __TestNothrowDestructible(int) -> decltype(DeclareValue<T>().~T(), BoolConstant<noexcept(DeclareValue<T>().~T())>{});

        template<typename T>
        static FalseType __TestNothrowDestructible(...);
        #endif
    }

    /// @brief Checks whether type is nothrow (noexcept) destructible
    /// @tparam T Type to check
    /// @note This trait mainly uses `__is_nothrow_destructible` builtin, but if it's not available,
    /// it will use a fallback implementation that generally works but may not be 100% accurate in all cases. Requires `__WSTL_EXCEPTIONS__` to be defined.
    /// Requires `__WSTL_EXCEPTIONS__` to be defined.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_destructible
    template<typename T>
    struct IsNothrowDestructible : BoolConstant<
        #if !defined(__WSTL_TYPETRAITS_NO_BUILTINS__) && (__WSTL_HAS_BUILTIN__(__is_nothrow_destructible) \
            || defined(__WSTL_MSVC__) || defined(__WSTL_ICC__))
            __is_nothrow_destructible(T)
        #elif defined(__WSTL_CXX11__)
            decltype(__private::__TestNothrowDestructible<T>(0))::Value
        #else
            false
        #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsNothrowDestructible
    /// @since C++17
    template<typename T>
    inline constexpr bool IsNothrowDestructibleValue = IsNothrowDestructible<T>::Value;
    #endif
    #endif

    // Is trivially copyable

    /// @brief Checks whether type is trivially copyable
    /// @tparam T Type to check
    /// @note This trait may not work correctly on some compilers that do not support used builtins.
    /// In such cases, it will always return false.
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_trivially_copyable
    template<typename T>
    struct IsTriviallyCopyable : BoolConstant<
        #ifdef __WSTL_TYPETRAITS_NO_BUILTINS__
            false
        #elif __WSTL_HAS_BUILTIN__(__is_trivially_copyable) || defined(__WSTL_MSVC__) || defined(__WSTL_ICC__)
            __is_trivially_copyable(T)
        #elif defined(__WSTL_CLANG__) || defined(__WSTL_GCC__)
            __has_trivial_constructor(T) && __has_trivial_assign(T) && __has_trivial_destructor(T)
        #else
            false
        #endif
    > {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTriviallyCopyable
    /// @since C++17
    template<typename T>
    inline constexpr bool IsTriviallyCopyableValue = IsTriviallyCopyable<T>::Value;
    #endif

    // Is signed

    /// @brief Checks whether type is signed
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_signed
    template<typename T>
    struct IsSigned;

    template<typename T>
    struct IsSigned : FalseType {};

    template<> struct IsSigned<signed char> : TrueType {};
    template<> struct IsSigned<char> : BoolConstant<(char(255) < char(0))> {};
    template<> struct IsSigned<wchar_t> : BoolConstant<(wchar_t(-1) < wchar_t(0))> {};
    template<> struct IsSigned<short> : TrueType {};
    template<> struct IsSigned<int> : TrueType {};
    template<> struct IsSigned<long> : TrueType {};
    template<> struct IsSigned<long long> : TrueType {};
    template<> struct IsSigned<float> : TrueType {};
    template<> struct IsSigned<double> : TrueType {};
    template<> struct IsSigned<long double> : TrueType {};

    #ifdef __WSTL_CXX20__
    template<> struct IsSigned<char8_t> : BoolConstant<(char8_t(-1) < char8_t(0))> {};
    #endif

    #ifdef __WSTL_CXX11__
    template<> struct IsSigned<char16_t> : BoolConstant<(char16_t(-1) < char16_t(0))> {};
    template<> struct IsSigned<char32_t> : BoolConstant<(char32_t(-1) < char32_t(0))> {};
    #endif

    template<typename T> struct IsSigned<const T> : IsSigned<T> {};
    template<typename T> struct IsSigned<volatile T> : IsSigned<T> {};
    template<typename T> struct IsSigned<const volatile T> : IsSigned<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsSigned
    /// @since C++17
    template<typename T>
    inline constexpr bool IsSignedValue = IsSigned<T>::Value;
    #endif

    // IsUnsigned

    /// @brief Checks whether type is unsigned
    /// @tparam T Type to check
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/is_unsigned
    template<typename T>
    struct IsUnsigned;

    template<typename T>
    struct IsUnsigned : FalseType {};

    template<> struct IsUnsigned<bool> : TrueType {};
    template<> struct IsUnsigned<char> : BoolConstant<(char(255) > char(0))> {};
    template<> struct IsUnsigned<wchar_t> : BoolConstant<(wchar_t(-1) > wchar_t(0))> {};
    template<> struct IsUnsigned<unsigned char> : TrueType {};
    template<> struct IsUnsigned<unsigned short> : TrueType {};
    template<> struct IsUnsigned<unsigned int> : TrueType {};
    template<> struct IsUnsigned<unsigned long> : TrueType {};
    template<> struct IsUnsigned<unsigned long long> : TrueType {};

    #ifdef __WSTL_CXX20__
    template<> struct IsUnsigned<char8_t> : BoolConstant<(char8_t(-1) > char8_t(0))> {};
    #endif

    #ifdef __WSTL_CXX11__
    template<> struct IsUnsigned<char16_t> : BoolConstant<(char16_t(-1) > char16_t(0))> {};
    template<> struct IsUnsigned<char32_t> : BoolConstant<(char32_t(-1) > char32_t(0))> {};
    #endif

    template<typename T> struct IsUnsigned<const T> : IsUnsigned<T> {};
    template<typename T> struct IsUnsigned<volatile T> : IsUnsigned<T> {};
    template<typename T> struct IsUnsigned<const volatile T> : IsUnsigned<T> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsUnsigned
    /// @since C++17
    template<typename T>
    inline constexpr bool IsUnsignedValue = IsUnsigned<T>::Value;
    #endif

    // Decay

    /// @brief Performs conversions same as when passing function arguments by value
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/decay
    template<typename T>
    struct Decay {
    private:
        typedef typename RemoveReference<T>::Type U;

    public:
        typedef typename Conditional<IsArray<U>::Value, 
            typename AddPointer<typename RemoveExtent<T>::Type>::Type,
            typename Conditional<IsFunction<U>::Value, typename AddPointer<U>::Type, 
            typename RemoveCV<U>::Type>::Type>::Type Type;
    };

    #ifdef __WSTL_CXX11__
    /// @copydoc Decay
    /// @since C++11
    template<typename T>
    using DecayType = typename Decay<T>::Type;
    #endif

    // Unwrap reference

    /// @brief Extracts underlying type from reference wrappers
    /// @tparam T Reference wrapper type
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/utility/functional/unwrap_reference
    template<typename T>
    struct UnwrapReference;
    
    template<typename T>
    struct UnwrapReference { typedef T Type; };

    #ifdef __WSTL_CXX11__
    /// @copydoc UnwrapReference
    /// @since C++11
    template<typename T>
    using UnwrapReferenceType = typename UnwrapReference<T>::Type;
    #endif

    // Unwrap reference decay

    /// @brief Combines the behaviour of Decay and UnwrapReference
    /// @tparam T Type to process
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/utility/functional/unwrap_reference
    template<typename T>
    struct UnwrapReferenceDecay : UnwrapReference<typename Decay<T>::Type> {};

    #ifdef __WSTL_CXX11__
    /// @copydoc UnwrapReferenceDecay
    /// @since C++11
    template<typename T>
    using UnwrapReferenceDecayType = typename UnwrapReferenceDecay<T>::Type;
    #endif

    // Common type

    #ifdef __WSTL_CXX11__
    /// @brief Determines the common type among all types
    /// @tparam Ts Types to find common type among
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/common_type
    template<typename... Ts>
    struct CommonType;

    // One type

    template<typename T>
    struct CommonType<T> : CommonType<T, T> {};

    namespace __private {
        template<typename T1, typename T2>
        using __ConditionalResultType = decltype(false ? DeclareValue<T1>() : DeclareValue<T2>());

        template<typename, typename, typename = void>
        struct __DecayConditionalResult {};

        template<typename T1, typename T2>
        struct __DecayConditionalResult<T1, T2, VoidType<__ConditionalResultType<T1, T2>>> :
            public Decay<__ConditionalResultType<T1, T2>> {};

        template<typename T1, typename T2, typename = void>
        struct __CommonType2 : __DecayConditionalResult<const T1&, const T2&> {};

        template<typename T1, typename T2>
        struct __CommonType2<T1, T2, VoidType<__ConditionalResultType<T1, T2>>> : 
            public __DecayConditionalResult<T1, T2> {};
    }

    // Two types

    template<typename T1, typename T2>
    struct CommonType<T1, T2> : Conditional<IsSame<T1, typename Decay<T1>::Type>::Value &&
        IsSame<T2, typename Decay<T2>::Type>::Value, __private::__CommonType2<T1, T2>, 
        CommonType<typename Decay<T1>::Type, typename Decay<T2>::Type>>::Type {};

    // 3+ types

    namespace __private {
        template<typename AlwaysVoid, typename T1, typename T2, typename... R>
        struct __CommonTypeMany {};

        template<typename T1, typename T2, typename... R>
        struct __CommonTypeMany<VoidType<typename CommonType<T1, T2>::Type>, T1, T2, R...> :
            public CommonType<typename CommonType<T1, T2>::Type, R...> {};
    }

    template<typename T1, typename T2, typename... R>
    struct CommonType<T1, T2, R...> : __private::__CommonTypeMany<void, T1, T2, R...> {};

    #ifdef __WSTL_CXX11__
    /// @copydoc CommonType
    /// @since C++11
    template<typename... T>
    using CommonTypeType = typename CommonType<T...>::Type;
    #endif
    #endif
    
    // Make signed

    /// @brief Makes unsigned type signed
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/make_signed
    template<typename T>
    struct MakeSigned;

    template<typename T>
    struct MakeSigned { typedef T Type; };

    template<> struct MakeSigned<char> { typedef signed char Type; };
    template<> struct MakeSigned<unsigned char> { typedef signed char Type; };
    template<> struct MakeSigned<unsigned short> { typedef short Type; };
    template<> struct MakeSigned<unsigned int> { typedef int Type; };
    template<> struct MakeSigned<unsigned long> { typedef long Type; };
    template<> struct MakeSigned<unsigned long long> { typedef long long Type; };

    template<> 
    struct MakeSigned<wchar_t> {
        typedef Conditional<sizeof(wchar_t) == sizeof(int16_t), int16_t, 
            Conditional<sizeof(wchar_t) == sizeof(int32_t), int32_t, void>::Type>::Type Type;
    };

    template<typename T>
    struct MakeSigned<const T> { typedef const typename MakeSigned<T>::Type Type; };

    template<typename T>
    struct MakeSigned<volatile T> { typedef volatile typename MakeSigned<T>::Type Type; };

    template<typename T>
    struct MakeSigned<const volatile T> { typedef const volatile typename MakeSigned<T>::Type Type; };

    #ifdef __WSTL_CXX20__
    template<> struct MakeSigned<char8_t> { typedef signed char Type; };
    #endif

    #ifdef __WSTL_CXX11__
    template<> struct MakeSigned<char16_t> { typedef short Type; };
    template<> struct MakeSigned<char32_t> { typedef int Type; };

    /// @copydoc MakeSigned
    /// @since C++11
    template<typename T>
    using MakeSignedType = typename MakeSigned<T>::Type;
    #endif
    
    // Make unsigned

    /// @brief Makes signed type unsigned
    /// @tparam T Type to convert
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/make_unsigned
    template<typename T>
    struct MakeUnsigned;

    template<typename T>
    struct MakeUnsigned { typedef T Type; };

    template<> struct MakeUnsigned<char> { typedef unsigned char Type; };
    template<> struct MakeUnsigned<signed char> { typedef unsigned char Type; };
    template<> struct MakeUnsigned<short> { typedef unsigned short Type; };
    template<> struct MakeUnsigned<int> { typedef unsigned int Type; };
    template<> struct MakeUnsigned<long> { typedef unsigned long Type; };
    template<> struct MakeUnsigned<long long> { typedef unsigned long long Type; };

    template<> 
    struct MakeUnsigned<wchar_t> {
        typedef Conditional<sizeof(wchar_t) == sizeof(uint16_t), uint16_t, 
            Conditional<sizeof(wchar_t) == sizeof(uint32_t), uint32_t, void>::Type>::Type Type;
    };

    template<typename T>
    struct MakeUnsigned<const T> { typedef const typename MakeUnsigned<T>::Type Type; };

    template<typename T>
    struct MakeUnsigned<volatile T> { typedef volatile typename MakeUnsigned<T>::Type Type; };

    template<typename T>
    struct MakeUnsigned<const volatile T> { typedef const volatile typename MakeUnsigned<T>::Type Type; };

    #ifdef __WSTL_CXX20__
    template<> struct MakeUnsigned<char8_t> { typedef unsigned char Type; };
    #endif

    #ifdef __WSTL_CXX11__
    template<> struct MakeUnsigned<char16_t> { typedef unsigned short Type; };
    template<> struct MakeUnsigned<char32_t> { typedef unsigned int Type; };

    /// @copydoc MakeUnsigned
    /// @since C++11
    template<typename T>
    using MakeUnsignedType = typename MakeUnsigned<T>::Type;
    #endif

    // Type with alignment

    /// @brief Provides a type with specified alignment
    /// @tparam Alignment Alignment in bytes
    /// @ingroup type_traits
    template<size_t Alignment>
    struct TypeWithAlignment;

    #ifdef __WSTL_CXX11__
    template<size_t Alignment>
    struct TypeWithAlignment {
        typedef struct { alignas(Alignment) char __Dummy; } Type;
    };

    /// @copydoc TypeWithAlignment
    /// @since C++11
    template<size_t Alignment>
    using TypeWithAlignmentType = typename TypeWithAlignment<Alignment>::Type;
    #else
    #define __MATCH_ALIGNMENT(T) \
        template<> struct TypeWithAlignment<AlignmentOf<T>::Value> { typedef T Type; }

    __MATCH_ALIGNMENT(int_least8_t);
    __MATCH_ALIGNMENT(int_least16_t);
    __MATCH_ALIGNMENT(int32_t);
    __MATCH_ALIGNMENT(int64_t);
    __MATCH_ALIGNMENT(float);
    __MATCH_ALIGNMENT(double);
    __MATCH_ALIGNMENT(void*);

    #undef __MATCH_ALIGNMENT
    #endif

    // Is type aligned

    /// @brief Checks whether type is aligned to specified alignment
    /// @tparam T Type to check
    /// @tparam Alignment Alignment to check against in bytes
    /// @ingroup type_traits
    template<typename T, size_t Alignment>
    struct IsTypeAligned : BoolConstant<((Alignment % AlignmentOf<T>::Value) == 0)> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsTypeAligned
    /// @since C++17
    template<typename T, size_t Alignment>
    inline constexpr bool IsTypeAlignedValue = IsTypeAligned<T, Alignment>::Value;
    #endif

    // Aligned storage

    /// @brief Provides a storage type with specified size and alignment
    /// @tparam Length Size in bytes
    /// @tparam Alignment Alignment in bytes
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/aligned_storage
    template<size_t Length, size_t Alignment>
    struct AlignedStorage {
        struct Type {
            /// @brief Conversion operator to reference type
            /// @tparam T Type to convert to
            /// @return Reference to the data
            template<typename T>
            operator T&() {
                WSTL_STATIC_ASSERT((IsSame<T*, void*>::Value || IsTypeAligned<T, Alignment>::Value), "Incompatible alignment");
                return *reinterpret_cast<T*>(Data);
            }
            
            /// @brief Conversion operator to const reference type
            /// @tparam T Type to convert to
            /// @return Const reference to the data
            template<typename T>
            operator const T&() const {
                WSTL_STATIC_ASSERT((IsSame<T*, void*>::Value || IsTypeAligned<T, Alignment>::Value), "Incompatible alignment");
                return *reinterpret_cast<const T*>(Data);
            }

            /// @brief Conversion operator to pointer type
            /// @tparam T Type to convert to
            /// @return Pointer to the data
            template<typename T>
            operator T*() {
                WSTL_STATIC_ASSERT((IsSame<T*, void*>::Value || IsTypeAligned<T, Alignment>::Value), "Incompatible alignment");
                return reinterpret_cast<T*>(Data);
            }

            /// @brief Conversion operator to const pointer type
            /// @tparam T Type to convert to
            /// @return Const pointer to the data
            template<typename T>
            operator const T*() const {
                WSTL_STATIC_ASSERT((IsSame<T*, void*>::Value || IsTypeAligned<T, Alignment>::Value), "Incompatible alignment");
                return reinterpret_cast<const T*>(Data);
            }

            /// @brief Gets pointer to the data
            /// @tparam T Type to convert to
            /// @return Pointer to the data
            template<typename T>
            T* GetPointer() {
                WSTL_STATIC_ASSERT((IsSame<T*, void*>::Value || IsTypeAligned<T, Alignment>::Value), "Incompatible alignment");
                return reinterpret_cast<T*>(Data);
            }

            /// @brief Gets const pointer to the data
            /// @tparam T Type to convert to
            /// @return Const pointer to the data
            template<typename T>
            const T* GetPointer() const {
                WSTL_STATIC_ASSERT((IsSame<T*, void*>::Value || IsTypeAligned<T, Alignment>::Value), "Incompatible alignment");
                return reinterpret_cast<const T*>(Data);
            }

            /// @brief Gets reference to the data
            /// @tparam T Type to convert to
            /// @return Reference to the data
            template<typename T>
            T& GetReference() {
                WSTL_STATIC_ASSERT((IsSame<T*, void*>::Value || IsTypeAligned<T, Alignment>::Value), "Incompatible alignment");
                return *reinterpret_cast<T*>(Data);
            }

            /// @brief Gets const reference to the data
            /// @tparam T Type to convert to
            /// @return Const reference to the data
            template<typename T>
            const T& GetReference() const {
                WSTL_STATIC_ASSERT((IsSame<T*, void*>::Value || IsTypeAligned<T, Alignment>::Value), "Incompatible alignment");
                return *reinterpret_cast<const T*>(Data);
            }

            #ifdef __WSTL_CXX11__
            alignas(Alignment) char Data[Length];
            #else
            union {
                char Data[Length];
                typename TypeWithAlignment<Alignment>::Type __Aligner;
            };
            #endif
        };
    };

    #ifdef __WSTL_CXX11__
    /// @copydoc AlignedStorage
    /// @since C++11
    template<size_t Length, size_t Alignment>
    using AlignedStorageType = typename AlignedStorage<Length, Alignment>::Type;
    #endif

    // Is aligned

    /// @brief Checks whether pointer is aligned to specified alignment
    /// @param pointer Pointer to check
    /// @param alignment Alignment in bytes
    /// @ingroup type_traits
    inline bool IsAligned(const void* pointer, size_t alignment) {
        return (reinterpret_cast<uintptr_t>(pointer) % alignment) == 0;
    }

    /// @brief Checks whether pointer is aligned to specified alignment
    /// @tparam Alignment Alignment in bytes
    /// @param pointer Pointer to check
    /// @ingroup type_traits
    template<size_t Alignment>
    inline bool IsAligned(const void* pointer) {
        return (reinterpret_cast<uintptr_t>(pointer) % Alignment) == 0;
    }

    /// @brief Checks whether pointer has the same alignment as specified type
    /// @tparam T Type to check against
    /// @param pointer Pointer to check
    /// @ingroup type_traits
    template<typename T>
    inline bool IsAligned(const void* pointer) {
        return IsAligned<AlignmentOf<T>::Value>(pointer);
    }

    // Nth type

    #ifdef __WSTL_CXX11__
    namespace __private {
        template<size_t Index, typename T, typename... Ts>
        struct __NthType {
            typedef typename __NthType<Index - 1, Ts...>::Type Type;
        };

        template<typename T, typename... Ts>
        struct __NthType<0, T, Ts...> {
            typedef T Type;
        };
    }

    /// @brief Extracts the N-th type from a parameter pack
    /// @tparam Index Zero-based index of the type to extract
    /// @tparam Types Parameter pack to extract from
    /// @ingroup type_traits
    /// @since C++11
    template<size_t Index, typename... Types>
    struct NthType {
        WSTL_STATIC_ASSERT((Index < sizeof...(Types)), "Index out of bounds");

        typedef typename __private::__NthType<Index, Types...>::Type Type;
    };

    /// @copydoc NthType
    /// @since C++11
    template<size_t Index, typename... Types>
    using NthTypeType = typename NthType<Index, Types...>::Type;
    #endif

    #ifdef __WSTL_CXX11__
    // Invoke result

    /// @warning Include `Functional.hpp` to use it
    template<typename T>
    class ReferenceWrapper;

    namespace __private {
        template<typename T>
        constexpr T&& __Forward(RemoveReferenceType<T>& t) __WSTL_NOEXCEPT__ {
            return static_cast<T&&>(t);
        }

        template<typename T>
        constexpr T&& __Forward(RemoveReferenceType<T>&& t) __WSTL_NOEXCEPT__ {
            WSTL_STATIC_ASSERT(!IsLValueReference<T>::Value, "Invalid conversion rvalue to lvalue");
            return static_cast<T&&>(t);
        }

        template<typename T>
        struct __IsReferenceWrapper : FalseType {};

        template<typename U>
        struct __IsReferenceWrapper<ReferenceWrapper<U>> : TrueType {};

        // Suppress warnings related to noexcept specification because without it everything was fine
        WSTL_DIAGNOSTIC_PUSH

        #if defined(__WSTL_CLANG__)
            WSTL_DIAGNOSTIC_IGNORE("-Wimplicit-int-conversion")
            WSTL_DIAGNOSTIC_IGNORE("-Wshorten-64-to-32")
        #elif defined(__WSTL_GCC__)
            WSTL_DIAGNOSTIC_IGNORE("-Wconversion")
        #elif defined(__WSTL_MSVC__)
            WSTL_DIAGNOSTIC_IGNORE(4244)
        #endif

        // General callable

        template<typename Function, typename... Args>
        constexpr auto __Invoke(Function&& function, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
            noexcept((__Forward<Function>(function))(__Forward<Args>(args)...))
        ) -> EnableIfType<!IsMemberPointer<Function>::Value, 
        decltype((__Forward<Function>(function))(__Forward<Args>(args)...))> {
            return (__Forward<Function>(function))(__Forward<Args>(args)...);
        }

        // Member function pointer, object

        template<typename Function, typename Object, typename... Args>
        constexpr auto __Invoke(Function&& function, Object&& object, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
            noexcept((__Forward<Object>(object).*function)(__Forward<Args>(args)...))
        ) -> EnableIfType<!__private::__IsReferenceWrapper<DecayType<Object>>::Value && !IsPointer<DecayType<Object>>::Value && 
        IsMemberFunctionPointer<DecayType<Function>>::Value, decltype((__Forward<Object>(object).*function)(__Forward<Args>(args)...))> {
            return (__Forward<Object>(object).*function)(__Forward<Args>(args)...);
        }

        // Member function pointer, reference wrapper

        template<typename Function, typename Reference, typename... Args>
        constexpr auto __Invoke(Function&& function, Reference&& reference, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
            noexcept((reference.Get().*function)(__Forward<Args>(args)...))
        ) -> EnableIfType<__private::__IsReferenceWrapper<DecayType<Reference>>::Value && 
        IsMemberFunctionPointer<DecayType<Function>>::Value, decltype((reference.Get().*function)(__Forward<Args>(args)...))> {
            return (reference.Get().*function)(__Forward<Args>(args)...);
        }

        // Member function pointer, pointer

        template<typename Function, typename Pointer, typename... Args>
        constexpr auto __Invoke(Function&& function, Pointer&& pointer, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
            noexcept(((*__Forward<Pointer>(pointer)).*function)(__Forward<Args>(args)...))
        ) -> EnableIfType<IsPointer<DecayType<Pointer>>::Value && 
        IsMemberFunctionPointer<DecayType<Function>>::Value, decltype(((*__Forward<Pointer>(pointer)).*function)(__Forward<Args>(args)...))> {
            return ((*__Forward<Pointer>(pointer)).*function)(__Forward<Args>(args)...);
        }

        // Member object pointer, object

        template<typename Function, typename Object>
        constexpr auto __Invoke(Function&& function, Object&& object) __WSTL_NOEXCEPT_EXPR__(
            noexcept(__Forward<Object>(object).*function)
        ) -> EnableIfType<IsMemberObjectPointer<DecayType<Function>>::Value && !IsPointer<DecayType<Object>>::Value && 
        !__private::__IsReferenceWrapper<DecayType<Object>>::Value, decltype(__Forward<Object>(object).*function)> {
            return __Forward<Object>(object).*function;
        }

        // Member object pointer, reference wrapper

        template<typename Function, typename Reference>
        constexpr auto __Invoke(Function&& function, Reference&& reference) __WSTL_NOEXCEPT_EXPR__(
            noexcept(reference.Get().*function)
        ) -> EnableIfType<IsMemberObjectPointer<DecayType<Function>>::Value && 
        __private::__IsReferenceWrapper<DecayType<Reference>>::Value, decltype(reference.Get().*function)> {
            return reference.Get().*function;
        }

        // Member object pointer, pointer

        template<typename Function, typename Pointer>
        constexpr auto __Invoke(Function&& function, Pointer&& pointer) __WSTL_NOEXCEPT_EXPR__(
            noexcept((*__Forward<Pointer>(pointer)).*function)
        ) -> EnableIfType<IsMemberObjectPointer<DecayType<Function>>::Value && 
        IsPointer<DecayType<Pointer>>::Value, decltype((*__Forward<Pointer>(pointer)).*function)> {
            return (*__Forward<Pointer>(pointer)).*function;
        }

        WSTL_DIAGNOSTIC_POP

        template<typename, typename Function, typename... Args>
        struct __InvokeResult {};

        template<typename Function, typename... Args>
        struct __InvokeResult<VoidType<decltype(__Invoke(DeclareValue<Function>(), DeclareValue<Args>()...))>, Function, Args...> {
            using Type = decltype(__Invoke(DeclareValue<Function>(), DeclareValue<Args>()...));
        };
    }

    /// @brief Determines the result type of invoking a callable with specified arguments
    /// @tparam Function Type of the callable to invoke
    /// @tparam ...Args Types of the arguments to invoke with
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/result_of
    template<typename Function, typename... Args>
    struct InvokeResult : __private::__InvokeResult<void, Function, Args...> {};

    /// @copydoc InvokeResult
    /// @since C++11
    template<typename Function, typename... Args>
    using InvokeResultType = typename InvokeResult<Function, Args...>::Type;

    // Is invocable
    
    namespace __private {
        template<typename, typename Function, typename... Args>
        struct __IsInvocable : FalseType {};

        template<typename Function, typename... Args>
        struct __IsInvocable<VoidType<decltype(__Invoke(DeclareValue<Function>(), DeclareValue<Args>()...))>, Function, Args...> : TrueType {};
    }

    /// @brief Checks whether a callable can be invoked with specified arguments
    /// @tparam Function Type of the callable to invoke
    /// @tparam ...Args Types of the arguments to invoke with
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_invocable
    template<typename Function, typename... Args>
    struct IsInvocable : __private::__IsInvocable<void, Function, Args...> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsInvocable
    /// @since C++17
    template<typename Function, typename... Args>
    inline constexpr bool IsInvocableValue = IsInvocable<Function, Args...>::Value;
    #endif

    // Is invocable return

    namespace __private {
        template<typename, typename Result, typename Function, typename... Args>
        struct __IsInvocableReturn : FalseType {};

        template<typename Result, typename Function, typename... Args>
        struct __IsInvocableReturn<VoidType<decltype(__Invoke(DeclareValue<Function>(), DeclareValue<Args>()...))>, Result, Function, Args...> 
            : BoolConstant<IsVoid<Result>::Value || IsConvertible<decltype(__Invoke(DeclareValue<Function>(), DeclareValue<Args>()...)), Result>::Value> {};
    }

    /// @brief Checks whether a callable can be invoked with specified arguments and return type is convertible to specified type
    /// @tparam Result Type to check return type against
    /// @tparam Function Type of the callable to invoke
    /// @tparam ...Args Types of the arguments to invoke with
    /// @ingroup type_traits
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/types/is_invocable
    template<typename Result, typename Function, typename... Args>
    struct IsInvocableReturn : __private::__IsInvocableReturn<void, Result, Function, Args...> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsInvocableReturn
    /// @since C++17
    template<typename Result, typename Function, typename... Args>
    inline constexpr bool IsInvocableReturnValue = IsInvocableReturn<Result, Function, Args...>::Value;
    #endif

    #ifdef __WSTL_EXCEPTIONS__
    #ifdef __WSTL_CXX17__
    // Is nothrow invocable

    namespace __private {
        template<typename, typename Function, typename... Args>
        struct __IsNothrowInvocable : FalseType {};

        template<typename Function, typename... Args>
        struct __IsNothrowInvocable<VoidType<decltype(__Invoke(DeclareValue<Function>(), DeclareValue<Args>()...))>, Function, Args...> : 
            BoolConstant<noexcept(__Invoke(DeclareValue<Function>(), DeclareValue<Args>()...))> {};
    }

    /// @brief Checks whether a callable can be invoked with specified arguments and not throw any exceptions
    /// @details Only available from C++17 and higher because noexcept doesn't work the same in lower versions
    /// @tparam Function Type of the callable to invoke
    /// @tparam ...Args Types of the arguments to invoke with
    /// @ingroup type_traits
    /// @since C++17
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_invocable
    template<typename Function, typename... Args>
    struct IsNothrowInvocable : __private::__IsNothrowInvocable<void, Function, Args...> {};

    /// @copydoc IsNothrowInvocable
    /// @since C++17
    template<typename Function, typename... Args>
    inline constexpr bool IsNothrowInvocableValue = IsNothrowInvocable<Function, Args...>::Value;

    // Is nothrow invocable return

    namespace __private {
        template<typename, typename Result, typename Function, typename... Args>
        struct __IsNothrowInvocableReturn : FalseType {};

        template<typename Result, typename Function, typename... Args>
        struct __IsNothrowInvocableReturn<VoidType<decltype(static_cast<Result>(__Invoke(DeclareValue<Function>(), DeclareValue<Args>()...)))>, Result, Function, Args...> : 
            BoolConstant<noexcept(static_cast<Result>(__Invoke(DeclareValue<Function>(), DeclareValue<Args>()...)))> {};

        template<typename Function, typename... Args>
        struct __IsNothrowInvocableReturn<VoidType<decltype(__Invoke(DeclareValue<Function>(), DeclareValue<Args>()...))>, void, Function, Args...> : 
            BoolConstant<noexcept(__Invoke(DeclareValue<Function>(), DeclareValue<Args>()...))> {};
    }

    /// @brief Checks whether a callable can be invoked with specified arguments 
    /// and return type is convertible to specified type, and not throw any exceptions
    /// @details Only available from C++17 and higher because noexcept doesn't work the same in lower versions
    /// @tparam Result Type to check return type against
    /// @tparam Function Type of the callable to invoke
    /// @tparam ...Args Types of the arguments to invoke with
    /// @ingroup type_traits
    /// @since C++17
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/types/is_invocable
    template<typename Result, typename Function, typename... Args>
    struct IsNothrowInvocableReturn : __private::__IsNothrowInvocableReturn<void, Result, Function, Args...> {};

    /// @copydoc IsNothrowInvocableReturn
    /// @since C++17
    template<typename Result, typename Function, typename... Args>
    inline constexpr bool IsNothrowInvocableReturnValue = IsNothrowInvocableReturn<Result, Function, Args...>::Value;
    #endif
    #endif
    #endif

    // Underlying type

    #if !defined(__WSTL_TYPETRAITS_NO_BUILTINS__) && defined(__WSTL_CXX11__) && defined(__WSTL_SUPPORTED_COMPILER__)
    /// @brief Extracts underlying type from enumeration type
    /// @tparam T Enumeration type to extract from
    /// @ingroup type_traits
    /// @since C++11
    /// @note This trait uses `__underlying_type` builtin, but if it's not available, 
    /// it will fall back to a less accurate implementation that may not work correctly in all cases
    /// and return `int` as underlying type for all enums. Besides C++11, also requires supported compiler.
    /// @see https://en.cppreference.com/w/cpp/types/underlying_type
    template<typename T, bool = IsEnum<T>::Value>
    struct UnderlyingType;

    template<typename T>
    struct UnderlyingType<T, false> {
        WSTL_STATIC_ASSERT(IsEnum<T>::Value, "Provided type must be enumeration type");
    };

    template<typename T>
    struct UnderlyingType<T, true> {
        typedef __underlying_type(T) Type;
    };
    #else
    /// @brief Extracts underlying type from enumeration type, less accurate fallback implementation
    /// @tparam T Enumeration type to extract from
    /// @ingroup type_traits
    /// @see https://en.cppreference.com/w/cpp/types/underlying_type
    template<typename T>
    struct UnderlyingType {
        typedef int Type;
    };
    #endif

    #ifdef __WSTL_CXX11__
    /// @copydoc UnderlyingType
    /// @since C++11
    template<typename T>
    using UnderlyingTypeType = typename UnderlyingType<T>::Type;
    #endif
}

#endif