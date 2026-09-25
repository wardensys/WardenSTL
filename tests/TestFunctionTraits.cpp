// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/FunctionTraits.hpp>


struct MemberFunction {
    int Fn0();
    int Fn1(int);
    int Fn2(int, char);
    int Fn3(int, char, double);
    int Fn0c() const;
    int Fn1c(int) const;
    int Fn2c(int, char) const;
    int Fn3c(int, char, double) const;
    int Fn0v() volatile;
    int Fn1v(int) volatile;
    int Fn2v(int, char) volatile;
    int Fn3v(int, char, double) volatile;
    int Fn0cv() const volatile;
    int Fn1cv(int) const volatile;
    int Fn2cv(int, char) const volatile;
    int Fn3cv(int, char, double) const volatile;
    void VoidFn(int);
    long LongFn();
    short ShortFn(int, char);

    int FnNoexcept(char) __WSTL_NOEXCEPT__;
    
    #ifdef __WSTL_CXX11__
    int FnRefOnly(char) &;
    int FnRRefOnly(char) &&;
    #endif

    static long FnStatic(int);
    char FnVariadic(long, char, ...) __WSTL_NOEXCEPT__;
};

void FreeVoid(int);
int Free0();
int Free1(int);
int Free2(int, char);
int Free3(int, char, double);
template<typename T> T Free0t();
int FreeNoexcept(char) __WSTL_NOEXCEPT__;
long FreeVariadic(int, ...);

struct FunctorNoexcept { 
    int operator()() __WSTL_NOEXCEPT__; 
};

struct Functor1 { 
    typedef int ResultType;
    typedef long ArgumentType;

    int operator()(long); 
};

struct Functor2 { 
    typedef long ResultType;
    typedef int FirstArgumentType;
    typedef char SecondArgumentType;
    
    long operator()(int, char); 
};

struct Functor0 {
    typedef long ResultType;

    long operator()() const;
};

