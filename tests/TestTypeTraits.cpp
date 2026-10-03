// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s test_type_traits.cpp
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// Some tests have been adapted and extended by Artem Bezruchko (WardenHD)
// to improve coverage and match WardenSTL's implementation.
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/TypeTraits.hpp>
#ifdef __WSTL_CXX11__
#include <type_traits>
#endif
#include <cstddef>
#include <climits>

#include "Utils.hpp"


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
    char FnVariadic(long, char, ...) const __WSTL_NOEXCEPT__;
};

void FreeVoid(int);
int Free0();
int Free1(int);
int Free2(int, char);
int Free3(int, char, double);
template<typename T> T Free0t();
int FreeNoexcept(char) __WSTL_NOEXCEPT__;
long FreeVariadic(int, ...);

struct FunctorNoexcept { typedef int ResultType; int operator()() __WSTL_NOEXCEPT__; };
struct Functor0 { typedef int ResultType; int operator()(); };
struct Functor2 { typedef long ResultType; long operator()(int, char); };

template<typename T>
T TestTypeIdentity(T first, typename wstl::TypeIdentity<T>::Type second) {
    return first + second;
}

struct TestData {};
class ClassData {};
enum EnumData {};

#ifdef __WSTL_CXX11__
enum class EnumClassData {};
#endif

struct FakeEnum {
    operator int();
};

class A {
public:
    int M;
};

class BBaseA : public A {
public:
    int MB;
};

class CBaseB : BBaseA {};
class D {};

struct Explicit {
    explicit Explicit(int);
};

struct ExplicitDefault {
    explicit ExplicitDefault() {}
};

struct Implicit {
    Implicit(int);
};

struct ToBool {
    operator bool() const;
};

struct TrivialConstructor {
    int M;
};

struct VirtualFunction {
    virtual void Foo();
};

struct NonTrivialConstructor {
    NonTrivialConstructor() {}
};

struct NothrowData {
    NothrowData() __WSTL_NOEXCEPT__ {}

    NothrowData(const NothrowData&) __WSTL_NOEXCEPT__ {}
    NothrowData& operator=(const NothrowData&) __WSTL_NOEXCEPT__;
    
    #ifdef __WSTL_CXX11__
    NothrowData(NothrowData&&) noexcept {}
    NothrowData& operator=(NothrowData&&) noexcept;
    #endif
    
    ~NothrowData() __WSTL_NOEXCEPT__ {}
};

struct PrivateDefaultConstructor {
private:
    PrivateDefaultConstructor() __WSTL_DELETE__;
};

struct NoCopyConstructor {
private:
    NoCopyConstructor(const NoCopyConstructor&) __WSTL_DELETE__;
};

struct NoCopyAssignment {
private:
    NoCopyAssignment& operator=(const NoCopyAssignment&) __WSTL_DELETE__;
};

#ifdef __WSTL_CXX11__
struct NoMoveAssignment {
    NoMoveAssignment& operator=(NoMoveAssignment&&) = delete;
};
#endif

struct NoDestructor {
private:
    ~NoDestructor() __WSTL_DELETE__;
};

struct PrivateDestructor {
private:
    ~PrivateDestructor() {}
};

struct Abstract {
    virtual void Foo() = 0;
};

struct FakeCopyAssignment {
    FakeCopyAssignment& operator=(FakeCopyAssignment&) { return *this; }
};

#ifdef __WSTL_CXX11__
struct NoMoveConstructor {
    NoMoveConstructor(NoMoveConstructor&&) = delete;
};
#endif

struct FakeCopyConstructor {
    FakeCopyConstructor(FakeCopyConstructor&) {}
};

struct CustomCopyMoveConstructor {
    CustomCopyMoveConstructor(const CustomCopyMoveConstructor&) {}
    
    #ifdef __WSTL_CXX11__
    CustomCopyMoveConstructor(CustomCopyMoveConstructor&&) {}
    #endif
};

union UnionData {
    class ClassData {};
};

typedef wstl::AlignedStorage<sizeof(uint16_t), wstl::AlignmentOf<uint32_t>::Value>::Type StorageType;

struct Object {
    int a;
    char b;
    float c;
};


