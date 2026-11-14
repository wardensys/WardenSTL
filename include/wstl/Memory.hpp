// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s memory.h,
// which are licensed under the MIT License.
//
// It has been restructured and adapted for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_MEMORY_HPP__
#define __WSTL_MEMORY_HPP__

#include "private/Platform.hpp"
#include "PlacementNew.hpp"
#include "Algorithm.hpp"

#ifdef __WSTL_MEMOPS_USE_LIBC__
#include <string.h>
#endif


/// @defgroup memory Memory
/// @brief A set of memory management utilities 
/// @ingroup wstl

// Defines introduced

/// @def __WSTL_MEMOPS_USE_LIBC__
/// @brief If defined, the library will use functions from the C standard library for memory operations
/// @ingroup memory
#ifdef __DOXYGEN__
    #define __WSTL_MEMOPS_USE_LIBC__ 
#endif

/// @def __WSTL_MEMOPS_USE_BUILTINS__
/// @brief If defined, the library will use compiler built-in functions for memory operations where available
/// @ingroup memory
#ifdef __DOXYGEN__
    #define __WSTL_MEMOPS_USE_BUILTINS__ 
#endif

namespace wstl {
    // Pointer traits

    namespace __private {
        template<typename T, typename U>
        static long __TestRebind(typename T::template Rebind<U>* = 0);

        template<typename T, typename U>
        static char __TestRebind(...);

        template<typename T, typename U, bool = (sizeof(__TestRebind<T, U>(0)) == sizeof(long))>
        struct __Rebind {
            typedef typename T::template Rebind<U>::Other Type;
        };

        #ifdef __WSTL_CXX11__
        template<template<typename, typename...> class P, typename T, typename U, typename... Args>
        struct __Rebind<P<T, Args...>, U, true> {
            typedef typename P<T, Args...>::template Rebind<U>::Other Type;
        };

        template<template<typename, typename...> class P, typename T, typename U, typename... Args>
        struct __Rebind<P<T, Args...>, U, false> {
            typedef P<U, Args...> Type;
        };
        #else
        template<template<typename> class P, typename T, typename U>
        struct __Rebind<P<T>, U, true> {
            typedef typename P<T>::template Rebind<U>::Other Type;
        };

        template<template<typename, typename> class P, typename T, typename U, typename Arg>
        struct __Rebind<P<T, Arg>, U, true> {
            typedef typename P<T, Arg>::template Rebind<U>::Other Type;
        };

        template<template<typename, typename, typename> class P, typename T, typename U, typename Arg1, typename Arg2>
        struct __Rebind<P<T, Arg1, Arg2>, U, true> {
            typedef typename P<T, Arg1, Arg2>::template Rebind<U>::Other Type;
        };

        template<template<typename, typename, typename, typename> class P, typename T, typename U, typename Arg1, typename Arg2, typename Arg3>
        struct __Rebind<P<T, Arg1, Arg2, Arg3>, U, true> {
            typedef typename P<T, Arg1, Arg2, Arg3>::template Rebind<U>::Other Type;
        };

        template<template<typename> class P, typename T, typename U>
        struct __Rebind<P<T>, U, false> {
            typedef P<U> Type;
        };

        template<template<typename, typename> class P, typename T, typename U, typename Arg>
        struct __Rebind<P<T, Arg>, U, false> {
            typedef P<U, Arg> Type;
        };

        template<template<typename, typename, typename> class P, typename T, typename U, typename Arg1, typename Arg2>
        struct __Rebind<P<T, Arg1, Arg2>, U, false> {
            typedef P<U, Arg1, Arg2> Type;
        };

        template<template<typename, typename, typename, typename> class P, typename T, typename U, typename Arg1, typename Arg2, typename Arg3>
        struct __Rebind<P<T, Arg1, Arg2, Arg3>, U, false> {
            typedef P<U, Arg1, Arg2, Arg3> Type;
        };
        #endif

        WSTL_DECLARE_TYPEDEF_TEST(__TestElementType, ElementType)
        WSTL_DECLARE_TYPEDEF_TEST(__TestDifferenceType, DifferenceType)

        template<typename P, bool = WSTL_TYPEDEF_TEST_RESULT(__TestElementType, P, 1)>
        struct __PointerTraitsElementType;

        template<typename P>
        struct __PointerTraitsElementType<P, true> {
            typedef typename P::ElementType Type;
        };

        #ifdef __WSTL_CXX11__
        template<template<typename, typename...> class P, typename E, typename... Args>
        struct __PointerTraitsElementType<P<E, Args...>, false> {
            typedef E Type;
        };
        #else
        template<template<typename, typename, typename, typename> class P, typename E, typename A1, typename A2, typename A3>
        struct __PointerTraitsElementType<P<E, A1, A2, A3>, false> {
            typedef E Type;
        };

        template<template<typename, typename, typename> class P, typename E, typename A1, typename A2>
        struct __PointerTraitsElementType<P<E, A1, A2>, false> {
            typedef E Type;
        };

        template<template<typename, typename> class P, typename E, typename A>
        struct __PointerTraitsElementType<P<E, A>, false> {
            typedef E Type;
        };

        template<template<typename> class P, typename E>
        struct __PointerTraitsElementType<P<E>, false> {
            typedef E Type;
        };
        #endif

        template<typename P, bool = WSTL_TYPEDEF_TEST_RESULT(__TestDifferenceType, P, 1)>
        struct __PointerTraitsDifferenceType;

        template<typename P>
        struct __PointerTraitsDifferenceType<P, true> {
            typedef typename P::DifferenceType Type;
        };

        template<typename P>
        struct __PointerTraitsDifferenceType<P, false> {
            typedef ptrdiff_t Type;
        };
    }

    /// @brief Provides uniform interface for accessing properties of a pointer
    /// @tparam Pointer Type of the pointer
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/pointer_traits
    template<typename Pointer>
    struct PointerTraits {
        typedef Pointer PointerType;
        typedef typename __private::__PointerTraitsElementType<Pointer>::Type ElementType;
        typedef typename __private::__PointerTraitsDifferenceType<Pointer>::Type DifferenceType;

        /// @brief Rebinds the pointer to a different type
        /// @tparam U The new type to bind to
        template<typename U>
        struct Rebind { typedef typename __private::__Rebind<Pointer, U>::Type Other; };

        /// @brief Returns a pointer to the given reference
        static __WSTL_CONSTEXPR14__ PointerType PointerTo(ElementType& reference) {
            return Pointer::PointerTo(reference);
        }
    };

    /// @brief Provides uniform interface for accessing properties of a raw pointer
    /// @tparam T Type of the element pointed to
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/pointer_traits
    template<typename T>
    struct PointerTraits<T*> {
        typedef T* PointerType;
        typedef T ElementType;
        typedef ptrdiff_t DifferenceType;

        /// @brief Rebinds the pointer to a different type
        /// @tparam U The new type to bind to
        template<typename U>
        struct Rebind { typedef U* Other; };

        /// @brief Returns a pointer to the given reference
        static __WSTL_CONSTEXPR14__ PointerType PointerTo(ElementType& reference) __WSTL_NOEXCEPT__ {
            return AddressOf(reference);
        }
    };

    // Default delete

