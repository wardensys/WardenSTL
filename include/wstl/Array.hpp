// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is inspired by the Embedded Template Library (ETL)'s array container implementation,
// and some concepts and functions have been adapted for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_ARRAY_HPP__
#define __WSTL_ARRAY_HPP__

#include "private/Platform.hpp"
#include "private/Error.hpp"
#include "Algorithm.hpp"
#include "StandardExceptions.hpp"
#include <stddef.h>


/// @defgroup array Array
/// @brief A container that encapsulates a fixed size array
/// @ingroup containers

namespace wstl {
    // Array class

    /// @brief A container that encapsulates a fixed size array
    /// @tparam T Type of the elements in the array
    /// @tparam N Size of the array
    /// @ingroup array
    /// @see https://en.cppreference.com/w/cpp/container/array
    template<typename T, size_t N>
    struct Array {
    private:
        typedef typename __private::__ParameterType<T>::Type ParameterType;

    public:
        typedef T ValueType;
        typedef size_t SizeType;
        typedef T& ReferenceType;
        typedef const T& ConstReferenceType;
        typedef T* PointerType;
        typedef const T* ConstPointerType;
        typedef T* Iterator;
        typedef const T* ConstIterator;
        typedef wstl::ReverseIterator<Iterator> ReverseIterator;
        typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;

        /// @brief The static size, needed for metaprogramming
        static const __WSTL_CONSTEXPR__ SizeType StaticSize = N;
    
        T __m_Elements[N];

        /// @brief Returns a reference to the element at the specified index with bounds checking
        /// @param index Index of the element to return
        /// @return Reference to the element at the specified index
        /// @throws `OutOfRange` if the index is out of range
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType At(SizeType index) {
            __WSTL_ASSERT__(index < N,  WSTL_MAKE_EXCEPTION(OutOfRange, "Array index out of range"));
            return __m_Elements[index];
        }

        /// @brief Returns a const reference to the element at the specified index with bounds checking
        /// @param index Index of the element to return
        /// @return Const reference to the element at the specified index
        /// @throws `OutOfRange` if the index is out of range
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType At(SizeType index) const {
            __WSTL_ASSERT__(index < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Array index out of range"));
            return __m_Elements[index];
        }

        /// @brief Access operator
        /// @param index Index of the element to return
        /// @return Reference to the element at the specified index
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType operator[](SizeType index) {
            __WSTL_ASSERT_INDEXOPERATOR__(index < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Array index out of range"));
            return __m_Elements[index];
        }

        /// @brief Access operator
        /// @param index Index of the element to return
        /// @return Const reference to the element at the specified index
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstReferenceType operator[](SizeType index) const {
            #if defined(__WSTL_CXX11__) && !defined(__WSTL_CXX14__) && defined(__WSTL_EXCEPTIONS__) && defined(__WSTL_ASSERT_INDEXOPERATOR__)
            return index < N ? __m_Elements[index] : throw(WSTL_MAKE_EXCEPTION(OutOfRange, "Array index out of range"));
            #else
            __WSTL_ASSERT_INDEXOPERATOR__(index < N, WSTL_MAKE_EXCEPTION(OutOfRange, "Array index out of range"));
            return __m_Elements[index];
            #endif
        }

        /// @brief Gets reference to the first element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType Front() __WSTL_NOEXCEPT__ {
            return __m_Elements[0];
        }

        /// @brief Gets const reference to the first element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstReferenceType Front() const __WSTL_NOEXCEPT__ {
            return __m_Elements[0];
        }

        /// @brief Gets reference to the last element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType Back() __WSTL_NOEXCEPT__ {
            return __m_Elements[N - 1];
        }

