// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_FUNCTIONTRAITS_HPP__
#define __WSTL_FUNCTIONTRAITS_HPP__

#include "private/Platform.hpp"
#include "TypeList.hpp"


namespace wstl {
    /// @brief Enumeration for the type of the function
    /// @ingroup utilities
    enum FunctionType {
        FUNCTION_TYPE_FREE,
        FUNCTION_TYPE_MEMBER,
        FUNCTION_TYPE_FUNCTOR
    };

    #ifdef __WSTL_CXX11__
    /// @brief Enumeration for the reference qualifier of the function
    /// @ingroup utilities
    enum FunctionRefQualifier {
        REF_QUALIFIER_NONE,
        REF_QUALIFIER_LVALUE,
        REF_QUALIFIER_RVALUE
    };
    #endif

    namespace __private {
        template<typename T, typename = void>
        struct __FunctionTraits;

        WSTL_DECLARE_TYPEDEF_TEST(__TestFunctionArg1, ArgumentType)
        WSTL_DECLARE_TYPEDEF_TEST(__TestFunctionArg2, FirstArgumentType, SecondArgumentType)

        // Functor with typedefs, no arguments

        template<typename T>
        struct __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            !(WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1) || 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2))>::Type
        > {
            typedef typename T::ResultType ResultType;
            typedef T ObjectType;
            typedef TypeList<> ArgumentTypes;

            static const __WSTL_CONSTEXPR__ size_t Arity = 0;
            static const __WSTL_CONSTEXPR__ FunctionType FunctionType = FUNCTION_TYPE_FUNCTOR;
            static const __WSTL_CONSTEXPR__ bool IsConst = false;
            static const __WSTL_CONSTEXPR__ bool IsVolatile = false;
            static const __WSTL_CONSTEXPR__ bool IsVariadic = false;
        };

        template<typename T>
        const __WSTL_CONSTEXPR__ size_t __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            !(WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1) || 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2))>::Type
        >::Arity;

        template<typename T>
        const __WSTL_CONSTEXPR__ FunctionType __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            !(WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1) || 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2))>::Type
        >::FunctionType;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            !(WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1) || 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2))>::Type
        >::IsConst;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            !(WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1) || 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2))>::Type
        >::IsVolatile;
        
        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            !(WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1) || 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2))>::Type
        >::IsVariadic;

        // Functor with typedefs, 1 argument

        template<typename T>
        struct __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1)>::Type
        > {
            typedef typename T::ResultType ResultType;
            typedef T ObjectType;
            typedef TypeList<typename T::ArgumentType> ArgumentTypes;

            static const __WSTL_CONSTEXPR__ size_t Arity = 1;
            static const __WSTL_CONSTEXPR__ FunctionType FunctionType = FUNCTION_TYPE_FUNCTOR;
            static const __WSTL_CONSTEXPR__ bool IsConst = false;
            static const __WSTL_CONSTEXPR__ bool IsVolatile = false;
            static const __WSTL_CONSTEXPR__ bool IsVariadic = false;
        };

        template<typename T>
        const __WSTL_CONSTEXPR__ size_t __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1)>::Type
        >::Arity;

        template<typename T>
        const __WSTL_CONSTEXPR__ FunctionType __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1)>::Type
        >::FunctionType;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1)>::Type
        >::IsConst;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1)>::Type
        >::IsVolatile;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg1, T, 1)>::Type
        >::IsVariadic;

        // Functor with typedefs, 2 arguments

        template<typename T>
        struct __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2)>::Type
        > {
            typedef typename T::ResultType ResultType;
            typedef T ObjectType;
            typedef TypeList<typename T::FirstArgumentType, typename T::SecondArgumentType> ArgumentTypes;

            static const __WSTL_CONSTEXPR__ size_t Arity = 2;
            static const __WSTL_CONSTEXPR__ FunctionType FunctionType = FUNCTION_TYPE_FUNCTOR;
            static const __WSTL_CONSTEXPR__ bool IsConst = false;
            static const __WSTL_CONSTEXPR__ bool IsVolatile = false;
            static const __WSTL_CONSTEXPR__ bool IsVariadic = false;
        };

        template<typename T>
        const __WSTL_CONSTEXPR__ size_t __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2)>::Type
        >::Arity;

        template<typename T>
        const __WSTL_CONSTEXPR__ FunctionType __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2)>::Type
        >::FunctionType;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2)>::Type
        >::IsConst;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2)>::Type
        >::IsVolatile;

        template<typename T>
        const __WSTL_CONSTEXPR__ bool __FunctionTraits<T, typename EnableIf<
            WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1) && 
            WSTL_TYPEDEF_TEST_RESULT(__TestFunctionArg2, T, 2)>::Type
        >::IsVariadic;

        #ifdef __WSTL_CXX11__
        // Free function

        template<typename Return, typename... Args>
        struct __FunctionTraits<Return(Args...), void> {
            using ResultType = Return;
            using ObjectType = void;
            using ArgumentTypes = TypeList<Args...>;

            static constexpr size_t Arity = sizeof...(Args);
            static constexpr bool IsVariadic = false;

            static constexpr FunctionType FunctionType = FUNCTION_TYPE_FREE;

            static constexpr bool IsConst = false;
            static constexpr bool IsVolatile = false;
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_NONE;

            #if defined(__WSTL_CXX17__) && defined(__WSTL_EXCEPTIONS__)
            static constexpr bool IsNothrow = false;
            #endif
        };

        template<typename Return, typename... Args>
        constexpr size_t __FunctionTraits<Return(Args...)>::Arity;

        template<typename Return, typename... Args>
        constexpr bool __FunctionTraits<Return(Args...)>::IsVariadic;

        template<typename Return, typename... Args>
        constexpr FunctionType __FunctionTraits<Return(Args...)>::FunctionType;

        template<typename Return, typename... Args>
        constexpr bool __FunctionTraits<Return(Args...)>::IsConst;

        template<typename Return, typename... Args>
        constexpr bool __FunctionTraits<Return(Args...)>::IsVolatile;

        template<typename Return, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return(Args...)>::RefQualifier;

        #if defined(__WSTL_CXX17__) && defined(__WSTL_EXCEPTIONS__)
        template<typename Return, typename... Args>
        constexpr bool __FunctionTraits<Return(Args...)>::IsNothrow;

        template<typename Return, typename... Args>
        struct __FunctionTraits<Return(Args...) noexcept, void> : __FunctionTraits<Return(Args...)> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename... Args>
        constexpr bool __FunctionTraits<Return(Args...) noexcept>::IsNothrow;
        #endif

        // Free function, variadic

        template<typename Return, typename... Args>
        struct __FunctionTraits<Return(Args..., ...), void> : __FunctionTraits<Return(Args...)> {
            static constexpr bool IsVariadic = true;
        };

        template<typename Return, typename... Args>
        constexpr bool __FunctionTraits<Return(Args..., ...)>::IsVariadic;
        
        #if defined(__WSTL_CXX17__) && defined(__WSTL_EXCEPTIONS__)
        template<typename Return, typename... Args>
        struct __FunctionTraits<Return(Args..., ...) noexcept, void> : __FunctionTraits<Return(Args...)> {
            static constexpr bool IsVariadic = true;
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename... Args>
        constexpr bool __FunctionTraits<Return(Args..., ...) noexcept>::IsNothrow;
        #endif

        // Functor

        template<typename T>
        static auto __TestCallOperator(int) -> decltype(&T::operator(), TrueType{}) {}

        template<typename>
        static auto __TestCallOperator(...) -> FalseType {}

        template<typename T>
        struct __FunctionTraits<T, EnableIfType<IsClass<DecayType<T>>::Value && 
        decltype(__TestCallOperator<T>(0))::Value && !(WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1))>> : __FunctionTraits<decltype(&DecayType<T>::operator())> {
            static constexpr FunctionType FunctionType = FUNCTION_TYPE_FUNCTOR;
        };

        template<typename T>
        constexpr FunctionType __FunctionTraits<T, EnableIfType<IsClass<DecayType<T>>::Value && 
        decltype(__TestCallOperator<T>(0))::Value && !(WSTL_TYPEDEF_TEST_RESULT(__TestResultType, T, 1))>>::FunctionType;

        // Member function

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...), void> {
            using ResultType = Return;
            using ObjectType = Object;
            using ArgumentTypes = TypeList<Args...>;

            static constexpr size_t Arity = sizeof...(Args);
            static constexpr bool IsVariadic = false;

            static constexpr FunctionType FunctionType = FUNCTION_TYPE_MEMBER;

            static constexpr bool IsConst = false;
            static constexpr bool IsVolatile = false;
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_NONE;

            #if defined(__WSTL_CXX17__) && defined(__WSTL_EXCEPTIONS__)
            static constexpr bool IsNothrow = false;
            #endif
        };

        template<typename Return, typename Object, typename... Args>
        constexpr size_t __FunctionTraits<Return (Object::*)(Args...)>::Arity;

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...)>::IsVariadic;

        template<typename Return, typename Object, typename... Args>
        constexpr FunctionType __FunctionTraits<Return (Object::*)(Args...)>::FunctionType;

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...)>::IsConst;

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...)>::IsVolatile;

        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args...)>::RefQualifier;

        #if defined(__WSTL_CXX17__) && defined(__WSTL_EXCEPTIONS__)
        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...)>::IsNothrow;
        #endif

        // Member function, reference qualifier

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) &, void> : __FunctionTraits<Return (Object::*)(Args...)> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_LVALUE;
        };
        
        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args...) &>::RefQualifier;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) &&, void> : __FunctionTraits<Return (Object::*)(Args...)> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_RVALUE;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args...) &&>::RefQualifier;

        // Member function, variadic

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...), void> : __FunctionTraits<Return (Object::*)(Args...)> {
            static constexpr bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...)>::IsVariadic;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) &, void> : __FunctionTraits<Return (Object::*)(Args..., ...)> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_LVALUE;
        };
        
        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args..., ...) &>::RefQualifier;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) &&, void> : __FunctionTraits<Return (Object::*)(Args..., ...)> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_RVALUE;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args..., ...) &&>::RefQualifier;

        // Member function, noexcept

        #if defined(__WSTL_CXX17__) && defined(__WSTL_EXCEPTIONS__)
        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) noexcept, void> : __FunctionTraits<Return (Object::*)(Args...)> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...)> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) & noexcept, void> : __FunctionTraits<Return (Object::*)(Args...) &> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) & noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) & noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...) &> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) & noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) && noexcept, void> : __FunctionTraits<Return (Object::*)(Args...) &&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) && noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) && noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...) &&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) && noexcept>::IsNothrow;
        #endif


        // Member function, const

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const, void> : __FunctionTraits<Return (Object::*)(Args...)>  {
            static constexpr bool IsConst = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) const>::IsConst;

        // Member function, const reference qualifier

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const&, void> : __FunctionTraits<Return (Object::*)(Args...) const> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_LVALUE;
        };
        
        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args...) const&>::RefQualifier;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const&&, void> : __FunctionTraits<Return (Object::*)(Args...) const> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_RVALUE;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args...) const&&>::RefQualifier;

        // Member function, const variadic

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const, void> : __FunctionTraits<Return (Object::*)(Args...) const> {
            static constexpr bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) const>::IsVariadic;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const&, void> : __FunctionTraits<Return (Object::*)(Args..., ...) const> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_LVALUE;
        };
        
        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return(Object::*)(Args..., ...) const&>::RefQualifier;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const&&, void> : __FunctionTraits<Return (Object::*)(Args..., ...) const> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_RVALUE;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args..., ...) const&&>::RefQualifier;

        // Member function, const noexcept

        #if defined(__WSTL_CXX17__) && defined(__WSTL_EXCEPTIONS__)
        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const noexcept, void> : __FunctionTraits<Return (Object::*)(Args...) const> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) const noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...) const> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) const noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const& noexcept, void> : __FunctionTraits<Return (Object::*)(Args...) const&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) const& noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const& noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...) const&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) const& noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const&& noexcept, void> : __FunctionTraits<Return (Object::*)(Args...) const&&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) const&& noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const&& noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...) const&&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) const&& noexcept>::IsNothrow;
        #endif


        // Member function, volatile

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) volatile, void> : __FunctionTraits<Return (Object::*)(Args...)>  {
            static constexpr bool IsVolatile = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) volatile>::IsVolatile;

        // Member function, volatile reference qualifier

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) volatile&, void> : __FunctionTraits<Return (Object::*)(Args...) volatile> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_LVALUE;
        };
        
        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args...) volatile&>::RefQualifier;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) volatile&&, void> : __FunctionTraits<Return (Object::*)(Args...) volatile> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_RVALUE;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args...) volatile&&>::RefQualifier;

        // Member function, volatile variadic

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) volatile, void> : __FunctionTraits<Return (Object::*)(Args...) volatile> {
            static constexpr bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) volatile>::IsVariadic;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) volatile&, void> : __FunctionTraits<Return (Object::*)(Args..., ...) volatile> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_LVALUE;
        };
        
        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return(Object::*)(Args..., ...) volatile&>::RefQualifier;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) volatile&&, void> : __FunctionTraits<Return (Object::*)(Args..., ...) volatile> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_RVALUE;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args..., ...) volatile&&>::RefQualifier;

        // Member function, volatile noexcept

        #if defined(__WSTL_CXX17__) && defined(__WSTL_EXCEPTIONS__)
        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) volatile noexcept, void> : __FunctionTraits<Return (Object::*)(Args...) volatile> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) volatile noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) volatile noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...) volatile> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) volatile noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) volatile& noexcept, void> : __FunctionTraits<Return (Object::*)(Args...) volatile&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) volatile& noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) volatile& noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...) volatile&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) volatile& noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) volatile&& noexcept, void> : __FunctionTraits<Return (Object::*)(Args...) volatile&&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) volatile&& noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) volatile&& noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...) volatile&&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) volatile&& noexcept>::IsNothrow;
        #endif
        

        // Member function, const volatile

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const volatile, void> : __FunctionTraits<Return (Object::*)(Args...)>  {
            static constexpr bool IsConst = true;
            static constexpr bool IsVolatile = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) const volatile>::IsConst;

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) const volatile>::IsVolatile;

        // Member function, const volatile reference qualifier

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const volatile&, void> : __FunctionTraits<Return (Object::*)(Args...) const volatile> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_LVALUE;
        };
        
        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args...) const volatile&>::RefQualifier;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const volatile&&, void> : __FunctionTraits<Return (Object::*)(Args...) const volatile> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_RVALUE;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args...) const volatile&&>::RefQualifier;

        // Member function, const volatile variadic

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const volatile, void> : __FunctionTraits<Return (Object::*)(Args...) const volatile> {
            static constexpr bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) const volatile>::IsVariadic;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const volatile&, void> : __FunctionTraits<Return (Object::*)(Args..., ...) const volatile> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_LVALUE;
        };
        
        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return(Object::*)(Args..., ...) const volatile&>::RefQualifier;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const volatile&&, void> : __FunctionTraits<Return (Object::*)(Args..., ...) const volatile> {
            static constexpr FunctionRefQualifier RefQualifier = REF_QUALIFIER_RVALUE;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr FunctionRefQualifier __FunctionTraits<Return (Object::*)(Args..., ...) const volatile&&>::RefQualifier;

        // Member function, const volatile noexcept

        #if defined(__WSTL_CXX17__) && defined(__WSTL_EXCEPTIONS__)
        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const volatile noexcept, void> : __FunctionTraits<Return (Object::*)(Args...) const volatile> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) const volatile noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const volatile noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...) const volatile> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) const volatile noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const volatile& noexcept, void> : __FunctionTraits<Return (Object::*)(Args...) const volatile&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) const volatile& noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const volatile& noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...) const volatile&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) const volatile& noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args...) const volatile&& noexcept, void> : __FunctionTraits<Return (Object::*)(Args...) const volatile&&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args...) const volatile&& noexcept>::IsNothrow;

        template<typename Return, typename Object, typename... Args>
        struct __FunctionTraits<Return (Object::*)(Args..., ...) const volatile&& noexcept, void> : __FunctionTraits<Return (Object::*)(Args..., ...) const volatile&&> {
            static constexpr bool IsNothrow = true;
        };

        template<typename Return, typename Object, typename... Args>
        constexpr bool __FunctionTraits<Return (Object::*)(Args..., ...) const volatile&& noexcept>::IsNothrow;
        #endif
        #else
        // Free function

        template<typename Return, typename Arg1, typename Arg2>
        struct __FunctionTraits<Return(Arg1, Arg2), void> {
            typedef Return ResultType;
            typedef void ObjectType;
            typedef TypeList<Arg1, Arg2> ArgumentTypes;

            static const size_t Arity = 2;
            static const FunctionType FunctionType = FUNCTION_TYPE_FREE;
            static const bool IsConst = false;
            static const bool IsVolatile = false;
            static const bool IsVariadic = false;
        };

        template<typename Return, typename Arg1, typename Arg2>
        const size_t __FunctionTraits<Return(Arg1, Arg2)>::Arity;

        template<typename Return, typename Arg1, typename Arg2>
        const FunctionType __FunctionTraits<Return(Arg1, Arg2)>::FunctionType;

        template<typename Return, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return(Arg1, Arg2)>::IsConst;

        template<typename Return, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return(Arg1, Arg2)>::IsVolatile;

        template<typename Return, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return(Arg1, Arg2)>::IsVariadic;

        template<typename Return, typename Arg>
        struct __FunctionTraits<Return(Arg), void> {
            typedef Return ResultType;
            typedef void ObjectType;
            typedef TypeList<Arg> ArgumentTypes;

            static const size_t Arity = 1;
            static const FunctionType FunctionType = FUNCTION_TYPE_FREE;
            static const bool IsConst = false;
            static const bool IsVolatile = false;
            static const bool IsVariadic = false;
        };

        template<typename Return, typename Arg>
        const size_t __FunctionTraits<Return(Arg)>::Arity;

        template<typename Return, typename Arg>
        const FunctionType __FunctionTraits<Return(Arg)>::FunctionType;

        template<typename Return, typename Arg>
        const bool __FunctionTraits<Return(Arg)>::IsConst;

        template<typename Return, typename Arg>
        const bool __FunctionTraits<Return(Arg)>::IsVolatile;

        template<typename Return, typename Arg>
        const bool __FunctionTraits<Return(Arg)>::IsVariadic;

        template<typename Return>
        struct __FunctionTraits<Return(), void> {
            typedef Return ResultType;
            typedef void ObjectType;
            typedef TypeList<> ArgumentTypes;

            static const size_t Arity = 0;
            static const FunctionType FunctionType = FUNCTION_TYPE_FREE;
            static const bool IsConst = false;
            static const bool IsVolatile = false;
            static const bool IsVariadic = false;
        };

        template<typename Return>
        const size_t __FunctionTraits<Return()>::Arity;

        template<typename Return>
        const FunctionType __FunctionTraits<Return()>::FunctionType;

        template<typename Return>
        const bool __FunctionTraits<Return()>::IsConst;

        template<typename Return>
        const bool __FunctionTraits<Return()>::IsVolatile;

        template<typename Return>
        const bool __FunctionTraits<Return()>::IsVariadic;

        // Free function, variadic

        template<typename Return, typename Arg1, typename Arg2>
        struct __FunctionTraits<Return(Arg1, Arg2, ...), void> : __FunctionTraits<Return(Arg1, Arg2)> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return(Arg1, Arg2, ...)>::IsVariadic;

        template<typename Return, typename Arg>
        struct __FunctionTraits<Return(Arg, ...), void> : __FunctionTraits<Return(Arg)> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Arg>
        const bool __FunctionTraits<Return(Arg, ...)>::IsVariadic;

        template<typename Return>
        struct __FunctionTraits<Return(...), void> : __FunctionTraits<Return()> {
            static const bool IsVariadic = true;
        };

        template<typename Return>
        const bool __FunctionTraits<Return(...)>::IsVariadic;


        // Member function

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        struct __FunctionTraits<Return (Object::*)(Arg1, Arg2), void> {
            typedef Return ResultType;
            typedef Object ObjectType;
            typedef TypeList<Arg1, Arg2> ArgumentTypes;

            static const size_t Arity = 2;
            static const FunctionType FunctionType = FUNCTION_TYPE_MEMBER;
            static const bool IsConst = false;
            static const bool IsVolatile = false;
            static const bool IsVariadic = false;
        };

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const size_t __FunctionTraits<Return (Object::*)(Arg1, Arg2)>::Arity;

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const FunctionType __FunctionTraits<Return (Object::*)(Arg1, Arg2)>::FunctionType;

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return (Object::*)(Arg1, Arg2)>::IsConst;

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return (Object::*)(Arg1, Arg2)>::IsVolatile;

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return (Object::*)(Arg1, Arg2)>::IsVariadic;

        template<typename Return, typename Object, typename Arg>
        struct __FunctionTraits<Return (Object::*)(Arg), void> {
            typedef Return ResultType;
            typedef Object ObjectType;
            typedef TypeList<Arg> ArgumentTypes;

            static const size_t Arity = 1;
            static const FunctionType FunctionType = FUNCTION_TYPE_MEMBER;
            static const bool IsConst = false;
            static const bool IsVolatile = false;
            static const bool IsVariadic = false;
        };

        template<typename Return, typename Object, typename Arg>
        const size_t __FunctionTraits<Return (Object::*)(Arg)>::Arity;

        template<typename Return, typename Object, typename Arg>
        const FunctionType __FunctionTraits<Return (Object::*)(Arg)>::FunctionType;

        template<typename Return, typename Object, typename Arg>
        const bool __FunctionTraits<Return (Object::*)(Arg)>::IsConst;

        template<typename Return, typename Object, typename Arg>
        const bool __FunctionTraits<Return (Object::*)(Arg)>::IsVolatile;

        template<typename Return, typename Object, typename Arg>
        const bool __FunctionTraits<Return (Object::*)(Arg)>::IsVariadic;

        template<typename Return, typename Object>
        struct __FunctionTraits<Return (Object::*)(), void> {
            typedef Return ResultType;
            typedef Object ObjectType;
            typedef TypeList<> ArgumentTypes;

            static const size_t Arity = 0;
            static const FunctionType FunctionType = FUNCTION_TYPE_MEMBER;
            static const bool IsConst = false;
            static const bool IsVolatile = false;
            static const bool IsVariadic = false;
        };

        template<typename Return, typename Object>
        const size_t __FunctionTraits<Return (Object::*)()>::Arity;

        template<typename Return, typename Object>
        const FunctionType __FunctionTraits<Return (Object::*)()>::FunctionType;

        template<typename Return, typename Object>
        const bool __FunctionTraits<Return (Object::*)()>::IsConst;

        template<typename Return, typename Object>
        const bool __FunctionTraits<Return (Object::*)()>::IsVolatile;

        template<typename Return, typename Object>
        const bool __FunctionTraits<Return (Object::*)()>::IsVariadic;
        
        // Member function, variadic

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        struct __FunctionTraits<Return (Object::*)(Arg1, Arg2, ...), void> : __FunctionTraits<Return (Object::*)(Arg1, Arg2)> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return (Object::*)(Arg1, Arg2, ...)>::IsVariadic;

        template<typename Return, typename Object, typename Arg>
        struct __FunctionTraits<Return (Object::*)(Arg, ...), void> : __FunctionTraits<Return (Object::*)(Arg)> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename Arg>
        const bool __FunctionTraits<Return (Object::*)(Arg, ...)>::IsVariadic;

        template<typename Return, typename Object>
        struct __FunctionTraits<Return (Object::*)(...), void> : __FunctionTraits<Return (Object::*)()> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object>
        const bool __FunctionTraits<Return (Object::*)(...)>::IsVariadic;

        // Member function, const

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        struct __FunctionTraits<Return (Object::*)(Arg1, Arg2) const, void> : __FunctionTraits<Return (Object::*)(Arg1, Arg2)> {
            static const bool IsConst = true;
        };

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return (Object::*)(Arg1, Arg2) const>::IsConst;

        template<typename Return, typename Object, typename Arg>
        struct __FunctionTraits<Return (Object::*)(Arg) const, void> : __FunctionTraits<Return (Object::*)(Arg)> {
            static const bool IsConst = true;
        };

        template<typename Return, typename Object, typename Arg>
        const bool __FunctionTraits<Return (Object::*)(Arg) const>::IsConst;

        template<typename Return, typename Object>
        struct __FunctionTraits<Return (Object::*)() const, void> : __FunctionTraits<Return (Object::*)()> {
            static const bool IsConst = true;
        };

        template<typename Return, typename Object>
        const bool __FunctionTraits<Return (Object::*)() const>::IsConst;

        // Member function, const variadic

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        struct __FunctionTraits<Return (Object::*)(Arg1, Arg2, ...) const, void> : __FunctionTraits<Return (Object::*)(Arg1, Arg2) const> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return (Object::*)(Arg1, Arg2, ...) const>::IsVariadic;

        template<typename Return, typename Object, typename Arg>
        struct __FunctionTraits<Return (Object::*)(Arg, ...) const, void> : __FunctionTraits<Return (Object::*)(Arg) const> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename Arg>
        const bool __FunctionTraits<Return (Object::*)(Arg, ...) const>::IsVariadic;

        template<typename Return, typename Object>
        struct __FunctionTraits<Return (Object::*)(...) const, void> : __FunctionTraits<Return (Object::*)() const> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object>
        const bool __FunctionTraits<Return (Object::*)(...) const>::IsVariadic;

        // Member function, volatile

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        struct __FunctionTraits<Return (Object::*)(Arg1, Arg2) volatile, void> : __FunctionTraits<Return (Object::*)(Arg1, Arg2)> {
            static const bool IsVolatile = true;
        };

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return (Object::*)(Arg1, Arg2) volatile>::IsVolatile;

        template<typename Return, typename Object, typename Arg>
        struct __FunctionTraits<Return (Object::*)(Arg) volatile, void> : __FunctionTraits<Return (Object::*)(Arg)> {
            static const bool IsVolatile = true;
        };

        template<typename Return, typename Object, typename Arg>
        const bool __FunctionTraits<Return (Object::*)(Arg) volatile>::IsVolatile;

        template<typename Return, typename Object>
        struct __FunctionTraits<Return (Object::*)() volatile, void> : __FunctionTraits<Return (Object::*)()> {
            static const bool IsVolatile = true;
        };

        template<typename Return, typename Object>
        const bool __FunctionTraits<Return (Object::*)() volatile>::IsVolatile;

        // Member function, volatile variadic

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        struct __FunctionTraits<Return (Object::*)(Arg1, Arg2, ...) volatile, void> : __FunctionTraits<Return (Object::*)(Arg1, Arg2) volatile> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return (Object::*)(Arg1, Arg2, ...) volatile>::IsVariadic;

        template<typename Return, typename Object, typename Arg>
        struct __FunctionTraits<Return (Object::*)(Arg, ...) volatile, void> : __FunctionTraits<Return (Object::*)(Arg) volatile> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename Arg>
        const bool __FunctionTraits<Return (Object::*)(Arg, ...) volatile>::IsVariadic;

        template<typename Return, typename Object>
        struct __FunctionTraits<Return (Object::*)(...) volatile, void> : __FunctionTraits<Return (Object::*)() volatile> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object>
        const bool __FunctionTraits<Return (Object::*)(...) volatile>::IsVariadic;

        // Member function, const volatile

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        struct __FunctionTraits<Return (Object::*)(Arg1, Arg2) const volatile, void> : __FunctionTraits<Return (Object::*)(Arg1, Arg2)> {
            static const bool IsConst = true;
            static const bool IsVolatile = true;
        };

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return (Object::*)(Arg1, Arg2) const volatile>::IsConst;

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return (Object::*)(Arg1, Arg2) const volatile>::IsVolatile;


        template<typename Return, typename Object, typename Arg>
        struct __FunctionTraits<Return (Object::*)(Arg) const volatile, void> : __FunctionTraits<Return (Object::*)(Arg)> {
            static const bool IsConst = true;
            static const bool IsVolatile = true;
        };

        template<typename Return, typename Object, typename Arg>
        const bool __FunctionTraits<Return (Object::*)(Arg) const volatile>::IsConst;

        template<typename Return, typename Object, typename Arg>
        const bool __FunctionTraits<Return (Object::*)(Arg) const volatile>::IsVolatile;

        template<typename Return, typename Object>
        struct __FunctionTraits<Return (Object::*)() const volatile, void> : __FunctionTraits<Return (Object::*)()> {
            static const bool IsConst = true;
            static const bool IsVolatile = true;
        };

        template<typename Return, typename Object>
        const bool __FunctionTraits<Return (Object::*)() const volatile>::IsConst;

        template<typename Return, typename Object>
        const bool __FunctionTraits<Return (Object::*)() const volatile>::IsVolatile;

        // Member function, const volatile variadic

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        struct __FunctionTraits<Return (Object::*)(Arg1, Arg2, ...) const volatile, void> : __FunctionTraits<Return (Object::*)(Arg1, Arg2) const volatile> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename Arg1, typename Arg2>
        const bool __FunctionTraits<Return (Object::*)(Arg1, Arg2, ...) const volatile>::IsVariadic;


        template<typename Return, typename Object, typename Arg>
        struct __FunctionTraits<Return (Object::*)(Arg, ...) const volatile, void> : __FunctionTraits<Return (Object::*)(Arg) const volatile> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object, typename Arg>
        const bool __FunctionTraits<Return (Object::*)(Arg, ...) const volatile>::IsVariadic;

        template<typename Return, typename Object>
        struct __FunctionTraits<Return (Object::*)(...) const volatile, void> : __FunctionTraits<Return (Object::*)() const volatile> {
            static const bool IsVariadic = true;
        };

        template<typename Return, typename Object>
        const bool __FunctionTraits<Return (Object::*)(...) const volatile>::IsVariadic;
        #endif
    }

    /// @brief Traits class to extract information about a function type
    /// @tparam T The function type to extract information from
    /// @ingroup utilities
    template<typename T>
    struct FunctionTraits : __private::__FunctionTraits<typename RemovePointer<typename RemoveCVReference<T>::Type>::Type> {};
}

#endif
