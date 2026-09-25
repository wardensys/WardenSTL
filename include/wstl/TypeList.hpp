#ifndef __WSTL_TYPELIST_HPP__
#define __WSTL_TYPELIST_HPP__

#include "private/Platform.hpp"
#include "private/TupleUtils.hpp"


namespace wstl {
    #ifdef __WSTL_CXX11__
    /// @brief A compile-time list of types
    /// @tparam ...Types Types in the list
    /// @ingroup utilities
    template<typename... Types>
    struct TypeList {};

    // Tuple element specialization
    
    template<typename Head, typename... Tail>
    struct TupleElement<0, TypeList<Head, Tail...>> { typedef Head Type; };

    template<size_t Index, typename Head, typename... Tail>
    struct TupleElement<Index, TypeList<Head, Tail...>> {
        typedef typename TupleElement<Index - 1, TypeList<Tail...>>::Type Type;
    };

    // Tuple size specialization

    template<typename... Types>
    struct TupleSize<TypeList<Types...>> : IntegralConstant<size_t, sizeof...(Types)> {};

    // Type list concatenate

    /// @brief Concatenates multiple type lists into a single type list
    /// @tparam ...Lists Type lists to concatenate
    /// @ingroup utilities
    template<typename... Lists>
    struct TypeListConcatenate;

    template<typename... Types1, typename... Types2, typename... Tail>
    struct TypeListConcatenate<TypeList<Types1...>, TypeList<Types2...>, Tail...> {
        typedef typename TypeListConcatenate<TypeList<Types1..., Types2...>, Tail...>::Type Type;
    };

    template<typename... Types>
    struct TypeListConcatenate<TypeList<Types...>> {
        typedef TypeList<Types...> Type;
    };

    template<>
    struct TypeListConcatenate<> {
        typedef TypeList<> Type;
    };

    /// @copydoc TypeListConcatenate
    template<typename... Lists>
    using TypeListConcatenateType = typename TypeListConcatenate<Lists...>::Type;

    // Type list push front

    /// @brief Pushes a type to the front of a type list
    /// @tparam List Type list to push to
    /// @tparam T Type to push
    /// @ingroup utilities
    template<typename List, typename T>
    struct TypeListPushFront;

    template<typename... Types, typename T>
    struct TypeListPushFront<TypeList<Types...>, T> {
        typedef TypeList<T, Types...> Type;
    };

    /// @copydoc TypeListPushFront
    template<typename List, typename T>
    using TypeListPushFrontType = typename TypeListPushFront<List, T>::Type;

    // Type list push back

    /// @brief Pushes a type to the back of a type list
    /// @tparam List Type list to push to
    /// @tparam T Type to push
    /// @ingroup utilities
    template<typename List, typename T>
    struct TypeListPushBack;

    template<typename... Types, typename T>
    struct TypeListPushBack<TypeList<Types...>, T> {
        typedef TypeList<Types..., T> Type;
    };

    /// @copydoc TypeListPushBack
    template<typename List, typename T>
    using TypeListPushBackType = typename TypeListPushBack<List, T>::Type;

    // Type list pop front

    /// @brief Pops a type from the front of a type list
    /// @tparam List Type list to pop from
    /// @ingroup utilities
    template<typename List>
    struct TypeListPopFront;

    template<typename Head, typename... Tail>
    struct TypeListPopFront<TypeList<Head, Tail...>> {
        typedef TypeList<Tail...> Type;
    };

    template<>
    struct TypeListPopFront<TypeList<>> {
        typedef TypeList<> Type;
    };

    /// @copydoc TypeListPopFront
    template<typename List>
    using TypeListPopFrontType = typename TypeListPopFront<List>::Type;

    // Type list pop back

    namespace __private {
        template<typename T, typename Sequence>
        struct __TypeListPopBack;

        template<typename T, size_t... Is>
        struct __TypeListPopBack<T, IndexSequence<Is...>> {
            typedef TypeList<typename TupleElement<Is, T>::Type...> Type;
        };
    }

    /// @brief Pops a type from the back of a type list
    /// @tparam List Type list to pop from
    /// @ingroup utilities
    template<typename List>
    struct TypeListPopBack;

    template<typename... Types>
    struct TypeListPopBack<TypeList<Types...>> : __private::__TypeListPopBack<TypeList<Types...>, 
        MakeIndexSequence<sizeof...(Types) - 1>> {};

    template<>
    struct TypeListPopBack<TypeList<>> {
        typedef TypeList<> Type;
    };

    /// @copydoc TypeListPopBack
    template<typename List>
    using TypeListPopBackType = typename TypeListPopBack<List>::Type;

    #else
    namespace __private {
        struct __NoType;
    }

