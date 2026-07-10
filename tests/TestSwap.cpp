// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/private/Swap.hpp>
#include <algorithm>


struct MemberSwappable {
    MemberSwappable() : Value(), SwapCalls(0) {}
    MemberSwappable(int x) : Value(x), SwapCalls(0) {}

    void Swap(MemberSwappable& other) {
        std::swap(other.Value, Value);
        ++other.SwapCalls;
        ++SwapCalls;
    }

    int Value;
    int SwapCalls;
};

TEST_SUITE("Utility") {
    TEST_CASE("Swap") {
        // Normal types, standard overload
        int a = 100;
        int b = 120;

        wstl::Swap(a, b);

        CHECK_EQ(a, 120);
        CHECK_EQ(b, 100);

        int* pa = &a;
        int* pb = &b;

        wstl::Swap(pa, pb);

        CHECK_EQ(a, 120);
        CHECK_EQ(b, 100);
        CHECK_EQ(pa, &b);
        CHECK_EQ(pb, &a);

        // Arrays
        int data[] = {1, 2, 3, 4};
        int data2[] = {5, 6, 7, 8};

        wstl::Swap(data, data2);

        CHECK_EQ(data[0], 5);
        CHECK_EQ(data[1], 6);
        CHECK_EQ(data[2], 7);
        CHECK_EQ(data[3], 8);
        CHECK_EQ(data2[0], 1);
        CHECK_EQ(data2[1], 2);
        CHECK_EQ(data2[2], 3);
        CHECK_EQ(data2[3], 4);

        // Types with .Swap method overload
        MemberSwappable ma(100);
        MemberSwappable mb(120);

        wstl::Swap(ma, mb);

        CHECK_EQ(ma.Value, 120);
        CHECK_EQ(mb.Value, 100);

        // Check if member swap was actually called
        CHECK_EQ(ma.SwapCalls, 1);
        CHECK_EQ(mb.SwapCalls, 1);
    }
}