    /// @brief Default deleter for unique pointers
    /// @tparam T Type of the object being deleted
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/default_delete
    template<typename T>
    struct DefaultDelete {
        /// @brief Constructor
        __WSTL_CONSTEXPR__ DefaultDelete() __WSTL_NOEXCEPT__ {}

        /// @brief Templated copy constructor
        template<typename U>
        __WSTL_CONSTEXPR__ DefaultDelete(const DefaultDelete<U>&) __WSTL_NOEXCEPT__ {}

        /// @brief Calls `delete` on the pointer
        void operator()(T* pointer) const __WSTL_NOEXCEPT__ {
            delete pointer;
        }
    };

    /// @brief Default deleter for unique pointers to arrays
    /// @tparam T Type of the object being deleted
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/default_delete
    template<typename T>
    struct DefaultDelete<T[]> {
        /// @brief Constructor
        __WSTL_CONSTEXPR__ DefaultDelete() __WSTL_NOEXCEPT__ {}

        /// @brief Templated copy constructor
        template<typename U>
        __WSTL_CONSTEXPR__ DefaultDelete(const DefaultDelete<U[]>&) __WSTL_NOEXCEPT__ {}

        /// @brief Calls `delete[]` on the pointer
        template<typename U>
        void operator()(U* pointer) const {
            delete[] pointer;
        }
    };

    // Unique pointer

    template<typename T, typename Deleter = DefaultDelete<T> > 
    class UniquePointer;
    
    /// @brief A smart pointer that owns and manages another object through a pointer
    /// @details The object is destroyed and its memory deallocated when the `UniquePointer` goes out of scope
    /// @tparam Deleter The type of the deleter used to destroy the object
    /// @tparam T The type of the object being managed
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/unique_ptr
    template<typename T, typename Deleter>
    class UniquePointer {
    public:
        typedef Deleter DeleterType;
        typedef T ElementType;
        typedef T* PointerType;
        typedef T& ReferenceType;
        
        /// @brief Default constructor
        __WSTL_CONSTEXPR__ UniquePointer() __WSTL_NOEXCEPT__ : m_Pointer(__WSTL_NULLPTR__), m_Deleter() {}

        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        __WSTL_CONSTEXPR__ explicit UniquePointer(PointerType pointer) __WSTL_NOEXCEPT__ : m_Pointer(pointer), 
            m_Deleter() {}

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The object to move from
        UniquePointer(UniquePointer&& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()), 
            m_Deleter(Forward<DeleterType>(other.GetDeleter())) {}
        
        /// @brief Templated move constructor
        /// @param other The object to move from
        template<typename U, typename E, EnableIfType<IsConvertible<U*, T*>::Value && !IsArray<U>::Value &&
        IsConvertible<E, Deleter>::Value, int> = 0>
        UniquePointer(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()), 
            m_Deleter(Forward<E>(other.GetDeleter())) {}

        #else
        /// @brief Constructor from another unique pointer, transfers ownership
        /// @param other The object to copy from
        UniquePointer(UniquePointer& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()),
            m_Deleter(other.GetDeleter()) {}
        #endif

        /// @brief Parameterized constructor, copies deleter
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter used to destroy the object
        UniquePointer(PointerType pointer, const Deleter& deleter) __WSTL_NOEXCEPT__ : m_Pointer(pointer), 
            m_Deleter(deleter) {}

        #ifdef __WSTL_CXX11__
        /// @brief Parameterized constructor, moves deleter
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter used to destroy the object
        UniquePointer(PointerType pointer, Deleter&& deleter) __WSTL_NOEXCEPT__ : m_Pointer(pointer), 
            m_Deleter(Move(deleter)) {}
        #else
        /// @brief Parameterized constructor, pseudo-moves deleter
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter used to destroy the object
        UniquePointer(PointerType pointer, Deleter& deleter) __WSTL_NOEXCEPT__ : m_Pointer(pointer), 
            m_Deleter(deleter) {}
        #endif

        /// @brief Destructor
        ~UniquePointer() {
            Reset();
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The object to move from
        UniquePointer& operator=(UniquePointer&& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = Forward<DeleterType>(other.GetDeleter());
            }

            return *this;
        }

        /// @brief Templated move assignment operator
        /// @param other The object to move from
        template<typename U, typename E, EnableIfType<IsConvertible<U*, T*>::Value && !IsArray<U>::Value &&
        IsConvertible<E, Deleter>::Value, int> = 0>
        UniquePointer& operator=(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ {
            Reset(other.Release());
            m_Deleter = Forward<E>(other.GetDeleter());

            return *this;
        }
        #else
        /// @brief Assignment operator that transfers ownership from another unique pointer
        /// @param other The object to transfer ownership from
        UniquePointer& operator=(UniquePointer& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = other.m_Deleter;
            }

            return *this;
        }
        #endif

        /// @brief Assignment operator, assigns a null pointer
        UniquePointer& operator=(wstl::NullPointerType) __WSTL_NOEXCEPT__ {
            Reset();
            return *this;
        }

        /// @brief Releases the ownership of the managed object
        /// @return The pointer to the object being managed
        PointerType Release() __WSTL_NOEXCEPT__ {
            PointerType temp = m_Pointer;
            m_Pointer = PointerType();    
            return temp;
        }

        /// @brief Resets the managed object to a new pointer
        void Reset(PointerType pointer = PointerType()) __WSTL_NOEXCEPT__ {
            PointerType temp = m_Pointer;
            m_Pointer = pointer;
            if(temp) m_Deleter(temp);
        }

        /// @brief Swaps the contents of two `UniquePointer` objects
        /// @param other The object to swap with
        void Swap(UniquePointer& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Pointer, other.m_Pointer);
            wstl::Swap(m_Deleter, other.m_Deleter);
        }

        /// @brief Returns the managed pointer
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ PointerType Get() const __WSTL_NOEXCEPT__ {
            return m_Pointer;
        }

        /// @brief Returns reference to the deleter used to destroy the object
        __WSTL_NODISCARD__ Deleter& GetDeleter() __WSTL_NOEXCEPT__ {
            return m_Deleter;
        }

        /// @brief Returns const reference to the deleter used to destroy the object
        __WSTL_NODISCARD__ const Deleter& GetDeleter() const __WSTL_NOEXCEPT__ {
            return m_Deleter;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Returns true if the pointer is not null
        explicit operator bool() const __WSTL_NOEXCEPT__ {
            return m_Pointer != __WSTL_NULLPTR__;
        }
        #else
        /// @brief Returns true if the pointer is not null, workaround for C++98
        operator const void*() const __WSTL_NOEXCEPT__ {
            return m_Pointer ? m_Pointer : __WSTL_NULLPTR__;
        }
        #endif

        /// @brief Dereference operator
        /// @return A reference to the object pointed to by the pointer
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType operator*() const __WSTL_NOEXCEPT__ {
            return *Get();
        }

        /// @brief Arrow operator
        /// @return A pointer to the object pointed to by the pointer
        __WSTL_CONSTEXPR__ PointerType operator->() const __WSTL_NOEXCEPT__ {
            return Get();
        }

        /// @brief Subscript operator, accesses any element starting from the pointer
        /// @param index The index to access
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType operator[](size_t index) const {
            return m_Pointer[index];
        }

    private:
        PointerType m_Pointer;
        Deleter m_Deleter;

        UniquePointer(const UniquePointer&) __WSTL_DELETE__;
        UniquePointer& operator=(const UniquePointer&) __WSTL_DELETE__;
    };