TEST_SUITE("FunctionTraits") {
    TEST_CASE("Free void") {
        typedef decltype(FreeVoid) Function;
        typedef decltype(&FreeVoid) const volatile FunctionPtr;
        typedef decltype(FreeVoid)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, void>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, void>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FREE);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Free 0 parameters") {
        typedef decltype(Free0) Function;
        typedef decltype(&Free0) const volatile FunctionPtr;
        typedef decltype(Free0)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, void>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 0);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FREE);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Free 1 parameter") {
        typedef decltype(Free1) Function;
        typedef decltype(&Free1) const volatile FunctionPtr;
        typedef decltype(Free1)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, void>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FREE);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Free 2 parameters") {
        typedef decltype(Free2) Function;
        typedef decltype(&Free2) const volatile FunctionPtr;
        typedef decltype(Free2)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, void>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 2);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FREE);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsNothrow);
        #endif
        #endif
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("Free 3 parameters") {
        typedef decltype(Free3) Function;
        typedef decltype(&Free3) const volatile FunctionPtr;
        typedef decltype(Free3)& FunctionRef;

        CHECK(wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value);
        CHECK(wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value);
        CHECK(wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value);

        CHECK(wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, void>::Value);
        CHECK(wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value);
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 3);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FREE);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsNothrow);
        #endif
    }
    #endif

    TEST_CASE("Free template") {
        typedef decltype(Free0t<char>) Function;
        typedef decltype(&Free0t<char>) const volatile FunctionPtr;
        typedef decltype(Free0t<char>)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, void>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, char>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 0);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FREE);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Free noexcept") {
        typedef decltype(FreeNoexcept) Function;
        typedef decltype(&FreeNoexcept) const volatile FunctionPtr;
        typedef decltype(FreeNoexcept)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, void>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FREE);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Free variadic") {
        typedef decltype(FreeVariadic) Function;
        typedef decltype(&FreeVariadic) const volatile FunctionPtr;
        typedef decltype(FreeVariadic)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, void>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, long>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FREE);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("Functor noexcept") {
        typedef FunctorNoexcept Function;
        typedef FunctorNoexcept* const volatile FunctionPtr;
        typedef FunctorNoexcept& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, FunctorNoexcept>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 0);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FUNCTOR);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
    }
    #endif

    TEST_CASE("Functor 0 parameter") {
        typedef Functor0 Function;
        typedef Functor0* const volatile FunctionPtr;
        typedef Functor0& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, Functor0>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, long>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 0);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FUNCTOR);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);
    }

    TEST_CASE("Functor 1 parameter") {
        typedef Functor1 Function;
        typedef Functor1* const volatile FunctionPtr;
        typedef Functor1& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<long> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<long> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<long> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, Functor1>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FUNCTOR);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);
    }

    TEST_CASE("Functor 2 parameters") {
        typedef Functor2 Function;
        typedef Functor2* const volatile FunctionPtr;
        typedef Functor2& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, Functor2>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, long>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 2);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FUNCTOR);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);
    }

    TEST_CASE("Member 0 parameters") {
        typedef decltype(&MemberFunction::Fn0) Function;
        typedef decltype(&MemberFunction::Fn0) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn0)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 0);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member 0 parameters const") {
        typedef decltype(&MemberFunction::Fn0c) Function;
        typedef decltype(&MemberFunction::Fn0c) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn0c)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 0);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member 0 parameters volatile") {
        typedef decltype(&MemberFunction::Fn0v) Function;
        typedef decltype(&MemberFunction::Fn0v) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn0v)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 0);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member 0 parameters const volatile") {
        typedef decltype(&MemberFunction::Fn0cv) Function;
        typedef decltype(&MemberFunction::Fn0cv) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn0cv)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 0);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK(wstl::FunctionTraits<Function>::IsConst);
        CHECK(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member 1 parameter") {
        typedef decltype(&MemberFunction::Fn1) Function;
        typedef decltype(&MemberFunction::Fn1) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn1)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member 1 parameter const") {
        typedef decltype(&MemberFunction::Fn1c) Function;
        typedef decltype(&MemberFunction::Fn1c) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn1c)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member 1 parameter volatile") {
        typedef decltype(&MemberFunction::Fn1v) Function;
        typedef decltype(&MemberFunction::Fn1v) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn1v)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member 1 parameter const volatile") {
        typedef decltype(&MemberFunction::Fn1cv) Function;
        typedef decltype(&MemberFunction::Fn1cv) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn1cv)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK(wstl::FunctionTraits<Function>::IsConst);
        CHECK(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }



    TEST_CASE("Member 2 parameters") {
        typedef decltype(&MemberFunction::Fn2) Function;
        typedef decltype(&MemberFunction::Fn2) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn2)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 2);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member 2 parameters const") {
        typedef decltype(&MemberFunction::Fn2c) Function;
        typedef decltype(&MemberFunction::Fn2c) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn2c)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 2);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member 2 parameters volatile") {
        typedef decltype(&MemberFunction::Fn2v) Function;
        typedef decltype(&MemberFunction::Fn2v) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn2v)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 2);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member 2 parameters const volatile") {
        typedef decltype(&MemberFunction::Fn2cv) Function;
        typedef decltype(&MemberFunction::Fn2cv) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn2cv)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 2);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK(wstl::FunctionTraits<Function>::IsConst);
        CHECK(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("Member 3 parameters") {
        typedef decltype(&MemberFunction::Fn3) Function;
        typedef decltype(&MemberFunction::Fn3) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn3)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 3);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
    }

    TEST_CASE("Member 3 parameters const") {
        typedef decltype(&MemberFunction::Fn3c) Function;
        typedef decltype(&MemberFunction::Fn3c) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn3c)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 3);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
    }

    TEST_CASE("Member 3 parameters volatile") {
        typedef decltype(&MemberFunction::Fn3v) Function;
        typedef decltype(&MemberFunction::Fn3v) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn3v)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 3);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
    }

    TEST_CASE("Member 3 parameters const volatile") {
        typedef decltype(&MemberFunction::Fn3cv) Function;
        typedef decltype(&MemberFunction::Fn3cv) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::Fn3cv)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char, double>>::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 3);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK(wstl::FunctionTraits<Function>::IsConst);
        CHECK(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
    }
    #endif

    TEST_CASE("Member void function") {
        typedef decltype(&MemberFunction::VoidFn) Function;
        typedef decltype(&MemberFunction::VoidFn) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::VoidFn)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, void>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member long function") {
        typedef decltype(&MemberFunction::LongFn) Function;
        typedef decltype(&MemberFunction::LongFn) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::LongFn)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, long>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 0);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member short function") {
        typedef decltype(&MemberFunction::ShortFn) Function;
        typedef decltype(&MemberFunction::ShortFn) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::ShortFn)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int, char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, short>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 2);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member noexcept function") {
        typedef decltype(&MemberFunction::FnNoexcept) Function;
        typedef decltype(&MemberFunction::FnNoexcept) const volatile FunctionPtr;

        #ifdef __WSTL_CXX17__
        typedef decltype(&MemberFunction::FnNoexcept)& FunctionRef;
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<char> >::Value));
        #endif

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("Member lvalue reference qualified function") {
        typedef decltype(&MemberFunction::FnRefOnly) Function;
        typedef decltype(&MemberFunction::FnRefOnly) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::FnRefOnly)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_LVALUE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
    }
    
    TEST_CASE("Member rvalue reference qualified function") {
        typedef decltype(&MemberFunction::FnRRefOnly) Function;
        typedef decltype(&MemberFunction::FnRRefOnly) const volatile FunctionPtr;
        typedef decltype(&MemberFunction::FnRRefOnly)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, int>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_RVALUE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
    }
    #endif

    TEST_CASE("Member static function") {
        typedef decltype(MemberFunction::FnStatic) Function;
        typedef decltype(&MemberFunction::FnStatic) const volatile FunctionPtr;
        typedef decltype(MemberFunction::FnStatic)& FunctionRef;

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<int> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, void>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, long>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 1);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_FREE);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }

    TEST_CASE("Member variadic function") {
        typedef decltype(&MemberFunction::FnVariadic) Function;
        typedef decltype(&MemberFunction::FnVariadic) const volatile FunctionPtr;

        #ifdef __WSTL_CXX17__
        typedef decltype(&MemberFunction::FnVariadic)& FunctionRef;
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionRef>::ArgumentTypes, wstl::TypeList<long, char> >::Value));
        #endif

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ArgumentTypes, wstl::TypeList<long, char> >::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<FunctionPtr>::ArgumentTypes, wstl::TypeList<long, char> >::Value));

        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ObjectType, MemberFunction>::Value));
        CHECK((wstl::IsSame<wstl::FunctionTraits<Function>::ResultType, char>::Value));
        CHECK_EQ(wstl::FunctionTraits<Function>::Arity, 2);
        CHECK_EQ(wstl::FunctionTraits<Function>::FunctionType, wstl::FUNCTION_TYPE_MEMBER);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsConst);
        CHECK_FALSE(wstl::FunctionTraits<Function>::IsVolatile);
        CHECK(wstl::FunctionTraits<Function>::IsVariadic);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::FunctionTraits<Function>::RefQualifier, wstl::REF_QUALIFIER_NONE);

        #ifdef __WSTL_CXX17__
        CHECK(wstl::FunctionTraits<FunctionPtr>::IsNothrow);
        #endif
        #endif
    }
}