    template<
        typename T0 = __private::__NoType, typename T1 = __private::__NoType, typename T2 = __private::__NoType,
        typename T3 = __private::__NoType, typename T4 = __private::__NoType, typename T5 = __private::__NoType
    >
    struct TypeList {};

    // Tuple size specialization

    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    struct TupleSize<TypeList<T0, T1, T2, T3, T4, T5> > : IntegralConstant<
        size_t, 
        IsSame<T0, __private::__NoType>::Value ? 0 : IsSame<T1, __private::__NoType>::Value ? 1 :
        IsSame<T2, __private::__NoType>::Value ? 2 : IsSame<T3, __private::__NoType>::Value ? 3 :
        IsSame<T4, __private::__NoType>::Value ? 4 : IsSame<T5, __private::__NoType>::Value ? 5 : 6
    > {};

    // Tuple element specialization

    namespace __private {
        template<typename T, typename = typename EnableIf<!IsSame<T, __NoType>::Value>::Type>
        struct __TypeListElement;

        template<typename T>
        struct __TypeListElement<T, void> { typedef T Type; };
    }

    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    struct TupleElement<0, TypeList<T0, T1, T2, T3, T4, T5> > : __private::__TypeListElement<T0> {};

    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    struct TupleElement<1, TypeList<T0, T1, T2, T3, T4, T5> > : __private::__TypeListElement<T1> {};

    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    struct TupleElement<2, TypeList<T0, T1, T2, T3, T4, T5> > : __private::__TypeListElement<T2> {};

    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    struct TupleElement<3, TypeList<T0, T1, T2, T3, T4, T5> > : __private::__TypeListElement<T3> {};

    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    struct TupleElement<4, TypeList<T0, T1, T2, T3, T4, T5> > : __private::__TypeListElement<T4> {};

    template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    struct TupleElement<5, TypeList<T0, T1, T2, T3, T4, T5> > : __private::__TypeListElement<T5> {};

    // Type list push front

    namespace __private {
        template<typename List, typename T, size_t = TupleSize<List>::Value>
        struct __TypeListPushFront;

        template<typename T>
        struct __TypeListPushFront<TypeList<>, T, 0> {
            typedef TypeList<T> Type;
        };

        template<typename T, typename T0>
        struct __TypeListPushFront<TypeList<T0>, T, 1> {
            typedef TypeList<T, T0> Type;
        };

        template<typename T, typename T0, typename T1>
        struct __TypeListPushFront<TypeList<T0, T1>, T, 2> {
            typedef TypeList<T, T0, T1> Type;
        };

        template<typename T, typename T0, typename T1, typename T2>
        struct __TypeListPushFront<TypeList<T0, T1, T2>, T, 3> {
            typedef TypeList<T, T0, T1, T2> Type;
        };

        template<typename T, typename T0, typename T1, typename T2, typename T3>
        struct __TypeListPushFront<TypeList<T0, T1, T2, T3>, T, 4> {
            typedef TypeList<T, T0, T1, T2, T3> Type;
        };

        template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
        struct __TypeListPushFront<TypeList<T0, T1, T2, T3, T4>, T, 5> {
            typedef TypeList<T, T0, T1, T2, T3, T4> Type;
        };
    }

    /// @brief Pushes a type to the front of a type list
    /// @tparam List Type list to push to
    /// @tparam T Type to push
    /// @ingroup utilities
    template<typename List, typename T>
    struct TypeListPushFront : __private::__TypeListPushFront<List, T> {};

    // Type list push back

    namespace __private {
        template<typename List, typename T, size_t = TupleSize<List>::Value>
        struct __TypeListPushBack;

        template<typename T>
        struct __TypeListPushBack<TypeList<>, T, 0> {
            typedef TypeList<T> Type;
        };

        template<typename T, typename T0>
        struct __TypeListPushBack<TypeList<T0>, T, 1> {
            typedef TypeList<T0, T> Type;
        };

        template<typename T, typename T0, typename T1>
        struct __TypeListPushBack<TypeList<T0, T1>, T, 2> {
            typedef TypeList<T0, T1, T> Type;
        };

        template<typename T, typename T0, typename T1, typename T2>
        struct __TypeListPushBack<TypeList<T0, T1, T2>, T, 3> {
            typedef TypeList<T0, T1, T2, T> Type;
        };

        template<typename T, typename T0, typename T1, typename T2, typename T3>
        struct __TypeListPushBack<TypeList<T0, T1, T2, T3>, T, 4> {
            typedef TypeList<T0, T1, T2, T3, T> Type;
        };

        template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
        struct __TypeListPushBack<TypeList<T0, T1, T2, T3, T4>, T, 5> {
            typedef TypeList<T0, T1, T2, T3, T4, T> Type;
        };
    }

