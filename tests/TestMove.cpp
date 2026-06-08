// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/private/Move.hpp>
#include <utility>

#include "Utils.hpp"


struct NonNoexceptMovable {
    NonNoexceptMovable() {}
    NonNoexceptMovable(const NonNoexceptMovable&) {}
    NonNoexceptMovable(NonNoexceptMovable&&) {}
};

enum class ForwardTestType {
    LValue,
    RValue,
    ConstLValue,
    ConstRValue
};

ForwardTestType GetForwardType(Dummy&) { return ForwardTestType::LValue; }
ForwardTestType GetForwardType(Dummy&&) { return ForwardTestType::RValue; }
ForwardTestType GetForwardType(const Dummy&) { return ForwardTestType::ConstLValue; }
ForwardTestType GetForwardType(const Dummy&&) { return ForwardTestType::ConstRValue; }

template<typename T>
ForwardTestType TestForward(T&& t) {
    return GetForwardType(wstl::Forward<T>(t));
}

template<typename T, typename U>
ForwardTestType TestForwardLike(U&& u) {
    return GetForwardType(wstl::ForwardLike<T>(u));
}

TEST_SUITE("Utility") {
    TEST_CASE("Forward") {
        Dummy d;
        const Dummy cd;

        CHECK_EQ(TestForward(d), ForwardTestType::LValue);
        CHECK_EQ(TestForward(wstl::Move(d)), ForwardTestType::RValue);
        CHECK_EQ(TestForward(cd), ForwardTestType::ConstLValue);
        CHECK_EQ(TestForward(wstl::Move(cd)), ForwardTestType::ConstRValue);
    }

    TEST_CASE("Move") {
        CHECK(wstl::IsSame<decltype(wstl::Move(wstl::DeclareValue<int&>())), int&&>::Value);
        CHECK(wstl::IsSame<decltype(wstl::Move(wstl::DeclareValue<const int&>())), const int&&>::Value);
        CHECK(wstl::IsSame<decltype(wstl::Move(wstl::DeclareValue<int>())), int&&>::Value);
        CHECK(wstl::IsSame<decltype(wstl::Move(wstl::DeclareValue<const int>())), const int&&>::Value);
    }

    TEST_CASE("MoveIfNoexcept") {
        int x = 0;
        const int cx = 0;
        NonNoexceptMovable m;
        const NonNoexceptMovable cm;

        CHECK(wstl::IsSame<decltype(wstl::MoveIfNoexcept(x)), int&&>::Value);
        CHECK(wstl::IsSame<decltype(wstl::MoveIfNoexcept(cx)), const int&&>::Value);
        CHECK(wstl::IsSame<decltype(wstl::MoveIfNoexcept(m)), const NonNoexceptMovable&>::Value);
        CHECK(wstl::IsSame<decltype(wstl::MoveIfNoexcept(cm)), const NonNoexceptMovable&>::Value);
    }

    TEST_CASE("ForwardLike") {
        Dummy d;
        const Dummy cd;

        CHECK_EQ(TestForwardLike<Dummy2&>(d), ForwardTestType::LValue);
        CHECK_EQ(TestForwardLike<Dummy2&>(wstl::Move(d)), ForwardTestType::LValue);
        CHECK_EQ(TestForwardLike<Dummy2&>(cd), ForwardTestType::ConstLValue);
        CHECK_EQ(TestForwardLike<Dummy2&>(wstl::Move(cd)), ForwardTestType::ConstLValue);
        CHECK_EQ(TestForwardLike<const Dummy2&>(d), ForwardTestType::ConstLValue);
        CHECK_EQ(TestForwardLike<const Dummy2&>(wstl::Move(d)), ForwardTestType::ConstLValue);
        CHECK_EQ(TestForwardLike<const Dummy2&>(cd), ForwardTestType::ConstLValue);
        CHECK_EQ(TestForwardLike<const Dummy2&>(wstl::Move(cd)), ForwardTestType::ConstLValue);
        CHECK_EQ(TestForwardLike<Dummy2&&>(d), ForwardTestType::RValue);
        CHECK_EQ(TestForwardLike<Dummy2&&>(wstl::Move(d)), ForwardTestType::RValue);
        CHECK_EQ(TestForwardLike<Dummy2&&>(cd), ForwardTestType::ConstRValue);
        CHECK_EQ(TestForwardLike<Dummy2&&>(wstl::Move(cd)), ForwardTestType::ConstRValue);
        CHECK_EQ(TestForwardLike<const Dummy2&&>(d), ForwardTestType::ConstRValue);
        CHECK_EQ(TestForwardLike<const Dummy2&&>(wstl::Move(d)), ForwardTestType::ConstRValue);
        CHECK_EQ(TestForwardLike<const Dummy2&&>(cd), ForwardTestType::ConstRValue);
        CHECK_EQ(TestForwardLike<const Dummy2&&>(wstl::Move(cd)), ForwardTestType::ConstRValue);
    }
}