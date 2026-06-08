// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s move implementation,
// licensed under the MIT License, with minor modifications made for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for full details.

#ifndef __WSTL_MOVE_HPP__
#define __WSTL_MOVE_HPP__

#include "Platform.hpp"
#include "../TypeTraits.hpp"
#include "../StaticAssert.hpp"


namespace wstl {
    #ifdef __WSTL_CXX11__
    // Forward

    template<typename T>
    /// @brief Forwards an object to another function or context
    /// @param t Object to forward
    /// @return Forwarded object
    /// @ingroup utility
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/forward
    constexpr T&& Forward(RemoveReferenceType<T>& t) __WSTL_NOEXCEPT__ {
        return __private::__Forward<T>(t);
    }

    /// @copydoc Forward(wstl::RemoveReferenceType<T>&)
    template<typename T>
    constexpr T&& Forward(RemoveReferenceType<T>&& t) __WSTL_NOEXCEPT__ {
        return __private::__Forward<T>(t);
    }
    
    // Move
    
    template<typename T>
    /// @brief Converts an object to rvalue reference
    /// @param t Object to convert
    /// @return Rvalue reference to the object
    /// @ingroup utility
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/move
    constexpr RemoveReferenceType<T>&& Move(T&& t) __WSTL_NOEXCEPT__ {
        return static_cast<RemoveReferenceType<T>&&>(t);
    }

    // Move if noexcept

    #ifdef __WSTL_EXCEPTIONS__
    template<typename T>
    /// @brief Converts an object to rvalue reference if the move constructor is noexcept
    /// @param t Object to convert
    /// @return Rvalue reference to the object if the move constructor is noexcept, otherwise const lvalue reference
    /// @ingroup utility
    /// @since C++11
    /// @note Requires `__WSTL_EXCEPTIONS__` to be defined
    /// @see https://en.cppreference.com/w/cpp/utility/move_if_noexcept
    constexpr EnableIfType<IsNothrowMoveConstructible<T>::Value || !IsCopyConstructible<T>::Value, T&&> MoveIfNoexcept(T& t) __WSTL_NOEXCEPT__ {
        return Move(t);
    }

    template<typename T>
    /// @copydoc MoveIfNoexcept(T&)
    constexpr EnableIfType<!IsNothrowMoveConstructible<T>::Value && IsCopyConstructible<T>::Value, const T&> MoveIfNoexcept(T& t) __WSTL_NOEXCEPT__ {
        return t;
    }
    #endif

    // Forward like

    // T is const & lvalue

    /// @brief Forwards an object with a value category and constness similar to `T`
    /// @tparam T Type to mimick
    /// @tparam U Type of the object to be forwarded
    /// @param u Object to forward
    /// @return Forwarded object
    /// @ingroup utility
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/forward_like
    template<typename T, typename U>
    constexpr EnableIfType<IsConst<RemoveReferenceType<T>>::Value && IsLValueReference<T>::Value, const RemoveReferenceType<U>&> ForwardLike(U&& u) __WSTL_NOEXCEPT__ {
        return static_cast<const RemoveReferenceType<U>&>(u);
    }

    // T is const & rvalue

    /// @copydoc ForwardLike(U&&)
    template<typename T, typename U>
    constexpr EnableIfType<IsConst<RemoveReferenceType<T>>::Value && !IsLValueReference<T>::Value, const RemoveReferenceType<U>&&> ForwardLike(U&& u) __WSTL_NOEXCEPT__ {
        return static_cast<const RemoveReferenceType<U>&&>(u);
    }

    // T is not const & lvalue

    /// @copydoc ForwardLike(U&&)
    template<typename T, typename U>
    constexpr EnableIfType<!IsConst<RemoveReferenceType<T>>::Value && IsLValueReference<T>::Value, RemoveReferenceType<U>&> ForwardLike(U&& u) __WSTL_NOEXCEPT__ {
        return static_cast<RemoveReferenceType<U>&>(u);
    }

    // T is not const & rvalue

    /// @copydoc ForwardLike(U&&)
    template<typename T, typename U>
    constexpr EnableIfType<!IsConst<RemoveReferenceType<T>>::Value && !IsLValueReference<T>::Value, RemoveReferenceType<U>&&> ForwardLike(U&& u) __WSTL_NOEXCEPT__ {
        return static_cast<RemoveReferenceType<U>&&>(u);
    }

    /// @brief Determines the type resulting from forwarding an object like `T` with a value of type `U`
    /// @tparam T Type to mimick
    /// @tparam U Type of the object to be forwarded
    /// @ingroup utility
    /// @since C++11
    template<typename T, typename U>
    using ForwardLikeType = decltype(ForwardLike<T>(DeclareValue<U&>()));
    #endif
}

#endif