    /// @brief A smart pointer that owns and manages another object through a pointer - specialization for reference deleters
    /// @details The object is destroyed and its memory deallocated when the `UniquePointer` goes out of scope
    /// @tparam Deleter The type of the deleter used to destroy the object, passed by reference
    /// @tparam T The type of the object being managed
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/unique_ptr
    template<typename T, typename Deleter>
    class UniquePointer<T, Deleter&> {
    public:
        typedef Deleter& DeleterType;
        typedef T ElementType;
        typedef T* PointerType;
        typedef T& ReferenceType;

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The object to move from
        UniquePointer(UniquePointer&& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()), 
            m_Deleter(Forward<DeleterType>(other.GetDeleter())) {}
        
        /// @brief Templated move constructor
        /// @param other The object to move from
        template<typename U, typename E, EnableIfType<IsConvertible<U*, T*>::Value && !IsArray<U>::Value &&
        IsSame<E, Deleter&>::Value, int> = 0>
        UniquePointer(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()), 
            m_Deleter(Forward<E>(other.GetDeleter())) {}

        #else
        /// @brief Constructor from another unique pointer, transfers ownership
        /// @param other The object to transfer ownership from
        UniquePointer(UniquePointer& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()),
            m_Deleter(other.GetDeleter()) {}
        #endif

        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter used to destroy the object
        UniquePointer(PointerType pointer, DeleterType deleter) __WSTL_NOEXCEPT__
            : m_Pointer(pointer), m_Deleter(deleter) {}

        /// @brief Destructor
        ~UniquePointer() {
            Reset();
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The object to move from
        UniquePointer& operator=(UniquePointer&& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = Forward<DeleterType>(other.GetDeleter());
            }

            return *this;
        }

        /// @brief Templated move assignment operator
        /// @param other The object to move from
        template<typename U, typename E, EnableIfType<IsConvertible<U*, T*>::Value && !IsArray<U>::Value &&
        IsSame<E, Deleter&>::Value, int> = 0>
        UniquePointer& operator=(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ {
            Reset(other.Release());
            m_Deleter = Forward<E>(other.GetDeleter());
                
            return *this;
        }
        #else
        /// @brief Assignment operator that transfers ownership from another unique pointer
        /// @param other The object to transfer ownership from
        UniquePointer& operator=(UniquePointer& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = other.m_Deleter;
            }

            return *this;
        }
        #endif

        /// @brief Assignment operator, assigns a null pointer
        UniquePointer& operator=(wstl::NullPointerType) __WSTL_NOEXCEPT__ {
            Reset();
            return *this;
        }

        /// @brief Releases the ownership of the managed object
        /// @return The pointer to the object being managed
        PointerType Release() __WSTL_NOEXCEPT__ {
            PointerType temp = m_Pointer;
            m_Pointer = PointerType();    
            return temp;
        }

        /// @brief Resets the managed object to a new pointer
        void Reset(PointerType pointer = PointerType()) __WSTL_NOEXCEPT__ {
            PointerType temp = m_Pointer;
            m_Pointer = pointer;
            if(temp) m_Deleter(temp);
        }

        /// @brief Swaps the contents of two `UniquePointer` objects
        /// @param other The object to swap with
        void Swap(UniquePointer& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Pointer, other.m_Pointer);
            wstl::Swap(m_Deleter, other.m_Deleter);
        }

        /// @brief Returns the managed pointer
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ PointerType Get() const __WSTL_NOEXCEPT__ {
            return m_Pointer;
        }

        /// @brief Returns reference to the deleter used to destroy the object
        __WSTL_NODISCARD__ DeleterType GetDeleter() const __WSTL_NOEXCEPT__ {
            return m_Deleter;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Returns true if the pointer is not null
        explicit operator bool() const __WSTL_NOEXCEPT__ {
            return m_Pointer != __WSTL_NULLPTR__;
        }
        #else
        /// @brief Returns true if the pointer is not null, workaround for C++98
        operator const void*() const __WSTL_NOEXCEPT__ {
            return m_Pointer ? m_Pointer : __WSTL_NULLPTR__;
        }
        #endif

        /// @brief Dereference operator
        /// @return A reference to the object pointed to by the pointer
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType operator*() const __WSTL_NOEXCEPT__ {
            return *Get();
        }

        /// @brief Arrow operator
        /// @return A pointer to the object pointed to by the pointer
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ PointerType operator->() const __WSTL_NOEXCEPT__ {
            return Get();
        }

        /// @brief Subscript operator, accesses any element starting from the pointer
        /// @param index The index to access
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType operator[](size_t index) const {
            return m_Pointer[index];
        }

    private:
        PointerType m_Pointer;
        DeleterType m_Deleter;

        UniquePointer(const UniquePointer&) __WSTL_DELETE__;
        UniquePointer& operator=(const UniquePointer&) __WSTL_DELETE__;
    };

    /// @brief A smart pointer that owns and manages another object through a pointer - specialization for arrays
    /// @details The object is destroyed and its memory deallocated when the `UniquePointer` goes out of scope
    /// @tparam Deleter The type of the deleter used to destroy the object
    /// @tparam T The type of the object being managed
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/unique_ptr
    template<typename T, typename Deleter>
    class UniquePointer<T[], Deleter> {
    public:
        typedef Deleter DeleterType;
        typedef T ElementType;
        typedef T* PointerType;
        typedef T& ReferenceType;

        /// @brief Default constructor
        __WSTL_CONSTEXPR__ UniquePointer() __WSTL_NOEXCEPT__ : m_Pointer(__WSTL_NULLPTR__) {}

        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        __WSTL_CONSTEXPR__ UniquePointer(PointerType pointer) __WSTL_NOEXCEPT__ : m_Pointer(pointer) {}

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The object to move from
        UniquePointer(UniquePointer&& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()), 
            m_Deleter(Forward<DeleterType>(other.GetDeleter())) {}

        /// @brief Templated move constructor
        /// @param other The object to move from
        template<typename U, typename E, EnableIfType<IsConvertible<RemoveAllExtentsType<U>*, RemoveAllExtentsType<T>*>::Value && 
        IsArray<U>::Value && IsConvertible<E, Deleter>::Value, int> = 0>
        UniquePointer(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()), 
            m_Deleter(Forward<E>(other.GetDeleter())) {}

        #else
        /// @brief Copy constructor
        /// @param other The object to copy from
        UniquePointer(UniquePointer& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()), 
            m_Deleter(other.GetDeleter()) {}

        #endif

        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter to use to destroy the object
        UniquePointer(PointerType pointer, const Deleter& deleter) __WSTL_NOEXCEPT__ : m_Pointer(pointer), 
            m_Deleter(deleter) {}