TEST_SUITE("TypeTraits") {
    TEST_CASE("IntegralConstant") {
        #ifdef __WSTL_CXX17__
        CHECK_EQ(wstl::IntegralConstantValue<int, 1>, 1);
        CHECK(wstl::IsSameValue<int, wstl::IntegralConstant<int, 1>::ValueType>);
        
        CHECK(wstl::BoolConstantValue<true>);
        CHECK_FALSE(wstl::BoolConstantValue<false>);
        CHECK(wstl::IsSameValue<bool, wstl::BoolConstant<true>::ValueType>);

        CHECK(wstl::NegationValue<wstl::BoolConstant<false>>);
        CHECK_FALSE(wstl::NegationValue<wstl::BoolConstant<true>>);
        #else
        CHECK_EQ((wstl::IntegralConstant<int, 1>::Value), 1);
        CHECK((wstl::IsSame<int, wstl::IntegralConstant<int, 1>::ValueType>::Value));
        
        CHECK(wstl::BoolConstant<true>::Value);
        CHECK_FALSE(wstl::BoolConstant<false>::Value);
        CHECK((wstl::IsSame<bool, wstl::BoolConstant<true>::ValueType>::Value));

        CHECK((wstl::Negation<wstl::BoolConstant<false> >::Value));
        CHECK_FALSE((wstl::Negation<wstl::BoolConstant<true> >::Value));
        #endif
    }

    TEST_CASE("RemoveReference") {
        CHECK((wstl::IsSame<wstl::RemoveReference<int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveReference<int&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveReference<const int&>::Type, const int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveReference<volatile int&>::Type, volatile int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveReference<const volatile int&>::Type, const volatile int>::Value));

        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::RemoveReference<int>::Type, std::remove_reference<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveReference<int&>::Type, std::remove_reference<int&>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveReference<const int&>::Type, std::remove_reference<const int&>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveReference<volatile int&>::Type, std::remove_reference<volatile int&>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveReference<const volatile int&>::Type, std::remove_reference<const volatile int&>::type>::Value));
        CHECK(wstl::IsSame<wstl::RemoveReference<int&&>::Type, std::remove_reference<int&&>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveReference<const int&&>::Type, std::remove_reference<const int&&>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveReference<volatile int&&>::Type, std::remove_reference<volatile int&&>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveReference<const volatile int&&>::Type, std::remove_reference<const volatile int&&>::type>::Value);
        #endif
    }

    TEST_CASE("RemovePointer") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::RemovePointer<int>::Type, std::remove_pointer<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<const int>::Type, std::remove_pointer<const int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<int*>::Type, std::remove_pointer<int*>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<const int*>::Type, std::remove_pointer<const int*>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<volatile int*>::Type, std::remove_pointer<volatile int*>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<const volatile int*>::Type, std::remove_pointer<const volatile int*>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<int* const>::Type, std::remove_pointer<int* const>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<int* volatile>::Type, std::remove_pointer<int* volatile>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<int* const volatile>::Type, std::remove_pointer<int* const volatile>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::RemovePointer<int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<const int>::Type, const int>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<int*>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<const int*>::Type, const int>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<volatile int*>::Type, volatile int>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<const volatile int*>::Type, const volatile int>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<int* const>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<int* volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemovePointer<int* const volatile>::Type, int>::Value));
        #endif
    }

    TEST_CASE("RemoveConst") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::RemoveConst<int>::Type, std::remove_const<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveConst<const int>::Type, std::remove_const<const int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveConst<const volatile int>::Type, std::remove_const<const volatile int>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::RemoveConst<int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveConst<const int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveConst<const volatile int>::Type, volatile int>::Value));
        #endif
    }

    TEST_CASE("RemoveVolatile") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::RemoveVolatile<int>::Type, std::remove_volatile<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveVolatile<volatile int>::Type, std::remove_volatile<volatile int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveVolatile<const volatile int>::Type, std::remove_volatile<const volatile int>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::RemoveVolatile<int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveVolatile<volatile int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveVolatile<const volatile int>::Type, const int>::Value));
        #endif
    }

    TEST_CASE("RemoveCV") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::RemoveCV<int>::Type, std::remove_cv<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCV<const int>::Type, std::remove_cv<const int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCV<volatile int>::Type, std::remove_cv<volatile int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCV<const volatile int>::Type, std::remove_cv<const volatile int>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::RemoveCV<int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCV<const int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCV<volatile int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCV<const volatile int>::Type, int>::Value));
        #endif
    }

    TEST_CASE("RemoveExtent") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::RemoveExtent<int>::Type, std::remove_extent<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveExtent<int[]>::Type, std::remove_extent<int[]>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveExtent<int[10]>::Type, std::remove_extent<int[10]>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveExtent<int[10][50]>::Type, std::remove_extent<int[10][50]>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::RemoveExtent<int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveExtent<int[]>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveExtent<int[10]>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveExtent<int[10][50]>::Type, int[50]>::Value));
        #endif
    }

    TEST_CASE("RemoveAllExtents") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::RemoveAllExtents<int>::Type, std::remove_all_extents<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveAllExtents<int[10]>::Type, std::remove_all_extents<int[10]>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveAllExtents<int[10][10]>::Type, std::remove_all_extents<int[10][10]>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::RemoveAllExtents<int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveAllExtents<int[10]>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveAllExtents<int[10][10]>::Type, int>::Value));
        #endif
    }

    TEST_CASE("AddPointer") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::AddPointer<int>::Type, std::add_pointer<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<const int>::Type, std::add_pointer<const int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<int*>::Type, std::add_pointer<int*>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<const int*>::Type, std::add_pointer<const int*>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<int* const>::Type, std::add_pointer<int* const>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<int* volatile>::Type, std::add_pointer<int* volatile>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<int* const volatile>::Type, std::add_pointer<int* const volatile>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<int**>::Type, std::add_pointer<int**>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::AddPointer<int>::Type, int*>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<const int>::Type, const int*>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<int*>::Type, int**>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<const int*>::Type, const int**>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<int* const>::Type, int* const*>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<int* volatile>::Type, int* volatile*>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<int* const volatile>::Type, int* const volatile*>::Value));
        CHECK((wstl::IsSame<wstl::AddPointer<int**>::Type, int***>::Value));
        #endif
    }

    TEST_CASE("AddLValueReference") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::AddLValueReference<int&&>::Type, std::add_lvalue_reference<int&&>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddLValueReference<void>::Type, std::add_lvalue_reference<void>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddLValueReference<int>::Type, std::add_lvalue_reference<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddLValueReference<int*>::Type, std::add_lvalue_reference<int*>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddLValueReference<int&>::Type, std::add_lvalue_reference<int&>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::AddLValueReference<void>::Type, void>::Value));
        CHECK((wstl::IsSame<wstl::AddLValueReference<int>::Type, int&>::Value));
        CHECK((wstl::IsSame<wstl::AddLValueReference<int*>::Type, int*&>::Value));
        CHECK((wstl::IsSame<wstl::AddLValueReference<int&>::Type, int&>::Value));
        #endif
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("AddRValueReference") {
        CHECK(wstl::IsSame<wstl::AddRValueReference<void>::Type, std::add_rvalue_reference<void>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddRValueReference<int>::Type, std::add_rvalue_reference<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddRValueReference<int*>::Type, std::add_rvalue_reference<int*>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddRValueReference<int&>::Type, std::add_rvalue_reference<int&>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddRValueReference<int&&>::Type, std::add_rvalue_reference<int&&>::type>::Value);
    }
    #endif

    TEST_CASE("AddConst") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::AddConst<int>::Type, std::add_const<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddConst<const int>::Type, std::add_const<const int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddConst<volatile int>::Type, std::add_const<volatile int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddConst<int*>::Type, std::add_const<int*>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::AddConst<int>::Type, const int>::Value));
        CHECK((wstl::IsSame<wstl::AddConst<const int>::Type, const int>::Value));
        CHECK((wstl::IsSame<wstl::AddConst<volatile int>::Type, const volatile int>::Value));
        CHECK((wstl::IsSame<wstl::AddConst<int*>::Type, int* const>::Value));
        #endif
    }

    TEST_CASE("AddVolatile") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::AddVolatile<int>::Type, std::add_volatile<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddVolatile<const int>::Type, std::add_volatile<const int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddVolatile<volatile int>::Type, std::add_volatile<volatile int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddVolatile<int*>::Type, std::add_volatile<int*>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::AddVolatile<int>::Type, volatile int>::Value));
        CHECK((wstl::IsSame<wstl::AddVolatile<const int>::Type, const volatile int>::Value));
        CHECK((wstl::IsSame<wstl::AddVolatile<volatile int>::Type, volatile int>::Value));
        CHECK((wstl::IsSame<wstl::AddVolatile<int*>::Type, int* volatile>::Value));
        #endif
    }

    TEST_CASE("AddCV") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::AddCV<int>::Type, std::add_cv<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddCV<const int>::Type, std::add_cv<const int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddCV<volatile int>::Type, std::add_cv<volatile int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddCV<const volatile int>::Type, std::add_cv<const volatile int>::type>::Value));
        CHECK((wstl::IsSame<wstl::AddCV<int*>::Type, std::add_cv<int*>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::AddCV<int>::Type, const volatile int>::Value));
        CHECK((wstl::IsSame<wstl::AddCV<const int>::Type, const volatile int>::Value));
        CHECK((wstl::IsSame<wstl::AddCV<volatile int>::Type, const volatile int>::Value));
        CHECK((wstl::IsSame<wstl::AddCV<const volatile int>::Type, const volatile int>::Value));
        CHECK((wstl::IsSame<wstl::AddCV<int*>::Type, int* const volatile>::Value));
        #endif
    }

    TEST_CASE("RemoveCVReference") {
        #ifdef __WSTL_CXX11__
        CHECK(wstl::IsSame<wstl::RemoveCVReference<int&&>::Type, std::remove_cv<std::remove_reference<int&&>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<volatile int&&>::Type, std::remove_cv<std::remove_reference<volatile int&&>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<const int&&>::Type, std::remove_cv<std::remove_reference<const int&&>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<const volatile int&&>::Type, std::remove_cv<std::remove_reference<const volatile int&&>::type>::type>::Value);
        CHECK((wstl::IsSame<wstl::RemoveCVReference<int>::Type, std::remove_cv<std::remove_reference<int>::type>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<int* const>::Type, std::remove_cv<std::remove_reference<int* const>::type>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<int* volatile>::Type, std::remove_cv<std::remove_reference<int* volatile>::type>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<int* const volatile>::Type, std::remove_cv<std::remove_reference<int* const volatile>::type>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<const int* const&>::Type, std::remove_cv<std::remove_reference<const int* const&>::type>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<int&>::Type, std::remove_cv<std::remove_reference<int&>::type>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<const int>::Type, std::remove_cv<std::remove_reference<const int>::type>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<const int&>::Type, std::remove_cv<std::remove_reference<const int&>::type>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<volatile int>::Type, std::remove_cv<std::remove_reference<volatile int>::type>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<volatile int&>::Type, std::remove_cv<std::remove_reference<volatile int&>::type>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<const volatile int>::Type, std::remove_cv<std::remove_reference<const volatile int>::type>::type>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<const volatile int&>::Type, std::remove_cv<std::remove_reference<const volatile int&>::type>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::RemoveCVReference<int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<int* const>::Type, int*>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<int* volatile>::Type, int*>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<int* const volatile>::Type, int*>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<const int* const&>::Type, const int*>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<int&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<const int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<const int&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<volatile int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<volatile int&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<const volatile int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::RemoveCVReference<const volatile int&>::Type, int>::Value));
        #endif
    }

    TEST_CASE("AlignmentOf") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::AlignmentOf<char>::Value, std::alignment_of<char>::value);
        CHECK_EQ(wstl::AlignmentOf<unsigned char>::Value, std::alignment_of<unsigned char>::value);
        CHECK_EQ(wstl::AlignmentOf<short>::Value, std::alignment_of<short>::value);
        CHECK_EQ(wstl::AlignmentOf<unsigned short>::Value, std::alignment_of<unsigned short>::value);
        CHECK_EQ(wstl::AlignmentOf<int>::Value, std::alignment_of<int>::value);
        CHECK_EQ(wstl::AlignmentOf<unsigned int>::Value, std::alignment_of<unsigned int>::value);
        CHECK_EQ(wstl::AlignmentOf<long>::Value, std::alignment_of<long>::value);
        CHECK_EQ(wstl::AlignmentOf<unsigned long>::Value, std::alignment_of<unsigned long>::value);
        CHECK_EQ(wstl::AlignmentOf<long long>::Value, std::alignment_of<long long>::value);
        CHECK_EQ(wstl::AlignmentOf<unsigned long long>::Value, std::alignment_of<unsigned long long>::value);
        CHECK_EQ(wstl::AlignmentOf<float>::Value, std::alignment_of<float>::value);
        CHECK_EQ(wstl::AlignmentOf<double>::Value, std::alignment_of<double>::value);
        CHECK_EQ(wstl::AlignmentOf<Object>::Value, std::alignment_of<Object>::value);
        #else
        CHECK_EQ(wstl::AlignmentOf<char>::Value, 1UL);
        CHECK_EQ(wstl::AlignmentOf<unsigned char>::Value, 1UL);
        CHECK_EQ(wstl::AlignmentOf<short>::Value, 2UL);
        CHECK_EQ(wstl::AlignmentOf<unsigned short>::Value, 2UL);
        CHECK_EQ(wstl::AlignmentOf<int>::Value, 4UL);
        CHECK_EQ(wstl::AlignmentOf<unsigned int>::Value, 4UL);
        CHECK_EQ(wstl::AlignmentOf<long>::Value, 8UL);
        CHECK_EQ(wstl::AlignmentOf<unsigned long>::Value, 8UL);
        CHECK_EQ(wstl::AlignmentOf<long long>::Value, 8UL);
        CHECK_EQ(wstl::AlignmentOf<unsigned long long>::Value, 8UL);
        CHECK_EQ(wstl::AlignmentOf<float>::Value, 4UL);
        CHECK_EQ(wstl::AlignmentOf<double>::Value, 8UL);
        CHECK_EQ(wstl::AlignmentOf<Object>::Value, 4UL);
        #endif
    }

    TEST_CASE("Rank") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::Rank<int>::Value, std::rank<int>::value);
        CHECK_EQ(wstl::Rank<int[10]>::Value, std::rank<int[10]>::value);
        CHECK_EQ(wstl::Rank<int[10][10]>::Value, std::rank<int[10][10]>::value);
        #else
        CHECK_EQ(wstl::Rank<int>::Value, 0UL);
        CHECK_EQ(wstl::Rank<int[10]>::Value, 1UL);
        CHECK_EQ(wstl::Rank<int[10][10]>::Value, 2UL);
        #endif
    }

    TEST_CASE("Extent") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::Extent<int>::Value, std::extent<int>::value);
        CHECK_EQ(wstl::Extent<int[]>::Value, std::extent<int[]>::value);
        CHECK_EQ(wstl::Extent<int[10]>::Value, std::extent<int[10]>::value);
        #else
        CHECK_EQ(wstl::Extent<int>::Value, 0UL);
        CHECK_EQ(wstl::Extent<int[]>::Value, 0UL);
        CHECK_EQ(wstl::Extent<int[10]>::Value, 10UL);
        #endif
    }

    TEST_CASE("Conditional") {
        CHECK((wstl::IsSame<wstl::Conditional<true, int, char>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::Conditional<false, int, char>::Type, char>::Value));
    }

    TEST_CASE("ResultOf") {
        // Free functions
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(Free0)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(Free1)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(Free2)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(Free3)>::Type, int>::Value));
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(Free0t<char>)>::Type, char>::Value));
        #else
        CHECK((wstl::IsSame<wstl::ResultOf<char ()>::Type, char>::Value));
        #endif
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(FreeNoexcept)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(FreeVariadic)>::Type, long>::Value));

        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free0)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free1)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free2)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free3)>::Type, int>::Value));
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free0t<char>)>::Type, char>::Value));
        #else
        CHECK((wstl::IsSame<wstl::ResultOf<char (*)()>::Type, char>::Value));
        #endif
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&FreeNoexcept)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&FreeVariadic)>::Type, long>::Value));

        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free0) const>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free1) const>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free2) const>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free3) const>::Type, int>::Value));
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free0t<char>) const>::Type, char>::Value));
        #else
        CHECK((wstl::IsSame<wstl::ResultOf<char (*const)()>::Type, char>::Value));
        #endif
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&FreeNoexcept) const>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&FreeVariadic) const>::Type, long>::Value));

        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free0) volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free1) volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free2) volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free3) volatile>::Type, int>::Value));
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free0t<char>) volatile>::Type, char>::Value));
        #else
        CHECK((wstl::IsSame<wstl::ResultOf<char (*volatile)()>::Type, char>::Value));
        #endif
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&FreeNoexcept) volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&FreeVariadic) volatile>::Type, long>::Value));

        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free0) const volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free1) const volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free2) const volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free3) const volatile>::Type, int>::Value));
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&Free0t<char>) const volatile>::Type, char>::Value));
        #else
        CHECK((wstl::IsSame<wstl::ResultOf<char (*const volatile)()>::Type, char>::Value));
        #endif
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&FreeNoexcept) const volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&FreeVariadic) const volatile>::Type, long>::Value));

        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(Free0)&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(Free1)&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(Free2)&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(Free3)&>::Type, int>::Value));
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(Free0t<char>)&>::Type, char>::Value));
        #else
        CHECK((wstl::IsSame<wstl::ResultOf<char (&)()>::Type, char>::Value));
        #endif
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(FreeNoexcept)&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(FreeVariadic)&>::Type, long>::Value));

        // Member functions
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn0)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn1)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn2)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn3)>::Type, int>::Value));

        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn0) const>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn1) const>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn2) const>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn3) const>::Type, int>::Value));

        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn0) volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn1) volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn2) volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn3) volatile>::Type, int>::Value));
        
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn0) const volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn1) const volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn2) const volatile>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn3) const volatile>::Type, int>::Value));

        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn0)&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn1)&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn2)&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn3)&>::Type, int>::Value));

        // const
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn0c)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn1c)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn2c)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn3c)>::Type, int>::Value));

        // volatile
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn0v)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn1v)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn2v)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn3v)>::Type, int>::Value));

        // const volatile
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn0cv)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn1cv)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn2cv)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::Fn3cv)>::Type, int>::Value));

        // Return type variations
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::VoidFn)>::Type, void>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::LongFn)>::Type, long>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::ShortFn)>::Type, short>::Value));

        // Noexcept
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept)>::Type, int>::Value));

        // Variadic
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::FnVariadic)>::Type, char>::Value));

        // Ref qualifiers
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::FnRefOnly)>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<__TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly)>::Type, int>::Value));
        #endif

        // Functors
        CHECK((wstl::IsSame<wstl::ResultOf<Functor0>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::ResultOf<Functor2>::Type, long>::Value));
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("Conjunction") {
        CHECK(wstl::Conjunction<wstl::TrueType, wstl::TrueType, wstl::TrueType>::Value);
        CHECK_FALSE(wstl::Conjunction<wstl::FalseType, wstl::FalseType, wstl::FalseType>::Value);
        CHECK_FALSE(wstl::Conjunction<wstl::TrueType, wstl::FalseType, wstl::TrueType>::Value);
        
        #ifdef __WSTL_CXX17__
        CHECK(wstl::ConjunctionValue<wstl::TrueType, wstl::TrueType, wstl::TrueType>);
        CHECK_FALSE(wstl::ConjunctionValue<wstl::FalseType, wstl::FalseType, wstl::FalseType>);
        CHECK_FALSE(wstl::ConjunctionValue<wstl::TrueType, wstl::FalseType, wstl::TrueType>);
        #endif
    }

    TEST_CASE("Disjunction") {
        CHECK(wstl::Disjunction<wstl::TrueType, wstl::TrueType, wstl::TrueType>::Value);
        CHECK_FALSE(wstl::Disjunction<wstl::FalseType, wstl::FalseType, wstl::FalseType>::Value);
        CHECK(wstl::Disjunction<wstl::TrueType, wstl::FalseType, wstl::TrueType>::Value);
        
        #ifdef __WSTL_CXX17__
        CHECK(wstl::DisjunctionValue<wstl::TrueType, wstl::TrueType, wstl::TrueType>);
        CHECK_FALSE(wstl::DisjunctionValue<wstl::FalseType, wstl::FalseType, wstl::FalseType>);
        CHECK(wstl::DisjunctionValue<wstl::TrueType, wstl::FalseType, wstl::TrueType>);
        #endif
    }
    #endif

    TEST_CASE("Negation") {
        CHECK(wstl::Negation<wstl::FalseType>::Value);
        CHECK_FALSE(wstl::Negation<wstl::TrueType>::Value);
        
        #ifdef __WSTL_CXX17__
        CHECK(wstl::NegationValue<wstl::FalseType>);
        CHECK_FALSE(wstl::NegationValue<wstl::TrueType>);
        #endif
    }

    TEST_CASE("TypeIdentity") {
        CHECK_EQ(TestTypeIdentity(1.5f, 2), 3.5f);
    }

    TEST_CASE("IsConst") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsConst<int>::Value, std::is_const<int>::value);
        CHECK_EQ(wstl::IsConst<volatile int>::Value, std::is_const<volatile int>::value);
        CHECK_EQ(wstl::IsConst<const int>::Value, std::is_const<const int>::value);
        CHECK_EQ(wstl::IsConst<const volatile int>::Value, std::is_const<const volatile int>::value);
        #else
        CHECK_FALSE(wstl::IsConst<int>::Value);
        CHECK_FALSE(wstl::IsConst<volatile int>::Value);
        CHECK(wstl::IsConst<const int>::Value);
        CHECK(wstl::IsConst<const volatile int>::Value);
        #endif
    }

    TEST_CASE("IsVolatile") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsVolatile<int>::Value, std::is_volatile<int>::value);
        CHECK_EQ(wstl::IsVolatile<volatile int>::Value, std::is_volatile<volatile int>::value);
        CHECK_EQ(wstl::IsVolatile<const int>::Value, std::is_volatile<const int>::value);
        CHECK_EQ(wstl::IsVolatile<const volatile int>::Value, std::is_volatile<const volatile int>::value);
        #else
        CHECK_FALSE(wstl::IsVolatile<int>::Value);
        CHECK(wstl::IsVolatile<volatile int>::Value);
        CHECK_FALSE(wstl::IsVolatile<const int>::Value);
        CHECK(wstl::IsVolatile<const volatile int>::Value);
        #endif
    }

    TEST_CASE("IsSame") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsSame<int, int>::Value), (std::is_same<int, int>::value));
        CHECK_EQ((wstl::IsSame<int, char>::Value), (std::is_same<int, char>::value));
        #else
        CHECK((wstl::IsSame<int, int>::Value));
        CHECK_FALSE((wstl::IsSame<int, char>::Value));
        #endif
    }

    TEST_CASE("IsNullPointer") {
        #ifdef __WSTL_CXX11__
        CHECK(wstl::IsNullPointer<std::nullptr_t>::Value);
        CHECK(wstl::IsNullPointer<const std::nullptr_t>::Value);
        CHECK(wstl::IsNullPointer<volatile std::nullptr_t>::Value);
        CHECK(wstl::IsNullPointer<const volatile std::nullptr_t>::Value);
        #endif

        CHECK(wstl::IsNullPointer<wstl::nullptr_t>::Value);
        CHECK(wstl::IsNullPointer<wstl::NullPointerType>::Value);
        CHECK_FALSE(wstl::IsNullPointer<int>::Value);
    }

    TEST_CASE("IsVoid") {
        #ifdef __WSTL_CXX11__
        CHECK(wstl::IsVoid<wstl::VoidType<int, char>>::Value);

        CHECK_EQ(wstl::IsVoid<void>::Value, std::is_void<void>::value);
        CHECK_EQ(wstl::IsVoid<const void>::Value, std::is_void<const void>::value);
        CHECK_EQ(wstl::IsVoid<volatile void>::Value, std::is_void<volatile void>::value);
        CHECK_EQ(wstl::IsVoid<int>::Value, std::is_void<int>::value);
        #else
        CHECK(wstl::IsVoid<void>::Value);
        CHECK(wstl::IsVoid<const void>::Value);
        CHECK(wstl::IsVoid<volatile void>::Value);
        CHECK_FALSE(wstl::IsVoid<int>::Value);
        #endif
    }

    TEST_CASE("IsIntegral") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsIntegral<bool>::Value, std::is_integral<bool>::value);
        CHECK_EQ(wstl::IsIntegral<char>::Value, std::is_integral<char>::value);
        CHECK_EQ(wstl::IsIntegral<signed char>::Value, std::is_integral<signed char>::value);
        CHECK_EQ(wstl::IsIntegral<unsigned char>::Value, std::is_integral<unsigned char>::value);
        CHECK_EQ(wstl::IsIntegral<wchar_t>::Value, std::is_integral<wchar_t>::value);
        CHECK_EQ(wstl::IsIntegral<short>::Value, std::is_integral<short>::value);
        CHECK_EQ(wstl::IsIntegral<signed short>::Value, std::is_integral<signed short>::value);
        CHECK_EQ(wstl::IsIntegral<unsigned short>::Value, std::is_integral<unsigned short>::value);
        CHECK_EQ(wstl::IsIntegral<int>::Value, std::is_integral<int>::value);
        CHECK_EQ(wstl::IsIntegral<signed int>::Value, std::is_integral<signed int>::value);
        CHECK_EQ(wstl::IsIntegral<unsigned int>::Value, std::is_integral<unsigned int>::value);
        CHECK_EQ(wstl::IsIntegral<long>::Value, std::is_integral<long>::value);
        CHECK_EQ(wstl::IsIntegral<signed long>::Value, std::is_integral<signed long>::value);
        CHECK_EQ(wstl::IsIntegral<unsigned long>::Value, std::is_integral<unsigned long>::value);
        CHECK_EQ(wstl::IsIntegral<long long>::Value, std::is_integral<long long>::value);
        CHECK_EQ(wstl::IsIntegral<signed long long>::Value, std::is_integral<signed long long>::value);
        CHECK_EQ(wstl::IsIntegral<unsigned long long>::Value, std::is_integral<unsigned long long>::value);
        CHECK_EQ(wstl::IsIntegral<const int>::Value, std::is_integral<const int>::value);
        CHECK_EQ(wstl::IsIntegral<volatile int>::Value, std::is_integral<volatile int>::value);
        CHECK_EQ(wstl::IsIntegral<const int>::Value, std::is_integral<const int>::value);
        CHECK_EQ(wstl::IsIntegral<const volatile int>::Value, std::is_integral<const volatile int>::value);
        CHECK_EQ(wstl::IsIntegral<float>::Value, std::is_integral<float>::value);
        CHECK_EQ(wstl::IsIntegral<double>::Value, std::is_integral<double>::value);
        CHECK_EQ(wstl::IsIntegral<long double>::Value, std::is_integral<long double>::value);
        CHECK_EQ(wstl::IsIntegral<char16_t>::Value, std::is_integral<char16_t>::value);
        CHECK_EQ(wstl::IsIntegral<char32_t>::Value, std::is_integral<char32_t>::value);

        CHECK_EQ(wstl::IsIntegral<void>::Value, std::is_integral<void>::value);
        CHECK_EQ(wstl::IsIntegral<TestData>::Value, std::is_integral<TestData>::value);
        CHECK_EQ(wstl::IsIntegral<EnumData>::Value, std::is_integral<EnumData>::value);
        CHECK_EQ(wstl::IsIntegral<UnionData>::Value, std::is_integral<UnionData>::value);
        CHECK_EQ(wstl::IsIntegral<int[]>::Value, std::is_integral<int[]>::value);
        CHECK_EQ(wstl::IsIntegral<int&>::Value, std::is_integral<int&>::value);
        CHECK_EQ(wstl::IsIntegral<int TestData::*>::Value, std::is_integral<int TestData::*>::value);
        CHECK_EQ(wstl::IsIntegral<EnumData>::Value, std::is_integral<EnumData>::value);
        #else
        CHECK(wstl::IsIntegral<bool>::Value);
        CHECK(wstl::IsIntegral<char>::Value);
        CHECK(wstl::IsIntegral<signed char>::Value);
        CHECK(wstl::IsIntegral<unsigned char>::Value);
        CHECK(wstl::IsIntegral<wchar_t>::Value);
        CHECK(wstl::IsIntegral<short>::Value);
        CHECK(wstl::IsIntegral<signed short>::Value);
        CHECK(wstl::IsIntegral<unsigned short>::Value);
        CHECK(wstl::IsIntegral<int>::Value);
        CHECK(wstl::IsIntegral<signed int>::Value);
        CHECK(wstl::IsIntegral<unsigned int>::Value);
        CHECK(wstl::IsIntegral<long>::Value);
        CHECK(wstl::IsIntegral<signed long>::Value);
        CHECK(wstl::IsIntegral<unsigned long>::Value);
        CHECK(wstl::IsIntegral<long long>::Value);
        CHECK(wstl::IsIntegral<signed long long>::Value);
        CHECK(wstl::IsIntegral<unsigned long long>::Value);
        CHECK(wstl::IsIntegral<const int>::Value);
        CHECK(wstl::IsIntegral<volatile int>::Value);
        CHECK(wstl::IsIntegral<const int>::Value);
        CHECK(wstl::IsIntegral<const volatile int>::Value);
        CHECK_FALSE(wstl::IsIntegral<float>::Value);
        CHECK_FALSE(wstl::IsIntegral<double>::Value);
        CHECK_FALSE(wstl::IsIntegral<long double>::Value);

        CHECK_FALSE(wstl::IsIntegral<void>::Value);
        CHECK_FALSE(wstl::IsIntegral<TestData>::Value);
        CHECK_FALSE(wstl::IsIntegral<EnumData>::Value);
        CHECK_FALSE(wstl::IsIntegral<UnionData>::Value);
        CHECK_FALSE(wstl::IsIntegral<int[]>::Value);
        CHECK_FALSE(wstl::IsIntegral<int&>::Value);
        CHECK_FALSE(wstl::IsIntegral<int TestData::*>::Value);
        CHECK_FALSE(wstl::IsIntegral<EnumData>::Value);
        #endif

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsIntegral<EnumClassData>::Value, std::is_integral<EnumClassData>::value);
        #endif

        #ifdef __WSTL_CXX20__
        CHECK_EQ(wstl::IsIntegral<char8_t>::Value, std::is_integral<char8_t>::value);
        #endif
    }

    TEST_CASE("IsFloatingPoint") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsFloatingPoint<EnumClassData>::Value, std::is_floating_point<EnumClassData>::value);
        CHECK_EQ(wstl::IsFloatingPoint<char16_t>::Value, std::is_floating_point<char16_t>::value);
        CHECK_EQ(wstl::IsFloatingPoint<char32_t>::Value, std::is_floating_point<char32_t>::value);

        CHECK_EQ(wstl::IsFloatingPoint<bool>::Value, std::is_floating_point<bool>::value);
        CHECK_EQ(wstl::IsFloatingPoint<char>::Value, std::is_floating_point<char>::value);
        CHECK_EQ(wstl::IsFloatingPoint<signed char>::Value, std::is_floating_point<signed char>::value);
        CHECK_EQ(wstl::IsFloatingPoint<unsigned char>::Value, std::is_floating_point<unsigned char>::value);
        CHECK_EQ(wstl::IsFloatingPoint<wchar_t>::Value, std::is_floating_point<wchar_t>::value);
        CHECK_EQ(wstl::IsFloatingPoint<short>::Value, std::is_floating_point<short>::value);
        CHECK_EQ(wstl::IsFloatingPoint<signed short>::Value, std::is_floating_point<signed short>::value);
        CHECK_EQ(wstl::IsFloatingPoint<unsigned short>::Value, std::is_floating_point<unsigned short>::value);
        CHECK_EQ(wstl::IsFloatingPoint<int>::Value, std::is_floating_point<int>::value);
        CHECK_EQ(wstl::IsFloatingPoint<signed int>::Value, std::is_floating_point<signed int>::value);
        CHECK_EQ(wstl::IsFloatingPoint<unsigned int>::Value, std::is_floating_point<unsigned int>::value);
        CHECK_EQ(wstl::IsFloatingPoint<long>::Value, std::is_floating_point<long>::value);
        CHECK_EQ(wstl::IsFloatingPoint<signed long>::Value, std::is_floating_point<signed long>::value);
        CHECK_EQ(wstl::IsFloatingPoint<unsigned long>::Value, std::is_floating_point<unsigned long>::value);
        CHECK_EQ(wstl::IsFloatingPoint<long long>::Value, std::is_floating_point<long long>::value);
        CHECK_EQ(wstl::IsFloatingPoint<signed long long>::Value, std::is_floating_point<signed long long>::value);
        CHECK_EQ(wstl::IsFloatingPoint<unsigned long long>::Value, std::is_floating_point<unsigned long long>::value);
        CHECK_EQ(wstl::IsFloatingPoint<const int>::Value, std::is_floating_point<const int>::value);
        CHECK_EQ(wstl::IsFloatingPoint<volatile int>::Value, std::is_floating_point<volatile int>::value);
        CHECK_EQ(wstl::IsFloatingPoint<const volatile int>::Value, std::is_floating_point<const volatile int>::value);
        CHECK_EQ(wstl::IsFloatingPoint<float>::Value, std::is_floating_point<float>::value);
        CHECK_EQ(wstl::IsFloatingPoint<double>::Value, std::is_floating_point<double>::value);
        CHECK_EQ(wstl::IsFloatingPoint<long double>::Value, std::is_floating_point<long double>::value);

        CHECK_EQ(wstl::IsFloatingPoint<void>::Value, std::is_floating_point<void>::value);
        CHECK_EQ(wstl::IsFloatingPoint<TestData>::Value, std::is_floating_point<TestData>::value);
        CHECK_EQ(wstl::IsFloatingPoint<const TestData>::Value, std::is_floating_point<const TestData>::value);
        CHECK_EQ(wstl::IsFloatingPoint<EnumData>::Value, std::is_floating_point<EnumData>::value);
        CHECK_EQ(wstl::IsFloatingPoint<UnionData>::Value, std::is_floating_point<UnionData>::value);
        CHECK_EQ(wstl::IsFloatingPoint<int[]>::Value, std::is_floating_point<int[]>::value);
        CHECK_EQ(wstl::IsFloatingPoint<int&>::Value, std::is_floating_point<int&>::value);
        CHECK_EQ(wstl::IsFloatingPoint<int TestData::*>::Value, std::is_floating_point<int TestData::*>::value);
        CHECK_EQ(wstl::IsFloatingPoint<EnumData>::Value, std::is_floating_point<EnumData>::value);

        CHECK_EQ(wstl::IsFloatingPoint<const float>::Value, std::is_floating_point<const float>::value);
        CHECK_EQ(wstl::IsFloatingPoint<volatile float>::Value, std::is_floating_point<volatile float>::value);
        CHECK_EQ(wstl::IsFloatingPoint<const volatile float>::Value, std::is_floating_point<const volatile float>::value);
        #else
        CHECK_FALSE(wstl::IsFloatingPoint<bool>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<char>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<signed char>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<unsigned char>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<wchar_t>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<short>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<signed short>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<unsigned short>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<int>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<signed int>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<unsigned int>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<long>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<signed long>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<unsigned long>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<long long>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<signed long long>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<unsigned long long>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<const int>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<volatile int>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<const volatile int>::Value);
        CHECK(wstl::IsFloatingPoint<float>::Value);
        CHECK(wstl::IsFloatingPoint<double>::Value);
        CHECK(wstl::IsFloatingPoint<long double>::Value);

        CHECK_FALSE(wstl::IsFloatingPoint<void>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<TestData>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<const TestData>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<EnumData>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<UnionData>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<int[]>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<int&>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<int TestData::*>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<EnumData>::Value);

        CHECK(wstl::IsFloatingPoint<const float>::Value);
        CHECK(wstl::IsFloatingPoint<volatile float>::Value);
        CHECK(wstl::IsFloatingPoint<const volatile float>::Value);
        #endif

        #ifdef __WSTL_CXX20__
        CHECK_EQ(wstl::IsFloatingPoint<char8_t>::Value, std::is_floating_point<char8_t>::value);
        #endif
    }

    TEST_CASE("IsArithmetic") {
        

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsArithmetic<EnumClassData>::Value, std::is_arithmetic<EnumClassData>::value);
        CHECK_EQ(wstl::IsArithmetic<char16_t>::Value, std::is_arithmetic<char16_t>::value);
        CHECK_EQ(wstl::IsArithmetic<char32_t>::Value, std::is_arithmetic<char32_t>::value);

        CHECK_EQ(wstl::IsArithmetic<bool>::Value, std::is_arithmetic<bool>::value);
        CHECK_EQ(wstl::IsArithmetic<char>::Value, std::is_arithmetic<char>::value);
        CHECK_EQ(wstl::IsArithmetic<signed char>::Value, std::is_arithmetic<signed char>::value);
        CHECK_EQ(wstl::IsArithmetic<unsigned char>::Value, std::is_arithmetic<unsigned char>::value);
        CHECK_EQ(wstl::IsArithmetic<wchar_t>::Value, std::is_arithmetic<wchar_t>::value);
        CHECK_EQ(wstl::IsArithmetic<short>::Value, std::is_arithmetic<short>::value);
        CHECK_EQ(wstl::IsArithmetic<signed short>::Value, std::is_arithmetic<signed short>::value);
        CHECK_EQ(wstl::IsArithmetic<unsigned short>::Value, std::is_arithmetic<unsigned short>::value);
        CHECK_EQ(wstl::IsArithmetic<int>::Value, std::is_arithmetic<int>::value);
        CHECK_EQ(wstl::IsArithmetic<signed int>::Value, std::is_arithmetic<signed int>::value);
        CHECK_EQ(wstl::IsArithmetic<unsigned int>::Value, std::is_arithmetic<unsigned int>::value);
        CHECK_EQ(wstl::IsArithmetic<long>::Value, std::is_arithmetic<long>::value);
        CHECK_EQ(wstl::IsArithmetic<signed long>::Value, std::is_arithmetic<signed long>::value);
        CHECK_EQ(wstl::IsArithmetic<unsigned long>::Value, std::is_arithmetic<unsigned long>::value);
        CHECK_EQ(wstl::IsArithmetic<long long>::Value, std::is_arithmetic<long long>::value);
        CHECK_EQ(wstl::IsArithmetic<signed long long>::Value, std::is_arithmetic<signed long long>::value);
        CHECK_EQ(wstl::IsArithmetic<unsigned long long>::Value, std::is_arithmetic<unsigned long long>::value);
        CHECK_EQ(wstl::IsArithmetic<const int>::Value, std::is_arithmetic<const int>::value);
        CHECK_EQ(wstl::IsArithmetic<volatile int>::Value, std::is_arithmetic<volatile int>::value);
        CHECK_EQ(wstl::IsArithmetic<const int>::Value, std::is_arithmetic<const int>::value);
        CHECK_EQ(wstl::IsArithmetic<const volatile int>::Value, std::is_arithmetic<const volatile int>::value);
        CHECK_EQ(wstl::IsArithmetic<float>::Value, std::is_arithmetic<float>::value);
        CHECK_EQ(wstl::IsArithmetic<double>::Value, std::is_arithmetic<double>::value);
        CHECK_EQ(wstl::IsArithmetic<long double>::Value, std::is_arithmetic<long double>::value);

        CHECK_EQ(wstl::IsArithmetic<void>::Value, std::is_arithmetic<void>::value);
        CHECK_EQ(wstl::IsArithmetic<TestData>::Value, std::is_arithmetic<TestData>::value);
        CHECK_EQ(wstl::IsArithmetic<EnumData>::Value, std::is_arithmetic<EnumData>::value);
        CHECK_EQ(wstl::IsArithmetic<UnionData>::Value, std::is_arithmetic<UnionData>::value);
        CHECK_EQ(wstl::IsArithmetic<int[]>::Value, std::is_arithmetic<int[]>::value);
        CHECK_EQ(wstl::IsArithmetic<int&>::Value, std::is_arithmetic<int&>::value);
        CHECK_EQ(wstl::IsArithmetic<int TestData::*>::Value, std::is_arithmetic<int TestData::*>::value);
        CHECK_EQ(wstl::IsArithmetic<EnumData>::Value, std::is_arithmetic<EnumData>::value);
        #else
        CHECK(wstl::IsArithmetic<bool>::Value);
        CHECK(wstl::IsArithmetic<char>::Value);
        CHECK(wstl::IsArithmetic<signed char>::Value);
        CHECK(wstl::IsArithmetic<unsigned char>::Value);
        CHECK(wstl::IsArithmetic<wchar_t>::Value);
        CHECK(wstl::IsArithmetic<short>::Value);
        CHECK(wstl::IsArithmetic<signed short>::Value);
        CHECK(wstl::IsArithmetic<unsigned short>::Value);
        CHECK(wstl::IsArithmetic<int>::Value);
        CHECK(wstl::IsArithmetic<signed int>::Value);
        CHECK(wstl::IsArithmetic<unsigned int>::Value);
        CHECK(wstl::IsArithmetic<long>::Value);
        CHECK(wstl::IsArithmetic<signed long>::Value);
        CHECK(wstl::IsArithmetic<unsigned long>::Value);
        CHECK(wstl::IsArithmetic<long long>::Value);
        CHECK(wstl::IsArithmetic<signed long long>::Value);
        CHECK(wstl::IsArithmetic<unsigned long long>::Value);
        CHECK(wstl::IsArithmetic<const int>::Value);
        CHECK(wstl::IsArithmetic<volatile int>::Value);
        CHECK(wstl::IsArithmetic<const int>::Value);
        CHECK(wstl::IsArithmetic<const volatile int>::Value);
        CHECK(wstl::IsArithmetic<float>::Value);
        CHECK(wstl::IsArithmetic<double>::Value);
        CHECK(wstl::IsArithmetic<long double>::Value);

        CHECK_FALSE(wstl::IsArithmetic<void>::Value);
        CHECK_FALSE(wstl::IsArithmetic<TestData>::Value);
        CHECK_FALSE(wstl::IsArithmetic<EnumData>::Value);
        CHECK_FALSE(wstl::IsArithmetic<UnionData>::Value);
        CHECK_FALSE(wstl::IsArithmetic<int[]>::Value);
        CHECK_FALSE(wstl::IsArithmetic<int&>::Value);
        CHECK_FALSE(wstl::IsArithmetic<int TestData::*>::Value);
        CHECK_FALSE(wstl::IsArithmetic<EnumData>::Value);
        #endif

        #ifdef __WSTL_CXX20__
        CHECK_EQ(wstl::IsArithmetic<char8_t>::Value, std::is_arithmetic<char8_t>::value);
        #endif
    }

    TEST_CASE("IsFundamental") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsFundamental<EnumClassData>::Value, std::is_fundamental<EnumClassData>::value);
        CHECK_EQ(wstl::IsFundamental<char16_t>::Value, std::is_fundamental<char16_t>::value);
        CHECK_EQ(wstl::IsFundamental<char32_t>::Value, std::is_fundamental<char32_t>::value);
        CHECK_EQ(wstl::IsFundamental<std::nullptr_t>::Value, std::is_fundamental<std::nullptr_t>::value);

        CHECK_EQ(wstl::IsFundamental<bool>::Value, std::is_fundamental<bool>::value);
        CHECK_EQ(wstl::IsFundamental<char>::Value, std::is_fundamental<char>::value);
        CHECK_EQ(wstl::IsFundamental<signed char>::Value, std::is_fundamental<signed char>::value);
        CHECK_EQ(wstl::IsFundamental<unsigned char>::Value, std::is_fundamental<unsigned char>::value);
        CHECK_EQ(wstl::IsFundamental<wchar_t>::Value, std::is_fundamental<wchar_t>::value);
        CHECK_EQ(wstl::IsFundamental<short>::Value, std::is_fundamental<short>::value);
        CHECK_EQ(wstl::IsFundamental<signed short>::Value, std::is_fundamental<signed short>::value);
        CHECK_EQ(wstl::IsFundamental<unsigned short>::Value, std::is_fundamental<unsigned short>::value);
        CHECK_EQ(wstl::IsFundamental<int>::Value, std::is_fundamental<int>::value);
        CHECK_EQ(wstl::IsFundamental<signed int>::Value, std::is_fundamental<signed int>::value);
        CHECK_EQ(wstl::IsFundamental<unsigned int>::Value, std::is_fundamental<unsigned int>::value);
        CHECK_EQ(wstl::IsFundamental<long>::Value, std::is_fundamental<long>::value);
        CHECK_EQ(wstl::IsFundamental<signed long>::Value, std::is_fundamental<signed long>::value);
        CHECK_EQ(wstl::IsFundamental<unsigned long>::Value, std::is_fundamental<unsigned long>::value);
        CHECK_EQ(wstl::IsFundamental<long long>::Value, std::is_fundamental<long long>::value);
        CHECK_EQ(wstl::IsFundamental<signed long long>::Value, std::is_fundamental<signed long long>::value);
        CHECK_EQ(wstl::IsFundamental<unsigned long long>::Value, std::is_fundamental<unsigned long long>::value);
        CHECK_EQ(wstl::IsFundamental<const int>::Value, std::is_fundamental<const int>::value);
        CHECK_EQ(wstl::IsFundamental<volatile int>::Value, std::is_fundamental<volatile int>::value);
        CHECK_EQ(wstl::IsFundamental<const int>::Value, std::is_fundamental<const int>::value);
        CHECK_EQ(wstl::IsFundamental<const volatile int>::Value, std::is_fundamental<const volatile int>::value);
        CHECK_EQ(wstl::IsFundamental<float>::Value, std::is_fundamental<float>::value);
        CHECK_EQ(wstl::IsFundamental<double>::Value, std::is_fundamental<double>::value);
        CHECK_EQ(wstl::IsFundamental<long double>::Value, std::is_fundamental<long double>::value);

        CHECK_EQ(wstl::IsFundamental<void>::Value, std::is_fundamental<void>::value);
        CHECK_EQ(wstl::IsFundamental<TestData>::Value, std::is_fundamental<TestData>::value);
        CHECK_EQ(wstl::IsFundamental<EnumData>::Value, std::is_fundamental<EnumData>::value);
        CHECK_EQ(wstl::IsFundamental<UnionData>::Value, std::is_fundamental<UnionData>::value);
        CHECK_EQ(wstl::IsFundamental<int[]>::Value, std::is_fundamental<int[]>::value);
        CHECK_EQ(wstl::IsFundamental<int&>::Value, std::is_fundamental<int&>::value);
        CHECK_EQ(wstl::IsFundamental<int TestData::*>::Value, std::is_fundamental<int TestData::*>::value);
        CHECK_EQ(wstl::IsFundamental<EnumData>::Value, std::is_fundamental<EnumData>::value);
        #else
        CHECK(wstl::IsFundamental<bool>::Value);
        CHECK(wstl::IsFundamental<char>::Value);
        CHECK(wstl::IsFundamental<signed char>::Value);
        CHECK(wstl::IsFundamental<unsigned char>::Value);
        CHECK(wstl::IsFundamental<wchar_t>::Value);
        CHECK(wstl::IsFundamental<short>::Value);
        CHECK(wstl::IsFundamental<signed short>::Value);
        CHECK(wstl::IsFundamental<unsigned short>::Value);
        CHECK(wstl::IsFundamental<int>::Value);
        CHECK(wstl::IsFundamental<signed int>::Value);
        CHECK(wstl::IsFundamental<unsigned int>::Value);
        CHECK(wstl::IsFundamental<long>::Value);
        CHECK(wstl::IsFundamental<signed long>::Value);
        CHECK(wstl::IsFundamental<unsigned long>::Value);
        CHECK(wstl::IsFundamental<long long>::Value);
        CHECK(wstl::IsFundamental<signed long long>::Value);
        CHECK(wstl::IsFundamental<unsigned long long>::Value);
        CHECK(wstl::IsFundamental<const int>::Value);
        CHECK(wstl::IsFundamental<volatile int>::Value);
        CHECK(wstl::IsFundamental<const int>::Value);
        CHECK(wstl::IsFundamental<const volatile int>::Value);
        CHECK(wstl::IsFundamental<float>::Value);
        CHECK(wstl::IsFundamental<double>::Value);
        CHECK(wstl::IsFundamental<long double>::Value);

        CHECK(wstl::IsFundamental<void>::Value);
        CHECK_FALSE(wstl::IsFundamental<TestData>::Value);
        CHECK_FALSE(wstl::IsFundamental<EnumData>::Value);
        CHECK_FALSE(wstl::IsFundamental<UnionData>::Value);
        CHECK_FALSE(wstl::IsFundamental<int[]>::Value);
        CHECK_FALSE(wstl::IsFundamental<int&>::Value);
        CHECK_FALSE(wstl::IsFundamental<int TestData::*>::Value);
        CHECK_FALSE(wstl::IsFundamental<EnumData>::Value);
        #endif

        CHECK(wstl::IsFundamental<wstl::NullPointerType>::Value);

        #ifdef __WSTL_CXX20__
        CHECK_EQ(wstl::IsFundamental<char8_t>::Value, std::is_fundamental<char8_t>::value);
        #endif
    }

    TEST_CASE("IsCompound") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsCompound<EnumClassData>::Value, std::is_compound<EnumClassData>::value);
        CHECK_EQ(wstl::IsCompound<char16_t>::Value, std::is_compound<char16_t>::value);
        CHECK_EQ(wstl::IsCompound<char32_t>::Value, std::is_compound<char32_t>::value);
        CHECK_EQ(wstl::IsCompound<std::nullptr_t>::Value, std::is_compound<std::nullptr_t>::value);

        CHECK_EQ(wstl::IsCompound<bool>::Value, std::is_compound<bool>::value);
        CHECK_EQ(wstl::IsCompound<char>::Value, std::is_compound<char>::value);
        CHECK_EQ(wstl::IsCompound<signed char>::Value, std::is_compound<signed char>::value);
        CHECK_EQ(wstl::IsCompound<unsigned char>::Value, std::is_compound<unsigned char>::value);
        CHECK_EQ(wstl::IsCompound<wchar_t>::Value, std::is_compound<wchar_t>::value);
        CHECK_EQ(wstl::IsCompound<short>::Value, std::is_compound<short>::value);
        CHECK_EQ(wstl::IsCompound<signed short>::Value, std::is_compound<signed short>::value);
        CHECK_EQ(wstl::IsCompound<unsigned short>::Value, std::is_compound<unsigned short>::value);
        CHECK_EQ(wstl::IsCompound<int>::Value, std::is_compound<int>::value);
        CHECK_EQ(wstl::IsCompound<signed int>::Value, std::is_compound<signed int>::value);
        CHECK_EQ(wstl::IsCompound<unsigned int>::Value, std::is_compound<unsigned int>::value);
        CHECK_EQ(wstl::IsCompound<long>::Value, std::is_compound<long>::value);
        CHECK_EQ(wstl::IsCompound<signed long>::Value, std::is_compound<signed long>::value);
        CHECK_EQ(wstl::IsCompound<unsigned long>::Value, std::is_compound<unsigned long>::value);
        CHECK_EQ(wstl::IsCompound<long long>::Value, std::is_compound<long long>::value);
        CHECK_EQ(wstl::IsCompound<signed long long>::Value, std::is_compound<signed long long>::value);
        CHECK_EQ(wstl::IsCompound<unsigned long long>::Value, std::is_compound<unsigned long long>::value);
        CHECK_EQ(wstl::IsCompound<const int>::Value, std::is_compound<const int>::value);
        CHECK_EQ(wstl::IsCompound<volatile int>::Value, std::is_compound<volatile int>::value);
        CHECK_EQ(wstl::IsCompound<const int>::Value, std::is_compound<const int>::value);
        CHECK_EQ(wstl::IsCompound<const volatile int>::Value, std::is_compound<const volatile int>::value);
        CHECK_EQ(wstl::IsCompound<float>::Value, std::is_compound<float>::value);
        CHECK_EQ(wstl::IsCompound<double>::Value, std::is_compound<double>::value);
        CHECK_EQ(wstl::IsCompound<long double>::Value, std::is_compound<long double>::value);

        CHECK_EQ(wstl::IsCompound<void>::Value, std::is_compound<void>::value);
        CHECK_EQ(wstl::IsCompound<TestData>::Value, std::is_compound<TestData>::value);
        CHECK_EQ(wstl::IsCompound<EnumData>::Value, std::is_compound<EnumData>::value);
        CHECK_EQ(wstl::IsCompound<UnionData>::Value, std::is_compound<UnionData>::value);
        CHECK_EQ(wstl::IsCompound<int[]>::Value, std::is_compound<int[]>::value);
        CHECK_EQ(wstl::IsCompound<int&>::Value, std::is_compound<int&>::value);
        CHECK_EQ(wstl::IsCompound<int TestData::*>::Value, std::is_compound<int TestData::*>::value);
        CHECK_EQ(wstl::IsCompound<EnumData>::Value, std::is_compound<EnumData>::value);
        #else
        CHECK_FALSE(wstl::IsCompound<bool>::Value);
        CHECK_FALSE(wstl::IsCompound<char>::Value);
        CHECK_FALSE(wstl::IsCompound<signed char>::Value);
        CHECK_FALSE(wstl::IsCompound<unsigned char>::Value);
        CHECK_FALSE(wstl::IsCompound<wchar_t>::Value);
        CHECK_FALSE(wstl::IsCompound<short>::Value);
        CHECK_FALSE(wstl::IsCompound<signed short>::Value);
        CHECK_FALSE(wstl::IsCompound<unsigned short>::Value);
        CHECK_FALSE(wstl::IsCompound<int>::Value);
        CHECK_FALSE(wstl::IsCompound<signed int>::Value);
        CHECK_FALSE(wstl::IsCompound<unsigned int>::Value);
        CHECK_FALSE(wstl::IsCompound<long>::Value);
        CHECK_FALSE(wstl::IsCompound<signed long>::Value);
        CHECK_FALSE(wstl::IsCompound<unsigned long>::Value);
        CHECK_FALSE(wstl::IsCompound<long long>::Value);
        CHECK_FALSE(wstl::IsCompound<signed long long>::Value);
        CHECK_FALSE(wstl::IsCompound<unsigned long long>::Value);
        CHECK_FALSE(wstl::IsCompound<const int>::Value);
        CHECK_FALSE(wstl::IsCompound<volatile int>::Value);
        CHECK_FALSE(wstl::IsCompound<const int>::Value);
        CHECK_FALSE(wstl::IsCompound<const volatile int>::Value);
        CHECK_FALSE(wstl::IsCompound<float>::Value);
        CHECK_FALSE(wstl::IsCompound<double>::Value);
        CHECK_FALSE(wstl::IsCompound<long double>::Value);

        CHECK_FALSE(wstl::IsCompound<void>::Value);
        CHECK(wstl::IsCompound<TestData>::Value);
        CHECK(wstl::IsCompound<EnumData>::Value);
        CHECK(wstl::IsCompound<UnionData>::Value);
        CHECK(wstl::IsCompound<int[]>::Value);
        CHECK(wstl::IsCompound<int&>::Value);
        CHECK(wstl::IsCompound<int TestData::*>::Value);
        CHECK(wstl::IsCompound<EnumData>::Value);
        #endif

        CHECK_FALSE(wstl::IsCompound<wstl::NullPointerType>::Value);

        #ifdef __WSTL_CXX20__
        CHECK_EQ(wstl::IsCompound<char8_t>::Value, std::is_compound<char8_t>::value);
        #endif
    }

    TEST_CASE("IsLValueReference") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsLValueReference<int&&>::Value, std::is_lvalue_reference<int&&>::value);
        CHECK_EQ(wstl::IsLValueReference<int&>::Value, std::is_lvalue_reference<int&>::value);
        CHECK_EQ(wstl::IsLValueReference<const int&>::Value, std::is_lvalue_reference<const int&>::value);
        CHECK_EQ(wstl::IsLValueReference<volatile int&>::Value, std::is_lvalue_reference<volatile int&>::value);
        CHECK_EQ(wstl::IsLValueReference<const volatile int&>::Value, std::is_lvalue_reference<const volatile int&>::value);
        CHECK_EQ(wstl::IsLValueReference<int*>::Value, std::is_lvalue_reference<int*>::value);
        CHECK_EQ(wstl::IsLValueReference<int>::Value, std::is_lvalue_reference<int>::value);
        CHECK_EQ(wstl::IsLValueReference<void>::Value, std::is_lvalue_reference<void>::value);
        #else
        CHECK(wstl::IsLValueReference<int&>::Value);
        CHECK(wstl::IsLValueReference<const int&>::Value);
        CHECK(wstl::IsLValueReference<volatile int&>::Value);
        CHECK(wstl::IsLValueReference<const volatile int&>::Value);
        CHECK_FALSE(wstl::IsLValueReference<int*>::Value);
        CHECK_FALSE(wstl::IsLValueReference<int>::Value);
        CHECK_FALSE(wstl::IsLValueReference<void>::Value);
        #endif
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("IsRValueReference") {
        CHECK_EQ(wstl::IsRValueReference<int&&>::Value, std::is_rvalue_reference<int&&>::value);
        CHECK_EQ(wstl::IsRValueReference<const int&&>::Value, std::is_rvalue_reference<const int&&>::value);
        CHECK_EQ(wstl::IsRValueReference<volatile int&&>::Value, std::is_rvalue_reference<volatile int&&>::value);
        CHECK_EQ(wstl::IsRValueReference<const volatile int&&>::Value, std::is_rvalue_reference<const volatile int&&>::value);
        CHECK_EQ(wstl::IsRValueReference<int&>::Value, std::is_rvalue_reference<int&>::value);
        CHECK_EQ(wstl::IsRValueReference<int*>::Value, std::is_rvalue_reference<int*>::value);
        CHECK_EQ(wstl::IsRValueReference<int>::Value, std::is_rvalue_reference<int>::value);
        CHECK_EQ(wstl::IsRValueReference<void>::Value, std::is_rvalue_reference<void>::value);
    }
    #endif

    TEST_CASE("IsReference") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsReference<int&&>::Value, std::is_reference<int&&>::value);
        CHECK_EQ(wstl::IsReference<int&>::Value, std::is_reference<int&>::value);
        CHECK_EQ(wstl::IsReference<const int&>::Value, std::is_reference<const int&>::value);
        CHECK_EQ(wstl::IsReference<volatile int&>::Value, std::is_reference<volatile int&>::value);
        CHECK_EQ(wstl::IsReference<const volatile int&>::Value, std::is_reference<const volatile int&>::value);
        CHECK_EQ(wstl::IsReference<int*>::Value, std::is_reference<int*>::value);
        CHECK_EQ(wstl::IsReference<int>::Value, std::is_reference<int>::value);
        CHECK_EQ(wstl::IsReference<void>::Value, std::is_reference<void>::value);
        #else
        CHECK(wstl::IsReference<int&>::Value);
        CHECK(wstl::IsReference<const int&>::Value);
        CHECK(wstl::IsReference<volatile int&>::Value);
        CHECK(wstl::IsReference<const volatile int&>::Value);
        CHECK_FALSE(wstl::IsReference<int*>::Value);
        CHECK_FALSE(wstl::IsReference<int>::Value);
        CHECK_FALSE(wstl::IsReference<void>::Value);
        #endif
    }

    TEST_CASE("IsFunction") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsFunction<__TESTING_DECLTYPE__(Free0)>::Value, std::is_function<__TESTING_DECLTYPE__(Free0)>::value);
        CHECK_EQ(wstl::IsFunction<__TESTING_DECLTYPE__(Free1)>::Value, std::is_function<__TESTING_DECLTYPE__(Free1)>::value);
        CHECK_EQ(wstl::IsFunction<__TESTING_DECLTYPE__(Free2)>::Value, std::is_function<__TESTING_DECLTYPE__(Free2)>::value);
        CHECK_EQ(wstl::IsFunction<__TESTING_DECLTYPE__(Free3)>::Value, std::is_function<__TESTING_DECLTYPE__(Free3)>::value);
        CHECK_EQ(wstl::IsFunction<__TESTING_DECLTYPE__(Free0t<char>)>::Value, std::is_function<__TESTING_DECLTYPE__(Free0t<char>)>::value);
        CHECK_EQ(wstl::IsFunction<int MemberFunction::*>::Value, std::is_function<int MemberFunction::*>::value);
        CHECK_EQ(wstl::IsFunction<int*>::Value, std::is_function<int*>::value);
        CHECK_EQ(wstl::IsFunction<int (MemberFunction::*)(int)>::Value, std::is_function<int (MemberFunction::*)(int)>::value);
        CHECK_EQ(wstl::IsFunction<int>::Value, std::is_function<int>::value);
        #else
        CHECK(wstl::IsFunction<__TESTING_DECLTYPE__(Free0)>::Value);
        CHECK(wstl::IsFunction<__TESTING_DECLTYPE__(Free1)>::Value);
        CHECK(wstl::IsFunction<__TESTING_DECLTYPE__(Free2)>::Value);
        CHECK(wstl::IsFunction<__TESTING_DECLTYPE__(Free3)>::Value);
        CHECK(wstl::IsFunction<char()>::Value);
        CHECK_FALSE(wstl::IsFunction<int MemberFunction::*>::Value);
        CHECK_FALSE(wstl::IsFunction<int*>::Value);
        CHECK_FALSE(wstl::IsFunction<int (MemberFunction::*)(int)>::Value);
        CHECK_FALSE(wstl::IsFunction<int>::Value);
        #endif
    }
    TEST_CASE("IsArray") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsArray<int[10]>::Value, std::is_array<int[10]>::value);
        CHECK_EQ(wstl::IsArray<int[]>::Value, std::is_array<int[]>::value);
        CHECK_EQ(wstl::IsArray<int[10][10]>::Value, std::is_array<int[10][10]>::value);
        CHECK_EQ(wstl::IsArray<int>::Value, std::is_array<int>::value);
        #else
        CHECK(wstl::IsArray<int[10]>::Value);
        CHECK(wstl::IsArray<int[]>::Value);
        CHECK(wstl::IsArray<int[10][10]>::Value);
        CHECK_FALSE(wstl::IsArray<int>::Value);
        #endif
    }
    TEST_CASE("IsUnion") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsUnion<UnionData>::Value, std::is_union<UnionData>::value);
        CHECK_EQ(wstl::IsUnion<TestData>::Value, std::is_union<TestData>::value);
        #else
        CHECK(wstl::IsUnion<UnionData>::Value);
        CHECK_FALSE(wstl::IsUnion<TestData>::Value);
        #endif
    }
    TEST_CASE("IsMemberPointer") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsMemberPointer<int MemberFunction::*>::Value, std::is_member_pointer<int MemberFunction::*>::value);
        CHECK_EQ(wstl::IsMemberPointer<int (MemberFunction::*)(int)>::Value, std::is_member_pointer<int (MemberFunction::*)(int)>::value);
        CHECK_EQ(wstl::IsMemberPointer<int*>::Value, std::is_member_pointer<int*>::value);
        CHECK_EQ(wstl::IsMemberPointer<int>::Value, std::is_member_pointer<int>::value);
        CHECK_EQ(wstl::IsMemberPointer<__TESTING_DECLTYPE__(&Free0)>::Value, std::is_member_pointer<__TESTING_DECLTYPE__(&Free0)>::value);
        #else
        CHECK(wstl::IsMemberPointer<int MemberFunction::*>::Value);
        CHECK(wstl::IsMemberPointer<int (MemberFunction::*)(int)>::Value);
        CHECK_FALSE(wstl::IsMemberPointer<int*>::Value);
        CHECK_FALSE(wstl::IsMemberPointer<int>::Value);
        CHECK_FALSE(wstl::IsMemberPointer<__TESTING_DECLTYPE__(&Free0)>::Value);
        #endif
    }
    TEST_CASE("IsMemberFunctionPointer") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsMemberFunctionPointer<int MemberFunction::*>::Value, std::is_member_function_pointer<int MemberFunction::*>::value);
        CHECK_EQ(wstl::IsMemberFunctionPointer<int (MemberFunction::*)(int)>::Value, std::is_member_function_pointer<int (MemberFunction::*)(int)>::value);
        CHECK_EQ(wstl::IsMemberFunctionPointer<int*>::Value, std::is_member_function_pointer<int*>::value);
        CHECK_EQ(wstl::IsMemberFunctionPointer<int>::Value, std::is_member_function_pointer<int>::value);
        CHECK_EQ(wstl::IsMemberFunctionPointer<__TESTING_DECLTYPE__(&Free0)>::Value, std::is_member_function_pointer<__TESTING_DECLTYPE__(&Free0)>::value);
        #else
        CHECK_FALSE(wstl::IsMemberFunctionPointer<int MemberFunction::*>::Value);
        CHECK(wstl::IsMemberFunctionPointer<int (MemberFunction::*)(int)>::Value);
        CHECK_FALSE(wstl::IsMemberFunctionPointer<int*>::Value);
        CHECK_FALSE(wstl::IsMemberFunctionPointer<int>::Value);
        CHECK_FALSE(wstl::IsMemberFunctionPointer<__TESTING_DECLTYPE__(&Free0)>::Value);
        #endif
    }

    TEST_CASE("IsMemberObjectPointer") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsMemberObjectPointer<int MemberFunction::*>::Value, std::is_member_object_pointer<int MemberFunction::*>::value);
        CHECK_EQ(wstl::IsMemberObjectPointer<int (MemberFunction::*)(int)>::Value, std::is_member_object_pointer<int (MemberFunction::*)(int)>::value);
        CHECK_EQ(wstl::IsMemberObjectPointer<int*>::Value, std::is_member_object_pointer<int*>::value);
        CHECK_EQ(wstl::IsMemberObjectPointer<int>::Value, std::is_member_object_pointer<int>::value);
        CHECK_EQ(wstl::IsMemberObjectPointer<__TESTING_DECLTYPE__(&Free0)>::Value, std::is_member_object_pointer<__TESTING_DECLTYPE__(&Free0)>::value);
        #else
        CHECK(wstl::IsMemberObjectPointer<int MemberFunction::*>::Value);
        CHECK_FALSE(wstl::IsMemberObjectPointer<int (MemberFunction::*)(int)>::Value);
        CHECK_FALSE(wstl::IsMemberObjectPointer<int*>::Value);
        CHECK_FALSE(wstl::IsMemberObjectPointer<int>::Value);
        CHECK_FALSE(wstl::IsMemberObjectPointer<__TESTING_DECLTYPE__(&Free0)>::Value);
        #endif
    }

    TEST_CASE("IsClass") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsClass<EnumClassData>::Value, std::is_class<EnumClassData>::value);
        CHECK_EQ(wstl::IsClass<TestData>::Value, std::is_class<TestData>::value);
        CHECK_EQ(wstl::IsClass<ClassData>::Value, std::is_class<ClassData>::value);
        CHECK_EQ(wstl::IsClass<const ClassData>::Value, std::is_class<const ClassData>::value);
        CHECK_EQ(wstl::IsClass<UnionData::ClassData>::Value, std::is_class<UnionData::ClassData>::value);
        CHECK_EQ(wstl::IsClass<UnionData>::Value, std::is_class<UnionData>::value);
        CHECK_EQ(wstl::IsClass<EnumData>::Value, std::is_class<EnumData>::value);
        CHECK_EQ(wstl::IsClass<int>::Value, std::is_class<int>::value);
        #else
        CHECK(wstl::IsClass<TestData>::Value);
        CHECK(wstl::IsClass<ClassData>::Value);
        CHECK(wstl::IsClass<const ClassData>::Value);
        CHECK(wstl::IsClass<UnionData::ClassData>::Value);
        CHECK_FALSE(wstl::IsClass<UnionData>::Value);
        CHECK_FALSE(wstl::IsClass<EnumData>::Value);
        CHECK_FALSE(wstl::IsClass<int>::Value);
        #endif
    }

    TEST_CASE("IsBaseOf") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsBaseOf<A, A>::Value), (std::is_base_of<A, A>::value));
        CHECK_EQ((wstl::IsBaseOf<A, BBaseA>::Value), (std::is_base_of<A, BBaseA>::value));
        CHECK_EQ((wstl::IsBaseOf<A, CBaseB>::Value), (std::is_base_of<A, CBaseB>::value));
        CHECK_EQ((wstl::IsBaseOf<A, D>::Value), (std::is_base_of<A, D>::value));
        CHECK_EQ((wstl::IsBaseOf<BBaseA, A>::Value), (std::is_base_of<BBaseA, A>::value));
        CHECK_EQ((wstl::IsBaseOf<UnionData, UnionData>::Value), (std::is_base_of<UnionData, UnionData>::value));
        CHECK_EQ((wstl::IsBaseOf<int, int>::Value), (std::is_base_of<int, int>::value));
        #else
        CHECK((wstl::IsBaseOf<A, A>::Value));
        CHECK((wstl::IsBaseOf<A, BBaseA>::Value));
        CHECK((wstl::IsBaseOf<A, CBaseB>::Value));
        CHECK_FALSE((wstl::IsBaseOf<A, D>::Value));
        CHECK_FALSE((wstl::IsBaseOf<BBaseA, A>::Value));
        CHECK_FALSE((wstl::IsBaseOf<UnionData, UnionData>::Value));
        CHECK_FALSE((wstl::IsBaseOf<int, int>::Value));
        #endif
    }

    TEST_CASE("IsConvertible") {
        // Fundamental types
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsConvertible<int, double>::Value), (std::is_convertible<int, double>::value));
        CHECK_EQ((wstl::IsConvertible<double, int>::Value), (std::is_convertible<double, int>::value));
        CHECK_EQ((wstl::IsConvertible<int, int>::Value), (std::is_convertible<int, int>::value));
        #else
        CHECK((wstl::IsConvertible<int, double>::Value));
        CHECK((wstl::IsConvertible<double, int>::Value));
        CHECK((wstl::IsConvertible<int, int>::Value));
        #endif

        // Pointers
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsConvertible<int*, const int*>::Value), (std::is_convertible<int*, const int*>::value));
        CHECK_EQ((wstl::IsConvertible<const int*, int*>::Value), (std::is_convertible<const int*, int*>::value));
        #else
        CHECK((wstl::IsConvertible<int*, const int*>::Value));
        CHECK_FALSE((wstl::IsConvertible<const int*, int*>::Value));
        #endif

        // Inheritance
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsConvertible<BBaseA*, A*>::Value), (std::is_convertible<BBaseA*, A*>::value));
        CHECK_EQ((wstl::IsConvertible<A*, BBaseA*>::Value), (std::is_convertible<A*, BBaseA*>::value));
        #else
        CHECK((wstl::IsConvertible<BBaseA*, A*>::Value));
        CHECK_FALSE((wstl::IsConvertible<A*, BBaseA*>::Value));
        #endif

        // References
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsConvertible<int&, int>::Value), (std::is_convertible<int&, int>::value));
        CHECK_EQ((wstl::IsConvertible<int&, const int&>::Value), (std::is_convertible<int&, const int&>::value));
        CHECK_EQ((wstl::IsConvertible<const int&, int&>::Value), (std::is_convertible<const int&, int&>::value));
        #else
        CHECK((wstl::IsConvertible<int&, int>::Value));
        CHECK((wstl::IsConvertible<int&, const int&>::Value));
        CHECK_FALSE((wstl::IsConvertible<const int&, int&>::Value));
        #endif

        // Void
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsConvertible<void, void>::Value), (std::is_convertible<void, void>::value));
        CHECK_EQ((wstl::IsConvertible<int, void>::Value), (std::is_convertible<int, void>::value));
        CHECK_EQ((wstl::IsConvertible<void, int>::Value), (std::is_convertible<void, int>::value));
        #else
        CHECK((wstl::IsConvertible<void, void>::Value));
        CHECK_FALSE((wstl::IsConvertible<int, void>::Value));
        CHECK_FALSE((wstl::IsConvertible<void, int>::Value));
        #endif

        // User-defined implicit
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsConvertible<int, Implicit>::Value), (std::is_convertible<int, Implicit>::value));
        CHECK_EQ((wstl::IsConvertible<From, To>::Value), (std::is_convertible<From, To>::value));
        #else
        CHECK((wstl::IsConvertible<int, Implicit>::Value));
        CHECK((wstl::IsConvertible<From, To>::Value));
        #endif

        // User-defined explicit
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsConvertible<int, Explicit>::Value), (std::is_convertible<int, Explicit>::value));
        #else
        CHECK_FALSE((wstl::IsConvertible<int, Explicit>::Value));
        #endif

        // Conversion operator
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsConvertible<ToBool, bool>::Value), (std::is_convertible<ToBool, bool>::value));
        #else
        CHECK((wstl::IsConvertible<ToBool, bool>::Value));
        #endif

        // Array
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsConvertible<int[3], const int*>::Value), (std::is_convertible<int[3], const int*>::value));
        #else
        CHECK((wstl::IsConvertible<int[3], const int*>::Value));
        #endif

        // Function pointers
        typedef int Fn(int);

        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsConvertible<Fn, Fn*>::Value), (std::is_convertible<Fn, Fn*>::value));
        #else
        CHECK((wstl::IsConvertible<Fn, Fn*>::Value));
        #endif

        // CV qualifiers
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsConvertible<int, const int>::Value), (std::is_convertible<int, const int>::value));
        CHECK_EQ((wstl::IsConvertible<const int, int>::Value), (std::is_convertible<const int, int>::value));
        #else
        CHECK((wstl::IsConvertible<int, const int>::Value));
        CHECK((wstl::IsConvertible<const int, int>::Value));
        #endif

        // Enum
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsConvertible<EnumClassData, int>::Value, std::is_convertible<EnumClassData, int>::value);
        CHECK_EQ((wstl::IsConvertible<EnumData, int>::Value), (std::is_convertible<EnumData, int>::value));
        #else
        CHECK((wstl::IsConvertible<EnumData, int>::Value));
        #endif
    }

    TEST_CASE("IsEnum") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsEnum<EnumClassData>::Value, std::is_enum<EnumClassData>::value);
        CHECK_EQ(wstl::IsEnum<EnumData>::Value, std::is_enum<EnumData>::value);
        CHECK_EQ(wstl::IsEnum<TestData>::Value, std::is_enum<TestData>::value);
        CHECK_EQ(wstl::IsEnum<FakeEnum>::Value, std::is_enum<FakeEnum>::value);
        CHECK_EQ(wstl::IsEnum<int>::Value, std::is_enum<int>::value);
        #else
        CHECK(wstl::IsEnum<EnumData>::Value);
        CHECK_FALSE(wstl::IsEnum<TestData>::Value);
        CHECK_FALSE(wstl::IsEnum<FakeEnum>::Value);
        CHECK_FALSE(wstl::IsEnum<int>::Value);
        #endif
    }

    TEST_CASE("IsPointer") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsPointer<int*>::Value, std::is_pointer<int*>::value);
        CHECK_EQ(wstl::IsPointer<const int*>::Value, std::is_pointer<const int*>::value);
        CHECK_EQ(wstl::IsPointer<volatile int*>::Value, std::is_pointer<volatile int*>::value);
        CHECK_EQ(wstl::IsPointer<const volatile int*>::Value, std::is_pointer<const volatile int*>::value);
        CHECK_EQ(wstl::IsPointer<int>::Value, std::is_pointer<int>::value);
        #else
        CHECK(wstl::IsPointer<int*>::Value);
        CHECK(wstl::IsPointer<const int*>::Value);
        CHECK(wstl::IsPointer<volatile int*>::Value);
        CHECK(wstl::IsPointer<const volatile int*>::Value);
        CHECK_FALSE(wstl::IsPointer<int>::Value);
        #endif
    }

    TEST_CASE("IsScalar") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsScalar<EnumClassData>::Value, std::is_scalar<EnumClassData>::value);
        CHECK_EQ(wstl::IsScalar<int>::Value, std::is_scalar<int>::value);
        CHECK_EQ(wstl::IsScalar<float>::Value, std::is_scalar<float>::value);
        CHECK_EQ(wstl::IsScalar<EnumData>::Value, std::is_scalar<EnumData>::value);
        CHECK_EQ(wstl::IsScalar<int*>::Value, std::is_scalar<int*>::value);
        CHECK_EQ(wstl::IsScalar<int MemberFunction::*>::Value, std::is_scalar<int MemberFunction::*>::value);
        CHECK_EQ(wstl::IsScalar<int (MemberFunction::*)(int)>::Value, std::is_scalar<int (MemberFunction::*)(int)>::value);
        CHECK_EQ(wstl::IsScalar<__TESTING_DECLTYPE__(&Free0)>::Value, std::is_scalar<__TESTING_DECLTYPE__(&Free0)>::value);
        CHECK_EQ(wstl::IsScalar<int()>::Value, std::is_scalar<int()>::value);
        CHECK_EQ(wstl::IsScalar<ClassData>::Value, std::is_scalar<ClassData>::value);
        CHECK_EQ(wstl::IsScalar<TestData>::Value, std::is_scalar<TestData>::value);
        #else
        CHECK(wstl::IsScalar<int>::Value);
        CHECK(wstl::IsScalar<float>::Value);
        CHECK(wstl::IsScalar<EnumData>::Value);
        CHECK(wstl::IsScalar<int*>::Value);
        CHECK(wstl::IsScalar<int MemberFunction::*>::Value);
        CHECK(wstl::IsScalar<int (MemberFunction::*)(int)>::Value);
        CHECK(wstl::IsScalar<__TESTING_DECLTYPE__(&Free0)>::Value);
        CHECK_FALSE(wstl::IsScalar<int()>::Value);
        CHECK_FALSE(wstl::IsScalar<ClassData>::Value);
        CHECK_FALSE(wstl::IsScalar<TestData>::Value);
        #endif
    }

    TEST_CASE("IsTrivial") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsTrivial<int>::Value, std::is_trivial<int>::value);
        CHECK_EQ(wstl::IsTrivial<TrivialConstructor>::Value, std::is_trivial<TrivialConstructor>::value);
        CHECK_EQ(wstl::IsTrivial<PrivateDefaultConstructor>::Value, std::is_trivial<PrivateDefaultConstructor>::value);
        CHECK_EQ(wstl::IsTrivial<NonTrivialData>::Value, std::is_trivial<NonTrivialData>::value);
        CHECK_EQ(wstl::IsTrivial<NonTrivialConstructor>::Value, std::is_trivial<NonTrivialConstructor>::value);
        #else
        CHECK(wstl::IsTrivial<int>::Value);
        CHECK(wstl::IsTrivial<TrivialConstructor>::Value);
        CHECK_FALSE(wstl::IsTrivial<PrivateDefaultConstructor>::Value);
        CHECK_FALSE(wstl::IsTrivial<NonTrivialData>::Value);
        CHECK_FALSE(wstl::IsTrivial<NonTrivialConstructor>::Value);
        #endif
    }

    TEST_CASE("IsPOD") {
        CHECK(wstl::IsPOD<int>::Value);
        CHECK(wstl::IsPOD<A>::Value);
        CHECK_FALSE(wstl::IsPOD<BBaseA>::Value);
        CHECK_FALSE(wstl::IsPOD<VirtualFunction>::Value);
    }

    TEST_CASE("IsStandardLayout") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsStandardLayout<int>::Value, std::is_standard_layout<int>::value);
        CHECK_EQ(wstl::IsStandardLayout<A>::Value, std::is_standard_layout<A>::value);
        CHECK_EQ(wstl::IsStandardLayout<BBaseA>::Value, std::is_standard_layout<BBaseA>::value);
        CHECK_EQ(wstl::IsStandardLayout<VirtualFunction>::Value, std::is_standard_layout<VirtualFunction>::value);
        #else
        CHECK(wstl::IsStandardLayout<int>::Value);
        CHECK(wstl::IsStandardLayout<A>::Value);
        CHECK_FALSE(wstl::IsStandardLayout<BBaseA>::Value);
        CHECK_FALSE(wstl::IsStandardLayout<VirtualFunction>::Value);
        #endif
    }

    TEST_CASE("IsObject") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsObject<int>::Value, std::is_object<int>::value);
        CHECK_EQ(wstl::IsObject<int*>::Value, std::is_object<int*>::value);
        CHECK_EQ(wstl::IsObject<TestData>::Value, std::is_object<TestData>::value);
        CHECK_EQ(wstl::IsObject<int(*)()>::Value, std::is_object<int(*)()>::value);
        CHECK_EQ(wstl::IsObject<void>::Value, std::is_object<void>::value);
        CHECK_EQ(wstl::IsObject<int&>::Value, std::is_object<int&>::value);
        CHECK_EQ(wstl::IsObject<int*&>::Value, std::is_object<int*&>::value);
        CHECK_EQ(wstl::IsObject<TestData&>::Value, std::is_object<TestData&>::value);
        CHECK_EQ(wstl::IsObject<int()>::Value, std::is_object<int()>::value);
        CHECK_EQ(wstl::IsObject<int(&)()>::Value, std::is_object<int(&)()>::value);
        #else
        CHECK(wstl::IsObject<int>::Value);
        CHECK(wstl::IsObject<int*>::Value);
        CHECK(wstl::IsObject<TestData>::Value);
        CHECK(wstl::IsObject<int(*)()>::Value);
        CHECK_FALSE(wstl::IsObject<void>::Value);
        CHECK_FALSE(wstl::IsObject<int&>::Value);
        CHECK_FALSE(wstl::IsObject<int*&>::Value);
        CHECK_FALSE(wstl::IsObject<TestData&>::Value);
        CHECK_FALSE(wstl::IsObject<int()>::Value);
        CHECK_FALSE(wstl::IsObject<int(&)()>::Value);
        #endif
    }

    TEST_CASE("IsConstructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsConstructible<NonTrivialData, int, int>::Value, std::is_constructible<NonTrivialData, int, int>::value);

        CHECK_EQ(wstl::IsConstructible<TestData>::Value, std::is_constructible<TestData>::value);
        CHECK_EQ(wstl::IsConstructible<NonTrivialData>::Value, std::is_constructible<NonTrivialData>::value);
        CHECK_EQ((wstl::IsConstructible<Implicit, int>::Value), (std::is_constructible<Implicit, int>::value));
        CHECK_EQ((wstl::IsConstructible<const int&, int>::Value), (std::is_constructible<const int&, int>::value));
        CHECK_EQ(wstl::IsConstructible<Explicit>::Value, std::is_constructible<Explicit>::value);
        CHECK_EQ(wstl::IsConstructible<Implicit>::Value, std::is_constructible<Implicit>::value);
        CHECK_EQ((wstl::IsConstructible<NonTrivialData, int>::Value), (std::is_constructible<NonTrivialData, int>::value));
        CHECK_EQ(wstl::IsConstructible<PrivateDefaultConstructor>::Value, std::is_constructible<PrivateDefaultConstructor>::value);
        CHECK_EQ((wstl::IsConstructible<int&, int>::Value), (std::is_constructible<int&, int>::value));
        CHECK_EQ(wstl::IsConstructible<void>::Value, std::is_constructible<void>::value);
        #else
        CHECK(wstl::IsConstructible<TestData>::Value);
        CHECK(wstl::IsConstructible<NonTrivialData>::Value);
        CHECK((wstl::IsConstructible<Implicit, int>::Value));
        CHECK((wstl::IsConstructible<const int&, int>::Value));
        CHECK_FALSE(wstl::IsConstructible<Explicit>::Value);
        CHECK_FALSE(wstl::IsConstructible<Implicit>::Value);
        CHECK_FALSE((wstl::IsConstructible<NonTrivialData, int>::Value));
        CHECK_FALSE(wstl::IsConstructible<PrivateDefaultConstructor>::Value);
        CHECK_FALSE((wstl::IsConstructible<int&, int>::Value));
        CHECK_FALSE(wstl::IsConstructible<void>::Value);
        #endif
    }

    TEST_CASE("IsTriviallyConstructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsTriviallyConstructible<int>::Value, std::is_trivially_constructible<int>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<A>::Value, std::is_trivially_constructible<A>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<TestData>::Value, std::is_trivially_constructible<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<void>::Value, std::is_trivially_constructible<void>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<NonTrivialData>::Value, std::is_trivially_constructible<NonTrivialData>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<Implicit>::Value, std::is_trivially_constructible<Implicit>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<Explicit>::Value, std::is_trivially_constructible<Explicit>::value);
        #else
        CHECK(wstl::IsTriviallyConstructible<int>::Value);
        CHECK(wstl::IsTriviallyConstructible<A>::Value);
        CHECK(wstl::IsTriviallyConstructible<TestData>::Value);
        CHECK_FALSE(wstl::IsTriviallyConstructible<void>::Value);
        CHECK_FALSE(wstl::IsTriviallyConstructible<NonTrivialData>::Value);
        CHECK_FALSE(wstl::IsTriviallyConstructible<Implicit>::Value);
        CHECK_FALSE(wstl::IsTriviallyConstructible<Explicit>::Value);
        #endif
    }

    TEST_CASE("IsNothrowConstructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsNothrowConstructible<TestData>::Value, std::is_nothrow_constructible<TestData>::value);
        CHECK_EQ(wstl::IsNothrowConstructible<int>::Value, std::is_nothrow_constructible<int>::value);
        CHECK_EQ(wstl::IsNothrowConstructible<int*>::Value, std::is_nothrow_constructible<int*>::value);
        CHECK_EQ(wstl::IsNothrowConstructible<A>::Value, std::is_nothrow_constructible<A>::value);
        CHECK_EQ(wstl::IsNothrowConstructible<NothrowData>::Value, std::is_nothrow_constructible<NothrowData>::value);
        CHECK_EQ(wstl::IsNothrowConstructible<void>::Value, std::is_nothrow_constructible<void>::value);
        CHECK_EQ(wstl::IsNothrowConstructible<int&>::Value, std::is_nothrow_constructible<int&>::value);
        CHECK_EQ(wstl::IsNothrowConstructible<int[]>::Value, std::is_nothrow_constructible<int[]>::value);
        CHECK_EQ(wstl::IsNothrowConstructible<NonTrivialData>::Value, std::is_nothrow_constructible<NonTrivialData>::value);
        CHECK_EQ(wstl::IsNothrowConstructible<Explicit>::Value, std::is_nothrow_constructible<Explicit>::value);
        CHECK_EQ(wstl::IsNothrowConstructible<Implicit>::Value, std::is_nothrow_constructible<Implicit>::value);
        #else
        CHECK(wstl::IsNothrowConstructible<TestData>::Value);
        CHECK(wstl::IsNothrowConstructible<int>::Value);
        CHECK(wstl::IsNothrowConstructible<int*>::Value);
        CHECK(wstl::IsNothrowConstructible<A>::Value);
        CHECK(wstl::IsNothrowConstructible<NothrowData>::Value);
        CHECK_FALSE(wstl::IsNothrowConstructible<void>::Value);
        CHECK_FALSE(wstl::IsNothrowConstructible<int&>::Value);
        CHECK_FALSE(wstl::IsNothrowConstructible<int[]>::Value);
        CHECK_FALSE(wstl::IsNothrowConstructible<NonTrivialData>::Value);
        CHECK_FALSE(wstl::IsNothrowConstructible<Explicit>::Value);
        CHECK_FALSE(wstl::IsNothrowConstructible<Implicit>::Value);
        #endif
    }

    TEST_CASE("IsDefaultConstructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsDefaultConstructible<TestData>::Value, std::is_default_constructible<TestData>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<int>::Value, std::is_default_constructible<int>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<NonTrivialConstructor>::Value, std::is_default_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<PrivateDefaultConstructor>::Value, std::is_default_constructible<PrivateDefaultConstructor>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<void>::Value, std::is_default_constructible<void>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<Implicit>::Value, std::is_default_constructible<Implicit>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<Explicit>::Value, std::is_default_constructible<Explicit>::value);
        #else
        CHECK(wstl::IsDefaultConstructible<TestData>::Value);
        CHECK(wstl::IsDefaultConstructible<int>::Value);
        CHECK(wstl::IsDefaultConstructible<NonTrivialConstructor>::Value);
        CHECK_FALSE(wstl::IsDefaultConstructible<PrivateDefaultConstructor>::Value);
        CHECK_FALSE(wstl::IsDefaultConstructible<void>::Value);
        CHECK_FALSE(wstl::IsDefaultConstructible<Implicit>::Value);
        CHECK_FALSE(wstl::IsDefaultConstructible<Explicit>::Value);
        #endif
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("IsImplicitlyDefaultConstructible") {
        CHECK(wstl::IsImplicitlyDefaultConstructible<int>::Value);
        CHECK(wstl::IsImplicitlyDefaultConstructible<TestData>::Value);
        CHECK(wstl::IsImplicitlyDefaultConstructible<NonTrivialConstructor>::Value);
        CHECK_FALSE(wstl::IsImplicitlyDefaultConstructible<ExplicitDefault>::Value);
        CHECK_FALSE(wstl::IsImplicitlyDefaultConstructible<Implicit>::Value);
        CHECK_FALSE(wstl::IsImplicitlyDefaultConstructible<Explicit>::Value);
        CHECK_FALSE(wstl::IsImplicitlyDefaultConstructible<PrivateDefaultConstructor>::Value);
    }
    #endif

    TEST_CASE("IsTriviallyDefaultConstructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<int>::Value, std::is_trivially_default_constructible<int>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<TestData>::Value, std::is_trivially_default_constructible<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<NonTrivialConstructor>::Value, std::is_trivially_default_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<ExplicitDefault>::Value, std::is_trivially_default_constructible<ExplicitDefault>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<Implicit>::Value, std::is_trivially_default_constructible<Implicit>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<Explicit>::Value, std::is_trivially_default_constructible<Explicit>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<PrivateDefaultConstructor>::Value, std::is_trivially_default_constructible<PrivateDefaultConstructor>::value);
        #else
        CHECK(wstl::IsTriviallyDefaultConstructible<int>::Value);
        CHECK(wstl::IsTriviallyDefaultConstructible<TestData>::Value);
        CHECK_FALSE(wstl::IsTriviallyDefaultConstructible<NonTrivialConstructor>::Value);
        CHECK_FALSE(wstl::IsTriviallyDefaultConstructible<ExplicitDefault>::Value);
        CHECK_FALSE(wstl::IsTriviallyDefaultConstructible<Implicit>::Value);
        CHECK_FALSE(wstl::IsTriviallyDefaultConstructible<Explicit>::Value);
        CHECK_FALSE(wstl::IsTriviallyDefaultConstructible<PrivateDefaultConstructor>::Value);
        #endif
    }

    TEST_CASE("IsNothrowDefaultConstructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<int>::Value, std::is_nothrow_default_constructible<int>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<TestData>::Value, std::is_nothrow_default_constructible<TestData>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<NothrowData>::Value, std::is_nothrow_default_constructible<NothrowData>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<NonTrivialConstructor>::Value, std::is_nothrow_default_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<ExplicitDefault>::Value, std::is_nothrow_default_constructible<ExplicitDefault>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<Implicit>::Value, std::is_nothrow_default_constructible<Implicit>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<Explicit>::Value, std::is_nothrow_default_constructible<Explicit>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<PrivateDefaultConstructor>::Value, std::is_nothrow_default_constructible<PrivateDefaultConstructor>::value);
        #else
        CHECK(wstl::IsNothrowDefaultConstructible<int>::Value);
        CHECK(wstl::IsNothrowDefaultConstructible<TestData>::Value);
        CHECK(wstl::IsNothrowDefaultConstructible<NothrowData>::Value);
        CHECK_FALSE(wstl::IsNothrowDefaultConstructible<NonTrivialConstructor>::Value);
        CHECK_FALSE(wstl::IsNothrowDefaultConstructible<ExplicitDefault>::Value);
        CHECK_FALSE(wstl::IsNothrowDefaultConstructible<Implicit>::Value);
        CHECK_FALSE(wstl::IsNothrowDefaultConstructible<Explicit>::Value);
        CHECK_FALSE(wstl::IsNothrowDefaultConstructible<PrivateDefaultConstructor>::Value);
        #endif
    }

    TEST_CASE("IsCopyConstructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsCopyConstructible<int>::Value, std::is_copy_constructible<int>::value);
        CHECK_EQ(wstl::IsCopyConstructible<TestData>::Value, std::is_copy_constructible<TestData>::value);
        CHECK_EQ(wstl::IsCopyConstructible<NonTrivialConstructor>::Value, std::is_copy_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsCopyConstructible<CustomCopyMoveConstructor>::Value, std::is_copy_constructible<CustomCopyMoveConstructor>::value);
        CHECK_EQ(wstl::IsCopyConstructible<NoCopyConstructor>::Value, std::is_copy_constructible<NoCopyConstructor>::value);
        CHECK_EQ(wstl::IsCopyConstructible<FakeCopyConstructor>::Value, std::is_copy_constructible<FakeCopyConstructor>::value);
        #else
        CHECK(wstl::IsCopyConstructible<int>::Value);
        CHECK(wstl::IsCopyConstructible<TestData>::Value);
        CHECK(wstl::IsCopyConstructible<NonTrivialConstructor>::Value);
        CHECK(wstl::IsCopyConstructible<CustomCopyMoveConstructor>::Value);
        CHECK_FALSE(wstl::IsCopyConstructible<NoCopyConstructor>::Value);
        CHECK_FALSE(wstl::IsCopyConstructible<FakeCopyConstructor>::Value);
        #endif
    }

    TEST_CASE("IsTriviallyCopyConstructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<int>::Value, std::is_trivially_copy_constructible<int>::value);
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<TestData>::Value, std::is_trivially_copy_constructible<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<NonTrivialConstructor>::Value, std::is_trivially_copy_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<CustomCopyMoveConstructor>::Value, std::is_trivially_copy_constructible<CustomCopyMoveConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<NoCopyConstructor>::Value, std::is_trivially_copy_constructible<NoCopyConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<FakeCopyConstructor>::Value, std::is_trivially_copy_constructible<FakeCopyConstructor>::value);
        #else
        CHECK(wstl::IsTriviallyCopyConstructible<int>::Value);
        CHECK(wstl::IsTriviallyCopyConstructible<TestData>::Value);
        CHECK(wstl::IsTriviallyCopyConstructible<NonTrivialConstructor>::Value);
        CHECK_FALSE(wstl::IsTriviallyCopyConstructible<CustomCopyMoveConstructor>::Value);
        CHECK_FALSE(wstl::IsTriviallyCopyConstructible<NoCopyConstructor>::Value);
        CHECK_FALSE(wstl::IsTriviallyCopyConstructible<FakeCopyConstructor>::Value);
        #endif
    }

    TEST_CASE("IsNothrowCopyConstructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsNothrowCopyConstructible<int>::Value, std::is_nothrow_copy_constructible<int>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<TestData>::Value, std::is_nothrow_copy_constructible<TestData>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<NonTrivialConstructor>::Value, std::is_nothrow_copy_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<NothrowData>::Value, std::is_nothrow_copy_constructible<NothrowData>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<CustomCopyMoveConstructor>::Value, std::is_nothrow_copy_constructible<CustomCopyMoveConstructor>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<NoCopyConstructor>::Value, std::is_nothrow_copy_constructible<NoCopyConstructor>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<FakeCopyConstructor>::Value, std::is_nothrow_copy_constructible<FakeCopyConstructor>::value);
        #else
        CHECK(wstl::IsNothrowCopyConstructible<int>::Value);
        CHECK(wstl::IsNothrowCopyConstructible<TestData>::Value);
        CHECK(wstl::IsNothrowCopyConstructible<NonTrivialConstructor>::Value);
        CHECK(wstl::IsNothrowCopyConstructible<NothrowData>::Value);
        CHECK_FALSE(wstl::IsNothrowCopyConstructible<CustomCopyMoveConstructor>::Value);
        CHECK_FALSE(wstl::IsNothrowCopyConstructible<NoCopyConstructor>::Value);
        CHECK_FALSE(wstl::IsNothrowCopyConstructible<FakeCopyConstructor>::Value);
        #endif
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("IsMoveConstructible") {
        CHECK_EQ(wstl::IsMoveConstructible<int>::Value, std::is_move_constructible<int>::value);
        CHECK_EQ(wstl::IsMoveConstructible<TestData>::Value, std::is_move_constructible<TestData>::value);
        CHECK_EQ(wstl::IsMoveConstructible<NonTrivialConstructor>::Value, std::is_move_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsMoveConstructible<MovableData<int>>::Value, std::is_move_constructible<MovableData<int>>::value);
        CHECK_EQ(wstl::IsMoveConstructible<NoMoveConstructor>::Value, std::is_move_constructible<NoMoveConstructor>::value);
        CHECK_EQ(wstl::IsMoveConstructible<CustomCopyMoveConstructor>::Value, std::is_move_constructible<CustomCopyMoveConstructor>::value);
        CHECK_EQ(wstl::IsMoveConstructible<NoCopyConstructor>::Value, std::is_move_constructible<NoCopyConstructor>::value);
        CHECK_EQ(wstl::IsMoveConstructible<FakeCopyConstructor>::Value, std::is_move_constructible<FakeCopyConstructor>::value);
    }

    TEST_CASE("IsTriviallyMoveConstructible") {
        CHECK_EQ(wstl::IsTriviallyMoveConstructible<int>::Value, std::is_trivially_move_constructible<int>::value);
        CHECK_EQ(wstl::IsTriviallyMoveConstructible<TestData>::Value, std::is_trivially_move_constructible<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyMoveConstructible<NonTrivialConstructor>::Value, std::is_trivially_move_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyMoveConstructible<MovableData<int>>::Value, std::is_trivially_move_constructible<MovableData<int>>::value);
        CHECK_EQ(wstl::IsTriviallyMoveConstructible<NoMoveConstructor>::Value, std::is_trivially_move_constructible<NoMoveConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyMoveConstructible<CustomCopyMoveConstructor>::Value, std::is_trivially_move_constructible<CustomCopyMoveConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyMoveConstructible<NoCopyConstructor>::Value, std::is_trivially_move_constructible<NoCopyConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyMoveConstructible<FakeCopyConstructor>::Value, std::is_trivially_move_constructible<FakeCopyConstructor>::value);
    }

    TEST_CASE("IsNothrowMoveConstructible") {
        CHECK_EQ(wstl::IsNothrowMoveConstructible<int>::Value, std::is_nothrow_move_constructible<int>::value);
        CHECK_EQ(wstl::IsNothrowMoveConstructible<TestData>::Value, std::is_nothrow_move_constructible<TestData>::value);
        CHECK_EQ(wstl::IsNothrowMoveConstructible<NonTrivialConstructor>::Value, std::is_nothrow_move_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsNothrowMoveConstructible<MovableData<int>>::Value, std::is_nothrow_move_constructible<MovableData<int>>::value);
        CHECK_EQ(wstl::IsNothrowMoveConstructible<NoMoveConstructor>::Value, std::is_nothrow_move_constructible<NoMoveConstructor>::value);
        CHECK_EQ(wstl::IsNothrowMoveConstructible<CustomCopyMoveConstructor>::Value, std::is_nothrow_move_constructible<CustomCopyMoveConstructor>::value);
        CHECK_EQ(wstl::IsNothrowMoveConstructible<NoCopyConstructor>::Value, std::is_nothrow_move_constructible<NoCopyConstructor>::value);
        CHECK_EQ(wstl::IsNothrowMoveConstructible<FakeCopyConstructor>::Value, std::is_nothrow_move_constructible<FakeCopyConstructor>::value);
    }
    #endif

    TEST_CASE("IsAssignable") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsAssignable<int, int>::Value), (std::is_assignable<int, int>::value));
        CHECK_EQ((wstl::IsAssignable<int&, int>::Value), (std::is_assignable<int&, int>::value));
        CHECK_EQ((wstl::IsAssignable<int&, double>::Value), (std::is_assignable<int&, double>::value));
        CHECK_EQ((wstl::IsAssignable<NothrowData&, NothrowData>::Value), (std::is_assignable<NothrowData&, NothrowData>::value));
        CHECK_EQ((wstl::IsAssignable<TestData&, TestData>::Value), (std::is_assignable<TestData&, TestData>::value));
        CHECK_EQ((wstl::IsAssignable<FakeCopyAssignment&, FakeCopyAssignment>::Value), (std::is_assignable<FakeCopyAssignment&, FakeCopyAssignment>::value));
        CHECK_EQ((wstl::IsAssignable<FakeCopyConstructor&, FakeCopyConstructor>::Value), (std::is_assignable<FakeCopyConstructor&, FakeCopyConstructor>::value));
        CHECK_EQ((wstl::IsAssignable<NoCopyAssignment&, NoCopyAssignment>::Value), (std::is_assignable<NoCopyAssignment&, NoCopyAssignment>::value));
        #else
        CHECK_FALSE((wstl::IsAssignable<int, int>::Value));
        CHECK((wstl::IsAssignable<int&, int>::Value));
        CHECK((wstl::IsAssignable<int&, double>::Value));
        CHECK((wstl::IsAssignable<NothrowData&, NothrowData>::Value));
        CHECK((wstl::IsAssignable<TestData&, TestData>::Value));
        CHECK_FALSE((wstl::IsAssignable<FakeCopyAssignment&, FakeCopyAssignment>::Value));
        CHECK((wstl::IsAssignable<FakeCopyConstructor&, FakeCopyConstructor>::Value));
        CHECK_FALSE((wstl::IsAssignable<NoCopyAssignment&, NoCopyAssignment>::Value));
        #endif
    }

    TEST_CASE("IsTriviallyAssignable") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsTriviallyAssignable<int, int>::Value), (std::is_trivially_assignable<int, int>::value));
        CHECK_EQ((wstl::IsTriviallyAssignable<int&, int>::Value), (std::is_trivially_assignable<int&, int>::value));
        CHECK_EQ((wstl::IsTriviallyAssignable<int&, double>::Value), (std::is_trivially_assignable<int&, double>::value));
        CHECK_EQ((wstl::IsTriviallyAssignable<NothrowData&, NothrowData>::Value), (std::is_trivially_assignable<NothrowData&, NothrowData>::value));
        CHECK_EQ((wstl::IsTriviallyAssignable<TestData&, TestData>::Value), (std::is_trivially_assignable<TestData&, TestData>::value));
        CHECK_EQ((wstl::IsTriviallyAssignable<FakeCopyAssignment&, FakeCopyAssignment>::Value), (std::is_trivially_assignable<FakeCopyAssignment&, FakeCopyAssignment>::value));
        CHECK_EQ((wstl::IsTriviallyAssignable<FakeCopyConstructor&, FakeCopyConstructor>::Value), (std::is_trivially_assignable<FakeCopyConstructor&, FakeCopyConstructor>::value));
        CHECK_EQ((wstl::IsTriviallyAssignable<NoCopyAssignment&, NoCopyAssignment>::Value), (std::is_trivially_assignable<NoCopyAssignment&, NoCopyAssignment>::value));
        #else
        CHECK_FALSE((wstl::IsTriviallyAssignable<int, int>::Value));
        CHECK((wstl::IsTriviallyAssignable<int&, int>::Value));
        CHECK((wstl::IsTriviallyAssignable<int&, double>::Value));
        CHECK_FALSE((wstl::IsTriviallyAssignable<NothrowData&, NothrowData>::Value));
        CHECK((wstl::IsTriviallyAssignable<TestData&, TestData>::Value));
        CHECK_FALSE((wstl::IsTriviallyAssignable<FakeCopyAssignment&, FakeCopyAssignment>::Value));
        CHECK((wstl::IsTriviallyAssignable<FakeCopyConstructor&, FakeCopyConstructor>::Value));
        CHECK_FALSE((wstl::IsTriviallyAssignable<NoCopyAssignment&, NoCopyAssignment>::Value));
        #endif
    }

    TEST_CASE("IsNothrowAssignable") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ((wstl::IsNothrowAssignable<int, int>::Value), (std::is_nothrow_assignable<int, int>::value));
        CHECK_EQ((wstl::IsNothrowAssignable<int&, int>::Value), (std::is_nothrow_assignable<int&, int>::value));
        CHECK_EQ((wstl::IsNothrowAssignable<int&, double>::Value), (std::is_nothrow_assignable<int&, double>::value));
        CHECK_EQ((wstl::IsNothrowAssignable<NothrowData&, NothrowData>::Value), (std::is_nothrow_assignable<NothrowData&, NothrowData>::value));
        CHECK_EQ((wstl::IsNothrowAssignable<TestData&, TestData>::Value), (std::is_nothrow_assignable<TestData&, TestData>::value));
        CHECK_EQ((wstl::IsNothrowAssignable<FakeCopyAssignment&, FakeCopyAssignment>::Value), (std::is_nothrow_assignable<FakeCopyAssignment&, FakeCopyAssignment>::value));
        CHECK_EQ((wstl::IsNothrowAssignable<FakeCopyConstructor&, FakeCopyConstructor>::Value), (std::is_nothrow_assignable<FakeCopyConstructor&, FakeCopyConstructor>::value));
        CHECK_EQ((wstl::IsNothrowAssignable<NoCopyAssignment&, NoCopyAssignment>::Value), (std::is_nothrow_assignable<NoCopyAssignment&, NoCopyAssignment>::value));
        #else
        CHECK_FALSE((wstl::IsNothrowAssignable<int, int>::Value));
        CHECK((wstl::IsNothrowAssignable<int&, int>::Value));
        CHECK((wstl::IsNothrowAssignable<int&, double>::Value));
        CHECK((wstl::IsNothrowAssignable<NothrowData&, NothrowData>::Value));
        CHECK((wstl::IsNothrowAssignable<TestData&, TestData>::Value));
        CHECK_FALSE((wstl::IsNothrowAssignable<FakeCopyAssignment&, FakeCopyAssignment>::Value));
        CHECK((wstl::IsNothrowAssignable<FakeCopyConstructor&, FakeCopyConstructor>::Value));
        CHECK_FALSE((wstl::IsNothrowAssignable<NoCopyAssignment&, NoCopyAssignment>::Value));
        #endif
    }

    TEST_CASE("IsCopyAssignable") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsCopyAssignable<int>::Value, std::is_copy_assignable<int>::value);
        CHECK_EQ(wstl::IsCopyAssignable<NothrowData>::Value, std::is_copy_assignable<NothrowData>::value);
        CHECK_EQ(wstl::IsCopyAssignable<TestData>::Value, std::is_copy_assignable<TestData>::value);
        CHECK_EQ(wstl::IsCopyAssignable<FakeCopyAssignment>::Value, std::is_copy_assignable<FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsCopyAssignable<FakeCopyConstructor>::Value, std::is_copy_assignable<FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsCopyAssignable<TrivialConstructor>::Value, std::is_copy_assignable<TrivialConstructor>::value);
        CHECK_EQ(wstl::IsCopyAssignable<NoCopyAssignment>::Value, std::is_copy_assignable<NoCopyAssignment>::value);
        #else
        CHECK(wstl::IsCopyAssignable<int>::Value);
        CHECK(wstl::IsCopyAssignable<NothrowData>::Value);
        CHECK(wstl::IsCopyAssignable<TestData>::Value);
        CHECK_FALSE(wstl::IsCopyAssignable<FakeCopyAssignment>::Value);
        CHECK(wstl::IsCopyAssignable<FakeCopyConstructor>::Value);
        CHECK(wstl::IsCopyAssignable<TrivialConstructor>::Value);
        CHECK_FALSE(wstl::IsCopyAssignable<NoCopyAssignment>::Value);
        #endif
    }

    TEST_CASE("IsTriviallyCopyAssignable") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<int>::Value, std::is_trivially_copy_assignable<int>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<NothrowData>::Value, std::is_trivially_copy_assignable<NothrowData>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<TestData>::Value, std::is_trivially_copy_assignable<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<FakeCopyAssignment>::Value, std::is_trivially_copy_assignable<FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<FakeCopyConstructor>::Value, std::is_trivially_copy_assignable<FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<TrivialConstructor>::Value, std::is_trivially_copy_assignable<TrivialConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<NoCopyAssignment>::Value, std::is_trivially_copy_assignable<NoCopyAssignment>::value);
        #else
        CHECK(wstl::IsTriviallyCopyAssignable<int>::Value);
        CHECK_FALSE(wstl::IsTriviallyCopyAssignable<NothrowData>::Value);
        CHECK(wstl::IsTriviallyCopyAssignable<TestData>::Value);
        CHECK_FALSE(wstl::IsTriviallyCopyAssignable<FakeCopyAssignment>::Value);
        CHECK(wstl::IsTriviallyCopyAssignable<FakeCopyConstructor>::Value);
        CHECK(wstl::IsTriviallyCopyAssignable<TrivialConstructor>::Value);
        CHECK_FALSE(wstl::IsTriviallyCopyAssignable<NoCopyAssignment>::Value);
        #endif
    }

    TEST_CASE("IsNothrowCopyAssignable") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsNothrowCopyAssignable<int>::Value, std::is_nothrow_copy_assignable<int>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<NothrowData>::Value, std::is_nothrow_copy_assignable<NothrowData>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<TestData>::Value, std::is_nothrow_copy_assignable<TestData>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<FakeCopyAssignment>::Value, std::is_nothrow_copy_assignable<FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<FakeCopyConstructor>::Value, std::is_nothrow_copy_assignable<FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<TrivialConstructor>::Value, std::is_nothrow_copy_assignable<TrivialConstructor>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<NoCopyAssignment>::Value, std::is_nothrow_copy_assignable<NoCopyAssignment>::value);
        #else
        CHECK(wstl::IsNothrowCopyAssignable<int>::Value);
        CHECK(wstl::IsNothrowCopyAssignable<NothrowData>::Value);
        CHECK(wstl::IsNothrowCopyAssignable<TestData>::Value);
        CHECK_FALSE(wstl::IsNothrowCopyAssignable<FakeCopyAssignment>::Value);
        CHECK(wstl::IsNothrowCopyAssignable<FakeCopyConstructor>::Value);
        CHECK(wstl::IsNothrowCopyAssignable<TrivialConstructor>::Value);
        CHECK_FALSE(wstl::IsNothrowCopyAssignable<NoCopyAssignment>::Value);
        #endif
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("IsMoveAssignable") {
        CHECK_EQ(wstl::IsMoveAssignable<int>::Value, std::is_move_assignable<int>::value);
        CHECK_EQ(wstl::IsMoveAssignable<NothrowData>::Value, std::is_move_assignable<NothrowData>::value);
        CHECK_EQ(wstl::IsMoveAssignable<TestData>::Value, std::is_move_assignable<TestData>::value);
        CHECK_EQ(wstl::IsMoveAssignable<FakeCopyAssignment>::Value, std::is_move_assignable<FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsMoveAssignable<FakeCopyConstructor>::Value, std::is_move_assignable<FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsMoveAssignable<TrivialConstructor>::Value, std::is_move_assignable<TrivialConstructor>::value);
        CHECK_EQ(wstl::IsMoveAssignable<NoCopyAssignment>::Value, std::is_move_assignable<NoCopyAssignment>::value);
        CHECK_EQ(wstl::IsMoveAssignable<MovableData<int>>::Value, std::is_move_assignable<MovableData<int>>::value);
        CHECK_EQ(wstl::IsMoveAssignable<NoMoveAssignment>::Value, std::is_move_assignable<NoMoveAssignment>::value);
        CHECK_EQ(wstl::IsMoveAssignable<CustomCopyMoveConstructor>::Value, std::is_move_assignable<CustomCopyMoveConstructor>::value);
    }

    TEST_CASE("IsTriviallyMoveAssignable") {
        CHECK_EQ(wstl::IsTriviallyMoveAssignable<int>::Value, std::is_trivially_move_assignable<int>::value);
        CHECK_EQ(wstl::IsTriviallyMoveAssignable<NothrowData>::Value, std::is_trivially_move_assignable<NothrowData>::value);
        CHECK_EQ(wstl::IsTriviallyMoveAssignable<TestData>::Value, std::is_trivially_move_assignable<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyMoveAssignable<FakeCopyAssignment>::Value, std::is_trivially_move_assignable<FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsTriviallyMoveAssignable<FakeCopyConstructor>::Value, std::is_trivially_move_assignable<FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyMoveAssignable<TrivialConstructor>::Value, std::is_trivially_move_assignable<TrivialConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyMoveAssignable<NoCopyAssignment>::Value, std::is_trivially_move_assignable<NoCopyAssignment>::value);
        CHECK_EQ(wstl::IsTriviallyMoveAssignable<MovableData<int>>::Value, std::is_trivially_move_assignable<MovableData<int>>::value);
        CHECK_EQ(wstl::IsTriviallyMoveAssignable<NoMoveAssignment>::Value, std::is_trivially_move_assignable<NoMoveAssignment>::value);
        CHECK_EQ(wstl::IsTriviallyMoveAssignable<CustomCopyMoveConstructor>::Value, std::is_trivially_move_assignable<CustomCopyMoveConstructor>::value);
    }

    TEST_CASE("IsNothrowMoveAssignable") {
        CHECK_EQ(wstl::IsNothrowMoveAssignable<int>::Value, std::is_nothrow_move_assignable<int>::value);
        CHECK_EQ(wstl::IsNothrowMoveAssignable<NothrowData>::Value, std::is_nothrow_move_assignable<NothrowData>::value);
        CHECK_EQ(wstl::IsNothrowMoveAssignable<TestData>::Value, std::is_nothrow_move_assignable<TestData>::value);
        CHECK_EQ(wstl::IsNothrowMoveAssignable<FakeCopyAssignment>::Value, std::is_nothrow_move_assignable<FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsNothrowMoveAssignable<FakeCopyConstructor>::Value, std::is_nothrow_move_assignable<FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsNothrowMoveAssignable<TrivialConstructor>::Value, std::is_nothrow_move_assignable<TrivialConstructor>::value);
        CHECK_EQ(wstl::IsNothrowMoveAssignable<NoCopyAssignment>::Value, std::is_nothrow_move_assignable<NoCopyAssignment>::value);
        CHECK_EQ(wstl::IsNothrowMoveAssignable<MovableData<int>>::Value, std::is_nothrow_move_assignable<MovableData<int>>::value);
        CHECK_EQ(wstl::IsNothrowMoveAssignable<NoMoveAssignment>::Value, std::is_nothrow_move_assignable<NoMoveAssignment>::value);
        CHECK_EQ(wstl::IsNothrowMoveAssignable<CustomCopyMoveConstructor>::Value, std::is_nothrow_move_assignable<CustomCopyMoveConstructor>::value);
    }
    #endif

    TEST_CASE("IsDestructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsDestructible<int>::Value, std::is_destructible<int>::value);
        CHECK_EQ(wstl::IsDestructible<int&>::Value, std::is_destructible<int&>::value);
        CHECK_EQ(wstl::IsDestructible<TestData>::Value, std::is_destructible<TestData>::value);
        CHECK_EQ(wstl::IsDestructible<NothrowData>::Value, std::is_destructible<NothrowData>::value);
        CHECK_EQ(wstl::IsDestructible<NoDestructor>::Value, std::is_destructible<NoDestructor>::value);
        CHECK_EQ(wstl::IsDestructible<void>::Value, std::is_destructible<void>::value);
        CHECK_EQ(wstl::IsDestructible<Abstract>::Value, std::is_destructible<Abstract>::value);
        CHECK_EQ(wstl::IsDestructible<PrivateDestructor>::Value, std::is_destructible<PrivateDestructor>::value);
        #else
        CHECK(wstl::IsDestructible<int>::Value);
        CHECK(wstl::IsDestructible<int&>::Value);
        CHECK(wstl::IsDestructible<TestData>::Value);
        CHECK(wstl::IsDestructible<NothrowData>::Value);
        CHECK_FALSE(wstl::IsDestructible<NoDestructor>::Value);
        CHECK_FALSE(wstl::IsDestructible<void>::Value);
        CHECK(wstl::IsDestructible<Abstract>::Value);
        CHECK_FALSE(wstl::IsDestructible<PrivateDestructor>::Value);
        #endif
    }

    TEST_CASE("IsTriviallyDestructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsTriviallyDestructible<int>::Value, std::is_trivially_destructible<int>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<int&>::Value, std::is_trivially_destructible<int&>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<TestData>::Value, std::is_trivially_destructible<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<NothrowData>::Value, std::is_trivially_destructible<NothrowData>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<NoDestructor>::Value, std::is_trivially_destructible<NoDestructor>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<void>::Value, std::is_trivially_destructible<void>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<Abstract>::Value, std::is_trivially_destructible<Abstract>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<PrivateDestructor>::Value, std::is_trivially_destructible<PrivateDestructor>::value);
        #else
        CHECK(wstl::IsTriviallyDestructible<int>::Value);
        CHECK(wstl::IsTriviallyDestructible<int&>::Value);
        CHECK(wstl::IsTriviallyDestructible<TestData>::Value);
        CHECK_FALSE(wstl::IsTriviallyDestructible<NothrowData>::Value);
        CHECK_FALSE(wstl::IsTriviallyDestructible<NoDestructor>::Value);
        CHECK_FALSE(wstl::IsTriviallyDestructible<void>::Value);
        CHECK(wstl::IsTriviallyDestructible<Abstract>::Value);
        CHECK_FALSE(wstl::IsTriviallyDestructible<PrivateDestructor>::Value);
        #endif
    }

    TEST_CASE("IsNothrowDestructible") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsNothrowDestructible<int>::Value, std::is_nothrow_destructible<int>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<int&>::Value, std::is_nothrow_destructible<int&>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<TestData>::Value, std::is_nothrow_destructible<TestData>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<NothrowData>::Value, std::is_nothrow_destructible<NothrowData>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<NoDestructor>::Value, std::is_nothrow_destructible<NoDestructor>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<void>::Value, std::is_nothrow_destructible<void>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<Abstract>::Value, std::is_nothrow_destructible<Abstract>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<PrivateDestructor>::Value, std::is_nothrow_destructible<PrivateDestructor>::value);
        #else
        CHECK(wstl::IsNothrowDestructible<int>::Value);
        CHECK(wstl::IsNothrowDestructible<int&>::Value);
        CHECK(wstl::IsNothrowDestructible<TestData>::Value);
        CHECK(wstl::IsNothrowDestructible<NothrowData>::Value);
        CHECK_FALSE(wstl::IsNothrowDestructible<NoDestructor>::Value);
        CHECK_FALSE(wstl::IsNothrowDestructible<void>::Value);
        CHECK(wstl::IsNothrowDestructible<Abstract>::Value);
        CHECK_FALSE(wstl::IsNothrowDestructible<PrivateDestructor>::Value);
        #endif
    }

    TEST_CASE("IsTriviallyCopyable") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsTriviallyCopyable<int>::Value, std::is_trivially_copyable<int>::value);
        CHECK_EQ(wstl::IsTriviallyCopyable<TestData>::Value, std::is_trivially_copyable<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyCopyable<NonTrivialConstructor>::Value, std::is_trivially_copyable<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyable<CustomCopyMoveConstructor>::Value, std::is_trivially_copyable<CustomCopyMoveConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyable<NoCopyConstructor>::Value, std::is_trivially_copyable<NoCopyConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyable<FakeCopyConstructor>::Value, std::is_trivially_copyable<FakeCopyConstructor>::value);
        #else
        CHECK(wstl::IsTriviallyCopyable<int>::Value);
        CHECK(wstl::IsTriviallyCopyable<TestData>::Value);
        CHECK(wstl::IsTriviallyCopyable<NonTrivialConstructor>::Value);
        CHECK_FALSE(wstl::IsTriviallyCopyable<CustomCopyMoveConstructor>::Value);
        CHECK_FALSE(wstl::IsTriviallyCopyable<NoCopyConstructor>::Value);
        CHECK_FALSE(wstl::IsTriviallyCopyable<FakeCopyConstructor>::Value);
        #endif
    }

    TEST_CASE("IsSigned") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsSigned<bool>::Value, std::is_signed<bool>::value);
        CHECK_EQ(wstl::IsSigned<char>::Value, std::is_signed<char>::value);
        CHECK_EQ(wstl::IsSigned<signed char>::Value, std::is_signed<signed char>::value);
        CHECK_EQ(wstl::IsSigned<unsigned char>::Value, std::is_signed<unsigned char>::value);
        CHECK_EQ(wstl::IsSigned<wchar_t>::Value, std::is_signed<wchar_t>::value);
        CHECK_EQ(wstl::IsSigned<short>::Value, std::is_signed<short>::value);
        CHECK_EQ(wstl::IsSigned<signed short>::Value, std::is_signed<signed short>::value);
        CHECK_EQ(wstl::IsSigned<unsigned short>::Value, std::is_signed<unsigned short>::value);
        CHECK_EQ(wstl::IsSigned<int>::Value, std::is_signed<int>::value);
        CHECK_EQ(wstl::IsSigned<signed int>::Value, std::is_signed<signed int>::value);
        CHECK_EQ(wstl::IsSigned<unsigned int>::Value, std::is_signed<unsigned int>::value);
        CHECK_EQ(wstl::IsSigned<long>::Value, std::is_signed<long>::value);
        CHECK_EQ(wstl::IsSigned<signed long>::Value, std::is_signed<signed long>::value);
        CHECK_EQ(wstl::IsSigned<unsigned long>::Value, std::is_signed<unsigned long>::value);
        CHECK_EQ(wstl::IsSigned<long long>::Value, std::is_signed<long long>::value);
        CHECK_EQ(wstl::IsSigned<signed long long>::Value, std::is_signed<signed long long>::value);
        CHECK_EQ(wstl::IsSigned<unsigned long long>::Value, std::is_signed<unsigned long long>::value);
        CHECK_EQ(wstl::IsSigned<const int>::Value, std::is_signed<const int>::value);
        CHECK_EQ(wstl::IsSigned<volatile int>::Value, std::is_signed<volatile int>::value);
        CHECK_EQ(wstl::IsSigned<const int>::Value, std::is_signed<const int>::value);
        CHECK_EQ(wstl::IsSigned<const volatile int>::Value, std::is_signed<const volatile int>::value);
        CHECK_EQ(wstl::IsSigned<float>::Value, std::is_signed<float>::value);
        CHECK_EQ(wstl::IsSigned<double>::Value, std::is_signed<double>::value);
        CHECK_EQ(wstl::IsSigned<long double>::Value, std::is_signed<long double>::value);
        CHECK_EQ(wstl::IsSigned<TestData>::Value, std::is_signed<TestData>::value);

        CHECK_EQ(wstl::IsSigned<char16_t>::Value, std::is_signed<char16_t>::value);
        CHECK_EQ(wstl::IsSigned<char32_t>::Value, std::is_signed<char32_t>::value);
        #else
        CHECK_FALSE(wstl::IsSigned<bool>::Value);
        CHECK_EQ(wstl::IsSigned<char>::Value, CHAR_MIN < 0);
        CHECK(wstl::IsSigned<signed char>::Value);
        CHECK_FALSE(wstl::IsSigned<unsigned char>::Value);
        CHECK_EQ(wstl::IsSigned<wchar_t>::Value, WCHAR_MIN < 0);
        CHECK(wstl::IsSigned<short>::Value);
        CHECK(wstl::IsSigned<signed short>::Value);
        CHECK_FALSE(wstl::IsSigned<unsigned short>::Value);
        CHECK(wstl::IsSigned<int>::Value);
        CHECK(wstl::IsSigned<signed int>::Value);
        CHECK_FALSE(wstl::IsSigned<unsigned int>::Value);
        CHECK(wstl::IsSigned<long>::Value);
        CHECK(wstl::IsSigned<signed long>::Value);
        CHECK_FALSE(wstl::IsSigned<unsigned long>::Value);
        CHECK(wstl::IsSigned<long long>::Value);
        CHECK(wstl::IsSigned<signed long long>::Value);
        CHECK_FALSE(wstl::IsSigned<unsigned long long>::Value);
        CHECK(wstl::IsSigned<const int>::Value);
        CHECK(wstl::IsSigned<volatile int>::Value);
        CHECK(wstl::IsSigned<const int>::Value);
        CHECK(wstl::IsSigned<const volatile int>::Value);
        CHECK(wstl::IsSigned<float>::Value);
        CHECK(wstl::IsSigned<double>::Value);
        CHECK(wstl::IsSigned<long double>::Value);
        CHECK_FALSE(wstl::IsSigned<TestData>::Value);
        #endif

        #ifdef __WSTL_CXX20__
        CHECK_EQ(wstl::IsSigned<char8_t>::Value, std::is_signed<char8_t>::value);
        #endif
    }

    TEST_CASE("IsUnsigned") {
        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsUnsigned<bool>::Value, std::is_unsigned<bool>::value);
        CHECK_EQ(wstl::IsUnsigned<char>::Value, std::is_unsigned<char>::value);
        CHECK_EQ(wstl::IsUnsigned<signed char>::Value, std::is_unsigned<signed char>::value);
        CHECK_EQ(wstl::IsUnsigned<unsigned char>::Value, std::is_unsigned<unsigned char>::value);
        CHECK_EQ(wstl::IsUnsigned<wchar_t>::Value, std::is_unsigned<wchar_t>::value);
        CHECK_EQ(wstl::IsUnsigned<short>::Value, std::is_unsigned<short>::value);
        CHECK_EQ(wstl::IsUnsigned<signed short>::Value, std::is_unsigned<signed short>::value);
        CHECK_EQ(wstl::IsUnsigned<unsigned short>::Value, std::is_unsigned<unsigned short>::value);
        CHECK_EQ(wstl::IsUnsigned<int>::Value, std::is_unsigned<int>::value);
        CHECK_EQ(wstl::IsUnsigned<signed int>::Value, std::is_unsigned<signed int>::value);
        CHECK_EQ(wstl::IsUnsigned<unsigned int>::Value, std::is_unsigned<unsigned int>::value);
        CHECK_EQ(wstl::IsUnsigned<long>::Value, std::is_unsigned<long>::value);
        CHECK_EQ(wstl::IsUnsigned<signed long>::Value, std::is_unsigned<signed long>::value);
        CHECK_EQ(wstl::IsUnsigned<unsigned long>::Value, std::is_unsigned<unsigned long>::value);
        CHECK_EQ(wstl::IsUnsigned<long long>::Value, std::is_unsigned<long long>::value);
        CHECK_EQ(wstl::IsUnsigned<signed long long>::Value, std::is_unsigned<signed long long>::value);
        CHECK_EQ(wstl::IsUnsigned<unsigned long long>::Value, std::is_unsigned<unsigned long long>::value);
        CHECK_EQ(wstl::IsUnsigned<const int>::Value, std::is_unsigned<const int>::value);
        CHECK_EQ(wstl::IsUnsigned<volatile int>::Value, std::is_unsigned<volatile int>::value);
        CHECK_EQ(wstl::IsUnsigned<const int>::Value, std::is_unsigned<const int>::value);
        CHECK_EQ(wstl::IsUnsigned<const volatile int>::Value, std::is_unsigned<const volatile int>::value);
        CHECK_EQ(wstl::IsUnsigned<float>::Value, std::is_unsigned<float>::value);
        CHECK_EQ(wstl::IsUnsigned<double>::Value, std::is_unsigned<double>::value);
        CHECK_EQ(wstl::IsUnsigned<long double>::Value, std::is_unsigned<long double>::value);
        CHECK_EQ(wstl::IsUnsigned<TestData>::Value, std::is_unsigned<TestData>::value);
        #else
        CHECK(wstl::IsUnsigned<bool>::Value);
        CHECK_EQ(wstl::IsUnsigned<char>::Value, CHAR_MIN >= 0);
        CHECK_FALSE(wstl::IsUnsigned<signed char>::Value);
        CHECK(wstl::IsUnsigned<unsigned char>::Value);
        CHECK_EQ(wstl::IsUnsigned<wchar_t>::Value, WCHAR_MIN >= 0);
        CHECK_FALSE(wstl::IsUnsigned<short>::Value);
        CHECK_FALSE(wstl::IsUnsigned<signed short>::Value);
        CHECK(wstl::IsUnsigned<unsigned short>::Value);
        CHECK_FALSE(wstl::IsUnsigned<int>::Value);
        CHECK_FALSE(wstl::IsUnsigned<signed int>::Value);
        CHECK(wstl::IsUnsigned<unsigned int>::Value);
        CHECK_FALSE(wstl::IsUnsigned<long>::Value);
        CHECK_FALSE(wstl::IsUnsigned<signed long>::Value);
        CHECK(wstl::IsUnsigned<unsigned long>::Value);
        CHECK_FALSE(wstl::IsUnsigned<long long>::Value);
        CHECK_FALSE(wstl::IsUnsigned<signed long long>::Value);
        CHECK(wstl::IsUnsigned<unsigned long long>::Value);
        CHECK_FALSE(wstl::IsUnsigned<const int>::Value);
        CHECK_FALSE(wstl::IsUnsigned<volatile int>::Value);
        CHECK_FALSE(wstl::IsUnsigned<const int>::Value);
        CHECK_FALSE(wstl::IsUnsigned<const volatile int>::Value);
        CHECK_FALSE(wstl::IsUnsigned<float>::Value);
        CHECK_FALSE(wstl::IsUnsigned<double>::Value);
        CHECK_FALSE(wstl::IsUnsigned<long double>::Value);
        CHECK_FALSE(wstl::IsUnsigned<TestData>::Value);
        #endif

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::IsUnsigned<char16_t>::Value, std::is_unsigned<char16_t>::value);
        CHECK_EQ(wstl::IsUnsigned<char32_t>::Value, std::is_unsigned<char32_t>::value);
        #endif

        #ifdef __WSTL_CXX20__
        CHECK_EQ(wstl::IsUnsigned<char8_t>::Value, std::is_unsigned<char8_t>::value);
        #endif
    }

    TEST_CASE("Decay") {
        CHECK((wstl::IsSame<wstl::Decay<int>::Type, int>::Value));
        CHECK_FALSE((wstl::IsSame<wstl::Decay<int>::Type, float>::Value));
        CHECK((wstl::IsSame<wstl::Decay<int&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::Decay<const int&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::Decay<int[2]>::Type, int*>::Value));
        CHECK_FALSE((wstl::IsSame<wstl::Decay<int[4][2]>::Type, int*>::Value));
        CHECK_FALSE((wstl::IsSame<wstl::Decay<int[4][2]>::Type, int**>::Value));
        CHECK((wstl::IsSame<wstl::Decay<int[4][2]>::Type, int(*)[2]>::Value));
        CHECK((wstl::IsSame<wstl::Decay<int(int)>::Type, int(*)(int)>::Value));

        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::Decay<int&&>::Type, int>::Value));
        #endif
    }

    TEST_CASE("UnwrapReference") {
        CHECK((wstl::IsSame<wstl::UnwrapReference<int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::UnwrapReference<const int>::Type, const int>::Value));
        CHECK((wstl::IsSame<wstl::UnwrapReference<int&>::Type, int&>::Value));
        CHECK((wstl::IsSame<wstl::UnwrapReference<int*>::Type, int*>::Value));

        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::UnwrapReference<int&&>::Type, int&&>::Value));
        #endif
    }

    TEST_CASE("UnwrapReferenceDecay") {
        CHECK((wstl::IsSame<wstl::UnwrapReferenceDecay<int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::UnwrapReferenceDecay<const int>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::UnwrapReferenceDecay<int&>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::UnwrapReferenceDecay<int*>::Type, int*>::Value));

        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::UnwrapReferenceDecay<int&&>::Type, int>::Value));
        #endif
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("CommonType") {
        CHECK(wstl::IsSame<wstl::CommonType<int>::Type, std::common_type<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int, int>::Type, std::common_type<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int, float>::Type, std::common_type<int, float>::type>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int, double>::Type, std::common_type<int, double>::type>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int, const int>::Type, std::common_type<int, const int>::type>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int&, int>::Type, std::common_type<int&, int>::type>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int&, const int&>::Type, std::common_type<int&, const int&>::type>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int&&, int>::Type, std::common_type<int&&, int>::type>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int&&, const int&>::Type, std::common_type<int&&, const int&>::type>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int, char, double>::Type, std::common_type<int, char, double>::type>::Value);
    }
    #endif

    TEST_CASE("MakeSigned") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::MakeSigned<char>::Type, std::make_signed<char>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<signed char>::Type, std::make_signed<signed char>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<unsigned char>::Type, std::make_signed<unsigned char>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<short>::Type, std::make_signed<short>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<signed short>::Type, std::make_signed<signed short>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<unsigned short>::Type, std::make_signed<unsigned short>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<int>::Type, std::make_signed<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<signed int>::Type, std::make_signed<signed int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<unsigned int>::Type, std::make_signed<unsigned int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<long>::Type, std::make_signed<long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<signed long>::Type, std::make_signed<signed long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<unsigned long>::Type, std::make_signed<unsigned long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<long long>::Type, std::make_signed<long long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<signed long long>::Type, std::make_signed<signed long long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<unsigned long long>::Type, std::make_signed<unsigned long long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<const unsigned int>::Type, std::make_signed<const unsigned int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<volatile unsigned int>::Type, std::make_signed<volatile unsigned int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<const unsigned int>::Type, std::make_signed<const unsigned int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<const volatile unsigned int>::Type, std::make_signed<const volatile unsigned int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<size_t>::Type, std::make_signed<size_t>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::MakeSigned<char>::Type, signed char>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<signed char>::Type, signed char>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<unsigned char>::Type, signed char>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<short>::Type, signed short>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<signed short>::Type, signed short>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<unsigned short>::Type, signed short>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<int>::Type, signed int>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<signed int>::Type, signed int>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<unsigned int>::Type, signed int>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<long>::Type, signed long>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<signed long>::Type, signed long>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<unsigned long>::Type, signed long>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<long long>::Type, signed long long>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<signed long long>::Type, signed long long>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<unsigned long long>::Type, signed long long>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<const unsigned int>::Type, const signed int>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<volatile unsigned int>::Type, volatile signed int>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<const unsigned int>::Type, const signed int>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<const volatile unsigned int>::Type, const volatile signed int>::Value));
        CHECK((wstl::IsSame<wstl::MakeSigned<size_t>::Type, long>::Value));
        #endif

        CHECK(wstl::IsSigned<wstl::MakeSigned<wchar_t>::Type>::Value);
        CHECK_EQ(sizeof(wchar_t), sizeof(wstl::MakeSigned<wchar_t>::Type));

        #ifdef __WSTL_CXX11__
        enum class UnsignedEnum : unsigned int {};
        enum class SignedEnum : int {};

        CHECK(wstl::IsSame<wstl::MakeSigned<std::underlying_type<UnsignedEnum>::type>::Type, std::make_signed<UnsignedEnum>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<std::underlying_type<SignedEnum>::type>::Type, std::make_signed<SignedEnum>::type>::Value);
        #endif
    }

    TEST_CASE("MakeUnsigned") {
        #ifdef __WSTL_CXX11__
        CHECK((wstl::IsSame<wstl::MakeUnsigned<char>::Type, std::make_unsigned<char>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<signed char>::Type, std::make_unsigned<signed char>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<unsigned char>::Type, std::make_unsigned<unsigned char>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<short>::Type, std::make_unsigned<short>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<signed short>::Type, std::make_unsigned<signed short>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<unsigned short>::Type, std::make_unsigned<unsigned short>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<int>::Type, std::make_unsigned<int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<signed int>::Type, std::make_unsigned<signed int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<unsigned int>::Type, std::make_unsigned<unsigned int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<long>::Type, std::make_unsigned<long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<signed long>::Type, std::make_unsigned<signed long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<unsigned long>::Type, std::make_unsigned<unsigned long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<long long>::Type, std::make_unsigned<long long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<signed long long>::Type, std::make_unsigned<signed long long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<unsigned long long>::Type, std::make_unsigned<unsigned long long>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<const int>::Type, std::make_unsigned<const int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<volatile int>::Type, std::make_unsigned<volatile int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<const int>::Type, std::make_unsigned<const int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<const volatile int>::Type, std::make_unsigned<const volatile int>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<size_t>::Type, std::make_unsigned<size_t>::type>::Value));
        #else
        CHECK((wstl::IsSame<wstl::MakeUnsigned<char>::Type, unsigned char>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<signed char>::Type, unsigned char>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<unsigned char>::Type, unsigned char>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<short>::Type, unsigned short>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<signed short>::Type, unsigned short>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<unsigned short>::Type, unsigned short>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<int>::Type, unsigned int>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<signed int>::Type, unsigned int>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<unsigned int>::Type, unsigned int>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<long>::Type, unsigned long>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<signed long>::Type, unsigned long>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<unsigned long>::Type, unsigned long>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<long long>::Type, unsigned long long>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<signed long long>::Type, unsigned long long>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<unsigned long long>::Type, unsigned long long>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<const int>::Type, const unsigned int>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<volatile int>::Type, volatile unsigned int>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<const int>::Type, const unsigned int>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<const volatile int>::Type, const volatile unsigned int>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<size_t>::Type, size_t>::Value));
        #endif

        CHECK(wstl::IsUnsigned<wstl::MakeUnsigned<wchar_t>::Type>::Value);
        CHECK_EQ(sizeof(wchar_t), sizeof(wstl::MakeUnsigned<wchar_t>::Type));

        #ifdef __WSTL_CXX11__
        enum class UnsignedEnum : unsigned int {};
        enum class SignedEnum : int {};

        CHECK((wstl::IsSame<wstl::MakeUnsigned<std::underlying_type<UnsignedEnum>::type>::Type, std::make_unsigned<UnsignedEnum>::type>::Value));
        CHECK((wstl::IsSame<wstl::MakeUnsigned<std::underlying_type<SignedEnum>::type>::Type, std::make_unsigned<SignedEnum>::type>::Value));
        #endif
    }

    TEST_CASE("TypeWithAlignment") {
        CHECK_EQ(wstl::AlignmentOf<wstl::TypeWithAlignment<1UL>::Type>::Value, 1UL);
        CHECK_EQ(wstl::AlignmentOf<wstl::TypeWithAlignment<2UL>::Type>::Value, 2UL);
        CHECK_EQ(wstl::AlignmentOf<wstl::TypeWithAlignment<4UL>::Type>::Value, 4UL);
        CHECK_EQ(wstl::AlignmentOf<wstl::TypeWithAlignment<8UL>::Type>::Value, 8UL);

        #ifdef __WSTL_CXX11__
        CHECK_EQ(wstl::AlignmentOf<wstl::TypeWithAlignment<16UL>::Type>::Value, 16UL);
        CHECK_EQ(wstl::AlignmentOf<wstl::TypeWithAlignment<32UL>::Type>::Value, 32UL);
        CHECK_EQ(wstl::AlignmentOf<wstl::TypeWithAlignment<64UL>::Type>::Value, 64UL);
        #endif
    }

    TEST_CASE("IsTypeAligned") {
        CHECK((wstl::IsTypeAligned<char, 1>::Value));
        CHECK((wstl::IsTypeAligned<int, 4>::Value));
        CHECK((wstl::IsTypeAligned<double, 8>::Value));
        CHECK_FALSE((wstl::IsTypeAligned<double, 1>::Value));
        CHECK_FALSE((wstl::IsTypeAligned<int, 1>::Value));
        CHECK_FALSE((wstl::IsTypeAligned<int, 2>::Value));
    }

    TEST_CASE("AlignedStorage") {
        StorageType data[10];

        size_t alignment = wstl::AlignmentOf<StorageType>::Value;

        #ifdef __WSTL_CXX11__
        size_t expected = std::alignment_of<uint32_t>::value;
        #else
        size_t expected = 4UL;
        #endif

        CHECK_EQ(alignment, expected);

        // Test alignment of each element
        for(int i = 0; i < 10; ++i) CHECK_EQ((size_t(&data[i]) % expected), 0UL);
    }

    TEST_CASE("AlignedStorage conversion operators") {
        StorageType data;
        void* ptrData = &data.Data;

        uint32_t& ref = data;
        const uint32_t& cref = data;
        CHECK_EQ(&ref, ptrData);
        CHECK_EQ(&cref, ptrData);

        uint32_t* ptr = data;
        const uint32_t* cptr = data;
        CHECK_EQ(ptr, ptrData);
        CHECK_EQ(cptr, ptrData);

        uint32_t& ref2 = data.GetReference<uint32_t>();
        const uint32_t& cref2 = data.GetReference<uint32_t>();
        CHECK_EQ(&ref2, ptrData);
        CHECK_EQ(&cref2, ptrData);

        uint32_t* ptr2 = data.GetPointer<uint32_t>();
        const uint32_t* cptr2 = data.GetPointer<uint32_t>();
        CHECK_EQ(ptr2, ptrData);
        CHECK_EQ(cptr2, ptrData);
    }

    TEST_CASE("IsAligned") {
        union {
            uint32_t Value;
            char Data[2 * sizeof(uint32_t)];
        } cstorage;

        const char* cptr = cstorage.Data;
        char* ptr = cstorage.Data;

        CHECK(wstl::IsAligned(ptr, wstl::AlignmentOf<uint32_t>::Value));
        CHECK(wstl::IsAligned<wstl::AlignmentOf<uint32_t>::Value>(ptr));
        CHECK(wstl::IsAligned<uint32_t>(ptr));
        CHECK(wstl::IsAligned(cptr, wstl::AlignmentOf<const uint32_t>::Value));
        CHECK(wstl::IsAligned<wstl::AlignmentOf<const uint32_t>::Value>(cptr));
        CHECK(wstl::IsAligned<const uint32_t>(cptr));

        ++ptr;
        ++cptr;
        CHECK_FALSE(wstl::IsAligned(ptr, wstl::AlignmentOf<uint32_t>::Value));
        CHECK_FALSE(wstl::IsAligned<wstl::AlignmentOf<uint32_t>::Value>(ptr));
        CHECK_FALSE(wstl::IsAligned<uint32_t>(ptr));
        CHECK_FALSE(wstl::IsAligned(cptr, wstl::AlignmentOf<const uint32_t>::Value));
        CHECK_FALSE(wstl::IsAligned<wstl::AlignmentOf<const uint32_t>::Value>(cptr));
        CHECK_FALSE(wstl::IsAligned<const uint32_t>(cptr));
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("NthType") {
        CHECK(wstl::IsSame<wstl::NthType<0, short>::Type, short>::Value);
        CHECK(wstl::IsSame<wstl::NthType<0, int, char, double, float>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::NthType<1, int, char, double, float>::Type, char>::Value);
        CHECK(wstl::IsSame<wstl::NthType<2, int, char, double, float>::Type, double>::Value);
        CHECK(wstl::IsSame<wstl::NthType<3, int, char, double, float>::Type, float>::Value);

        // The following lines should fail with a compilation error
        // wstl::NthType<4, int, char, double, float>::Type i;
        // wstl::NthType<0>::Type j;
    }
    
    TEST_CASE("InvokeResult") {
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(Free0)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(Free1), int>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(Free2), int, char>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(Free3), int, char, double>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(Free0t<TestData>)>::Type, TestData>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(FreeNoexcept), char>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(FreeVariadic), int, float>::Type, long>::Value);

        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction*>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(&MemberFunction::Fn0c), const MemberFunction>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(&MemberFunction::Fn0v), volatile MemberFunction>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(&MemberFunction::Fn0cv), const volatile MemberFunction>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction&, char>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction, char>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(&MemberFunction::FnStatic), int>::Type, long>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, char>::Type, int>::Value);

        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(&A::M), A&>::Type, int&>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<__TESTING_DECLTYPE__(&A::M), A*>::Type, int&>::Value);

        CHECK(wstl::IsSame<wstl::InvokeResult<Functor0>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<Functor2, int, char>::Type, long>::Value);
    }

    TEST_CASE("IsInvocable") {
        // Free function
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(Free0)>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(Free0), float>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(Free1), int>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(Free1), long>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(Free1)>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(Free1), TestData>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(Free2), int, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(Free2), int>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(Free3), int, char, double>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(Free3), int, char, double, float>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(Free0t<TestData>)>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(Free0t<TestData>), int>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(FreeNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(FreeNoexcept), TestData>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(FreeVariadic), int, float>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(FreeVariadic), TestData>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(FreeVariadic)>::Value);

        // Member function pointer
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction*>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0)>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0), const MemberFunction>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0c), MemberFunction>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0c), const MemberFunction*>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0c), const MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0c), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0c)>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0c), volatile MemberFunction>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0v), MemberFunction>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0v), volatile MemberFunction*>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0v), volatile MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0v), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0v)>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0v), const MemberFunction>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0cv), MemberFunction>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0cv), const MemberFunction*>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0cv), volatile MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0cv), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0cv)>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction&, char>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction*, char>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction&, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction&, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), const MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRefOnly)>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction&, TestData>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction, char>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction&&, char>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly)>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction, TestData>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnStatic), int>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnStatic), char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnStatic)>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnStatic), TestData>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, char>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction*, double>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept)>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, TestData>::Value);

        // Member object pointer
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&A::M), A&>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&A::M), A>::Value);
        CHECK(wstl::IsInvocable<__TESTING_DECLTYPE__(&A::M), A*>::Value);
        CHECK_FALSE(wstl::IsInvocable<__TESTING_DECLTYPE__(&A::M)>::Value);

        // Functor
        CHECK(wstl::IsInvocable<Functor0>::Value);
        CHECK_FALSE(wstl::IsInvocable<Functor0, int>::Value);
        CHECK(wstl::IsInvocable<Functor2, int, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<Functor2>::Value);
        CHECK_FALSE(wstl::IsInvocable<Functor2, int, TestData>::Value);
        CHECK_FALSE(wstl::IsInvocable<Functor2, int>::Value);
    }

    TEST_CASE("IsInvocableReturn") {
        // Free function
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(FreeVoid), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(FreeVoid), TestData>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(FreeVoid)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(FreeVoid), int>::Value);

        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free0)>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(Free0)>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(Free0)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free0), float>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(Free0)>::Value);

        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free1), int>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(Free1), long>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(Free1), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free1)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free1), TestData>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(Free1), int>::Value);
        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free2), int, char>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(Free2), int, char>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(Free2), int, long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free2), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(Free2), int, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free2), int, TestData>::Value);
        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free3), int, char, double>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(Free3), int, char, double>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(Free3), int, char, float>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free3), int, char, double, float>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(Free3), int, char, double>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free3), int, char, TestData>::Value);
        CHECK(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(Free0t<TestData>)>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(Free0t<TestData>)>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(Free0t<int>)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(Free0t<TestData>), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(Free0t<TestData>), int>::Value);
        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(FreeNoexcept), char>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(FreeNoexcept), char>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(FreeNoexcept), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(FreeNoexcept), char, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(FreeNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(FreeNoexcept), TestData>::Value);
        CHECK(wstl::IsInvocableReturn<long, __TESTING_DECLTYPE__(FreeVariadic), int>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(FreeVariadic), int, float>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(FreeVariadic), int, double, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<long, __TESTING_DECLTYPE__(FreeVariadic)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(FreeVariadic), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<long, __TESTING_DECLTYPE__(FreeVariadic), TestData>::Value);

        // Member function pointer
        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction*>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0), const MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(&MemberFunction::Fn0)>::Value);
        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0c), MemberFunction>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::Fn0c), const MemberFunction*>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(&MemberFunction::Fn0c), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0c), volatile MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0c), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0c), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(&MemberFunction::Fn0c)>::Value);
        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0v), MemberFunction>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::Fn0v), volatile MemberFunction*>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(&MemberFunction::Fn0v), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0v), const MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0v), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0v), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(&MemberFunction::Fn0v)>::Value);
        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0cv), MemberFunction>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::Fn0cv), const MemberFunction*>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(&MemberFunction::Fn0cv), volatile MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0cv), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0cv), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(&MemberFunction::Fn0cv)>::Value);
        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction&, char>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction*, char>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction&, long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction&, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), const MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRefOnly), MemberFunction&, TestData>::Value);
        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction, char>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction&&, char>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction, long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction*, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnRRefOnly), MemberFunction, TestData>::Value);
        CHECK(wstl::IsInvocableReturn<long, __TESTING_DECLTYPE__(&MemberFunction::FnStatic), int>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::FnStatic), int>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(&MemberFunction::FnStatic), long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<long, __TESTING_DECLTYPE__(&MemberFunction::FnStatic)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(&MemberFunction::FnStatic), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<long, __TESTING_DECLTYPE__(&MemberFunction::FnStatic), TestData>::Value);
        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, char>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction*, char>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction&, long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, TestData>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::VoidFn), MemberFunction*, char>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::VoidFn), MemberFunction&, long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::VoidFn), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::VoidFn), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::VoidFn), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::VoidFn), char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::VoidFn), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::VoidFn), MemberFunction, TestData>::Value);

        // Member object pointer
        CHECK(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&A::M), A&>::Value);
        CHECK(wstl::IsInvocableReturn<void, __TESTING_DECLTYPE__(&A::M), A>::Value);
        CHECK(wstl::IsInvocableReturn<char, __TESTING_DECLTYPE__(&A::M), A*>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, __TESTING_DECLTYPE__(&A::M)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, __TESTING_DECLTYPE__(&A::M), A>::Value);

        // Functor
        CHECK(wstl::IsInvocableReturn<int, Functor0>::Value);
        CHECK(wstl::IsInvocableReturn<void, Functor0>::Value);
        CHECK(wstl::IsInvocableReturn<char, Functor0>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, Functor0, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, Functor0>::Value);
        CHECK(wstl::IsInvocableReturn<long, Functor2, int, char>::Value);
        CHECK(wstl::IsInvocableReturn<void, Functor2, int, char>::Value);
        CHECK(wstl::IsInvocableReturn<char, Functor2, int, long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<long, Functor2, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, Functor2, int, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, Functor2, int, TestData>::Value);
    }

    #ifdef __WSTL_CXX17__
    TEST_CASE("IsNothrowInvocable") {
        // Free function
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(Free0)>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(Free0), float>::Value);
        CHECK(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(FreeNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(FreeNoexcept), TestData>::Value);

        // Member function pointer
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction*>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0)>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::Fn0), const MemberFunction>::Value);
        CHECK(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, char>::Value);
        CHECK(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction*, double>::Value);
        CHECK(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept)>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, TestData>::Value);

        // Member object pointer
        CHECK(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&A::M), A&>::Value);
        CHECK(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&A::M), A>::Value);
        CHECK(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&A::M), A*>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<__TESTING_DECLTYPE__(&A::M)>::Value);

        // Functor
        CHECK_FALSE(wstl::IsNothrowInvocable<Functor0>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<Functor0, int>::Value);
        CHECK(wstl::IsNothrowInvocable<FunctorNoexcept>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocable<FunctorNoexcept, int>::Value);
    }

    TEST_CASE("IsNothrowInvocableReturn") {
        // Free function
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(Free0)>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<void, __TESTING_DECLTYPE__(Free0)>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<char, __TESTING_DECLTYPE__(Free0)>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(Free0), float>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<TestData, __TESTING_DECLTYPE__(Free0)>::Value);
        CHECK(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(FreeNoexcept), char>::Value);
        CHECK(wstl::IsNothrowInvocableReturn<void, __TESTING_DECLTYPE__(FreeNoexcept), char>::Value);
        CHECK(wstl::IsNothrowInvocableReturn<char, __TESTING_DECLTYPE__(FreeNoexcept), int>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(FreeNoexcept), char, int>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<TestData, __TESTING_DECLTYPE__(FreeNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(FreeNoexcept), TestData>::Value);

        // Member function pointer
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction*>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<char, __TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0), const MemberFunction>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::Fn0), int>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<TestData, __TESTING_DECLTYPE__(&MemberFunction::Fn0)>::Value);
        CHECK(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, char>::Value);
        CHECK(wstl::IsNothrowInvocableReturn<void, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction*, char>::Value);
        CHECK(wstl::IsNothrowInvocableReturn<char, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction&, long>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<TestData, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&MemberFunction::FnNoexcept), MemberFunction, TestData>::Value);

        // Member object pointer
        CHECK(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&A::M), A&>::Value);
        CHECK(wstl::IsNothrowInvocableReturn<void, __TESTING_DECLTYPE__(&A::M), A>::Value);
        CHECK(wstl::IsNothrowInvocableReturn<char, __TESTING_DECLTYPE__(&A::M), A*>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, __TESTING_DECLTYPE__(&A::M)>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<TestData, __TESTING_DECLTYPE__(&A::M), A>::Value);

        // Functor
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, Functor0>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<void, Functor0>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<char, Functor0>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, Functor0, int>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<TestData, Functor0>::Value);
        CHECK(wstl::IsNothrowInvocableReturn<int, FunctorNoexcept>::Value);
        CHECK(wstl::IsNothrowInvocableReturn<void, FunctorNoexcept>::Value);
        CHECK(wstl::IsNothrowInvocableReturn<char, FunctorNoexcept>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<int, FunctorNoexcept, int>::Value);
        CHECK_FALSE(wstl::IsNothrowInvocableReturn<TestData, FunctorNoexcept>::Value);
    }
    #endif
    #endif

    
    TEST_CASE("UnderlyingType") {
        #if !defined(__WSTL_TYPETRAITS_NO_BUILTINS__) && defined(__WSTL_CXX11__) && defined(__WSTL_SUPPORTED_COMPILER__)
        enum Enum1 : int {};
        enum Enum2 : unsigned long {};
        enum Enum3 : char {};
        enum class Enum4 : wchar_t {};
        enum class Enum5 : long long {};
        enum class Enum6 : unsigned char {};

        CHECK(wstl::IsSame<wstl::UnderlyingType<Enum1>::Type, std::underlying_type<Enum1>::type>::Value);
        CHECK(wstl::IsSame<wstl::UnderlyingType<Enum2>::Type, std::underlying_type<Enum2>::type>::Value);
        CHECK(wstl::IsSame<wstl::UnderlyingType<Enum3>::Type, std::underlying_type<Enum3>::type>::Value);
        CHECK(wstl::IsSame<wstl::UnderlyingType<Enum4>::Type, std::underlying_type<Enum4>::type>::Value);
        CHECK(wstl::IsSame<wstl::UnderlyingType<Enum5>::Type, std::underlying_type<Enum5>::type>::Value);
        CHECK(wstl::IsSame<wstl::UnderlyingType<Enum6>::Type, std::underlying_type<Enum6>::type>::Value);
        #else
        CHECK((wstl::IsSame<wstl::UnderlyingType<EnumData>::Type, int>::Value));
        #endif
    }
}
