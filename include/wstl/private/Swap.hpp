// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_SWAP_HPP__
#define __WSTL_SWAP_HPP__

#include "Platform.hpp"
#include "Move.hpp"
#include "../TypeTraits.hpp"


namespace wstl {
    namespace __private {
        #ifdef __WSTL_CXX11__
        template<typename, typename = void>
        struct __HasMemberSwap : FalseType {};

        template<typename T>
        struct __HasMemberSwap<T, VoidType<decltype(DeclareValue<T&>().Swap(DeclareValue<T&>()))>> : TrueType {};

        template<typename T>
        __WSTL_CONSTEXPR14__ inline EnableIfType<__HasMemberSwap<T>::Value> __Swap(T& a, T& b) __WSTL_NOEXCEPT_EXPR__(noexcept(a.Swap(b))) {
            a.Swap(b);
        }

        template<typename T>
        __WSTL_CONSTEXPR14__ EnableIfType<!__HasMemberSwap<T>::Value> __Swap(T& a, T& b) __WSTL_NOEXCEPT_EXPR__(IsNothrowMoveConstructible<T>::Value && IsNothrowMoveAssignable<T>::Value) {
            T temp = Move(a);
            a = Move(b);
            b = Move(temp);
        }
        #else
        template<typename T>
        inline void __Swap(T& a, T& b, char (*)[sizeof(&T::Swap)] = 0) {
            a.Swap(b);
        }

        template<typename T>
        void __Swap(T& a, T& b, ...) {
            T temp = a;
            a = b;
            b = temp;
        }
        #endif
    }
    
    template<typename T>
    /// @brief Swaps the values of two objects
    /// @details Also has the ability to handle types with defined `.Swap()` method by effectively calling it
    /// @tparam T Type of the objects to swap
    /// @param a First object to swap
    /// @param b Second object to swap
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/swap
    __WSTL_CONSTEXPR14__ inline void Swap(T& a, T&b) __WSTL_NOEXCEPT_EXPR__(noexcept(__private::__Swap(a, b))) {
        #ifdef __WSTL_CXX11__
        __private::__Swap(a, b);
        #else
        __private::__Swap(a, b, 0);
        #endif
    }

    template<typename T, size_t N>
    /// @brief Swaps the values of two arrays
    /// @tparam T Type of the elements in the arrays
    /// @tparam N Size of the arrays
    /// @param a First array to swap
    /// @param b Second array to swap
    /// @ingroup utility
    /// @see https://en.cppreference.com/w/cpp/utility/swap
    __WSTL_CONSTEXPR14__ void Swap(T (&a)[N], T (&b)[N]) __WSTL_NOEXCEPT_EXPR__(noexcept(Swap(*a, *b))) {
        for (size_t i = 0; i < N; ++i) Swap(a[i], b[i]);
    }
}

#endif