        #ifdef __WSTL_CXX11__
        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter to use to destroy the object
        UniquePointer(PointerType pointer, Deleter&& deleter) __WSTL_NOEXCEPT__ : m_Pointer(pointer), 
            m_Deleter(Move(deleter)) {}
        #else
        /// @brief Parameterized constructor, pseudo-moves deleter
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter used to destroy the object
        UniquePointer(PointerType pointer, Deleter& deleter) __WSTL_NOEXCEPT__ : m_Pointer(pointer), 
            m_Deleter(deleter) {}
        #endif
        
        /// @brief Destructor
        ~UniquePointer() {
            Reset();
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The object to move from
        UniquePointer& operator=(UniquePointer&& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = Forward<DeleterType>(other.GetDeleter());
            }

            return *this;
        }

        /// @brief Templated move assignment operator
        /// @param other The object to move from
        template<typename U, typename E, EnableIfType<IsConvertible<RemoveAllExtentsType<U>*, RemoveAllExtentsType<T>*>::Value && 
        IsArray<U>::Value && IsConvertible<E, Deleter>::Value, int> = 0>
        UniquePointer& operator=(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ {
            Reset(other.Release());
            m_Deleter = Forward<E>(other.GetDeleter());
                
            return *this;
        }
        #else
        /// @brief Copy assignment operator
        /// @param other The object to copy from
        UniquePointer& operator=(UniquePointer& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = other.m_Deleter;
            }

            return *this;
        }
        #endif

        /// @brief Assignment operator, assigns a null pointer
        UniquePointer& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            Reset();
            return *this;
        }

        /// @brief Releases the ownership of the managed object
        /// @return The pointer to the object being managed
        PointerType Release() __WSTL_NOEXCEPT__ {
            PointerType temp = m_Pointer;
            m_Pointer = PointerType();    
            return temp;
        }

        /// @brief Resets the managed object to a new pointer
        /// @param pointer The new pointer to the object being managed
        template<typename U>
        void Reset(U pointer) __WSTL_NOEXCEPT__ {
            PointerType temp = m_Pointer;
            m_Pointer = pointer;
            if(temp) m_Deleter(temp);
        }

        /// @brief Resets the managed object to a null pointer
        void Reset(NullPointerType = __WSTL_NULLPTR__) __WSTL_NOEXCEPT__ {
            Reset(PointerType());
        }

        /// @brief Swaps the contents of two `UniquePointer` objects
        /// @param other The object to swap with
        void Swap(UniquePointer& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Pointer, other.m_Pointer);
            wstl::Swap(m_Deleter, other.m_Deleter);
        }
    
        /// @brief Returns the managed pointer
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ PointerType Get() const __WSTL_NOEXCEPT__ {
            return m_Pointer;
        }

        /// @brief Returns reference to the deleter used to destroy the object
        __WSTL_NODISCARD__ Deleter& GetDeleter() __WSTL_NOEXCEPT__ {
            return m_Deleter;
        }

        /// @brief Returns const reference to the deleter used to destroy the object
        __WSTL_NODISCARD__ const Deleter& GetDeleter() const __WSTL_NOEXCEPT__ {
            return m_Deleter;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Returns true if the pointer is not null
        explicit operator bool() const __WSTL_NOEXCEPT__ {
            return m_Pointer != __WSTL_NULLPTR__;
        }
        #else
        /// @brief Returns true if the pointer is not null, workaround for C++98
        operator const void*() const __WSTL_NOEXCEPT__ {
            return m_Pointer ? m_Pointer : __WSTL_NULLPTR__;
        }
        #endif

        /// @brief Dereference operator, accesses the first element of the array
        /// @return The object pointed to by the pointer
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType operator*() const __WSTL_NOEXCEPT__ {
            return *m_Pointer;
        }

        /// @brief Arrow operator, accesses the first element of the array
        /// @return The object pointed to by the pointer
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ PointerType operator->() const __WSTL_NOEXCEPT__ {
            return m_Pointer;
        }

        /// @brief Subscript operator
        /// @param index The index to access
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType operator[](size_t index) const {
            return m_Pointer[index];
        }

    private:
        PointerType m_Pointer;
        DeleterType m_Deleter;

        UniquePointer(const UniquePointer&) __WSTL_DELETE__;
        UniquePointer& operator=(const UniquePointer&) __WSTL_DELETE__;
    };

    /// @brief A smart pointer that owns and manages another object through a pointer - specialization for 
    /// arrays and reference deleters
    /// @details The object is destroyed and its memory deallocated when the `UniquePointer` goes out of scope
    /// @tparam Deleter The type of the deleter used to destroy the object
    /// @tparam T The type of the object being managed
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/unique_ptr
    template<typename T, typename Deleter>
    class UniquePointer<T[], Deleter&> {
    public:
        typedef Deleter& DeleterType;
        typedef T ElementType;
        typedef T* PointerType;
        typedef T& ReferenceType;

        #ifdef __WSTL_CXX11__
        /// @brief Move constructor
        /// @param other The object to move from
        UniquePointer(UniquePointer&& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()),
            m_Deleter(Forward<DeleterType>(other.GetDeleter())) {}

        /// @brief Templated move constructor
        /// @param other The object to move from
        template<typename U, typename E, EnableIfType<IsConvertible<RemoveAllExtentsType<U>*, RemoveAllExtentsType<T>*>::Value && 
        IsArray<U>::Value && IsSame<E, Deleter&>::Value, int> = 0>
        UniquePointer(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()), 
            m_Deleter(Forward<E>(other.GetDeleter())) {}

        #else
        /// @brief Copy constructor
        /// @param other The object to copy from
        UniquePointer(UniquePointer& other) __WSTL_NOEXCEPT__ : m_Pointer(other.Release()),
            m_Deleter(other.GetDeleter()) {}
        #endif

        /// @brief Parameterized constructor
        /// @param pointer The pointer to the object being managed
        /// @param deleter The deleter to use to destroy the object
        UniquePointer(PointerType pointer, DeleterType deleter) __WSTL_NOEXCEPT__ : m_Pointer(pointer), 
            m_Deleter(deleter) {}
        
        /// @brief Destructor
        ~UniquePointer() {
            Reset();
        }

        #ifdef __WSTL_CXX11__
        /// @brief Move assignment operator
        /// @param other The object to move from
        UniquePointer& operator=(UniquePointer&& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = Forward<DeleterType>(other.GetDeleter());
            }

            return *this;
        }

        /// @brief Templated move assignment operator
        /// @param other The object to move from
        template<typename U, typename E, EnableIfType<IsConvertible<RemoveAllExtentsType<U>*, RemoveAllExtentsType<T>*>::Value && 
        IsArray<U>::Value && IsSame<E, Deleter&>::Value, int> = 0>
        UniquePointer& operator=(UniquePointer<U, E>&& other) __WSTL_NOEXCEPT__ {
            Reset(other.Release());
            m_Deleter = Forward<E>(other.GetDeleter());
                
            return *this;
        }
        #else
        /// @brief Copy assignment operator
        /// @param other The object to copy from
        UniquePointer& operator=(UniquePointer& other) __WSTL_NOEXCEPT__ {
            if(&other != this) {
                Reset(other.Release());
                m_Deleter = other.GetDeleter();
            }

            return *this;
        }
        #endif

        /// @brief Assignment operator, assigns a null pointer
        UniquePointer& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            Reset();
            return *this;
        }

        /// @brief Releases the ownership of the managed object
        /// @return The pointer to the object being managed
        PointerType Release() __WSTL_NOEXCEPT__ {
            PointerType temp = m_Pointer;
            m_Pointer = PointerType();    
            return temp;
        }

        /// @brief Resets the managed object to a new pointer
        /// @param pointer The new pointer to the object being managed
        template<typename U>
        void Reset(U pointer) __WSTL_NOEXCEPT__ {
            PointerType temp = m_Pointer;
            m_Pointer = pointer;
            if(temp) m_Deleter(temp);
        }

        /// @brief Resets the managed object to a null pointer
        void Reset(NullPointerType = __WSTL_NULLPTR__) __WSTL_NOEXCEPT__ {
            Reset(PointerType());
        }

        /// @brief Swaps the contents of two `UniquePointer` objects
        /// @param other The object to swap with
        void Swap(UniquePointer& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Pointer, other.m_Pointer);
            wstl::Swap(m_Deleter, other.m_Deleter);
        }
    
        /// @brief Returns the managed pointer
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ PointerType Get() const __WSTL_NOEXCEPT__ {
            return m_Pointer;
        }

        /// @brief Returns reference to the deleter used to destroy the object
        __WSTL_NODISCARD__ DeleterType GetDeleter() const __WSTL_NOEXCEPT__ {
            return m_Deleter;
        }

        #ifdef __WSTL_CXX11__
        /// @brief Returns true if the pointer is not null
        explicit operator bool() const __WSTL_NOEXCEPT__ {
            return m_Pointer != __WSTL_NULLPTR__;
        }
        #else
        /// @brief Returns true if the pointer is not null, workaround for C++98
        operator const void*() const __WSTL_NOEXCEPT__ {
            return m_Pointer ? m_Pointer : __WSTL_NULLPTR__;
        }
        #endif

        /// @brief Dereference operator, accesses the first element of the array
        /// @return The object pointed to by the pointer
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType operator*() const __WSTL_NOEXCEPT__ {
            return *m_Pointer;
        }

        /// @brief Arrow operator, accesses the first element of the array
        /// @return The object pointed to by the pointer
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ PointerType operator->() const __WSTL_NOEXCEPT__ {
            return m_Pointer;
        }

        /// @brief Subscript operator
        /// @param index The index to access
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ReferenceType operator[](size_t index) const {
            return m_Pointer[index];
        }

    private:
        PointerType m_Pointer;
        DeleterType m_Deleter;

        UniquePointer(const UniquePointer&) __WSTL_DELETE__;
        UniquePointer& operator=(const UniquePointer&) __WSTL_DELETE__;
    };

    // Make unique

    namespace __private {
        template<typename T>
        struct __IsBoundedArray : FalseType {};

        template<typename T, size_t N>
        struct __IsBoundedArray<T[N]> : TrueType {};

        template<typename T>
        struct __IsUnboundedArray : FalseType {};

        template<typename T>
        struct __IsUnboundedArray<T[]> : TrueType {};
    }

    #ifdef __WSTL_CXX11__
    /// @brief Creates a `UniquePointer` to a new object of type `T`
    /// @tparam T The type of the object to create 
    /// @param ...args The arguments to pass to the constructor
    /// @return A `UniquePointer` to the newly created object
    /// @ingroup memory
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/memory/make_unique
    template<typename T, typename... Args>
    constexpr EnableIfType<!IsArray<T>::Value, UniquePointer<T>> MakeUnique(Args&&... args) {
        return UniquePointer<T>(::new T(Forward<Args>(args)...));
    }

    /// @brief Creation of a `UniquePointer` for known bound array types is deleted
    /// @ingroup memory
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/memory/make_unique
    template<typename T, typename... Args>
    constexpr EnableIfType<__private::__IsBoundedArray<T>::Value> MakeUnique(Args&&...) = delete;

    /// @brief Creates a `UniquePointer` to a dynamically allocated array of type `T`
    /// @tparam T The type of the object that the array will hold
    /// @param size The size of the array
    /// @return A `UniquePointer` to the newly created array
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/make_unique
    template<typename T>
    __WSTL_CONSTEXPR14__
    inline typename EnableIf<__private::__IsUnboundedArray<T>::Value, UniquePointer<T> >::Type MakeUnique(size_t size) {
        return UniquePointer<T>(::new typename RemoveAllExtents<T>::Type[size]());
    }
    #endif
}