        /// @brief Gets const reference to the last element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType Back() const __WSTL_NOEXCEPT__ {
            return __m_Elements[N - 1];
        }

        /// @brief Gets the data pointer of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ PointerType Data() __WSTL_NOEXCEPT__ {
            return __m_Elements;
        }

        /// @brief Gets the const data pointer of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstPointerType Data() const __WSTL_NOEXCEPT__ {
            return __m_Elements;
        }

        /// @brief Gets the iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ Iterator Begin() __WSTL_NOEXCEPT__ {
            return __m_Elements;
        }

        /// @brief Gets the iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator Begin() const __WSTL_NOEXCEPT__ {
            return __m_Elements;
        }

        /// @brief Gets the const iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator ConstBegin() const __WSTL_NOEXCEPT__ {
            return Begin();
        }

        /// @brief Gets the iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ Iterator End() __WSTL_NOEXCEPT__ {
            return __m_Elements + N;
        }

        /// @brief Gets the iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator End() const __WSTL_NOEXCEPT__ {
            return __m_Elements + N;
        }

        /// @brief Gets the const iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator ConstEnd() const __WSTL_NOEXCEPT__ {
            return End();
        }

        __WSTL_CONTAINER_RANGE_COMPAT__(Array)

        /// @brief Gets the reverse iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
        ReverseIterator ReverseBegin() __WSTL_NOEXCEPT__ {
            return ReverseIterator(End());
        }

        /// @brief Gets the reverse iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstReverseIterator ReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(End());
        }

        /// @brief Gets the const reverse iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstReverseIterator ConstReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(End());
        }

        /// @brief Gets the reverse iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
        ReverseIterator ReverseEnd() __WSTL_NOEXCEPT__ {
            return ReverseIterator(Begin());
        }

        /// @brief Gets the reverse iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstReverseIterator ReverseEnd() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(Begin());
        }

        /// @brief Gets the const reverse iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstReverseIterator ConstReverseEnd() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(Begin());
        }

        /// @brief Checks whether the array is empty
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ bool Empty() const __WSTL_NOEXCEPT__{
            return N == 0;
        }

        /// @brief Gets the size of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType Size() const __WSTL_NOEXCEPT__{
            return N;
        }

        /// @brief Gets the maximum size of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType MaxSize() const __WSTL_NOEXCEPT__{
            return N;
        }

        /// @brief Fills the array with the specified value
        /// @param value The value to fill the array with
        __WSTL_CONSTEXPR14__ void Fill(ParameterType value) {
            wstl::Fill(Begin(), End(), value);
        }

        /// @brief Swaps the contents of the array with another array
        /// @param other The other array to swap with
        __WSTL_CONSTEXPR14__ void Swap(Array& other) __WSTL_NOEXCEPT_EXPR__(noexcept(wstl::Swap(DeclareValue<T&>(), DeclareValue<T&>()))) {
            for(size_t i = 0; i < N; ++i) wstl::Swap(__m_Elements[i], other.__m_Elements[i]);
        }

        // Additional functions

        /// @brief Assigns values from a range to the array
        /// @param first Iterator to the first element of the range
        /// @param last Iterator to the last element of the range
        template<typename InputIterator>
        void Assign(InputIterator first, InputIterator last) {
            CopySafe(first, last, Begin(), End());
        }

        /// @brief Assigns values from a range to the array, filling remaining elements with a specified value
        /// @param first Iterator to the first element of the range
        /// @param last Iterator to the last element of the range
        /// @param value The value to fill remaining elements with
        template<typename InputIterator>
        void Assign(InputIterator first, InputIterator last, ParameterType value) {
            Iterator iterator = CopySafe(first, last, Begin(), End());
            wstl::Fill(iterator, End(), value);
        }

        /// @brief Inserts a value at the specified position
        /// @param position Iterator to the position where the value should be inserted
        /// @param value The value to insert
        /// @return Iterator to the inserted value
        /// @throws `OutOfRange` if the position is out of range and `__WSTL_CHECK_ITERATOR__` is defined
        Iterator Insert(ConstIterator position, ParameterType value) {
            Iterator iterator = ToIterator(position);

            __WSTL_ASSERT_ITERATOR_RETURNVALUE__(ConstBegin() <= position && position < ConstEnd(),
                WSTL_MAKE_EXCEPTION(OutOfRange, "Array insert position out of range"), iterator);

            MoveBackward(iterator, End() - 1, End());
            *iterator = value;

            return iterator;
        }

        /// @brief Inserts a range of values at the specified position
        /// @param position Iterator to the position where the range should be inserted
        /// @param first Iterator to the first element of the range
        /// @param last Iterator to the last element of the range
        /// @return Iterator to the first element of the inserted range
        /// @throws `OutOfRange` if the position is out of range and `__WSTL_CHECK_ITERATOR__` is defined
        template<typename InputIterator>
        Iterator Insert(ConstIterator position, InputIterator first, InputIterator last) {
            Iterator iterator = ToIterator(position);

            __WSTL_ASSERT_ITERATOR_RETURNVALUE__(ConstBegin() <= position && position < ConstEnd(),
                WSTL_MAKE_EXCEPTION(OutOfRange, "Array insert position out of range"), iterator);

            SizeType sourceLength = static_cast<size_t>(Distance(first, last));
            SizeType destinationLength = static_cast<size_t>(Distance(position, ConstEnd()));
            
            if(sourceLength < destinationLength) 
                MoveBackward(iterator, End() - sourceLength, End());

            CopySafe(first, last, iterator, End());
            return iterator;
        }

        /// @brief Erases a value at the specified position and fills the last element with a specified value
        /// @param position Iterator to the position of the value to delete
        /// @param value The value to fill the last element with
        /// @return Iterator to the position where the deleted value was
        /// @throws `OutOfRange` if the position is out of range and `__WSTL_CHECK_ITERATOR__` is defined
        Iterator Erase(ConstIterator position, ParameterType value) {
            Iterator iterator = ToIterator(position);

            __WSTL_ASSERT_ITERATOR_RETURNVALUE__(ConstBegin() <= position && position < ConstEnd(),
                WSTL_MAKE_EXCEPTION(OutOfRange, "Array erase position out of range"), iterator);

            Move(iterator + 1, End(), iterator);
            Back() = value;
            return iterator;
        }

        /// @brief Erases a value at the specified position
        /// @param position Iterator to the position of the value to delete
        /// @return Iterator to the position where the deleted value was
        /// @throws `OutOfRange` if the position is out of range and `__WSTL_CHECK_ITERATOR__` is defined
        Iterator Erase(ConstIterator position) {
            Iterator iterator = ToIterator(position);

            __WSTL_ASSERT_ITERATOR_RETURNVALUE__(ConstBegin() <= position && position < ConstEnd(),
                WSTL_MAKE_EXCEPTION(OutOfRange, "Array erase position out of range"), iterator);

            Move(iterator + 1, End(), iterator);
            return iterator;
        }

        /// @brief Erases a range of values and fills the deleted range with a specified value
        /// @param first Iterator to the first element of the range to delete
        /// @param last Iterator to the last element of the range to delete
        /// @param value The value to fill the deleted range with
        /// @return Iterator to the position where the beginning of the deleted range was
        /// @throws `OutOfRange` if the positions are out of range and `__WSTL_CHECK_ITERATOR__` is defined
        Iterator Erase(ConstIterator first, ConstIterator last, ParameterType value) {
            Iterator iterator = ToIterator(first);

            __WSTL_ASSERT_ITERATOR_RETURNVALUE__(ConstBegin() <= first && first <= last && last <= ConstEnd(),
                WSTL_MAKE_EXCEPTION(OutOfRange, "Array insert positions out of range"), iterator);

            iterator = Move(last, ConstEnd(), iterator);
            wstl::Fill(iterator, End(), value);
            return ToIterator(first);
        }

        /// @brief Erases a range of values
        /// @param first Iterator to the first element of the range to delete
        /// @param last Iterator to the last element of the range to delete
        /// @return Iterator to the position where the beginning of the deleted range was
        /// @throws `OutOfRange` if the positions are out of range and `__WSTL_CHECK_ITERATOR__` is defined
        Iterator Erase(ConstIterator first, ConstIterator last) {
            Iterator iterator = ToIterator(first);

            __WSTL_ASSERT_ITERATOR_RETURNVALUE__(ConstBegin() <= first && first <= last && last <= ConstEnd(),
                WSTL_MAKE_EXCEPTION(OutOfRange, "Array insert positions out of range"), iterator);

            Move(last, ConstEnd(), iterator);
            return ToIterator(iterator);
        }

    private:
        Iterator ToIterator(ConstIterator iterator) const {
            return const_cast<Iterator>(iterator);
        }
    };

    template<typename T, size_t N>
    const __WSTL_CONSTEXPR__ typename Array<T, N>::SizeType Array<T, N>::StaticSize;

    // Specialization for size 0

    template<typename T>
    struct Array<T, 0> {
    private:
        typedef typename __private::__ParameterType<T>::Type ParameterType;
    
    public:
        typedef T ValueType;
        typedef size_t SizeType;
        typedef T& ReferenceType;
        typedef const T& ConstReferenceType;
        typedef T* PointerType;
        typedef const T* ConstPointerType;
        typedef T* Iterator;
        typedef const T* ConstIterator;
        typedef wstl::ReverseIterator<Iterator> ReverseIterator;
        typedef wstl::ReverseIterator<ConstIterator> ConstReverseIterator;
        
        /// @brief The static size, needed for metaprogramming
        static const __WSTL_CONSTEXPR__ SizeType StaticSize = 0;

        /// @brief Returns a reference to the element at the specified index with bounds checking
        /// @param index Index of the element to return
        /// @return Reference to the element at the specified index
        /// @throws `IndexOutOfRange` if the index is out of range
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType At(SizeType) {
            return *Data();
        }

        /// @brief Returns a const reference to the element at the specified index with bounds checking
        /// @param index Index of the element to return
        /// @return Const reference to the element at the specified index
        /// @throws `IndexOutOfRange` if the index is out of range
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType At(SizeType) const {
            return *Data();
        }

        /// @brief Access operator
        /// @param index Index of the element to return
        /// @return Reference to the element at the specified index
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType operator[](SizeType) {
            return *Data();
        }

        /// @brief Access operator
        /// @param index Index of the element to return
        /// @return Const reference to the element at the specified index
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType operator[](SizeType) const {
            return *Data();
        }

        /// @brief Gets reference to the first element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType Front() __WSTL_NOEXCEPT__ {
            return *Data();
        }

        /// @brief Gets const reference to the first element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType Front() const __WSTL_NOEXCEPT__ {
            return *Data();
        }

        /// @brief Gets reference to the last element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ReferenceType Back() __WSTL_NOEXCEPT__ {
            return *Data();
        }

        /// @brief Gets const reference to the last element of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ ConstReferenceType Back() const __WSTL_NOEXCEPT__ {
            return *Data();
        }

        /// @brief Gets the data pointer of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ PointerType Data() __WSTL_NOEXCEPT__ {
            return (const T*) 0;
        }

        /// @brief Gets the const data pointer of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstReferenceType Data() const __WSTL_NOEXCEPT__ {
            return (T*) 0;
        }

        /// @brief Gets the iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ Iterator Begin() __WSTL_NOEXCEPT__ {
            return Iterator();
        }

        /// @brief Gets the iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator Begin() const __WSTL_NOEXCEPT__ {
            return ConstIterator();
        }

        /// @brief Gets the const iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator ConstBegin() const __WSTL_NOEXCEPT__ {
            return ConstIterator();
        }

        /// @brief Gets the iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ Iterator End() __WSTL_NOEXCEPT__ {
            return Iterator();
        }

        /// @brief Gets the iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator End() const __WSTL_NOEXCEPT__ {
            return ConstIterator();
        }

        /// @brief Gets the const iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ ConstIterator ConstEnd() const __WSTL_NOEXCEPT__ {
            return ConstIterator();
        }

        /// @brief Gets the reverse iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
        ReverseIterator ReverseBegin() __WSTL_NOEXCEPT__ {
            return ReverseIterator(End());
        }

        /// @brief Gets the reverse iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstReverseIterator ReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(End());
        }

        /// @brief Gets the const reverse iterator to the beginning of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstReverseIterator ConstReverseBegin() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(End());
        }

        /// @brief Gets the reverse iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR14__ 
        ReverseIterator ReverseEnd() __WSTL_NOEXCEPT__ {
            return ReverseIterator(Begin());
        }

        /// @brief Gets the reverse iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstIterator ReverseEnd() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(Begin());
        }

        /// @brief Gets the const reverse iterator to the end of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ 
        ConstReverseIterator ConstReverseEnd() const __WSTL_NOEXCEPT__ {
            return ConstReverseIterator(Begin());
        }

        /// @brief Checks whether the array is empty
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ bool Empty() const __WSTL_NOEXCEPT__{
            return true;
        }

        /// @brief Gets the size of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType Size() const __WSTL_NOEXCEPT__{
            return 0;
        }

        /// @brief Gets the maximum size of the array
        __WSTL_NODISCARD__ __WSTL_CONSTEXPR__ SizeType MaxSize() const __WSTL_NOEXCEPT__{
            return 0;
        }

        /// @brief Fills the array with the specified value
        /// @param value The value to fill the array with
        __WSTL_CONSTEXPR14__ void Fill(ParameterType) __WSTL_NOEXCEPT__ {}

        /// @brief Swaps the contents of the array with another array
        /// @param other The other array to swap with
        __WSTL_CONSTEXPR14__ void Swap(Array&) __WSTL_NOEXCEPT__ {}

        // Additional functions

        /// @brief Assigns values from a range to the array
        /// @param first Iterator to the first element of the range
        /// @param last Iterator to the last element of the range
        template<typename InputIterator>
        void Assign(InputIterator, InputIterator) __WSTL_NOEXCEPT__ {}

        /// @brief Assigns values from a range to the array, filling remaining elements with a specified value
        /// @param first Iterator to the first element of the range
        /// @param last Iterator to the last element of the range
        /// @param value The value to fill remaining elements with
        template<typename InputIterator>
        void Assign(InputIterator, InputIterator, ParameterType) __WSTL_NOEXCEPT__ {}

        /// @brief Inserts a value at the specified position
        /// @param position Iterator to the position where the value should be inserted
        /// @param value The value to insert
        /// @return Iterator to the inserted value
        /// @throws `OutOfRange` if the position is out of range and `__WSTL_CHECK_ITERATOR__` is defined
        Iterator Insert(ConstIterator, ParameterType) __WSTL_NOEXCEPT__ {
            return Iterator();
        }

        /// @brief Inserts a range of values at the specified position
        /// @param position Iterator to the position where the range should be inserted
        /// @param first Iterator to the first element of the range
        /// @param last Iterator to the last element of the range
        /// @return Iterator to the first element of the inserted range
        /// @throws `OutOfRange` if the position is out of range and `__WSTL_CHECK_ITERATOR__` is defined
        template<typename InputIterator>
        Iterator Insert(ConstIterator, InputIterator, InputIterator) __WSTL_NOEXCEPT__ {
            return Iterator();
        }

        /// @brief Erases a value at the specified position and fills the last element with a specified value
        /// @param position Iterator to the position of the value to delete
        /// @param value The value to fill the last element with
        /// @return Iterator to the position where the deleted value was
        /// @throws `OutOfRange` if the position is out of range and `__WSTL_CHECK_ITERATOR__` is defined
        Iterator Erase(ConstIterator, ParameterType) __WSTL_NOEXCEPT__ {
            return Iterator();
        }

        /// @brief Erases a value at the specified position
        /// @param position Iterator to the position of the value to delete
        /// @return Iterator to the position where the deleted value was
        /// @throws `OutOfRange` if the position is out of range and `__WSTL_CHECK_ITERATOR__` is defined
        Iterator Erase(ConstIterator) __WSTL_NOEXCEPT__ {
            return Iterator();
        }

        /// @brief Erases a range of values and fills the deleted range with a specified value
        /// @param first Iterator to the first element of the range to delete
        /// @param last Iterator to the last element of the range to delete
        /// @param value The value to fill the deleted range with
        /// @return Iterator to the position where the beginning of the deleted range was
        /// @throws `OutOfRange` if the positions are out of range and `__WSTL_CHECK_ITERATOR__` is defined
        Iterator Erase(ConstIterator, ConstIterator, ParameterType) __WSTL_NOEXCEPT__ {
            return Iterator();
        }

        /// @brief Erases a range of values
        /// @param first Iterator to the first element of the range to delete
        /// @param last Iterator to the last element of the range to delete
        /// @return Iterator to the position where the beginning of the deleted range was
        /// @throws `OutOfRange` if the positions are out of range and `__WSTL_CHECK_ITERATOR__` is defined
        __WSTL_CONSTEXPR14__ Iterator Erase(ConstIterator, ConstIterator) __WSTL_NOEXCEPT__ {
            return Iterator();
        }
    };

    template<typename T>
    const __WSTL_CONSTEXPR__ typename Array<T, 0>::SizeType Array<T, 0>::StaticSize;

    // Template deduction guide

    #ifdef __WSTL_CXX17__
    template<typename T, typename... U>
    Array(T, U...) -> Array<T, sizeof...(U) + 1>;
    #endif

    // Tuple element specialization

    template<size_t Index, typename T, size_t N>
    struct TupleElement<Index, Array<T, N>> {
        WSTL_STATIC_ASSERT(Index < N, "Index out of bounds");
        typedef T Type;
    };

    // Tuple size specialization

    template<typename T, size_t N>
    struct TupleSize<Array<T, N>> : IntegralConstant<size_t, N> {};

    // Get specialization

    /// @brief Gets an element from array
    /// @tparam Index Index of element
    /// @param array Array from which to get element
    /// @return The element at the specified index
    /// @ingroup array
    /// @see https://en.cppreference.com/w/cpp/container/array/get
    template<size_t Index, typename T, size_t N>
    __WSTL_CONSTEXPR14__ 
    inline T& Get(Array<T, N>& array) __WSTL_NOEXCEPT__ {
        WSTL_STATIC_ASSERT(Index < N, "Index out of bounds");
        return array[Index];
    }

    /// @copydoc Get(Array<T, N>&)
    template<size_t Index, typename T, size_t N>
    __WSTL_CONSTEXPR14__ 
    inline const T& Get(const Array<T, N>& array) __WSTL_NOEXCEPT__ {
        WSTL_STATIC_ASSERT(Index < N, "Index out of bounds");
        return array[Index];
    }

    #ifdef __WSTL_CXX11__
    /// @copydoc Get(Array<T, N>&)
    template<size_t Index, typename T, size_t N>
    __WSTL_CONSTEXPR14__ 
    inline T&& Get(Array<T, N>&& array) __WSTL_NOEXCEPT__ {
        WSTL_STATIC_ASSERT(Index < N, "Index out of bounds");
        return Move(array[Index]);
    }

    /// @copydoc Get(Array<T, N>&)
    template<size_t Index, typename T, size_t N>
    __WSTL_CONSTEXPR14__ 
    inline const T&& Get(const Array<T, N>&& array) __WSTL_NOEXCEPT__ {
        WSTL_STATIC_ASSERT(Index < N, "Index out of bounds");
        return Move(array[Index]);
    }
    #endif

    // Comparison operators

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator==(const Array<T, N>& a, const Array<T, N>& b) {
        return Equal(a.ConstBegin(), a.ConstEnd(), b.ConstBegin());
    }

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator!=(const Array<T, N>& a, const Array<T, N>& b) {
        return !(a == b);
    }

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator<(const Array<T, N>& a, const Array<T, N>& b) {
        return LexicographicalCompare(a.ConstBegin(), a.ConstEnd(), b.ConstBegin(), b.ConstEnd());
    }

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator<=(const Array<T, N>& a, const Array<T, N>& b) {
        return !(b < a);
    }

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator>(const Array<T, N>& a, const Array<T, N>& b) {
        return (b < a);
    }

    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline bool operator>=(const Array<T, N>& a, const Array<T, N>& b) {
        return !(a < b);
    }

    // To array

    #ifdef __WSTL_CXX11__
    namespace __private {
        template<typename T, size_t N, size_t... Indices>
        __WSTL_CONSTEXPR14__
        inline Array<RemoveCVType<T>, N> __ToArray(T (&array)[N], IndexSequence<Indices...>) {
            return {{array[Indices]...}};
        }

        template<typename T, size_t N, size_t... Indices>
        __WSTL_CONSTEXPR14__
        inline Array<RemoveCVType<T>, N> __ToArray(T (&&array)[N], IndexSequence<Indices...>) {
            return {{Move(array[Indices])...}};
        }
    }

    /// @brief Converts a built-in array to an `Array`
    /// @param array Built-in array to convert
    /// @return `Array` containing the elements of the built-in array
    /// @ingroup array
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/container/array/to_array
    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline Array<RemoveCVType<T>, N> ToArray(T (&array)[N]) {
        return __private::__ToArray(array, MakeIndexSequence<N>{});
    }

    /// @brief Converts a built-in array to an `Array` using move semantics
    /// @param array Built-in array to convert
    /// @return `Array` containing the elements of the built-in array
    /// @ingroup array
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/container/array/to_array
    template<typename T, size_t N>
    __WSTL_CONSTEXPR14__
    inline Array<RemoveCVType<T>, N> ToArray(T (&&array)[N]) {
        return __private::__ToArray(Move(array), MakeIndexSequence<N>{});
    }
    #else
    /// @brief Converts a built-in array to an `Array`
    /// @param array Reference to the built-in array
    /// @return `Array` containing the elements of the built-in array
    /// @ingroup array
    /// @see https://en.cppreference.com/w/cpp/container/array/to_array
    template<typename T, size_t N>
    inline Array<typename RemoveCV<T>::Type, N> ToArray(T (&array)[N]) {
        Array<typename RemoveCV<T>::Type, N> result;
        Copy(Begin(array), End(array), Begin(result));
        return result;
    }
    #endif
}

#endif