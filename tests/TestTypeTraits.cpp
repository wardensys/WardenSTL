// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
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
#include <type_traits>

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

    int FnNoexcept(char) noexcept;
    int FnRefOnly(char) &;
    int FnRRefOnly(char) &&;
    static long FnStatic(int);
};

void FreeVoid(int);
int Free0();
int Free1(int);
int Free2(int, char);
int Free3(int, char, double);
template<typename T> T Free0t();
int FreeNoexcept(char) noexcept;
long FreeVariadic(int, ...);

struct Functor0 { typedef int ResultType; int operator()(); };
struct Functor2 { typedef long ResultType; long operator()(int, char); };

template<typename T>
T TestTypeIdentity(T first, typename wstl::TypeIdentity<T>::Type second) {
    return first + second;
}

struct TestData {};
class ClassData {};
enum EnumData {};
enum class EnumClassData {};

struct FakeEnum {
    operator int();
};

class A {
public:
    int M;
};

class BBaseA : A {
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
    NothrowData() noexcept {}
    NothrowData(const NothrowData&) noexcept {}
    NothrowData(NothrowData&&) noexcept {}
    ~NothrowData() noexcept {}
    NothrowData& operator=(const NothrowData&) noexcept;
};

struct PrivateDefaultConstructor {
private:
    PrivateDefaultConstructor() = delete;
};

struct NoCopyConstructor {
    NoCopyConstructor(const NoCopyConstructor&) = delete;
};

struct NoCopyAssignment {
    NoCopyAssignment& operator=(const NoCopyAssignment&) = delete;
};

struct NoMoveAssignment {
    NoMoveAssignment& operator=(NoMoveAssignment&&) = delete;
};

struct NoDestructor {
    ~NoDestructor() = delete;
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

struct NoMoveConstructor {
    NoMoveConstructor(NoMoveConstructor&&) = delete;
};

struct FakeCopyConstructor {
    FakeCopyConstructor(FakeCopyConstructor&) {}
};

struct CustomCopyMoveConstructor {
    CustomCopyMoveConstructor(const CustomCopyMoveConstructor&) {}
    CustomCopyMoveConstructor(CustomCopyMoveConstructor&&) {}
};

union UnionData {
    class ClassData {};
};

typedef wstl::AlignedStorage<sizeof(uint16_t), wstl::AlignmentOf<uint32_t>::Value>::Type StorageType;

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
        CHECK_EQ(wstl::IntegralConstant<int, 1>::Value, 1);
        CHECK(wstl::IsSame<int, wstl::IntegralConstant<int, 1>::ValueType>::Value);
        
        CHECK(wstl::BoolConstant<true>::Value);
        CHECK_FALSE(wstl::BoolConstant<false>::Value);
        CHECK(wstl::IsSame<bool, wstl::BoolConstant<true>::ValueType>::Value);