// Comparison operators for unique pointer

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator==(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return a.Get() == b.Get();
}

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator!=(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return !(a == b);
}

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator<(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return reinterpret_cast<char*>(a.Get()) < reinterpret_cast<char*>(b.Get());
}

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator<=(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return !(b < a);
}

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator>(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return b < a;
}

template<typename T1, typename T2, typename TD1, typename TD2>
bool operator>=(const wstl::UniquePointer<T1, TD1>& a, const wstl::UniquePointer<T2, TD2>& b) {
    return !(a < b);
}

namespace wstl {
    // Uninitialized copy

    /// @brief Copies elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param last End of the source range
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_copy
    template<typename InputIterator, typename OutputIterator>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type
    UninitializedCopy(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        return Copy(first, last, resultFirst);
    }

    /// @brief Copies elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param last End of the source range
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_copy
    template<typename InputIterator, typename OutputIterator>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedCopy(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        for(; first != last; ++first, ++resultFirst)
            ::new(static_cast<void*>(AddressOf(*resultFirst))) ValueType(*first);
        
        return resultFirst;
    }

    // Uninitialized copy in range

    /// @brief Copies N elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param count Number of elements to copy
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_copy_n
    template<typename InputIterator, typename Size, typename OutputIterator>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type
    UninitializedCopyInRange(InputIterator first, Size count, OutputIterator resultFirst) {
        return CopyInRange(first, count, resultFirst);
    }

    /// @brief Copies elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param count Number of elements to copy
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_copy_n
    template<typename InputIterator, typename Size, typename OutputIterator>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedCopyInRange(InputIterator first, Size count, OutputIterator resultFirst) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        for(; count > 0; --count, ++first, ++resultFirst)
            ::new(static_cast<void*>(AddressOf(*resultFirst))) ValueType(*first);
        