    /// @brief Pushes a type to the back of a type list
    /// @tparam List Type list to push to
    /// @tparam T Type to push
    /// @ingroup utilities
    template<typename List, typename T>
    struct TypeListPushBack : __private::__TypeListPushBack<List, T> {};

    // Type list pop front

    namespace __private {
        template<typename List, size_t = TupleSize<List>::Value>
        struct __TypeListPopFront;

        template<>
        struct __TypeListPopFront<TypeList<>, 0> {
            typedef TypeList<> Type;
        };

        template<typename T0>
        struct __TypeListPopFront<TypeList<T0>, 1> {
            typedef TypeList<> Type;
        };

        template<typename T0, typename T1>
        struct __TypeListPopFront<TypeList<T0, T1>, 2> {
            typedef TypeList<T1> Type;
        };

        template<typename T0, typename T1, typename T2>
        struct __TypeListPopFront<TypeList<T0, T1, T2>, 3> {
            typedef TypeList<T1, T2> Type;
        };

        template<typename T0, typename T1, typename T2, typename T3>
        struct __TypeListPopFront<TypeList<T0, T1, T2, T3>, 4> {
            typedef TypeList<T1, T2, T3> Type;
        };

        template<typename T0, typename T1, typename T2, typename T3, typename T4>
        struct __TypeListPopFront<TypeList<T0, T1, T2, T3, T4>, 5> {
            typedef TypeList<T1, T2, T3, T4> Type;
        };

        template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
        struct __TypeListPopFront<TypeList<T0, T1, T2, T3, T4, T5>, 6> {
            typedef TypeList<T1, T2, T3, T4, T5> Type;
        };
    }

    /// @brief Pops a type from the front of a type list
    /// @tparam List Type list to pop from
    /// @ingroup utilities
    template<typename List>
    struct TypeListPopFront : __private::__TypeListPopFront<List> {};

    // Type list pop back

    namespace __private {
        template<typename List, size_t = TupleSize<List>::Value>
        struct __TypeListPopBack;

        template<>
        struct __TypeListPopBack<TypeList<>, 0> {
            typedef TypeList<> Type;
        };

        template<typename T0>
        struct __TypeListPopBack<TypeList<T0>, 1> {
            typedef TypeList<> Type;
        };

        template<typename T0, typename T1>
        struct __TypeListPopBack<TypeList<T0, T1>, 2> {
            typedef TypeList<T0> Type;
        };

        template<typename T0, typename T1, typename T2>
        struct __TypeListPopBack<TypeList<T0, T1, T2>, 3> {
            typedef TypeList<T0, T1> Type;
        };

        template<typename T0, typename T1, typename T2, typename T3>
        struct __TypeListPopBack<TypeList<T0, T1, T2, T3>, 4> {
            typedef TypeList<T0, T1, T2> Type;
        };

        template<typename T0, typename T1, typename T2, typename T3, typename T4>
        struct __TypeListPopBack<TypeList<T0, T1, T2, T3, T4>, 5> {
            typedef TypeList<T0, T1, T2, T3> Type;
        };

        template<typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
        struct __TypeListPopBack<TypeList<T0, T1, T2, T3, T4, T5>, 6> {
            typedef TypeList<T0, T1, T2, T3, T4> Type;
        };
    }

    /// @brief Pops a type from the back of a type list
    /// @tparam List Type list to pop from
    /// @ingroup utilities
    template<typename List>
    struct TypeListPopBack : __private::__TypeListPopBack<List> {};

    // Type list concatenate

    namespace __private {
        template<typename L1, typename L2, bool = TupleIsEmpty<L2>::Value, bool = TupleIsEmpty<L1>::Value>
        struct __TypeListConcatenate;

        template<typename L1, typename L2>
        struct __TypeListConcatenate<L1, L2, false, true> {
            typedef L2 Type;
        };

        template<typename L1, typename L2>
        struct __TypeListConcatenate<L1, L2, true, true> {
            typedef wstl::TypeList<> Type;
        };

        template<typename L1, typename L2>
        struct __TypeListConcatenate<L1, L2, true, false> {
            typedef L1 Type;
        };

        template<typename L1, typename L2>
        struct __TypeListConcatenate<L1, L2, false, false> : __TypeListConcatenate<
            typename TypeListPushBack<L1, typename TupleFront<L2>::Type>::Type,
            typename TypeListPopFront<L2>::Type, TupleIsEmpty<typename TypeListPopFront<L2>::Type>::Value
        > {};
    }

    /// @brief Concatenates two type lists into a single type list
    /// @tparam ...Lists Type lists to concatenate
    /// @ingroup utilities
    template<typename L1 = void, typename L2 = void>
    struct TypeListConcatenate : __private::__TypeListConcatenate<L1, L2> {};

    #endif
}

#endif