        CHECK(wstl::Negation<wstl::BoolConstant<false>>::Value);
        CHECK_FALSE(wstl::Negation<wstl::BoolConstant<true>>::Value);
        #endif
    }

    TEST_CASE("RemoveReference") {
        CHECK(wstl::IsSame<wstl::RemoveReference<int>::Type, std::remove_reference<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveReference<int&>::Type, std::remove_reference<int&>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveReference<const int&>::Type, std::remove_reference<const int&>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveReference<volatile int&>::Type, std::remove_reference<volatile int&>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveReference<const volatile int&>::Type, std::remove_reference<const volatile int&>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveReference<int&&>::Type, std::remove_reference<int&&>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveReference<const int&&>::Type, std::remove_reference<const int&&>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveReference<volatile int&&>::Type, std::remove_reference<volatile int&&>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveReference<const volatile int&&>::Type, std::remove_reference<const volatile int&&>::type>::Value);
    }

    TEST_CASE("RemovePointer") {
        CHECK(wstl::IsSame<wstl::RemovePointer<int>::Type, std::remove_pointer<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemovePointer<const int>::Type, std::remove_pointer<const int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemovePointer<int*>::Type, std::remove_pointer<int*>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemovePointer<const int*>::Type, std::remove_pointer<const int*>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemovePointer<volatile int*>::Type, std::remove_pointer<volatile int*>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemovePointer<const volatile int*>::Type, std::remove_pointer<const volatile int*>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemovePointer<int* const>::Type, std::remove_pointer<int* const>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemovePointer<int* volatile>::Type, std::remove_pointer<int* volatile>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemovePointer<int* const volatile>::Type, std::remove_pointer<int* const volatile>::type>::Value);
    }

    TEST_CASE("RemoveConst") {
        CHECK(wstl::IsSame<wstl::RemoveConst<int>::Type, std::remove_const<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveConst<const int>::Type, std::remove_const<const int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveConst<const volatile int>::Type, std::remove_const<const volatile int>::type>::Value);
    }

    TEST_CASE("RemoveVolatile") {
        CHECK(wstl::IsSame<wstl::RemoveVolatile<int>::Type, std::remove_volatile<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveVolatile<volatile int>::Type, std::remove_volatile<volatile int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveVolatile<const volatile int>::Type, std::remove_volatile<const volatile int>::type>::Value);
    }

    TEST_CASE("RemoveCV") {
        CHECK(wstl::IsSame<wstl::RemoveCV<int>::Type, std::remove_cv<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCV<const int>::Type, std::remove_cv<const int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCV<volatile int>::Type, std::remove_cv<volatile int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCV<const volatile int>::Type, std::remove_cv<const volatile int>::type>::Value);
    }

    TEST_CASE("RemoveExtent") {
        CHECK(wstl::IsSame<wstl::RemoveExtent<int>::Type, std::remove_extent<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveExtent<int[]>::Type, std::remove_extent<int[]>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveExtent<int[10]>::Type, std::remove_extent<int[10]>::type>::Value);
    }

    TEST_CASE("RemoveAllExtents") {
        CHECK(wstl::IsSame<wstl::RemoveAllExtents<int>::Type, std::remove_all_extents<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveAllExtents<int[10]>::Type, std::remove_all_extents<int[10]>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveAllExtents<int[10][10]>::Type, std::remove_all_extents<int[10][10]>::type>::Value);
    }

    TEST_CASE("AddPointer") {
        CHECK(wstl::IsSame<wstl::AddPointer<int>::Type, std::add_pointer<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddPointer<const int>::Type, std::add_pointer<const int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddPointer<int*>::Type, std::add_pointer<int*>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddPointer<const int*>::Type, std::add_pointer<const int*>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddPointer<int* const>::Type, std::add_pointer<int* const>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddPointer<int* volatile>::Type, std::add_pointer<int* volatile>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddPointer<int* const volatile>::Type, std::add_pointer<int* const volatile>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddPointer<int**>::Type, std::add_pointer<int**>::type>::Value);
    }

    TEST_CASE("AddLValueReference") {
        CHECK(wstl::IsSame<wstl::AddLValueReference<void>::Type, std::add_lvalue_reference<void>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddLValueReference<int>::Type, std::add_lvalue_reference<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddLValueReference<int*>::Type, std::add_lvalue_reference<int*>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddLValueReference<int&>::Type, std::add_lvalue_reference<int&>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddLValueReference<int&&>::Type, std::add_lvalue_reference<int&&>::type>::Value);
    }

    TEST_CASE("AddLValueReference") {
        CHECK(wstl::IsSame<wstl::AddRValueReference<void>::Type, std::add_rvalue_reference<void>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddRValueReference<int>::Type, std::add_rvalue_reference<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddRValueReference<int*>::Type, std::add_rvalue_reference<int*>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddRValueReference<int&>::Type, std::add_rvalue_reference<int&>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddRValueReference<int&&>::Type, std::add_rvalue_reference<int&&>::type>::Value);
    }

    TEST_CASE("AddConst") {
        CHECK(wstl::IsSame<wstl::AddConst<int>::Type, std::add_const<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddConst<const int>::Type, std::add_const<const int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddConst<volatile int>::Type, std::add_const<volatile int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddConst<int*>::Type, std::add_const<int*>::type>::Value);
    }

    TEST_CASE("AddVolatile") {
        CHECK(wstl::IsSame<wstl::AddVolatile<int>::Type, std::add_volatile<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddVolatile<const int>::Type, std::add_volatile<const int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddVolatile<volatile int>::Type, std::add_volatile<volatile int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddVolatile<int*>::Type, std::add_volatile<int*>::type>::Value);
    }

    TEST_CASE("AddCV") {
        CHECK(wstl::IsSame<wstl::AddCV<int>::Type, std::add_cv<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddCV<const int>::Type, std::add_cv<const int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddCV<volatile int>::Type, std::add_cv<volatile int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddCV<const volatile int>::Type, std::add_cv<const volatile int>::type>::Value);
        CHECK(wstl::IsSame<wstl::AddCV<int*>::Type, std::add_cv<int*>::type>::Value);
    }

    TEST_CASE("RemoveCVReference") {
        CHECK(wstl::IsSame<wstl::RemoveCVReference<int>::Type, std::remove_cv<std::remove_reference<int>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<int* const>::Type, std::remove_cv<std::remove_reference<int* const>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<int* volatile>::Type, std::remove_cv<std::remove_reference<int* volatile>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<int* const volatile>::Type, std::remove_cv<std::remove_reference<int* const volatile>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<const int* const&>::Type, std::remove_cv<std::remove_reference<const int* const&>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<int&>::Type, std::remove_cv<std::remove_reference<int&>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<int&&>::Type, std::remove_cv<std::remove_reference<int&&>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<const int>::Type, std::remove_cv<std::remove_reference<const int>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<const int&>::Type, std::remove_cv<std::remove_reference<const int&>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<const int&&>::Type, std::remove_cv<std::remove_reference<const int&&>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<volatile int>::Type, std::remove_cv<std::remove_reference<volatile int>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<volatile int&>::Type, std::remove_cv<std::remove_reference<volatile int&>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<volatile int&&>::Type, std::remove_cv<std::remove_reference<volatile int&&>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<const volatile int>::Type, std::remove_cv<std::remove_reference<const volatile int>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<const volatile int&>::Type, std::remove_cv<std::remove_reference<const volatile int&>::type>::type>::Value);
        CHECK(wstl::IsSame<wstl::RemoveCVReference<const volatile int&&>::Type, std::remove_cv<std::remove_reference<const volatile int&&>::type>::type>::Value);
    }

    TEST_CASE("AlignmentOf") {
        struct Object {
            int a;
            char b;
            float c;
        };

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
    }

    TEST_CASE("Rank") {
        CHECK_EQ(wstl::Rank<int>::Value, std::rank<int>::value);
        CHECK_EQ(wstl::Rank<int[10]>::Value, std::rank<int[10]>::value);
        CHECK_EQ(wstl::Rank<int[10][10]>::Value, std::rank<int[10][10]>::value);
    }

    TEST_CASE("Extent") {
        CHECK_EQ(wstl::Extent<int>::Value, std::extent<int>::value);
        CHECK_EQ(wstl::Extent<int[]>::Value, std::extent<int[]>::value);
        CHECK_EQ(wstl::Extent<int[10]>::Value, std::extent<int[10]>::value);
    }

    TEST_CASE("Conditional") {
        CHECK(wstl::IsSame<wstl::Conditional<true, int, char>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::Conditional<false, int, char>::Type, char>::Value);
    }

    TEST_CASE("ResultOf") {
        // Free functions
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&Free0)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&Free1)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&Free2)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&Free3)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&Free0t<char>)>::Type, char>::Value);

        // Member functions
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn0)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn1)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn2)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn3)>::Type, int>::Value);

        // const
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn0c)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn1c)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn2c)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn3c)>::Type, int>::Value);

        // volatile
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn0v)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn1v)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn2v)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn3v)>::Type, int>::Value);

        // const volatile
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn0cv)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn1cv)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn2cv)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::Fn3cv)>::Type, int>::Value);

        // Return type variations
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::VoidFn)>::Type, void>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::LongFn)>::Type, long>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<decltype(&MemberFunction::ShortFn)>::Type, short>::Value);

        // Functors
        CHECK(wstl::IsSame<wstl::ResultOf<Functor0>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::ResultOf<Functor2>::Type, long>::Value);
    }

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
        CHECK_FALSE(wstl::IsConst<int>::Value);
        CHECK_FALSE(wstl::IsConst<volatile int>::Value);
        CHECK(wstl::IsConst<const int>::Value);
        CHECK(wstl::IsConst<const volatile int>::Value);
    }

    TEST_CASE("IsVolatile") {
        CHECK_FALSE(wstl::IsVolatile<int>::Value);
        CHECK(wstl::IsVolatile<volatile int>::Value);
        CHECK_FALSE(wstl::IsVolatile<const int>::Value);
        CHECK(wstl::IsVolatile<const volatile int>::Value);
    }

    TEST_CASE("IsSame") {
        CHECK(wstl::IsSame<int, int>::Value);
        CHECK_FALSE(wstl::IsSame<int, char>::Value);
    }

    TEST_CASE("IsNullPointer") {
        CHECK(wstl::IsNullPointer<nullptr_t>::Value);
        CHECK(wstl::IsNullPointer<const nullptr_t>::Value);
        CHECK(wstl::IsNullPointer<volatile nullptr_t>::Value);
        CHECK(wstl::IsNullPointer<const volatile nullptr_t>::Value);
        CHECK(wstl::IsNullPointer<wstl::nullptr_t>::Value);
        CHECK(wstl::IsNullPointer<wstl::NullPointerType>::Value);
        CHECK_FALSE(wstl::IsNullPointer<int>::Value);
    }

    TEST_CASE("IsVoid") {
        CHECK(wstl::IsVoid<void>::Value);
        CHECK(wstl::IsVoid<const void>::Value);
        CHECK(wstl::IsVoid<volatile void>::Value);
        CHECK(wstl::IsVoid<wstl::VoidType<int, char>>::Value);
        CHECK_FALSE(wstl::IsVoid<int>::Value);
    }

    TEST_CASE("IsIntegral") {
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
        CHECK(wstl::IsIntegral<char16_t>::Value);
        CHECK(wstl::IsIntegral<char32_t>::Value);
        CHECK_FALSE(wstl::IsIntegral<TestData>::Value);
        CHECK_FALSE(wstl::IsIntegral<EnumData>::Value);
        CHECK_FALSE(wstl::IsIntegral<EnumClassData>::Value);
        CHECK_FALSE(wstl::IsIntegral<UnionData>::Value);
        CHECK_FALSE(wstl::IsIntegral<int[]>::Value);
        CHECK_FALSE(wstl::IsIntegral<int&>::Value);
        CHECK_FALSE(wstl::IsIntegral<int TestData::*>::Value);
        CHECK_FALSE(wstl::IsIntegral<EnumData>::Value);

        #ifdef __WSTL_CXX20__
        CHECK(wstl::IsIntegral<char8_t>::Value);
        #endif
    }

    TEST_CASE("IsFloatingPoint") {
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
        CHECK_FALSE(wstl::IsFloatingPoint<const int>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<const volatile int>::Value);
        CHECK(wstl::IsFloatingPoint<float>::Value);
        CHECK(wstl::IsFloatingPoint<double>::Value);
        CHECK(wstl::IsFloatingPoint<long double>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<char16_t>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<char32_t>::Value);

        CHECK_FALSE(wstl::IsFloatingPoint<TestData>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<const TestData>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<EnumData>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<EnumClassData>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<UnionData>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<int[]>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<int&>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<int TestData::*>::Value);
        CHECK_FALSE(wstl::IsFloatingPoint<EnumData>::Value);

        CHECK(wstl::IsFloatingPoint<const float>::Value);
        CHECK(wstl::IsFloatingPoint<volatile float>::Value);
        CHECK(wstl::IsFloatingPoint<const volatile float>::Value);

        #ifdef __WSTL_CXX20__
        CHECK_FALSE(wstl::IsFloatingPoint<char8_t>::Value);
        #endif
    }

    TEST_CASE("IsArithmetic") {
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
        CHECK(wstl::IsArithmetic<char16_t>::Value);
        CHECK(wstl::IsArithmetic<char32_t>::Value);
        CHECK_FALSE(wstl::IsArithmetic<TestData>::Value);
        CHECK_FALSE(wstl::IsArithmetic<EnumData>::Value);
        CHECK_FALSE(wstl::IsArithmetic<EnumClassData>::Value);
        CHECK_FALSE(wstl::IsArithmetic<UnionData>::Value);
        CHECK_FALSE(wstl::IsArithmetic<int[]>::Value);
        CHECK_FALSE(wstl::IsArithmetic<int&>::Value);
        CHECK_FALSE(wstl::IsArithmetic<int TestData::*>::Value);
        CHECK_FALSE(wstl::IsArithmetic<EnumData>::Value);

        #ifdef __WSTL_CXX20__
        CHECK(wstl::IsArithmetic<char8_t>::Value);
        #endif
    }

    TEST_CASE("IsFundamental") {
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
        CHECK(wstl::IsFundamental<char16_t>::Value);
        CHECK(wstl::IsFundamental<char32_t>::Value);
        CHECK(wstl::IsFundamental<nullptr_t>::Value);
        CHECK_FALSE(wstl::IsFundamental<TestData>::Value);
        CHECK_FALSE(wstl::IsFundamental<EnumData>::Value);
        CHECK_FALSE(wstl::IsFundamental<EnumClassData>::Value);
        CHECK_FALSE(wstl::IsFundamental<UnionData>::Value);
        CHECK_FALSE(wstl::IsFundamental<int[]>::Value);
        CHECK_FALSE(wstl::IsFundamental<int&>::Value);
        CHECK_FALSE(wstl::IsFundamental<int TestData::*>::Value);
        CHECK_FALSE(wstl::IsFundamental<EnumData>::Value);

        #ifdef __WSTL_CXX20__
        CHECK(wstl::IsFundamental<char8_t>::Value);
        #endif
    }

    TEST_CASE("IsCompound") {
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
        CHECK_FALSE(wstl::IsCompound<char16_t>::Value);
        CHECK_FALSE(wstl::IsCompound<char32_t>::Value);
        CHECK_FALSE(wstl::IsCompound<nullptr_t>::Value);
        CHECK(wstl::IsCompound<TestData>::Value);
        CHECK(wstl::IsCompound<EnumData>::Value);
        CHECK(wstl::IsCompound<EnumClassData>::Value);
        CHECK(wstl::IsCompound<UnionData>::Value);
        CHECK(wstl::IsCompound<int[]>::Value);
        CHECK(wstl::IsCompound<int&>::Value);
        CHECK(wstl::IsCompound<int TestData::*>::Value);
        CHECK(wstl::IsCompound<EnumData>::Value);

        #ifdef __WSTL_CXX20__
        CHECK_FALSE(wstl::IsCompound<char8_t>::Value);
        #endif
    }

    TEST_CASE("IsLValueReference") {
        CHECK(wstl::IsLValueReference<int&>::Value);
        CHECK(wstl::IsLValueReference<const int&>::Value);
        CHECK(wstl::IsLValueReference<volatile int&>::Value);
        CHECK(wstl::IsLValueReference<const volatile int&>::Value);
        CHECK_FALSE(wstl::IsLValueReference<int&&>::Value);
        CHECK_FALSE(wstl::IsLValueReference<int*>::Value);
        CHECK_FALSE(wstl::IsLValueReference<int>::Value);
        CHECK_FALSE(wstl::IsLValueReference<void>::Value);
    }

    TEST_CASE("IsRValueReference") {
        CHECK(wstl::IsRValueReference<int&&>::Value);
        CHECK(wstl::IsRValueReference<const int&&>::Value);
        CHECK(wstl::IsRValueReference<volatile int&&>::Value);
        CHECK(wstl::IsRValueReference<const volatile int&&>::Value);
        CHECK_FALSE(wstl::IsRValueReference<int&>::Value);
        CHECK_FALSE(wstl::IsRValueReference<int*>::Value);
        CHECK_FALSE(wstl::IsRValueReference<int>::Value);
        CHECK_FALSE(wstl::IsRValueReference<void>::Value);
    }

    TEST_CASE("IsReference") {
        CHECK(wstl::IsReference<int&>::Value);
        CHECK(wstl::IsReference<const int&>::Value);
        CHECK(wstl::IsReference<volatile int&>::Value);
        CHECK(wstl::IsReference<const volatile int&>::Value);
        CHECK(wstl::IsReference<int&&>::Value);
        CHECK_FALSE(wstl::IsReference<int*>::Value);
        CHECK_FALSE(wstl::IsReference<int>::Value);
        CHECK_FALSE(wstl::IsReference<void>::Value);
    }

    TEST_CASE("IsFunction") {
        CHECK(wstl::IsFunction<decltype(Free0)>::Value);
        CHECK(wstl::IsFunction<decltype(Free1)>::Value);
        CHECK(wstl::IsFunction<decltype(Free2)>::Value);
        CHECK(wstl::IsFunction<decltype(Free3)>::Value);
        CHECK(wstl::IsFunction<decltype(Free0t<char>)>::Value);
        CHECK_FALSE(wstl::IsFunction<int MemberFunction::*>::Value);
        CHECK_FALSE(wstl::IsFunction<int*>::Value);
        CHECK_FALSE(wstl::IsFunction<int (MemberFunction::*)(int)>::Value);
        CHECK_FALSE(wstl::IsFunction<int>::Value);
    }

    TEST_CASE("IsArray") {
        CHECK(wstl::IsArray<int[10]>::Value);
        CHECK(wstl::IsArray<int[]>::Value);
        CHECK(wstl::IsArray<int[10][10]>::Value);
        CHECK_FALSE(wstl::IsArray<int>::Value);
    }

    TEST_CASE("IsUnion") {
        CHECK(wstl::IsUnion<UnionData>::Value);
        CHECK_FALSE(wstl::IsUnion<TestData>::Value);
    }

    TEST_CASE("IsMemberPointer") {
        CHECK(wstl::IsMemberPointer<int MemberFunction::*>::Value);
        CHECK(wstl::IsMemberPointer<int (MemberFunction::*)(int)>::Value);
        CHECK_FALSE(wstl::IsMemberPointer<int*>::Value);
        CHECK_FALSE(wstl::IsMemberPointer<int>::Value);
        CHECK_FALSE(wstl::IsMemberPointer<decltype(&Free0)>::Value);
    }

    TEST_CASE("IsMemberFunctionPointer") {
        CHECK_FALSE(wstl::IsMemberFunctionPointer<int MemberFunction::*>::Value);
        CHECK(wstl::IsMemberFunctionPointer<int (MemberFunction::*)(int)>::Value);
        CHECK_FALSE(wstl::IsMemberFunctionPointer<int*>::Value);
        CHECK_FALSE(wstl::IsMemberFunctionPointer<int>::Value);
        CHECK_FALSE(wstl::IsMemberFunctionPointer<decltype(&Free0)>::Value);
    }

    TEST_CASE("IsMemberObjectPointer") {
        CHECK(wstl::IsMemberObjectPointer<int MemberFunction::*>::Value);
        CHECK_FALSE(wstl::IsMemberObjectPointer<int (MemberFunction::*)(int)>::Value);
        CHECK_FALSE(wstl::IsMemberObjectPointer<int*>::Value);
        CHECK_FALSE(wstl::IsMemberObjectPointer<int>::Value);
        CHECK_FALSE(wstl::IsMemberObjectPointer<decltype(&Free0)>::Value);
    }

    TEST_CASE("IsClass") {
        CHECK(wstl::IsClass<TestData>::Value);
        CHECK(wstl::IsClass<ClassData>::Value);
        CHECK(wstl::IsClass<const ClassData>::Value);
        CHECK(wstl::IsClass<UnionData::ClassData>::Value);
        CHECK_FALSE(wstl::IsClass<EnumClassData>::Value);
        CHECK_FALSE(wstl::IsClass<UnionData>::Value);
        CHECK_FALSE(wstl::IsClass<EnumData>::Value);
        CHECK_FALSE(wstl::IsClass<int>::Value);
    }

    TEST_CASE("IsBaseOf") {
        CHECK(wstl::IsBaseOf<A, A>::Value);
        CHECK(wstl::IsBaseOf<A, BBaseA>::Value);
        CHECK(wstl::IsBaseOf<A, CBaseB>::Value);
        CHECK_FALSE(wstl::IsBaseOf<A, D>::Value);
        CHECK_FALSE(wstl::IsBaseOf<BBaseA, A>::Value);
        CHECK_FALSE(wstl::IsBaseOf<UnionData, UnionData>::Value);
        CHECK_FALSE(wstl::IsBaseOf<int, int>::Value);
    }

    TEST_CASE("IsConvertible") {
        // Fundamental types
        CHECK(wstl::IsConvertible<int, double>::Value);
        CHECK(wstl::IsConvertible<double, int>::Value);
        CHECK(wstl::IsConvertible<int, int>::Value);

        // Pointers
        CHECK(wstl::IsConvertible<int*, const int*>::Value);
        CHECK_FALSE(wstl::IsConvertible<const int*, int*>::Value);

        // Inheritance
        CHECK_FALSE(wstl::IsConvertible<BBaseA*, A*>::Value);
        CHECK_FALSE(wstl::IsConvertible<A*, BBaseA*>::Value);

        // References
        CHECK(wstl::IsConvertible<int&, int>::Value);
        CHECK(wstl::IsConvertible<int&, const int&>::Value);
        CHECK_FALSE(wstl::IsConvertible<const int&, int&>::Value);

        // Void
        CHECK(wstl::IsConvertible<void, void>::Value);
        CHECK_FALSE(wstl::IsConvertible<int, void>::Value);
        CHECK_FALSE(wstl::IsConvertible<void, int>::Value);

        // User-defined implicit
        CHECK(wstl::IsConvertible<int, Implicit>::Value);
        CHECK(wstl::IsConvertible<From, To>::Value);

        // User-defined explicit
        CHECK_FALSE(wstl::IsConvertible<int, Explicit>::Value);

        // Conversion operator
        CHECK(wstl::IsConvertible<ToBool, bool>::Value);

        // Array
        CHECK(wstl::IsConvertible<int[3], const int*>::Value);

        // Function pointers
        using Fn = int(int);
        CHECK(wstl::IsConvertible<Fn, Fn*>::Value);

        // CV qualifiers
        CHECK(wstl::IsConvertible<int, const int>::Value);
        CHECK(wstl::IsConvertible<const int, int>::Value);

        // Enum
        CHECK(wstl::IsConvertible<EnumData, int>::Value);
        CHECK_FALSE(wstl::IsConvertible<EnumClassData, int>::Value);
    }

    TEST_CASE("IsEnum") {
        CHECK(wstl::IsEnum<EnumData>::Value);
        CHECK(wstl::IsEnum<EnumClassData>::Value);
        CHECK_FALSE(wstl::IsEnum<TestData>::Value);
        CHECK_FALSE(wstl::IsEnum<FakeEnum>::Value);
        CHECK_FALSE(wstl::IsEnum<int>::Value);
    }

    TEST_CASE("IsPointer") {
        CHECK(wstl::IsPointer<int*>::Value);
        CHECK(wstl::IsPointer<const int*>::Value);
        CHECK(wstl::IsPointer<volatile int*>::Value);
        CHECK(wstl::IsPointer<const volatile int*>::Value);
        CHECK_FALSE(wstl::IsPointer<int>::Value);
    }

    TEST_CASE("IsScalar") {
        CHECK(wstl::IsScalar<int>::Value);
        CHECK(wstl::IsScalar<float>::Value);
        CHECK(wstl::IsScalar<EnumData>::Value);
        CHECK(wstl::IsScalar<EnumClassData>::Value);
        CHECK(wstl::IsScalar<int*>::Value);
        CHECK(wstl::IsScalar<int MemberFunction::*>::Value);
        CHECK(wstl::IsScalar<int (MemberFunction::*)(int)>::Value);
        CHECK(wstl::IsScalar<decltype(&Free0)>::Value);
        CHECK_FALSE(wstl::IsScalar<ClassData>::Value);
        CHECK_FALSE(wstl::IsScalar<TestData>::Value);
    }

    TEST_CASE("IsTrivial") {
        CHECK(wstl::IsTrivial<int>::Value);
        CHECK(wstl::IsTrivial<TrivialConstructor>::Value);
        CHECK(wstl::IsTrivial<PrivateDefaultConstructor>::Value);
        CHECK_FALSE(wstl::IsTrivial<NonTrivialData>::Value);
        CHECK_FALSE(wstl::IsTrivial<NonTrivialConstructor>::Value);
    }

    TEST_CASE("IsPOD") {
        CHECK(wstl::IsPOD<int>::Value);
        CHECK(wstl::IsPOD<A>::Value);
        CHECK_FALSE(wstl::IsPOD<BBaseA>::Value);
        CHECK_FALSE(wstl::IsPOD<VirtualFunction>::Value);
    }

    TEST_CASE("IsStandardLayout") {
        CHECK(wstl::IsStandardLayout<int>::Value);
        CHECK(wstl::IsStandardLayout<A>::Value);
        CHECK_FALSE(wstl::IsStandardLayout<BBaseA>::Value);
        CHECK_FALSE(wstl::IsStandardLayout<VirtualFunction>::Value);
    }

    TEST_CASE("IsObject") {
        CHECK(wstl::IsObject<int>::Value);
        CHECK(wstl::IsObject<int*>::Value);
        CHECK(wstl::IsObject<TestData>::Value);
        CHECK(wstl::IsObject<TestData>::Value);
        CHECK(wstl::IsObject<int(*)()>::Value);
        CHECK_FALSE(wstl::IsObject<void>::Value);
        CHECK_FALSE(wstl::IsObject<int&>::Value);
        CHECK_FALSE(wstl::IsObject<int*&>::Value);
        CHECK_FALSE(wstl::IsObject<TestData&>::Value);
        CHECK_FALSE(wstl::IsObject<int()>::Value);
        CHECK_FALSE(wstl::IsObject<int(&)()>::Value);
    }

    TEST_CASE("IsConstructible") {
        CHECK_EQ(wstl::IsConstructible<TestData>::Value, std::is_constructible<TestData>::value);
        CHECK_EQ(wstl::IsConstructible<NonTrivialData>::Value, std::is_constructible<NonTrivialData>::value);
        CHECK_EQ(wstl::IsConstructible<Implicit, int>::Value, std::is_constructible<Implicit, int>::value);
        CHECK_EQ(wstl::IsConstructible<NonTrivialData, int, int>::Value, std::is_constructible<NonTrivialData, int, int>::value);
        CHECK_EQ(wstl::IsConstructible<const int&, int>::Value, std::is_constructible<const int&, int>::value);
        CHECK_EQ(wstl::IsConstructible<Explicit>::Value, std::is_constructible<Explicit>::value);
        CHECK_EQ(wstl::IsConstructible<Implicit>::Value, std::is_constructible<Implicit>::value);
        CHECK_EQ(wstl::IsConstructible<NonTrivialData, int>::Value, std::is_constructible<NonTrivialData, int>::value);
        CHECK_EQ(wstl::IsConstructible<PrivateDefaultConstructor>::Value, std::is_constructible<PrivateDefaultConstructor>::value);
        CHECK_EQ(wstl::IsConstructible<int&, int>::Value, std::is_constructible<int&, int>::value);
        CHECK_EQ(wstl::IsConstructible<void>::Value, std::is_constructible<void>::value);
    }

    TEST_CASE("IsTriviallyConstructible") {
        CHECK_EQ(wstl::IsTriviallyConstructible<int>::Value, std::is_trivially_constructible<int>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<A>::Value, std::is_trivially_constructible<A>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<TestData>::Value, std::is_trivially_constructible<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<void>::Value, std::is_trivially_constructible<void>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<NonTrivialData>::Value, std::is_trivially_constructible<NonTrivialData>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<Implicit>::Value, std::is_trivially_constructible<Implicit>::value);
        CHECK_EQ(wstl::IsTriviallyConstructible<Explicit>::Value, std::is_trivially_constructible<Explicit>::value);
    }

    TEST_CASE("IsNothrowConstructible") {
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
    }

    TEST_CASE("IsDefaultConstructible") {
        CHECK_EQ(wstl::IsDefaultConstructible<TestData>::Value, std::is_default_constructible<TestData>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<int>::Value, std::is_default_constructible<int>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<NonTrivialConstructor>::Value, std::is_default_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<PrivateDefaultConstructor>::Value, std::is_default_constructible<PrivateDefaultConstructor>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<void>::Value, std::is_default_constructible<void>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<Implicit>::Value, std::is_default_constructible<Implicit>::value);
        CHECK_EQ(wstl::IsDefaultConstructible<Explicit>::Value, std::is_default_constructible<Explicit>::value);
    }

    TEST_CASE("IsImplicitlyDefaultConstructible") {
        CHECK(wstl::IsImplicitlyDefaultConstructible<int>::Value);
        CHECK(wstl::IsImplicitlyDefaultConstructible<TestData>::Value);
        CHECK(wstl::IsImplicitlyDefaultConstructible<NonTrivialConstructor>::Value);
        CHECK_FALSE(wstl::IsImplicitlyDefaultConstructible<ExplicitDefault>::Value);
        CHECK_FALSE(wstl::IsImplicitlyDefaultConstructible<Implicit>::Value);
        CHECK_FALSE(wstl::IsImplicitlyDefaultConstructible<Explicit>::Value);
        CHECK_FALSE(wstl::IsImplicitlyDefaultConstructible<PrivateDefaultConstructor>::Value);
    }

    TEST_CASE("IsTriviallyDefaultConstructible") {
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<int>::Value, std::is_trivially_default_constructible<int>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<TestData>::Value, std::is_trivially_default_constructible<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<NonTrivialConstructor>::Value, std::is_trivially_default_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<ExplicitDefault>::Value, std::is_trivially_default_constructible<ExplicitDefault>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<Implicit>::Value, std::is_trivially_default_constructible<Implicit>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<Explicit>::Value, std::is_trivially_default_constructible<Explicit>::value);
        CHECK_EQ(wstl::IsTriviallyDefaultConstructible<PrivateDefaultConstructor>::Value, std::is_trivially_default_constructible<PrivateDefaultConstructor>::value);
    }

    TEST_CASE("IsNothrowDefaultConstructible") {
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<int>::Value, std::is_nothrow_default_constructible<int>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<TestData>::Value, std::is_nothrow_default_constructible<TestData>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<NothrowData>::Value, std::is_nothrow_default_constructible<NothrowData>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<NonTrivialConstructor>::Value, std::is_nothrow_default_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<ExplicitDefault>::Value, std::is_nothrow_default_constructible<ExplicitDefault>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<Implicit>::Value, std::is_nothrow_default_constructible<Implicit>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<Explicit>::Value, std::is_nothrow_default_constructible<Explicit>::value);
        CHECK_EQ(wstl::IsNothrowDefaultConstructible<PrivateDefaultConstructor>::Value, std::is_nothrow_default_constructible<PrivateDefaultConstructor>::value);
    }

    TEST_CASE("IsCopyConstructible") {
        CHECK_EQ(wstl::IsCopyConstructible<int>::Value, std::is_copy_constructible<int>::value);
        CHECK_EQ(wstl::IsCopyConstructible<TestData>::Value, std::is_copy_constructible<TestData>::value);
        CHECK_EQ(wstl::IsCopyConstructible<NonTrivialConstructor>::Value, std::is_copy_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsCopyConstructible<CustomCopyMoveConstructor>::Value, std::is_copy_constructible<CustomCopyMoveConstructor>::value);
        CHECK_EQ(wstl::IsCopyConstructible<NoCopyConstructor>::Value, std::is_copy_constructible<NoCopyConstructor>::value);
        CHECK_EQ(wstl::IsCopyConstructible<FakeCopyConstructor>::Value, std::is_copy_constructible<FakeCopyConstructor>::value);
    }

    TEST_CASE("IsTriviallyCopyConstructible") {
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<int>::Value, std::is_trivially_copy_constructible<int>::value);
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<TestData>::Value, std::is_trivially_copy_constructible<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<NonTrivialConstructor>::Value, std::is_trivially_copy_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<CustomCopyMoveConstructor>::Value, std::is_trivially_copy_constructible<CustomCopyMoveConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<NoCopyConstructor>::Value, std::is_trivially_copy_constructible<NoCopyConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyConstructible<FakeCopyConstructor>::Value, std::is_trivially_copy_constructible<FakeCopyConstructor>::value);
    }

    TEST_CASE("IsNothrowCopyConstructible") {
        CHECK_EQ(wstl::IsNothrowCopyConstructible<int>::Value, std::is_nothrow_copy_constructible<int>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<TestData>::Value, std::is_nothrow_copy_constructible<TestData>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<NonTrivialConstructor>::Value, std::is_nothrow_copy_constructible<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<NothrowData>::Value, std::is_nothrow_copy_constructible<NothrowData>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<CustomCopyMoveConstructor>::Value, std::is_nothrow_copy_constructible<CustomCopyMoveConstructor>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<NoCopyConstructor>::Value, std::is_nothrow_copy_constructible<NoCopyConstructor>::value);
        CHECK_EQ(wstl::IsNothrowCopyConstructible<FakeCopyConstructor>::Value, std::is_nothrow_copy_constructible<FakeCopyConstructor>::value);
    }

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

    TEST_CASE("IsAssignable") {
        CHECK_EQ(wstl::IsAssignable<int, int>::Value, std::is_assignable<int, int>::value);
        CHECK_EQ(wstl::IsAssignable<int&, int>::Value, std::is_assignable<int&, int>::value);
        CHECK_EQ(wstl::IsAssignable<int&, double>::Value, std::is_assignable<int&, double>::value);
        CHECK_EQ(wstl::IsAssignable<NothrowData&, NothrowData>::Value, std::is_assignable<NothrowData&, NothrowData>::value);
        CHECK_EQ(wstl::IsAssignable<TestData&, TestData>::Value, std::is_assignable<TestData&, TestData>::value);
        CHECK_EQ(wstl::IsAssignable<FakeCopyAssignment&, FakeCopyAssignment>::Value, std::is_assignable<FakeCopyAssignment&, FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsAssignable<FakeCopyConstructor&, FakeCopyConstructor>::Value, std::is_assignable<FakeCopyConstructor&, FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsAssignable<NoCopyAssignment&, NoCopyAssignment>::Value, std::is_assignable<NoCopyAssignment&, NoCopyAssignment>::value);
    }

    TEST_CASE("IsTriviallyAssignable") {
        CHECK_EQ(wstl::IsTriviallyAssignable<int, int>::Value, std::is_trivially_assignable<int, int>::value);
        CHECK_EQ(wstl::IsTriviallyAssignable<int&, int>::Value, std::is_trivially_assignable<int&, int>::value);
        CHECK_EQ(wstl::IsTriviallyAssignable<int&, double>::Value, std::is_trivially_assignable<int&, double>::value);
        CHECK_EQ(wstl::IsTriviallyAssignable<NothrowData&, NothrowData>::Value, std::is_trivially_assignable<NothrowData&, NothrowData>::value);
        CHECK_EQ(wstl::IsTriviallyAssignable<TestData&, TestData>::Value, std::is_trivially_assignable<TestData&, TestData>::value);
        CHECK_EQ(wstl::IsTriviallyAssignable<FakeCopyAssignment&, FakeCopyAssignment>::Value, std::is_trivially_assignable<FakeCopyAssignment&, FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsTriviallyAssignable<FakeCopyConstructor&, FakeCopyConstructor>::Value, std::is_trivially_assignable<FakeCopyConstructor&, FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyAssignable<NoCopyAssignment&, NoCopyAssignment>::Value, std::is_trivially_assignable<NoCopyAssignment&, NoCopyAssignment>::value);
    }

    TEST_CASE("IsNothrowAssignable") {
        CHECK_EQ(wstl::IsNothrowAssignable<int, int>::Value, std::is_nothrow_assignable<int, int>::value);
        CHECK_EQ(wstl::IsNothrowAssignable<int&, int>::Value, std::is_nothrow_assignable<int&, int>::value);
        CHECK_EQ(wstl::IsNothrowAssignable<int&, double>::Value, std::is_nothrow_assignable<int&, double>::value);
        CHECK_EQ(wstl::IsNothrowAssignable<NothrowData&, NothrowData>::Value, std::is_nothrow_assignable<NothrowData&, NothrowData>::value);
        CHECK_EQ(wstl::IsNothrowAssignable<TestData&, TestData>::Value, std::is_nothrow_assignable<TestData&, TestData>::value);
        CHECK_EQ(wstl::IsNothrowAssignable<FakeCopyAssignment&, FakeCopyAssignment>::Value, std::is_nothrow_assignable<FakeCopyAssignment&, FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsNothrowAssignable<FakeCopyConstructor&, FakeCopyConstructor>::Value, std::is_nothrow_assignable<FakeCopyConstructor&, FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsNothrowAssignable<NoCopyAssignment&, NoCopyAssignment>::Value, std::is_nothrow_assignable<NoCopyAssignment&, NoCopyAssignment>::value);
    }

    TEST_CASE("IsCopyAssignable") {
        CHECK_EQ(wstl::IsCopyAssignable<int>::Value, std::is_copy_assignable<int>::value);
        CHECK_EQ(wstl::IsCopyAssignable<NothrowData>::Value, std::is_copy_assignable<NothrowData>::value);
        CHECK_EQ(wstl::IsCopyAssignable<TestData>::Value, std::is_copy_assignable<TestData>::value);
        CHECK_EQ(wstl::IsCopyAssignable<FakeCopyAssignment>::Value, std::is_copy_assignable<FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsCopyAssignable<FakeCopyConstructor>::Value, std::is_copy_assignable<FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsCopyAssignable<TrivialConstructor>::Value, std::is_copy_assignable<TrivialConstructor>::value);
        CHECK_EQ(wstl::IsCopyAssignable<NoCopyAssignment>::Value, std::is_copy_assignable<NoCopyAssignment>::value);
    }

    TEST_CASE("IsTriviallyCopyAssignable") {
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<int>::Value, std::is_trivially_copy_assignable<int>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<NothrowData>::Value, std::is_trivially_copy_assignable<NothrowData>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<TestData>::Value, std::is_trivially_copy_assignable<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<FakeCopyAssignment>::Value, std::is_trivially_copy_assignable<FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<FakeCopyConstructor>::Value, std::is_trivially_copy_assignable<FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<TrivialConstructor>::Value, std::is_trivially_copy_assignable<TrivialConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyAssignable<NoCopyAssignment>::Value, std::is_trivially_copy_assignable<NoCopyAssignment>::value);
    }

    TEST_CASE("IsNothrowCopyAssignable") {
        CHECK_EQ(wstl::IsNothrowCopyAssignable<int>::Value, std::is_nothrow_copy_assignable<int>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<NothrowData>::Value, std::is_nothrow_copy_assignable<NothrowData>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<TestData>::Value, std::is_nothrow_copy_assignable<TestData>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<FakeCopyAssignment>::Value, std::is_nothrow_copy_assignable<FakeCopyAssignment>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<FakeCopyConstructor>::Value, std::is_nothrow_copy_assignable<FakeCopyConstructor>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<TrivialConstructor>::Value, std::is_nothrow_copy_assignable<TrivialConstructor>::value);
        CHECK_EQ(wstl::IsNothrowCopyAssignable<NoCopyAssignment>::Value, std::is_nothrow_copy_assignable<NoCopyAssignment>::value);
    }

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

    TEST_CASE("IsDestructible") {
        CHECK_EQ(wstl::IsDestructible<int>::Value, std::is_destructible<int>::value);
        CHECK_EQ(wstl::IsDestructible<int&>::Value, std::is_destructible<int&>::value);
        CHECK_EQ(wstl::IsDestructible<TestData>::Value, std::is_destructible<TestData>::value);
        CHECK_EQ(wstl::IsDestructible<NothrowData>::Value, std::is_destructible<NothrowData>::value);
        CHECK_EQ(wstl::IsDestructible<NoDestructor>::Value, std::is_destructible<NoDestructor>::value);
        CHECK_EQ(wstl::IsDestructible<void>::Value, std::is_destructible<void>::value);
        CHECK_EQ(wstl::IsDestructible<Abstract>::Value, std::is_destructible<Abstract>::value);
        CHECK_EQ(wstl::IsDestructible<PrivateDestructor>::Value, std::is_destructible<PrivateDestructor>::value);
    }

    TEST_CASE("IsTriviallyDestructible") {
        CHECK_EQ(wstl::IsTriviallyDestructible<int>::Value, std::is_trivially_destructible<int>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<int&>::Value, std::is_trivially_destructible<int&>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<TestData>::Value, std::is_trivially_destructible<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<NothrowData>::Value, std::is_trivially_destructible<NothrowData>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<NoDestructor>::Value, std::is_trivially_destructible<NoDestructor>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<void>::Value, std::is_trivially_destructible<void>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<Abstract>::Value, std::is_trivially_destructible<Abstract>::value);
        CHECK_EQ(wstl::IsTriviallyDestructible<PrivateDestructor>::Value, std::is_trivially_destructible<PrivateDestructor>::value);
    }

    TEST_CASE("IsNothrowDestructible") {
        CHECK_EQ(wstl::IsNothrowDestructible<int>::Value, std::is_nothrow_destructible<int>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<int&>::Value, std::is_nothrow_destructible<int&>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<TestData>::Value, std::is_nothrow_destructible<TestData>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<NothrowData>::Value, std::is_nothrow_destructible<NothrowData>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<NoDestructor>::Value, std::is_nothrow_destructible<NoDestructor>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<void>::Value, std::is_nothrow_destructible<void>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<Abstract>::Value, std::is_nothrow_destructible<Abstract>::value);
        CHECK_EQ(wstl::IsNothrowDestructible<PrivateDestructor>::Value, std::is_nothrow_destructible<PrivateDestructor>::value);
    }

    TEST_CASE("IsTriviallyCopyable") {
        CHECK_EQ(wstl::IsTriviallyCopyable<int>::Value, std::is_trivially_copyable<int>::value);
        CHECK_EQ(wstl::IsTriviallyCopyable<TestData>::Value, std::is_trivially_copyable<TestData>::value);
        CHECK_EQ(wstl::IsTriviallyCopyable<NonTrivialConstructor>::Value, std::is_trivially_copyable<NonTrivialConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyable<CustomCopyMoveConstructor>::Value, std::is_trivially_copyable<CustomCopyMoveConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyable<NoCopyConstructor>::Value, std::is_trivially_copyable<NoCopyConstructor>::value);
        CHECK_EQ(wstl::IsTriviallyCopyable<FakeCopyConstructor>::Value, std::is_trivially_copyable<FakeCopyConstructor>::value);
    }

    TEST_CASE("IsSigned") {
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

        #ifdef __WSTL_CXX20__
        CHECK_EQ(wstl::IsSigned<char8_t>::Value, std::is_signed<char8_t>::value);
        #endif
    }

    TEST_CASE("IsUnsigned") {
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

        CHECK_EQ(wstl::IsUnsigned<char16_t>::Value, std::is_unsigned<char16_t>::value);
        CHECK_EQ(wstl::IsUnsigned<char32_t>::Value, std::is_unsigned<char32_t>::value);

        #ifdef __WSTL_CXX20__
        CHECK_EQ(wstl::IsUnsigned<char8_t>::Value, std::is_unsigned<char8_t>::value);
        #endif
    }

    TEST_CASE("Decay") {
        CHECK(wstl::IsSame<wstl::Decay<int>::Type, int>::Value);
        CHECK_FALSE(wstl::IsSame<wstl::Decay<int>::Type, float>::Value);
        CHECK(wstl::IsSame<wstl::Decay<int&>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::Decay<int&&>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::Decay<const int&>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::Decay<int[2]>::Type, int*>::Value);
        CHECK_FALSE(wstl::IsSame<wstl::Decay<int[4][2]>::Type, int*>::Value);
        CHECK_FALSE(wstl::IsSame<wstl::Decay<int[4][2]>::Type, int**>::Value);
        CHECK(wstl::IsSame<wstl::Decay<int[4][2]>::Type, int(*)[2]>::Value);
        CHECK(wstl::IsSame<wstl::Decay<int(int)>::Type, int(*)(int)>::Value);
    }

    TEST_CASE("UnwrapReference") {
        CHECK(wstl::IsSame<wstl::UnwrapReference<int>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::UnwrapReference<const int>::Type, const int>::Value);
        CHECK(wstl::IsSame<wstl::UnwrapReference<int&>::Type, int&>::Value);
        CHECK(wstl::IsSame<wstl::UnwrapReference<int&&>::Type, int&&>::Value);
        CHECK(wstl::IsSame<wstl::UnwrapReference<int*>::Type, int*>::Value);
    }

    TEST_CASE("UnwrapReferenceDecay") {
        CHECK(wstl::IsSame<wstl::UnwrapReferenceDecay<int>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::UnwrapReferenceDecay<const int>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::UnwrapReferenceDecay<int&>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::UnwrapReferenceDecay<int&&>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::UnwrapReferenceDecay<int*>::Type, int*>::Value);
    }

    TEST_CASE("CommonType") {
        CHECK(wstl::IsSame<wstl::CommonType<int>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int, int>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int, float>::Type, float>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int, double>::Type, double>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int, const int>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int&, int>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int&, const int&>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int&&, int>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int&&, const int&>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::CommonType<int, char, double>::Type, double>::Value);
    }

    TEST_CASE("MakeSigned") {
        CHECK(wstl::IsSame<wstl::MakeSigned<char>::Type, std::make_signed<char>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<signed char>::Type, std::make_signed<signed char>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<unsigned char>::Type, std::make_signed<unsigned char>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<short>::Type, std::make_signed<short>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<signed short>::Type, std::make_signed<signed short>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<unsigned short>::Type, std::make_signed<unsigned short>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<int>::Type, std::make_signed<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<signed int>::Type, std::make_signed<signed int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<unsigned int>::Type, std::make_signed<unsigned int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<long>::Type, std::make_signed<long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<signed long>::Type, std::make_signed<signed long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<unsigned long>::Type, std::make_signed<unsigned long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<long long>::Type, std::make_signed<long long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<signed long long>::Type, std::make_signed<signed long long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<unsigned long long>::Type, std::make_signed<unsigned long long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<const unsigned int>::Type, std::make_signed<const unsigned int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<volatile unsigned int>::Type, std::make_signed<volatile unsigned int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<const unsigned int>::Type, std::make_signed<const unsigned int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<const volatile unsigned int>::Type, std::make_signed<const volatile unsigned int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<size_t>::Type, std::make_signed<size_t>::type>::Value);

        CHECK(wstl::IsSigned<wstl::MakeSigned<wchar_t>::Type>::Value);
        CHECK_EQ(sizeof(wchar_t), sizeof(wstl::MakeSigned<wchar_t>::Type));

        enum class UnsignedEnum : unsigned int {};
        enum class SignedEnum : int {};

        CHECK(wstl::IsSame<wstl::MakeSigned<std::underlying_type<UnsignedEnum>::type>::Type, std::make_signed<UnsignedEnum>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeSigned<std::underlying_type<SignedEnum>::type>::Type, std::make_signed<SignedEnum>::type>::Value);
    }

    TEST_CASE("MakeUnsigned") {
        CHECK(wstl::IsSame<wstl::MakeUnsigned<char>::Type, std::make_unsigned<char>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<signed char>::Type, std::make_unsigned<signed char>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<unsigned char>::Type, std::make_unsigned<unsigned char>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<short>::Type, std::make_unsigned<short>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<signed short>::Type, std::make_unsigned<signed short>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<unsigned short>::Type, std::make_unsigned<unsigned short>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<int>::Type, std::make_unsigned<int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<signed int>::Type, std::make_unsigned<signed int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<unsigned int>::Type, std::make_unsigned<unsigned int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<long>::Type, std::make_unsigned<long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<signed long>::Type, std::make_unsigned<signed long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<unsigned long>::Type, std::make_unsigned<unsigned long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<long long>::Type, std::make_unsigned<long long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<signed long long>::Type, std::make_unsigned<signed long long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<unsigned long long>::Type, std::make_unsigned<unsigned long long>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<const int>::Type, std::make_unsigned<const int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<volatile int>::Type, std::make_unsigned<volatile int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<const int>::Type, std::make_unsigned<const int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<const volatile int>::Type, std::make_unsigned<const volatile int>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<size_t>::Type, std::make_unsigned<size_t>::type>::Value);

        CHECK(wstl::IsUnsigned<wstl::MakeUnsigned<wchar_t>::Type>::Value);
        CHECK_EQ(sizeof(wchar_t), sizeof(wstl::MakeUnsigned<wchar_t>::Type));

        enum class UnsignedEnum : unsigned int {};
        enum class SignedEnum : int {};

        CHECK(wstl::IsSame<wstl::MakeUnsigned<std::underlying_type<UnsignedEnum>::type>::Type, std::make_unsigned<UnsignedEnum>::type>::Value);
        CHECK(wstl::IsSame<wstl::MakeUnsigned<std::underlying_type<SignedEnum>::type>::Type, std::make_unsigned<SignedEnum>::type>::Value);
    }

    TEST_CASE("TypeWithAlignment") {
        CHECK_EQ(alignof(wstl::TypeWithAlignmentType<1UL>), 1UL);
        CHECK_EQ(alignof(wstl::TypeWithAlignmentType<2UL>), 2UL);
        CHECK_EQ(alignof(wstl::TypeWithAlignmentType<4UL>), 4UL);
        CHECK_EQ(alignof(wstl::TypeWithAlignmentType<8UL>), 8UL);
        CHECK_EQ(alignof(wstl::TypeWithAlignmentType<16UL>), 16UL);
        CHECK_EQ(alignof(wstl::TypeWithAlignmentType<32UL>), 32UL);
        CHECK_EQ(alignof(wstl::TypeWithAlignmentType<64UL>), 64UL);
    }

    TEST_CASE("IsTypeAligned") {
        CHECK(wstl::IsTypeAligned<char, 1>::Value);
        CHECK(wstl::IsTypeAligned<int, 4>::Value);
        CHECK(wstl::IsTypeAligned<double, 8>::Value);
        CHECK_FALSE(wstl::IsTypeAligned<double, 1>::Value);
        CHECK_FALSE(wstl::IsTypeAligned<int, 1>::Value);
        CHECK_FALSE(wstl::IsTypeAligned<int, 2>::Value);
    }

    TEST_CASE("AlignedStorage") {
        StorageType data[10];

        size_t alignment = wstl::AlignmentOf<StorageType>::Value;
        size_t expected = std::alignment_of<uint32_t>::value;
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
        alignas(uint32_t) const char cdata[2 * sizeof(uint32_t)] = {0, 1};
        const char* cptr = cdata;

        alignas(uint32_t) char data[2 * sizeof(uint32_t)];
        char* ptr = data;

        CHECK(wstl::IsAligned(ptr, wstl::AlignmentOf<uint32_t>::Value));
        CHECK(wstl::IsAligned<alignof(uint32_t)>(ptr));
        CHECK(wstl::IsAligned<uint32_t>(ptr));
        CHECK(wstl::IsAligned(cptr, wstl::AlignmentOf<const uint32_t>::Value));
        CHECK(wstl::IsAligned<alignof(const uint32_t)>(cptr));
        CHECK(wstl::IsAligned<const uint32_t>(cptr));

        ++ptr;
        ++cptr;
        CHECK_FALSE(wstl::IsAligned(ptr, wstl::AlignmentOf<uint32_t>::Value));
        CHECK_FALSE(wstl::IsAligned<alignof(uint32_t)>(ptr));
        CHECK_FALSE(wstl::IsAligned<uint32_t>(ptr));
        CHECK_FALSE(wstl::IsAligned(cptr, wstl::AlignmentOf<const uint32_t>::Value));
        CHECK_FALSE(wstl::IsAligned<alignof(const uint32_t)>(cptr));
        CHECK_FALSE(wstl::IsAligned<const uint32_t>(cptr));
    }

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
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(Free0)>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(Free1), int>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(Free2), int, char>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(Free3), int, char, double>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(Free0t<TestData>)>::Type, TestData>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(FreeNoexcept), char>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(FreeVariadic), int, float>::Type, long>::Value);

        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(&MemberFunction::Fn0), MemberFunction*>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(&MemberFunction::Fn0), MemberFunction>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(&MemberFunction::Fn0c), const MemberFunction>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(&MemberFunction::Fn0v), volatile MemberFunction>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(&MemberFunction::Fn0cv), const volatile MemberFunction>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(&MemberFunction::FnRefOnly), MemberFunction&, char>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(&MemberFunction::FnRRefOnly), MemberFunction, char>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(&MemberFunction::FnStatic), int>::Type, long>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(&MemberFunction::FnNoexcept), MemberFunction, char>::Type, int>::Value);

        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(&A::M), A&>::Type, int&>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<decltype(&A::M), A*>::Type, int&>::Value);

        CHECK(wstl::IsSame<wstl::InvokeResult<Functor0>::Type, int>::Value);
        CHECK(wstl::IsSame<wstl::InvokeResult<Functor2, int, char>::Type, long>::Value);
    }

    TEST_CASE("IsInvocable") {
        CHECK(wstl::IsInvocable<decltype(Free0)>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(Free0), float>::Value);
        CHECK(wstl::IsInvocable<decltype(Free1), int>::Value);
        CHECK(wstl::IsInvocable<decltype(Free1), long>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(Free1)>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(Free1), TestData>::Value);
        CHECK(wstl::IsInvocable<decltype(Free2), int, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(Free2), int>::Value);
        CHECK(wstl::IsInvocable<decltype(Free3), int, char, double>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(Free3), int, char, double, float>::Value);
        CHECK(wstl::IsInvocable<decltype(Free0t<TestData>)>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(Free0t<TestData>), int>::Value);
        CHECK(wstl::IsInvocable<decltype(FreeNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(FreeNoexcept), TestData>::Value);
        CHECK(wstl::IsInvocable<decltype(FreeVariadic), int, float>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(FreeVariadic), TestData>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(FreeVariadic)>::Value);

        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0), MemberFunction>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0), MemberFunction*>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::Fn0), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::Fn0)>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::Fn0), const MemberFunction>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0c), MemberFunction>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0c), const MemberFunction*>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0c), const MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::Fn0c), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::Fn0c)>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::Fn0c), volatile MemberFunction>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0v), MemberFunction>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0v), volatile MemberFunction*>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0v), volatile MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::Fn0v), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::Fn0v)>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::Fn0v), const MemberFunction>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0cv), MemberFunction>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0cv), const MemberFunction*>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::Fn0cv), volatile MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::Fn0cv), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::Fn0cv)>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::FnRefOnly), MemberFunction&, char>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::FnRefOnly), MemberFunction*, char>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::FnRefOnly), MemberFunction&, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRefOnly), MemberFunction&, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRefOnly), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRefOnly), const MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRefOnly), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRefOnly)>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRefOnly), MemberFunction&, TestData>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::FnRRefOnly), MemberFunction, char>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::FnRRefOnly), MemberFunction&&, char>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::FnRRefOnly), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRRefOnly), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRRefOnly), MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRRefOnly), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRRefOnly), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRRefOnly)>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnRRefOnly), MemberFunction, TestData>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::FnStatic), int>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::FnStatic), char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnStatic)>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnStatic), TestData>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::FnNoexcept), MemberFunction, char>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::FnNoexcept), MemberFunction*, double>::Value);
        CHECK(wstl::IsInvocable<decltype(&MemberFunction::FnNoexcept), MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnNoexcept), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnNoexcept), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnNoexcept), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnNoexcept)>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&MemberFunction::FnNoexcept), MemberFunction, TestData>::Value);

        CHECK(wstl::IsInvocable<decltype(&A::M), A&>::Value);
        CHECK(wstl::IsInvocable<decltype(&A::M), A>::Value);
        CHECK(wstl::IsInvocable<decltype(&A::M), A*>::Value);
        CHECK_FALSE(wstl::IsInvocable<decltype(&A::M)>::Value);

        CHECK(wstl::IsInvocable<Functor0>::Value);
        CHECK_FALSE(wstl::IsInvocable<Functor0, int>::Value);
        CHECK(wstl::IsInvocable<Functor2, int, char>::Value);
        CHECK_FALSE(wstl::IsInvocable<Functor2>::Value);
        CHECK_FALSE(wstl::IsInvocable<Functor2, int, TestData>::Value);
        CHECK_FALSE(wstl::IsInvocable<Functor2, int>::Value);
    }

    TEST_CASE("IsInvocableReturn") {
        CHECK(wstl::IsInvocableReturn<void, decltype(FreeVoid), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, decltype(FreeVoid), TestData>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, decltype(FreeVoid)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(FreeVoid), int>::Value);

        CHECK(wstl::IsInvocableReturn<int, decltype(Free0)>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(Free0)>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(Free0)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(Free0), float>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(Free0)>::Value);

        CHECK(wstl::IsInvocableReturn<int, decltype(Free1), int>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(Free1), long>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(Free1), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(Free1)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(Free1), TestData>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(Free1), int>::Value);
        CHECK(wstl::IsInvocableReturn<int, decltype(Free2), int, char>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(Free2), int, char>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(Free2), int, long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(Free2), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(Free2), int, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(Free2), int, TestData>::Value);
        CHECK(wstl::IsInvocableReturn<int, decltype(Free3), int, char, double>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(Free3), int, char, double>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(Free3), int, char, float>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(Free3), int, char, double, float>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(Free3), int, char, double>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(Free3), int, char, TestData>::Value);
        CHECK(wstl::IsInvocableReturn<TestData, decltype(Free0t<TestData>)>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(Free0t<TestData>)>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(Free0t<int>)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(Free0t<TestData>), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(Free0t<TestData>), int>::Value);
        CHECK(wstl::IsInvocableReturn<int, decltype(FreeNoexcept), char>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(FreeNoexcept), char>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(FreeNoexcept), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(FreeNoexcept), char, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(FreeNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(FreeNoexcept), TestData>::Value);
        CHECK(wstl::IsInvocableReturn<long, decltype(FreeVariadic), int>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(FreeVariadic), int, float>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(FreeVariadic), int, double, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<long, decltype(FreeVariadic)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(FreeVariadic), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<long, decltype(FreeVariadic), TestData>::Value);

        CHECK(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0), MemberFunction>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(&MemberFunction::Fn0), MemberFunction*>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(&MemberFunction::Fn0), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0), const MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(&MemberFunction::Fn0)>::Value);
        CHECK(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0c), MemberFunction>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(&MemberFunction::Fn0c), const MemberFunction*>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(&MemberFunction::Fn0c), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0c), volatile MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0c), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0c), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(&MemberFunction::Fn0c)>::Value);
        CHECK(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0v), MemberFunction>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(&MemberFunction::Fn0v), volatile MemberFunction*>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(&MemberFunction::Fn0v), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0v), const MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0v), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0v), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(&MemberFunction::Fn0v)>::Value);
        CHECK(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0cv), MemberFunction>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(&MemberFunction::Fn0cv), const MemberFunction*>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(&MemberFunction::Fn0cv), volatile MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0cv), MemberFunction, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::Fn0cv), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(&MemberFunction::Fn0cv)>::Value);
        CHECK(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRefOnly), MemberFunction&, char>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(&MemberFunction::FnRefOnly), MemberFunction*, char>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(&MemberFunction::FnRefOnly), MemberFunction&, long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRefOnly), MemberFunction&, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRefOnly), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRefOnly), const MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRefOnly), MemberFunction&>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRefOnly), char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(&MemberFunction::FnRefOnly), MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRefOnly), MemberFunction&, TestData>::Value);
        CHECK(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRRefOnly), MemberFunction, char>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(&MemberFunction::FnRRefOnly), MemberFunction&&, char>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(&MemberFunction::FnRRefOnly), MemberFunction, long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRRefOnly), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRRefOnly), MemberFunction&, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRRefOnly), MemberFunction*, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRRefOnly), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRRefOnly), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRRefOnly), char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(&MemberFunction::FnRRefOnly), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnRRefOnly), MemberFunction, TestData>::Value);
        CHECK(wstl::IsInvocableReturn<long, decltype(&MemberFunction::FnStatic), int>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(&MemberFunction::FnStatic), int>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(&MemberFunction::FnStatic), long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<long, decltype(&MemberFunction::FnStatic)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(&MemberFunction::FnStatic), int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<long, decltype(&MemberFunction::FnStatic), TestData>::Value);
        CHECK(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnNoexcept), MemberFunction, char>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(&MemberFunction::FnNoexcept), MemberFunction*, char>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(&MemberFunction::FnNoexcept), MemberFunction&, long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnNoexcept), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnNoexcept), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnNoexcept), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnNoexcept), char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(&MemberFunction::FnNoexcept), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::FnNoexcept), MemberFunction, TestData>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(&MemberFunction::VoidFn), MemberFunction*, char>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(&MemberFunction::VoidFn), MemberFunction&, long>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, decltype(&MemberFunction::VoidFn), MemberFunction, char, int>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, decltype(&MemberFunction::VoidFn), const MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, decltype(&MemberFunction::VoidFn), MemberFunction>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, decltype(&MemberFunction::VoidFn), char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&MemberFunction::VoidFn), MemberFunction, char>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<void, decltype(&MemberFunction::VoidFn), MemberFunction, TestData>::Value);

        CHECK(wstl::IsInvocableReturn<int, decltype(&A::M), A&>::Value);
        CHECK(wstl::IsInvocableReturn<void, decltype(&A::M), A>::Value);
        CHECK(wstl::IsInvocableReturn<char, decltype(&A::M), A*>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<int, decltype(&A::M)>::Value);
        CHECK_FALSE(wstl::IsInvocableReturn<TestData, decltype(&A::M), A>::Value);

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

    #if !defined(__WSTL_TYPETRAITS_NO_BUILTINS__) && defined(__WSTL_SUPPORTED_COMPILER__)
    TEST_CASE("UnderlyingType") {
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
    }
    #endif
}