// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Byte.hpp>


TEST_SUITE("Byte") {
    TEST_CASE("Equality") {
        wstl::Byte a = wstl::Byte(1);
        wstl::Byte b = wstl::Byte(1);
        wstl::Byte c = wstl::Byte(2);

        CHECK_EQ(a == b, true);
        CHECK_EQ(a != b, false);
        CHECK_EQ(a == c, false);
        CHECK_EQ(a != c, true);
        CHECK_EQ(c == a, false);
        CHECK_EQ(c != a, true);

        // The following lines in C++11 and higher should fail with a compilation error
        // a == 1;
        // 1 == a;
        // b != 'c';
        // 'c' != b;
    }

    TEST_CASE("Relational") {
        wstl::Byte a = wstl::Byte(1);
        wstl::Byte b = wstl::Byte(2);

        // Less than
        CHECK_EQ(a < b, true);
        CHECK_EQ(b < a, false);
        CHECK_EQ(a < a, false);

        // Greater than
        CHECK_EQ(b > a, true);
        CHECK_EQ(a > b, false);
        CHECK_EQ(a > a, false);

        // Less than or equal
        CHECK_EQ(a <= b, true);
        CHECK_EQ(b <= a, false);
        CHECK_EQ(a <= a, true);

        // Greater than or equal
        CHECK_EQ(b >= a, true);
        CHECK_EQ(a >= b, false);
        CHECK_EQ(a >= a, true);

        // The following lines in C++11 and higher should fail with a compilation error
        // a > 1;
        // 1 > a;
        // b < 'c';
        // 'c' < b;
        // a >= 1;
        // 1 >= a;
        // b <= 'c';
        // 'c' <= b;
    }

    TEST_CASE("ToInteger") {
        wstl::Byte b = wstl::Byte(0x51);

        CHECK_EQ(wstl::ToInteger<int8_t>(b), 0x51);
        CHECK_EQ(wstl::ToInteger<uint16_t>(b), 0x0051U);
        CHECK_EQ(wstl::ToInteger<uint32_t>(b), 0x00000051U);
    }

    TEST_CASE("Shift left") {
        const uint8_t x = 0xD7;
        wstl::Byte b = wstl::Byte(x);

        for(int i = 0; i < 9; ++i) CHECK_EQ(wstl::ToInteger<uint8_t>(b << i), uint8_t(x << i));

        for(int i = 0; i < 9; ++i) {
            CHECK_EQ(wstl::ToInteger<uint8_t>(b <<= i), uint8_t(x << i));
            CHECK_EQ(wstl::ToInteger<uint8_t>(b), uint8_t(x << i)); // changes are preserved
            b = wstl::Byte(x);
        }
    }

    TEST_CASE("Shift right") {
        const uint8_t x = 0xD7;
        wstl::Byte b = wstl::Byte(x);

        for(int i = 0; i < 9; ++i) CHECK_EQ(wstl::ToInteger<uint8_t>(b >> i), uint8_t(x >> i));

        for(int i = 0; i < 9; ++i) {
            CHECK_EQ(wstl::ToInteger<uint8_t>(b >>= i), uint8_t(x >> i));
            CHECK_EQ(wstl::ToInteger<uint8_t>(b), uint8_t(x >> i)); // changes are preserved
            b = wstl::Byte(x);
        }
    }

    TEST_CASE("Bitwise OR") {
        const uint8_t x = 0xD7;
        wstl::Byte b = wstl::Byte(x);

        for(int i = 0; i < 256; ++i) CHECK_EQ(wstl::ToInteger<uint8_t>(b | wstl::Byte(i)), uint8_t(x | i));

        for(int i = 0; i < 256; ++i) {
            CHECK_EQ(wstl::ToInteger<uint8_t>(b |= wstl::Byte(i)), uint8_t(x | i));
            CHECK_EQ(wstl::ToInteger<uint8_t>(b), uint8_t(x | i)); // changes are preserved
            b = wstl::Byte(x);
        }
    }

    TEST_CASE("Bitwise AND") {
        const uint8_t x = 0xD7;
        wstl::Byte b = wstl::Byte(x);

        for(int i = 0; i < 256; ++i) CHECK_EQ(wstl::ToInteger<uint8_t>(b & wstl::Byte(i)), uint8_t(x & i));

        for(int i = 0; i < 256; ++i) {
            CHECK_EQ(wstl::ToInteger<uint8_t>(b &= wstl::Byte(i)), uint8_t(x & i));
            CHECK_EQ(wstl::ToInteger<uint8_t>(b), uint8_t(x & i)); // changes are preserved
            b = wstl::Byte(x);
        }
    }

    TEST_CASE("Bitwise XOR") {
        const uint8_t x = 0xD7;
        wstl::Byte b = wstl::Byte(x);

        for(int i = 0; i < 256; ++i) CHECK_EQ(wstl::ToInteger<uint8_t>(b ^ wstl::Byte(i)), uint8_t(x ^ i));

        for(int i = 0; i < 256; ++i) {
            CHECK_EQ(wstl::ToInteger<uint8_t>(b ^= wstl::Byte(i)), uint8_t(x ^ i));
            CHECK_EQ(wstl::ToInteger<uint8_t>(b), uint8_t(x ^ i)); // changes are preserved
            b = wstl::Byte(x);
        }
    }

    TEST_CASE("Bitwise NOT") {
        for(int i = 0; i < 256; ++i) CHECK_EQ(wstl::ToInteger<uint8_t>(~wstl::Byte(i)), uint8_t(~i));
    }
}