        return resultFirst;
    }

    // Uninitialized fill

    /// @brief Fills uninitialized memory with a value, constructing the elements
    /// @param first Beginning of the uninitialized memory
    /// @param last End of the uninitialized memory
    /// @param value The value to fill with
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_fill
    template<typename ForwardIterator, typename T>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<ForwardIterator>::ValueType>::Value, void>::Type 
    UninitializedFill(ForwardIterator first, ForwardIterator last, const T& value) {
        Fill(first, last, value);
    }

    /// @brief Fills uninitialized memory with a value, constructing the elements
    /// @param first Beginning of the uninitialized memory
    /// @param last End of the uninitialized memory
    /// @param value The value to fill with
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_fill
    template<typename ForwardIterator, typename T>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<ForwardIterator>::ValueType>::Value, void>::Type 
    UninitializedFill(ForwardIterator first, ForwardIterator last, const T& value) {
        typedef typename IteratorTraits<ForwardIterator>::ValueType ValueType;
        for(; first != last; ++first) 
            ::new(static_cast<void*>(AddressOf(*first))) ValueType(value);
    }

    // Uninitialized fill in range

    /// @brief Fills N elements in uninitialized memory with a value, constructing them
    /// @param first Beginning of the uninitialized memory
    /// @param count Number of elements to fill
    /// @param value The value to fill with
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_fill_n
    template<typename OutputIterator, typename Size, typename T>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedFillInRange(OutputIterator first, Size count, const T& value) {
        return FillInRange(first, count, value);
    }

    /// @brief Fills N elements in uninitialized memory with a value, constructing them
    /// @param first Beginning of the uninitialized memory
    /// @param count Number of elements to fill
    /// @param value The value to fill with
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_fill_n
    template<typename OutputIterator, typename Size, typename T>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedFillInRange(OutputIterator first, Size count, const T& value) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        for(; count > 0; ++first, --count) 
            ::new(static_cast<void*>(AddressOf(*first))) ValueType(value);
        
        return first;
    }

    // Unitialized move

    /// @brief Moves elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param last End of the source range
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @note Trivial types are copied
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_move
    template<typename InputIterator, typename OutputIterator>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedMove(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        return Copy(first, last, resultFirst);
    }

    /// @brief Moves elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param last End of the source range
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @note In C++98 it copies because move semantics are not available
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_move
    template<typename InputIterator, typename OutputIterator>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedMove(InputIterator first, InputIterator last, OutputIterator resultFirst) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        for(; first != last; ++first, ++resultFirst) 
            ::new(static_cast<void*>(AddressOf(*resultFirst))) ValueType(__WSTL_MOVE__(*first));

        return resultFirst;
    }

    // Uninitialized move in range

    /// @brief Moves N elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param count Number of elements to move
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @note Trivial types are copied
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_move_n
    template<typename InputIterator, typename Size, typename OutputIterator>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedMoveInRange(InputIterator first, Size count, OutputIterator resultFirst) {
        return CopyInRange(first, count, resultFirst);
    }

    /// @brief Moves N elements to an uninitialized memory, constructing them
    /// @param first Beginning of the source range
    /// @param count Number of elements to move
    /// @param resultFirst Beginning of the uninitialized memory
    /// @return The end of the uninitialized memory
    /// @note In C++98 it copies because move semantics are not available
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_move_n
    template<typename InputIterator, typename Size, typename OutputIterator>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type 
    UninitializedMoveInRange(InputIterator first, Size count, OutputIterator resultFirst) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        for(; count > 0; --count, ++first, ++resultFirst) 
            ::new(static_cast<void*>(AddressOf(*resultFirst))) ValueType(__WSTL_MOVE__(*first));

        return resultFirst;
    }

    // Uninitialized default construct

    /// @brief Default constructs elements in uninitialized memory,
    /// for trivially constructible types does nothing
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_default_construct
    template<typename ForwardIterator>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<ForwardIterator>::ValueType>::Value, void>::Type
    UninitializedDefaultConstruct(ForwardIterator, ForwardIterator) {
        // Do nothing
    }
    
    /// @brief Default constructs elements in uninitialized memory
    /// @param first Beginning of the uninitialized memory
    /// @param last End of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_default_construct
    template<typename ForwardIterator>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<ForwardIterator>::ValueType>::Value, void>::Type
    UninitializedDefaultConstruct(ForwardIterator first, ForwardIterator last) {
        typedef typename IteratorTraits<ForwardIterator>::ValueType ValueType;
        for(; first != last; ++first) ::new(static_cast<void*>(AddressOf(*first))) ValueType;
    }

    // Uninitialized default construct in range 

    /// @brief Default constructs N elements in uninitialized memory, for trivially constructible 
    /// types does nothing
    /// @param first Beginning of the uninitialized memory
    /// @param count Number of elements to default construct
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_default_construct_n
    template<typename OutputIterator, typename Size>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type
    UninitializedDefaultConstructInRange(OutputIterator first, Size count) {
        // Do nothing
        return Next(first, count);
    }

    /// @brief Default constructs N elements in uninitialized memory
    /// @param first Beginning of the uninitialized memory
    /// @param count Number of elements to default construct
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_default_construct_n
    template<typename OutputIterator, typename Size>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type
    UninitializedDefaultConstructInRange(OutputIterator first, Size count) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        for(; count > 0; --count, ++first) ::new(static_cast<void*>(AddressOf(*first))) ValueType;

        return first;
    }

    // Uninitialized value construct

    /// @brief Value constructs elements in uninitialized memory
    /// @param first Beginning of the uninitialized memory
    /// @param last End of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_value_construct
    template<typename OutputIterator>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, void>::Type
    UninitializedValueConstruct(OutputIterator first, OutputIterator last) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        Fill(first, last, ValueType());
    }

    /// @brief Value constructs elements in uninitialized memory
    /// @param first Beginning of the uninitialized memory
    /// @param last End of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_value_construct
    template<typename OutputIterator>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, void>::Type
    UninitializedValueConstruct(OutputIterator first, OutputIterator last) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        for(; first != last; ++first) ::new(static_cast<void*>(AddressOf(*first))) ValueType();
    }

    // Uninitialized value construct in range

    /// @brief Value constructs N elements in uninitialized memory, for trivially constructible 
    /// types does nothing
    /// @param first Beginning of the uninitialized memory
    /// @param count Number of elements to value construct
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_value_construct_n
    template<typename OutputIterator, typename Size>
    typename EnableIf<IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type
    UninitializedValueConstructInRange(OutputIterator first, Size count) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        return FillInRange(first, count, ValueType());
    }

    /// @brief Value constructs N elements in uninitialized memory
    /// @param first Beginning of the uninitialized memory
    /// @param count Number of elements to value construct
    /// @return The end of the uninitialized memory
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/uninitialized_value_construct_n
    template<typename OutputIterator, typename Size>
    typename EnableIf<!IsTriviallyConstructible<typename IteratorTraits<OutputIterator>::ValueType>::Value, OutputIterator>::Type
    UninitializedValueConstructInRange(OutputIterator first, Size count) {
        typedef typename IteratorTraits<OutputIterator>::ValueType ValueType;
        for(; count > 0; --count, ++first) ::new(static_cast<void*>(AddressOf(*first))) ValueType();

        return first;
    }

    // Construct at

    #ifdef __WSTL_CXX11__
    /// @brief Constructs an object at the given address
    /// @param pointer The address to construct the object at
    /// @param ...args The arguments to pass to the constructor
    /// @return A pointer to the constructed object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/construct_at
    template<typename T, typename... Args>
    T* ConstructAt(T* pointer, Args&&... args) {
        return ::new(const_cast<void*>(static_cast<const volatile void*>(pointer))) T(Forward<Args>(args)...);
    }
    #else
    /// @brief Constructs an object at the given address
    /// @param pointer The address to construct the object at
    /// @return A pointer to the constructed object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/construct_at
    template<typename T>
    T* ConstructAt(T* pointer) {
        return ::new(const_cast<void*>(static_cast<const volatile void*>(pointer))) T();
    }

    /// @brief Constructs an object at the given address
    /// @param pointer The address to construct the object at
    /// @param arg The argument to pass to the constructor
    /// @return A pointer to the constructed object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/construct_at
    template<typename T, typename Arg>
    T* ConstructAt(T* pointer, const Arg& arg) {
        return ::new(const_cast<void*>(static_cast<const volatile void*>(pointer))) T(arg);
    }

    /// @brief Constructs an object at the given address
    /// @param pointer The address to construct the object at
    /// @param arg1 The first argument to pass to the constructor
    /// @param arg2 The second argument to pass to the constructor
    /// @return A pointer to the constructed object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/construct_at
    template<typename T, typename Arg1, typename Arg2>
    T* ConstructAt(T* pointer, const Arg1& arg1, const Arg2& arg2) {
        return ::new(const_cast<void*>(static_cast<const volatile void*>(pointer))) T(arg1, arg2);
    }

    /// @brief Constructs an object at the given address
    /// @param pointer The address to construct the object at
    /// @param arg1 The first argument to pass to the constructor
    /// @param arg2 The second argument to pass to the constructor
    /// @param arg3 The third argument to pass to the constructor
    /// @return A pointer to the constructed object
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/construct_at
    template<typename T, typename Arg1, typename Arg2, typename Arg3>
    T* ConstructAt(T* pointer, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
        return ::new(const_cast<void*>(static_cast<const volatile void*>(pointer))) T(arg1, arg2, arg3);
    }
    #endif
    
    // Destroy at

    /// @brief Destroys an object at the given address, for trivially destructible types does nothing
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy_at
    template<typename T>
    typename EnableIf<IsTriviallyDestructible<T>::Value, void>::Type DestroyAt(T*) {}

    /// @brief Destroys an object at the given address
    /// @param pointer The address to destroy the object at
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy_at
    template<typename T>
    typename EnableIf<!IsTriviallyDestructible<T>::Value, void>::Type DestroyAt(T* pointer) {
        pointer->~T();
    }

    // Destroy

    /// @brief Destroys a range of objects, for trivially destructible types does nothing
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy
    template<typename Iterator>
    typename EnableIf<IsTriviallyDestructible<typename IteratorTraits<Iterator>::ValueType>::Value, void>::Type 
    Destroy(Iterator, Iterator) {
        // Do nothing
    }

    /// @brief Destroys a range of objects
    /// @param first The beginning of the range
    /// @param last The end of the range
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy
    template<typename Iterator>
    typename EnableIf<!IsTriviallyDestructible<typename IteratorTraits<Iterator>::ValueType>::Value, void>::Type 
    Destroy(Iterator first, Iterator last) {
        for(; first != last; ++first) DestroyAt(AddressOf(*first));
    }

    // Destroy in range

    /// @brief Destroys N objects in a range, for trivially destructible types does nothing
    /// @returns The end of the range
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy_n
    template<typename Iterator, typename Size>
    typename EnableIf<IsTriviallyDestructible<typename IteratorTraits<Iterator>::ValueType>::Value, Iterator>::Type 
    DestroyInRange(Iterator first, Size count) {
        // Do nothing
        return Next(first, count);
    }

    /// @brief Destroys N objects in a range
    /// @param first The beginning of the range
    /// @param count The number of objects to destroy
    /// @returns The end of the range
    /// @ingroup memory
    /// @see https://en.cppreference.com/w/cpp/memory/destroy_n
    template<typename Iterator, typename Size>
    typename EnableIf<!IsTriviallyDestructible<typename IteratorTraits<Iterator>::ValueType>::Value, Iterator>::Type 
    DestroyInRange(Iterator first, Size count) {
        for(; count > 0; ++first, --count) DestroyAt(AddressOf(*first));
        return first;
    }

    // Aligned storage

    /// @brief Provides a storage type with specified size and alignment
    /// @tparam Length Size in bytes
    /// @tparam Alignment Alignment in bytes
    /// @ingroup memory
    template<size_t Length, size_t Alignment>
    struct AlignedStorage {
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

    // Uninitialized buffer

    /// @brief Aligned buffer of N objects as if they were with type T
    /// @tparam T The type of the objects in the buffer
    /// @tparam N The number of objects in the buffer
    /// @ingroup memory
    template<typename T, size_t N>
    class UninitializedBuffer {
    public:
        typedef T ValueType;
        typedef T& ReferenceType;
        typedef const T& ConstReferenceType;
        typedef T* PointerType;
        typedef const T* ConstPointerType;
        typedef T* Iterator;
        typedef const T* ConstIterator;

        static const __WSTL_CONSTEXPR__ size_t ObjectSize = sizeof(T);
        static const __WSTL_CONSTEXPR__ size_t StaticSize = N;
        static const __WSTL_CONSTEXPR__ size_t Alignment = AlignmentOf<T>::Value;

        /// @brief Accesses the element at the specified index
        /// @param index The index of the element to access
        /// @return A reference to the element at the specified index
        ReferenceType operator[](size_t index) {
            return Data()[index];
        }

        /// @brief Accesses the element at the specified index
        /// @param index The index of the element to access
        /// @return A const reference to the element at the specified index
        ConstReferenceType operator[](size_t index) const {
            return Data()[index];
        }

        /// @brief Conversion operator to reference type
        /// @return A reference to the first object in the buffer
        operator ReferenceType() {
            return m_Buffer.template GetReference<T>();
        }

        /// @brief Conversion operator to const reference type
        /// @return A const reference to the first object in the buffer
        operator ConstReferenceType() const {
            return m_Buffer.template GetReference<const T>();
        }
        
        /// @brief Conversion operator to pointer type
        /// @return A pointer to the first object in the buffer
        operator PointerType() {
            return m_Buffer.template GetPointer<T>();
        }

        /// @brief Conversion operator to const pointer type
        /// @return A const pointer to the first object in the buffer
        operator ConstPointerType() const {
            return m_Buffer.template GetPointer<const T>();
        }

        /// @brief Returns a pointer to the underlying data
        PointerType Data() {
            return m_Buffer.template GetPointer<T>();
        }

        /// @brief Returns a const pointer to the underlying data
        ConstPointerType Data() const {
            return m_Buffer.template GetPointer<const T>();
        }

        /// @brief Returns a pointer to the beginning of the buffer
        Iterator Begin() {
            return m_Buffer.template GetPointer<T>();
        }

        /// @brief Returns a const pointer to the beginning of the buffer
        ConstIterator Begin() const {
            return m_Buffer.template GetPointer<const T>();
        }

        /// @brief Returns a pointer to the end of the buffer
        Iterator End() {
            return Begin() + N;
        }

        /// @brief Returns a const pointer to the end of the buffer
        ConstIterator End() const {
            return Begin() + N;
        }

    private:
        AlignedStorage<N * ObjectSize, Alignment> m_Buffer;
    };

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ size_t UninitializedBuffer<T, N>::ObjectSize;

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ size_t UninitializedBuffer<T, N>::StaticSize;

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ size_t UninitializedBuffer<T, N>::Alignment;

    // Wrappers for libc functions (if there are user-defined ones)

    /// @brief Namespace for memory-related functions
    namespace memory {
        // Copy - memcpy

        /// @brief Copies bytes from source range to a destination range
        /// @details Default implementation is not optimized! Define `__WSTL_MEMOPS_USE_LIBC__` 
        /// to use `memcpy` from the <string.h> header (can be used from libc or provided manually), 
        /// or `__WSTL_MEMOPS_USE_BUILTINS__` to use compiler built-ins if available.
        /// @param destination Pointer to the destination range
        /// @param source Pointer to the source range
        /// @param count Number of bytes to copy
        /// @return Pointer to the destination range
        /// @ingroup memory
        inline void* Copy(void* destination, const void* source, size_t count) __WSTL_NOEXCEPT__ {
            #ifdef __WSTL_MEMOPS_USE_LIBC__
            return ::memcpy(destination, source, count);
            #elif defined(__WSTL_MEMOPS_USE_BUILTINS__)
            return __builtin_memcpy(destination, source, count);
            #else
            uint8_t* to = static_cast<uint8_t*>(destination);
            const uint8_t* from = static_cast<const uint8_t*>(source);

            for(size_t i = 0; i < count; ++i) to[i] = from[i];

            return destination;
            #endif
        }

        // Set - memset

        /// @brief Sets a range of memory to a specified value
        /// @details Default implementation is not optimized! Define `__WSTL_MEMOPS_USE_LIBC__` 
        /// to use `memset` from the <string.h> header (can be used from libc or provided manually), 
        /// or `__WSTL_MEMOPS_USE_BUILTINS__` to use compiler built-ins if available.
        /// @param pointer Pointer to the memory range
        /// @param value Value to set
        /// @param count Number of bytes to set
        /// @return Pointer to the memory range
        /// @ingroup memory
        inline void* Set(void* pointer, int value, size_t count) __WSTL_NOEXCEPT__ {
            #ifdef __WSTL_MEMOPS_USE_LIBC__
            return ::memset(pointer, value, count);
            #elif defined(__WSTL_MEMOPS_USE_BUILTINS__)
            return __builtin_memset(pointer, value, count);
            #else
            uint8_t* to = static_cast<uint8_t*>(pointer);

            for(size_t i = 0; i < count; ++i) to[i] = static_cast<uint8_t>(value);

            return pointer;
            #endif
        }

        // Move - memmove

        /// @brief Moves memory range from source to destination, handling overlapping regions
        /// @details Default implementation is not optimized! Define `__WSTL_MEMOPS_USE_LIBC__` 
        /// to use `memmove` from the <string.h> header (can be used from libc or provided manually), 
        /// or `__WSTL_MEMOPS_USE_BUILTINS__` to use compiler built-ins if available.
        /// @param destination Pointer to the destination range
        /// @param source Pointer to the source range
        /// @param count Number of bytes to move
        /// @return Pointer to the destination range
        /// @ingroup memory
        inline void* Move(void* destination, const void* source, size_t count) __WSTL_NOEXCEPT__ {
            #ifdef __WSTL_MEMOPS_USE_LIBC__
            return ::memmove(destination, source, count);
            #elif defined(__WSTL_MEMOPS_USE_BUILTINS__)
            return __builtin_memmove(destination, source, count);
            #else
            uint8_t* to = static_cast<uint8_t*>(destination);
            const uint8_t* from = static_cast<const uint8_t*>(source);

            if(destination < source) {
                for(size_t i = 0; i < count; ++i) to[i] = from[i];
            }
            else if(destination > source) {
                while(count--) to[count] = from[count];
            }

            return destination;
            #endif
        }

        // Compare - memcmp

        /// @brief Compares two ranges of memory
        /// @details Default implementation is not optimized! Define `__WSTL_MEMOPS_USE_LIBC__` 
        /// to use `memcmp` from the <string.h> header (can be used from libc or provided manually), 
        /// or `__WSTL_MEMOPS_USE_BUILTINS__` to use compiler built-ins if available.
        /// @param pointer1 Pointer to the first memory range
        /// @param pointer2 Pointer to the second memory range
        /// @param count Number of bytes to compare
        /// @return Zero if equal, negative if `pointer1 < pointer2`, positive if `pointer1 > pointer2`
        /// @ingroup memory
        inline int Compare(const void* pointer1, const void* pointer2, size_t count) __WSTL_NOEXCEPT__ {
            #ifdef __WSTL_MEMOPS_USE_LIBC__
            return ::memcmp(pointer1, pointer2, count);
            #elif defined(__WSTL_MEMOPS_USE_BUILTINS__)
            return __builtin_memcmp(pointer1, pointer2, count);
            #else
            const uint8_t* a = static_cast<const uint8_t*>(pointer1);
            const uint8_t* b = static_cast<const uint8_t*>(pointer2);

            while(count--) {
                if(*a != *b) return (*a < *b) ? -1 : 1;
                ++a;
                ++b;
            }

            return 0;
            #endif
        }

        // Locate - memchr

        /// @brief Finds the first occurrence of a byte in a memory range
        /// @details Default implementation is not optimized! Define `__WSTL_MEMOPS_USE_LIBC__` 
        /// to use `memchr` from the <string.h> header (can be used from libc or provided manually), 
        /// or `__WSTL_MEMOPS_USE_BUILTINS__` to use compiler built-ins if available.
        /// @param pointer Pointer to the memory range
        /// @param value Value to find
        /// @param count Number of bytes to search
        /// @return Pointer to the first occurrence of `value` in memory, or `nullptr` if not found
        /// @ingroup memory
        inline const void* Locate(const void* pointer, int value, size_t count) __WSTL_NOEXCEPT__ {
            #ifdef __WSTL_MEMOPS_USE_LIBC__
            return ::memchr(pointer, value, count);
            #elif defined(__WSTL_MEMOPS_USE_BUILTINS__)
            return __builtin_memchr(pointer, value, count);
            #else
            const uint8_t* to = static_cast<const uint8_t*>(pointer);

            while(count--) {
                if(*to == static_cast<uint8_t>(value)) 
                    return const_cast<void*>(static_cast<const void*>(to));

                ++to;
            }

            return NullPointer;
            #endif
        }

        /// @brief Finds the first occurrence of a byte in a memory range
        /// @details Default implementation is not optimized! Define `__WSTL_MEMOPS_USE_LIBC__` 
        /// to use `memchr` from the <string.h> header (can be used from libc or provided manually), 
        /// or `__WSTL_MEMOPS_USE_BUILTINS__` to use compiler built-ins if available.
        /// @param pointer Pointer to the memory range
        /// @param value Value to find
        /// @param count Number of bytes to search
        /// @return Pointer to the first occurrence of `value` in memory, or `nullptr` if not found
        /// @ingroup memory
        inline void* Locate(void* pointer, int value, size_t count) __WSTL_NOEXCEPT__ {
            #ifdef __WSTL_MEMOPS_USE_LIBC__
            return ::memchr(pointer, value, count);
            #elif defined(__WSTL_MEMOPS_USE_BUILTINS__)
            return __builtin_memchr(pointer, value, count);
            #else
            uint8_t* to = static_cast<uint8_t*>(pointer);

            while(count--) {
                if(*to == static_cast<uint8_t>(value)) 
                    return static_cast<void*>(to);

                ++to;
            }

            return NullPointer;
            #endif
        }
    }
}

#endif
