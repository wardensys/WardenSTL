// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Math.hpp>
#include <limits>


TEST_SUITE("Math") {
    TEST_CASE("Absolute") {
        CHECK_EQ(wstl::Absolute(123), 123);
        CHECK_EQ(wstl::Absolute(-123), 123);
        CHECK_EQ(wstl::Absolute(0), 0);
        CHECK_EQ(wstl::Absolute(69U), 69U);
        CHECK_EQ(wstl::Absolute(0.5), doctest::Approx(0.5).epsilon(0.001));
        CHECK_EQ(wstl::Absolute(-0.0), doctest::Approx(0.0).epsilon(0.001));
        CHECK_EQ(wstl::Absolute(-0.5), doctest::Approx(0.5).epsilon(0.001));

        constexpr int16_t a1 = wstl::Absolute(int16_t(0));
        constexpr int16_t a2 = wstl::Absolute(int16_t(32767));
        constexpr int16_t a3 = wstl::Absolute(int16_t(-32767));
        constexpr uint16_t a4 = wstl::AbsoluteUnsigned(int16_t(0));
        constexpr uint16_t a5 = wstl::AbsoluteUnsigned(int16_t(32767));
        constexpr uint16_t a6 = wstl::AbsoluteUnsigned(int16_t(-32767));

        // The following line should fail with a compilation error
        // constexpr int16_t a4 = wstl::Absolute(int16_t(-32768));

        CHECK_EQ(a1, 0);
        CHECK_EQ(a2, 32767);
        CHECK_EQ(a3, 32767);
        CHECK_EQ(a4, 0U);
        CHECK_EQ(a5, 32767U);
        CHECK_EQ(a6, 32767U);

        CHECK_THROWS_AS({ int i = wstl::Absolute(std::numeric_limits<int>::min()); (void) i; }, wstl::OverflowError);
    }

    TEST_CASE("AbsoluteUnsigned") {
        CHECK_EQ(wstl::AbsoluteUnsigned(123), 123U);
        CHECK_EQ(wstl::AbsoluteUnsigned(-123), 123U);
        CHECK_EQ(wstl::AbsoluteUnsigned(0), 0U);
        CHECK_EQ(wstl::AbsoluteUnsigned(69U), 69U);
        CHECK_EQ(wstl::AbsoluteUnsigned(std::numeric_limits<int>::min()), (uint32_t) std::numeric_limits<int>::max() + 1U);

        constexpr uint16_t a1 = wstl::AbsoluteUnsigned(int16_t(0));
        constexpr uint16_t a2 = wstl::AbsoluteUnsigned(int16_t(32767));
        constexpr uint16_t a3 = wstl::AbsoluteUnsigned(int16_t(-32767));
        constexpr uint16_t a4 = wstl::AbsoluteUnsigned(int16_t(-32768));

        CHECK_EQ(a1, 0U);
        CHECK_EQ(a2, 32767U);
        CHECK_EQ(a3, 32767U);
        CHECK_EQ(a4, 32768U);
    }

    TEST_CASE("Absolute compile-time version") {
        CHECK_EQ(wstl::compile::Absolute<int, 123>::Value, 123);
        CHECK_EQ(wstl::compile::Absolute<int, -123>::Value, 123);
        CHECK_EQ(wstl::compile::Absolute<int, 0>::Value, 0);
        CHECK_EQ(wstl::compile::Absolute<uint32_t, 69U>::Value, 69U);

        // The following line should fail with a compilation error
        // wstl::compile::Absolute<int, std::numeric_limits<int>::min()>::Value;
    }

    TEST_CASE("AbsoluteUnsigned compile-time version") {
        CHECK_EQ(wstl::compile::AbsoluteUnsigned<int, 123>::Value, 123U);
        CHECK_EQ(wstl::compile::AbsoluteUnsigned<int, -123>::Value, 123U);
        CHECK_EQ(wstl::compile::AbsoluteUnsigned<int, 0>::Value, 0U);
        CHECK_EQ(wstl::compile::AbsoluteUnsigned<uint32_t, 69U>::Value, 69U);
        CHECK_EQ(wstl::compile::AbsoluteUnsigned<int, std::numeric_limits<int>::min()>::Value, (uint32_t) std::numeric_limits<int>::max() + 1U);
    }

    TEST_CASE("Divide") {
        CHECK_EQ(wstl::Divide(11, 5).Quotient, 2);
        CHECK_EQ(wstl::Divide(11, 5).Remainder, 1);
        CHECK_EQ(wstl::Divide(12, 6).Quotient, 2);
        CHECK_EQ(wstl::Divide(12, 6).Remainder, 0);
        CHECK_EQ(wstl::Divide(0, 10).Quotient, 0);
        CHECK_EQ(wstl::Divide(0, 10).Remainder, 0);
    }

    TEST_CASE("Power") {
        CHECK_EQ(wstl::compile::Power<int, 2, 4>::Value, 16U);
        CHECK_EQ(wstl::compile::Power<int, 100, 0>::Value, 1U);
        CHECK_EQ(wstl::compile::Power<int, 0, 0>::Value, 0U);
        CHECK_EQ(wstl::compile::Power<int, 2, 1>::Value, 2U);
        CHECK_EQ(wstl::compile::Power<int, 3, 2>::Value, 9U);
        CHECK_EQ(wstl::compile::Power<int, 4, 3>::Value, 64U);
        CHECK_EQ(wstl::compile::Power<int, 5, 4>::Value, 625U);
        CHECK_EQ(wstl::compile::Power<int, 6, 5>::Value, 7776U); 
        CHECK_EQ(wstl::compile::Power<int, 7, 6>::Value, 117649UL);
        CHECK_EQ(wstl::compile::Power<int, 8, 7>::Value, 2097152UL);
        CHECK_EQ(wstl::compile::Power<int, 9, 8>::Value, 43046721UL);
        CHECK_EQ(wstl::compile::Power<int, 10, 9>::Value, 1000000000UL);
        CHECK_EQ(wstl::compile::Power<int, 2, 15>::Value, 0x8000U);
        CHECK_EQ(wstl::compile::Power<int, 2, 31>::Value, 0x80000000UL);
        CHECK_EQ(wstl::compile::Power<long long, 2, 63>::Value, 0x8000000000000000ULL);

        CHECK_EQ(wstl::compile::Power<int, 2, 4, int>::Value, 16);
        CHECK_EQ(wstl::compile::Power<int, 100, 0, int>::Value, 1);
        CHECK_EQ(wstl::compile::Power<int, 0, 0, int>::Value, 0);
        CHECK_EQ(wstl::compile::Power<int, 2, 1, int>::Value, 2);
        CHECK_EQ(wstl::compile::Power<int, 3, 2, int>::Value, 9);
        CHECK_EQ(wstl::compile::Power<int, 4, 3, int>::Value, 64);
        CHECK_EQ(wstl::compile::Power<int, 5, 4, int>::Value, 625);
        CHECK_EQ(wstl::compile::Power<int, 6, 5, int>::Value, 7776); 
        CHECK_EQ(wstl::compile::Power<int, 7, 6, int>::Value, 117649);
        CHECK_EQ(wstl::compile::Power<int, 8, 7, int>::Value, 2097152);
        CHECK_EQ(wstl::compile::Power<int, 9, 8, int>::Value, 43046721);
        CHECK_EQ(wstl::compile::Power<int, 10, 9, int>::Value, 1000000000);
        CHECK_EQ(wstl::compile::Power<int, 2, 15, int>::Value, 0x8000);
        CHECK_EQ(wstl::compile::Power<int, 2, 30, int>::Value, 0x40000000);
        CHECK_EQ(wstl::compile::Power<long long, 2, 62, long long>::Value, 0x4000000000000000);
        CHECK_EQ(wstl::compile::Power<int, -2, 4, int>::Value, 16);
        CHECK_EQ(wstl::compile::Power<int, -2, 3, int>::Value, -8);
    }

    TEST_CASE("Log") {
        CHECK_EQ(wstl::compile::Log<int, 0, 2>::Value, 0);
        CHECK_EQ(wstl::compile::Log<int, 0, 3>::Value, 0);
        CHECK_EQ(wstl::compile::Log<int, 0, 5>::Value, 0);
        CHECK_EQ(wstl::compile::Log<int, 1, 2>::Value, 0);
        CHECK_EQ(wstl::compile::Log<int, 1, 3>::Value, 0);
        CHECK_EQ(wstl::compile::Log<int, 1, 5>::Value, 0);
        CHECK_EQ(wstl::compile::Log<int, 2, 2>::Value, 1);
        CHECK_EQ(wstl::compile::Log<int, 3, 2>::Value, 1);
        CHECK_EQ(wstl::compile::Log<int, 4, 2>::Value, 2);
        CHECK_EQ(wstl::compile::Log<int, 5, 2>::Value, 2);
        CHECK_EQ(wstl::compile::Log<int, 6, 2>::Value, 2);
        CHECK_EQ(wstl::compile::Log<int, 7, 2>::Value, 2);
        CHECK_EQ(wstl::compile::Log<int, 8, 2>::Value, 3);
        CHECK_EQ(wstl::compile::Log<int, 9, 3>::Value, 2);
        CHECK_EQ(wstl::compile::Log<int, 125, 5>::Value, 3);

        CHECK_EQ(wstl::compile::Log<uint32_t, 0, 2>::Value, 0U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 0, 5>::Value, 0U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 0, 3>::Value, 0U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 1, 2>::Value, 0U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 1, 3>::Value, 0U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 1, 5>::Value, 0U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 2, 2>::Value, 1U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 3, 2>::Value, 1U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 4, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 5, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 6, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 7, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 8, 2>::Value, 3U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 9, 3>::Value, 2U);
        CHECK_EQ(wstl::compile::Log<uint32_t, 125, 5>::Value, 3U);
    }

    TEST_CASE("LogCeil") {
        CHECK_EQ(wstl::compile::LogCeil<int, 0, 2>::Value, 0);
        CHECK_EQ(wstl::compile::LogCeil<int, 0, 3>::Value, 0);
        CHECK_EQ(wstl::compile::LogCeil<int, 0, 5>::Value, 0);
        CHECK_EQ(wstl::compile::LogCeil<int, 1, 2>::Value, 0);
        CHECK_EQ(wstl::compile::LogCeil<int, 1, 3>::Value, 0);
        CHECK_EQ(wstl::compile::LogCeil<int, 1, 5>::Value, 0);
        CHECK_EQ(wstl::compile::LogCeil<int, 2, 2>::Value, 1);
        CHECK_EQ(wstl::compile::LogCeil<int, 3, 2>::Value, 2);
        CHECK_EQ(wstl::compile::LogCeil<int, 4, 2>::Value, 2);
        CHECK_EQ(wstl::compile::LogCeil<int, 5, 2>::Value, 3);
        CHECK_EQ(wstl::compile::LogCeil<int, 6, 2>::Value, 3);
        CHECK_EQ(wstl::compile::LogCeil<int, 7, 2>::Value, 3);
        CHECK_EQ(wstl::compile::LogCeil<int, 8, 2>::Value, 3);
        CHECK_EQ(wstl::compile::LogCeil<int, 9, 3>::Value, 2);
        CHECK_EQ(wstl::compile::LogCeil<int, 125, 5>::Value, 3);
        CHECK_EQ(wstl::compile::LogCeil<int, 126, 5>::Value, 4);

        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 0, 2>::Value, 0U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 0, 5>::Value, 0U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 0, 3>::Value, 0U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 1, 2>::Value, 0U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 1, 3>::Value, 0U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 1, 5>::Value, 0U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 2, 2>::Value, 1U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 3, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 4, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 5, 2>::Value, 3U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 6, 2>::Value, 3U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 7, 2>::Value, 3U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 8, 2>::Value, 3U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 9, 3>::Value, 2U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 125, 5>::Value, 3U);
        CHECK_EQ(wstl::compile::LogCeil<uint32_t, 126, 5>::Value, 4U);
    }

    TEST_CASE("Log2") {
        CHECK_EQ(wstl::compile::Log2<int, 0>::Value, 0);
        CHECK_EQ(wstl::compile::Log2<int, 1>::Value, 0);
        CHECK_EQ(wstl::compile::Log2<int, 2>::Value, 1);
        CHECK_EQ(wstl::compile::Log2<int, 3>::Value, 1);
        CHECK_EQ(wstl::compile::Log2<int, 4>::Value, 2);
        CHECK_EQ(wstl::compile::Log2<int, 5>::Value, 2);
        CHECK_EQ(wstl::compile::Log2<int, 6>::Value, 2);
        CHECK_EQ(wstl::compile::Log2<int, 7>::Value, 2);
        CHECK_EQ(wstl::compile::Log2<int, 8>::Value, 3);

        CHECK_EQ(wstl::compile::Log2<uint32_t, 0>::Value, 0U);
        CHECK_EQ(wstl::compile::Log2<uint32_t, 1>::Value, 0U);
        CHECK_EQ(wstl::compile::Log2<uint32_t, 2>::Value, 1U);
        CHECK_EQ(wstl::compile::Log2<uint32_t, 3>::Value, 1U);
        CHECK_EQ(wstl::compile::Log2<uint32_t, 4>::Value, 2U);
        CHECK_EQ(wstl::compile::Log2<uint32_t, 5>::Value, 2U);
        CHECK_EQ(wstl::compile::Log2<uint32_t, 6>::Value, 2U);
        CHECK_EQ(wstl::compile::Log2<uint32_t, 7>::Value, 2U);
        CHECK_EQ(wstl::compile::Log2<uint32_t, 8>::Value, 3U);
    }

    TEST_CASE("Log2Ceil") {
        CHECK_EQ(wstl::compile::Log2Ceil<int, 0>::Value, 0);
        CHECK_EQ(wstl::compile::Log2Ceil<int, 1>::Value, 0);
        CHECK_EQ(wstl::compile::Log2Ceil<int, 2>::Value, 1);
        CHECK_EQ(wstl::compile::Log2Ceil<int, 3>::Value, 2);
        CHECK_EQ(wstl::compile::Log2Ceil<int, 4>::Value, 2);
        CHECK_EQ(wstl::compile::Log2Ceil<int, 5>::Value, 3);
        CHECK_EQ(wstl::compile::Log2Ceil<int, 6>::Value, 3);
        CHECK_EQ(wstl::compile::Log2Ceil<int, 7>::Value, 3);
        CHECK_EQ(wstl::compile::Log2Ceil<int, 8>::Value, 3);

        CHECK_EQ(wstl::compile::Log2Ceil<uint32_t, 0>::Value, 0U);
        CHECK_EQ(wstl::compile::Log2Ceil<uint32_t, 1>::Value, 0U);
        CHECK_EQ(wstl::compile::Log2Ceil<uint32_t, 2>::Value, 1U);
        CHECK_EQ(wstl::compile::Log2Ceil<uint32_t, 3>::Value, 2U);
        CHECK_EQ(wstl::compile::Log2Ceil<uint32_t, 4>::Value, 2U);
        CHECK_EQ(wstl::compile::Log2Ceil<uint32_t, 5>::Value, 3U);
        CHECK_EQ(wstl::compile::Log2Ceil<uint32_t, 6>::Value, 3U);
        CHECK_EQ(wstl::compile::Log2Ceil<uint32_t, 7>::Value, 3U);
        CHECK_EQ(wstl::compile::Log2Ceil<uint32_t, 8>::Value, 3U);
    }

    TEST_CASE("Log10") {
        CHECK_EQ(wstl::compile::Log10<int, 0>::Value, 0);
        CHECK_EQ(wstl::compile::Log10<int, 1>::Value, 0);
        CHECK_EQ(wstl::compile::Log10<int, 5>::Value, 0);
        CHECK_EQ(wstl::compile::Log10<int, 10>::Value, 1);
        CHECK_EQ(wstl::compile::Log10<int, 50>::Value, 1);
        CHECK_EQ(wstl::compile::Log10<int, 100>::Value, 2);
        CHECK_EQ(wstl::compile::Log10<int, 999>::Value, 2);
        CHECK_EQ(wstl::compile::Log10<int, 1000>::Value, 3);
        CHECK_EQ(wstl::compile::Log10<int, 1500>::Value, 3);

        CHECK_EQ(wstl::compile::Log10<uint32_t, 0>::Value, 0U);
        CHECK_EQ(wstl::compile::Log10<uint32_t, 1>::Value, 0U);
        CHECK_EQ(wstl::compile::Log10<uint32_t, 5>::Value, 0U);
        CHECK_EQ(wstl::compile::Log10<uint32_t, 10>::Value, 1U);
        CHECK_EQ(wstl::compile::Log10<uint32_t, 50>::Value, 1U);
        CHECK_EQ(wstl::compile::Log10<uint32_t, 100>::Value, 2U);
        CHECK_EQ(wstl::compile::Log10<uint32_t, 999>::Value, 2U);
        CHECK_EQ(wstl::compile::Log10<uint32_t, 1000>::Value, 3U);
        CHECK_EQ(wstl::compile::Log10<uint32_t, 1500>::Value, 3U);
    }

    TEST_CASE("Log10Ceil") {
        CHECK_EQ(wstl::compile::Log10Ceil<int, 0>::Value, 0);
        CHECK_EQ(wstl::compile::Log10Ceil<int, 1>::Value, 0);
        CHECK_EQ(wstl::compile::Log10Ceil<int, 5>::Value, 1);
        CHECK_EQ(wstl::compile::Log10Ceil<int, 10>::Value, 1);
        CHECK_EQ(wstl::compile::Log10Ceil<int, 50>::Value, 2);
        CHECK_EQ(wstl::compile::Log10Ceil<int, 100>::Value, 2);
        CHECK_EQ(wstl::compile::Log10Ceil<int, 999>::Value, 3);
        CHECK_EQ(wstl::compile::Log10Ceil<int, 1000>::Value, 3);
        CHECK_EQ(wstl::compile::Log10Ceil<int, 1500>::Value, 4);

        CHECK_EQ(wstl::compile::Log10Ceil<uint32_t, 0>::Value, 0U);
        CHECK_EQ(wstl::compile::Log10Ceil<uint32_t, 1>::Value, 0U);
        CHECK_EQ(wstl::compile::Log10Ceil<uint32_t, 5>::Value, 1U);
        CHECK_EQ(wstl::compile::Log10Ceil<uint32_t, 10>::Value, 1U);
        CHECK_EQ(wstl::compile::Log10Ceil<uint32_t, 50>::Value, 2U);
        CHECK_EQ(wstl::compile::Log10Ceil<uint32_t, 100>::Value, 2U);
        CHECK_EQ(wstl::compile::Log10Ceil<uint32_t, 999>::Value, 3U);
        CHECK_EQ(wstl::compile::Log10Ceil<uint32_t, 1000>::Value, 3U);
        CHECK_EQ(wstl::compile::Log10Ceil<uint32_t, 1500>::Value, 4U);
    }

    TEST_CASE("NthRoot") {
        CHECK_EQ(wstl::compile::NthRoot<int, 0, 2>::Value, 0);
        CHECK_EQ(wstl::compile::NthRoot<int, 0, 10>::Value, 0);
        CHECK_EQ(wstl::compile::NthRoot<int, 1, 2>::Value, 1);
        CHECK_EQ(wstl::compile::NthRoot<int, 1, 10>::Value, 1);
        CHECK_EQ(wstl::compile::NthRoot<int, 32, 5>::Value, 2);
        CHECK_EQ(wstl::compile::NthRoot<int, 2, 2>::Value, 1);
        CHECK_EQ(wstl::compile::NthRoot<int, 3, 2>::Value, 1);
        CHECK_EQ(wstl::compile::NthRoot<int, 4, 2>::Value, 2);
        CHECK_EQ(wstl::compile::NthRoot<int, 5, 2>::Value, 2);
        CHECK_EQ(wstl::compile::NthRoot<int, 6, 2>::Value, 2);
        CHECK_EQ(wstl::compile::NthRoot<int, 7, 2>::Value, 2);
        CHECK_EQ(wstl::compile::NthRoot<int, 8, 2>::Value, 2);
        CHECK_EQ(wstl::compile::NthRoot<int, 9, 2>::Value, 3);
        CHECK_EQ(wstl::compile::NthRoot<int, 16, 2>::Value, 4);
        CHECK_EQ(wstl::compile::NthRoot<int, 256, 2>::Value, 16);
        CHECK_EQ(wstl::compile::NthRoot<int, 9801, 2>::Value, 99);
        CHECK_EQ(wstl::compile::NthRoot<int, 1048576, 2>::Value, 1024);
        CHECK_EQ(wstl::compile::NthRoot<long long, 5497558138880, 4>::Value, 1531);
        CHECK_EQ(wstl::compile::NthRoot<int, -1, 3>::Value, -1);
        CHECK_EQ(wstl::compile::NthRoot<int, -2, 3>::Value, -1);
        CHECK_EQ(wstl::compile::NthRoot<int, -3, 3>::Value, -1);
        CHECK_EQ(wstl::compile::NthRoot<int, -4, 3>::Value, -1);
        CHECK_EQ(wstl::compile::NthRoot<int, -5, 3>::Value, -1);
        CHECK_EQ(wstl::compile::NthRoot<int, -6, 3>::Value, -1);
        CHECK_EQ(wstl::compile::NthRoot<int, -7, 3>::Value, -1);
        CHECK_EQ(wstl::compile::NthRoot<int, -8, 3>::Value, -2);
        CHECK_EQ(wstl::compile::NthRoot<int, -27, 3>::Value, -3);
        CHECK_EQ(wstl::compile::NthRoot<int, -15625, 3>::Value, -25);
        CHECK_EQ(wstl::compile::NthRoot<int, -1000000000, 3>::Value, -1000);

        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 0, 2>::Value, 0U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 0, 10>::Value, 0U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 1, 2>::Value, 1U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 1, 10>::Value, 1U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 32, 5>::Value, 2U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 2, 2>::Value, 1U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 3, 2>::Value, 1U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 4, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 5, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 6, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 7, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 8, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 9, 2>::Value, 3U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 16, 2>::Value, 4U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 256, 2>::Value, 16U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 9801, 2>::Value, 99U);
        CHECK_EQ(wstl::compile::NthRoot<uint32_t, 1048576, 2>::Value, 1024U);
        CHECK_EQ(wstl::compile::NthRoot<uint64_t, 5497558138880, 4>::Value, 1531U);

        // The following lines should fail with a compilation error
        // wstl::compile::NthRoot<int, -1, 2>::Value;
        // wstl::compile::NthRoot<int, 2, 1>::Value;
    }

    TEST_CASE("SquareRoot") {
        CHECK_EQ(wstl::compile::SquareRoot<int, 0>::Value, 0);
        CHECK_EQ(wstl::compile::SquareRoot<int, 1>::Value, 1);
        CHECK_EQ(wstl::compile::SquareRoot<int, 2>::Value, 1);
        CHECK_EQ(wstl::compile::SquareRoot<int, 3>::Value, 1);
        CHECK_EQ(wstl::compile::SquareRoot<int, 4>::Value, 2);
        CHECK_EQ(wstl::compile::SquareRoot<int, 5>::Value, 2);
        CHECK_EQ(wstl::compile::SquareRoot<int, 6>::Value, 2);
        CHECK_EQ(wstl::compile::SquareRoot<int, 7>::Value, 2);
        CHECK_EQ(wstl::compile::SquareRoot<int, 8>::Value, 2);
        CHECK_EQ(wstl::compile::SquareRoot<int, 9>::Value, 3);
        CHECK_EQ(wstl::compile::SquareRoot<int, 16>::Value, 4);
        CHECK_EQ(wstl::compile::SquareRoot<int, 256>::Value, 16);
        CHECK_EQ(wstl::compile::SquareRoot<int, 9801>::Value, 99);
        CHECK_EQ(wstl::compile::SquareRoot<int, 1048576>::Value, 1024);

        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 0>::Value, 0U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 1>::Value, 1U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 2>::Value, 1U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 3>::Value, 1U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 4>::Value, 2U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 5>::Value, 2U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 6>::Value, 2U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 7>::Value, 2U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 8>::Value, 2U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 9>::Value, 3U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 16>::Value, 4U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 256>::Value, 16U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 9801>::Value, 99U);
        CHECK_EQ(wstl::compile::SquareRoot<uint32_t, 1048576>::Value, 1024U);

        // The following line should fail with a compilation error
        // wstl::compile::SquareRoot<int, -1>::Value;
    }

    TEST_CASE("Factorial") {
        CHECK_EQ(wstl::compile::Factorial<int, 0>::Value, 1);
        CHECK_EQ(wstl::compile::Factorial<int, 1>::Value, 1);
        CHECK_EQ(wstl::compile::Factorial<int, 2>::Value, 2);
        CHECK_EQ(wstl::compile::Factorial<int, 3>::Value, 6);
        CHECK_EQ(wstl::compile::Factorial<int, 4>::Value, 24);
        CHECK_EQ(wstl::compile::Factorial<int, 5>::Value, 120);
        CHECK_EQ(wstl::compile::Factorial<int, 6>::Value, 720);
        CHECK_EQ(wstl::compile::Factorial<int, 7>::Value, 5040);
        CHECK_EQ(wstl::compile::Factorial<int, 8>::Value, 40320);
        CHECK_EQ(wstl::compile::Factorial<int, 9>::Value, 362880);
        CHECK_EQ(wstl::compile::Factorial<int, 10>::Value, 3628800);
        CHECK_EQ(wstl::compile::Factorial<int, 11>::Value, 39916800);
        CHECK_EQ(wstl::compile::Factorial<int, 12>::Value, 479001600);

        CHECK_EQ(wstl::compile::Factorial<uint32_t, 0>::Value, 1U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 1>::Value, 1U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 2>::Value, 2U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 3>::Value, 6U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 4>::Value, 24U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 5>::Value, 120U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 6>::Value, 720U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 7>::Value, 5040U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 8>::Value, 40320U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 9>::Value, 362880U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 10>::Value, 3628800U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 11>::Value, 39916800U);
        CHECK_EQ(wstl::compile::Factorial<uint32_t, 12>::Value, 479001600U);
    }

    TEST_CASE("Fibonacci") {
        CHECK_EQ(wstl::compile::Fibonacci<int, 2>::Value, 1);
        CHECK_EQ(wstl::compile::Fibonacci<int, 3>::Value, 2);
        CHECK_EQ(wstl::compile::Fibonacci<int, 4>::Value, 3);
        CHECK_EQ(wstl::compile::Fibonacci<int, 5>::Value, 5);
        CHECK_EQ(wstl::compile::Fibonacci<int, 6>::Value, 8);
        CHECK_EQ(wstl::compile::Fibonacci<int, 7>::Value, 13);
        CHECK_EQ(wstl::compile::Fibonacci<int, 12>::Value, 144); 
        CHECK_EQ(wstl::compile::Fibonacci<int, 46>::Value, 1836311903);
        CHECK_EQ(wstl::compile::Fibonacci<long long, 50>::Value, 12586269025LL);

        CHECK_EQ(wstl::compile::Fibonacci<uint32_t, 2>::Value, 1U);
        CHECK_EQ(wstl::compile::Fibonacci<uint32_t, 3>::Value, 2U);
        CHECK_EQ(wstl::compile::Fibonacci<uint32_t, 4>::Value, 3U);
        CHECK_EQ(wstl::compile::Fibonacci<uint32_t, 5>::Value, 5U);
        CHECK_EQ(wstl::compile::Fibonacci<uint32_t, 6>::Value, 8U);
        CHECK_EQ(wstl::compile::Fibonacci<uint32_t, 7>::Value, 13U);
        CHECK_EQ(wstl::compile::Fibonacci<uint32_t, 12>::Value, 144U); 
        CHECK_EQ(wstl::compile::Fibonacci<uint32_t, 46>::Value, 1836311903U);
        CHECK_EQ(wstl::compile::Fibonacci<uint64_t, 50>::Value, 12586269025ULL);
        CHECK_EQ(wstl::compile::Fibonacci<uint64_t, 90>::Value, 2880067194370816120ULL);
    }

    TEST_CASE("Permutations") {
        CHECK_EQ(wstl::compile::Permutations<int, 6, 2>::Value, 30);
        CHECK_EQ(wstl::compile::Permutations<int, 7, 3>::Value, 210);
        CHECK_EQ(wstl::compile::Permutations<int, 8, 4>::Value, 1680);
        CHECK_EQ(wstl::compile::Permutations<int, 9, 5>::Value, 15120);
        CHECK_EQ(wstl::compile::Permutations<int, 10, 6>::Value, 151200);
        CHECK_EQ(wstl::compile::Permutations<int, 11, 7>::Value, 1663200);
        CHECK_EQ(wstl::compile::Permutations<int, 12, 8>::Value, 19958400);
        CHECK_EQ(wstl::compile::Permutations<int, 13, 9>::Value, 259459200);
        CHECK_EQ(wstl::compile::Permutations<long long, 14, 10>::Value, 3632428800LL);

        CHECK_EQ(wstl::compile::Permutations<uint32_t, 6, 2>::Value, 30U);
        CHECK_EQ(wstl::compile::Permutations<uint32_t, 7, 3>::Value, 210U);
        CHECK_EQ(wstl::compile::Permutations<uint32_t, 8, 4>::Value, 1680U);
        CHECK_EQ(wstl::compile::Permutations<uint32_t, 9, 5>::Value, 15120U);
        CHECK_EQ(wstl::compile::Permutations<uint32_t, 10, 6>::Value, 151200U);
        CHECK_EQ(wstl::compile::Permutations<uint32_t, 11, 7>::Value, 1663200U);
        CHECK_EQ(wstl::compile::Permutations<uint32_t, 12, 8>::Value, 19958400U);
        CHECK_EQ(wstl::compile::Permutations<uint32_t, 13, 9>::Value, 259459200U);
        CHECK_EQ(wstl::compile::Permutations<uint64_t, 14, 10>::Value, 3632428800ULL);

        // The following lines should fail with a compilation error
        // wstl::compile::Permutations<int, 5, 6>::Value;
        // wstl::compile::Permutations<int, 5, -1>::Value;
        // wstl::compile::Permutations<int, -1, 0>::Value;
        // wstl::compile::Permutations<int, -1, -1>::Value;
    }

    TEST_CASE("Combinations") {
        CHECK_EQ(wstl::compile::Combinations<int, 6, 2>::Value, 15);
        CHECK_EQ(wstl::compile::Combinations<int, 7, 3>::Value, 35);
        CHECK_EQ(wstl::compile::Combinations<int, 8, 4>::Value, 70);
        CHECK_EQ(wstl::compile::Combinations<int, 9, 5>::Value, 126);
        CHECK_EQ(wstl::compile::Combinations<int, 10, 6>::Value, 210);
        CHECK_EQ(wstl::compile::Combinations<int, 11, 7>::Value, 330);
        CHECK_EQ(wstl::compile::Combinations<int, 12, 8>::Value, 495);
        CHECK_EQ(wstl::compile::Combinations<int, 13, 9>::Value, 715);
        CHECK_EQ(wstl::compile::Combinations<long long, 14, 10>::Value, 1001LL);

        CHECK_EQ(wstl::compile::Combinations<uint32_t, 6, 2>::Value, 15U);
        CHECK_EQ(wstl::compile::Combinations<uint32_t, 7, 3>::Value, 35U);
        CHECK_EQ(wstl::compile::Combinations<uint32_t, 8, 4>::Value, 70U);
        CHECK_EQ(wstl::compile::Combinations<uint32_t, 9, 5>::Value, 126U);
        CHECK_EQ(wstl::compile::Combinations<uint32_t, 10, 6>::Value, 210U);
        CHECK_EQ(wstl::compile::Combinations<uint32_t, 11, 7>::Value, 330U);
        CHECK_EQ(wstl::compile::Combinations<uint32_t, 12, 8>::Value, 495U);
        CHECK_EQ(wstl::compile::Combinations<uint32_t, 13, 9>::Value, 715U);
        CHECK_EQ(wstl::compile::Combinations<uint64_t, 14, 10>::Value, 1001ULL);

        // The following lines should fail with a compilation error
        // wstl::compile::Combinations<int, 5, 6>::Value;
        // wstl::compile::Combinations<int, 5, -1>::Value;
        // wstl::compile::Combinations<int, -1, 0>::Value;
        // wstl::compile::Combinations<int, -1, -1>::Value;
    }

    TEST_CASE("DivideFloor") {
        CHECK_EQ(wstl::DivideFloor(10, 3), 3);
        CHECK_EQ(wstl::DivideFloor(10, -3), -4);
        CHECK_EQ(wstl::DivideFloor(-7, 3), -3);
        CHECK_EQ(wstl::DivideFloor(-10, 3), -4);
        CHECK_EQ(wstl::DivideFloor(-10, -3), 3);
        CHECK_EQ(wstl::DivideFloor(10U, 3U), 3U);
        CHECK_EQ(wstl::DivideFloor(10U, 4U), 2U);
        CHECK_EQ(wstl::DivideFloor(10U, 5U), 2U);
        CHECK_EQ(wstl::DivideFloor(10U, 6U), 1U);
        CHECK_EQ(wstl::DivideFloor(10U, 7U), 1U);
        CHECK_EQ(wstl::DivideFloor(10U, 8U), 1U);
        CHECK_EQ(wstl::DivideFloor(10U, 9U), 1U);
        CHECK_EQ(wstl::DivideFloor(10U, 10U), 1U);
        CHECK_EQ(wstl::DivideFloor(10U, 11U), 0U);
    }

    TEST_CASE("DivideCeil") {
        CHECK_EQ(wstl::DivideCeil(10, 3), 4);
        CHECK_EQ(wstl::DivideCeil(10, -3), -3);
        CHECK_EQ(wstl::DivideCeil(-7, 3), -2);
        CHECK_EQ(wstl::DivideCeil(-10, 3), -3);
        CHECK_EQ(wstl::DivideCeil(-10, -3), 4);
        CHECK_EQ(wstl::DivideCeil(10U, 3U), 4U);
        CHECK_EQ(wstl::DivideCeil(10U, 4U), 3U);
        CHECK_EQ(wstl::DivideCeil(10U, 5U), 2U);
        CHECK_EQ(wstl::DivideCeil(10U, 6U), 2U);
        CHECK_EQ(wstl::DivideCeil(10U, 7U), 2U);
        CHECK_EQ(wstl::DivideCeil(10U, 8U), 2U);
        CHECK_EQ(wstl::DivideCeil(10U, 9U), 2U);
        CHECK_EQ(wstl::DivideCeil(10U, 10U), 1U);
        CHECK_EQ(wstl::DivideCeil(10U, 11U), 1U);
    }

    TEST_CASE("IsNaN") {
        float f1 = NAN;
        float f2 = 0;
        double d1 = NAN;
        double d2 = 0;
        long double ld1 = NAN;
        long double ld2 = 0;
        int i = 0;

        CHECK(wstl::IsNaN(f1));
        CHECK_FALSE(wstl::IsNaN(f2));
        CHECK(wstl::IsNaN(d1));
        CHECK_FALSE(wstl::IsNaN(d2));
        CHECK(wstl::IsNaN(ld1));
        CHECK_FALSE(wstl::IsNaN(ld2));
        CHECK_FALSE(wstl::IsNaN(i));
    }

    TEST_CASE("IsInfinity") {
        float f1a = INFINITY;
        float f1b = -INFINITY;
        float f2 = 0;
        double d1a = INFINITY;
        double d1b = -INFINITY;
        double d2 = 0;
        long double ld1a = INFINITY;
        long double ld1b = -INFINITY;
        long double ld2 = 0;
        int ia = INT_MAX;
        int ib = 0;
        int ic = INT_MIN;

        CHECK(wstl::IsInfinity(f1a));
        CHECK(wstl::IsInfinity(f1b));
        CHECK_FALSE(wstl::IsInfinity(f2));
        CHECK(wstl::IsInfinity(d1a));
        CHECK(wstl::IsInfinity(d1b));
        CHECK_FALSE(wstl::IsInfinity(d2));
        CHECK(wstl::IsInfinity(ld1a));
        CHECK(wstl::IsInfinity(ld1b));
        CHECK_FALSE(wstl::IsInfinity(ld2));
        CHECK_FALSE(wstl::IsInfinity(ia));
        CHECK_FALSE(wstl::IsInfinity(ib));
        CHECK_FALSE(wstl::IsInfinity(ic));
    }

    TEST_CASE("IsZero") {
        float f1 = 0;
        float f2 = 1.0F;
        double d1 = 0;
        double d2 = 1.0;
        long double ld1 = 0;
        long double ld2 = 1.0L;
        int i1 = 0;
        int i2 = 1;

        CHECK(wstl::IsZero(f1));
        CHECK_FALSE(wstl::IsZero(f2));
        CHECK(wstl::IsZero(d1));
        CHECK_FALSE(wstl::IsZero(d2));
        CHECK(wstl::IsZero(ld1));
        CHECK_FALSE(wstl::IsZero(ld2));
        CHECK(wstl::IsZero(i1));
        CHECK_FALSE(wstl::IsZero(i2));
    }

    TEST_CASE("IsExactlyEqual") {
        float f1 = 0;
        float f2 = 1.0F;
        double d1 = 0;
        double d2 = 1.0;
        long double ld1 = 0;
        long double ld2 = 1.0L;
        int i1 = 0;
        int i2 = 1;

        CHECK(wstl::IsExactlyEqual(f1, f1));
        CHECK_FALSE(wstl::IsExactlyEqual(f1, f2));
        CHECK(wstl::IsExactlyEqual(d1, d1));
        CHECK_FALSE(wstl::IsExactlyEqual(d1, d2));
        CHECK(wstl::IsExactlyEqual(ld1, ld1));
        CHECK_FALSE(wstl::IsExactlyEqual(ld1, ld2));
        CHECK(wstl::IsExactlyEqual(i1, i1));
        CHECK_FALSE(wstl::IsExactlyEqual(i1, i2));
    }
}