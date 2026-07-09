// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s test_numeric.cpp
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// Some tests have been adapted and extended by Artem Bezruchko (WardenHD)
// to improve coverage and match WardenSTL's implementation.
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Numeric.hpp>
#include <numeric>
#include <list>
#include <forward_list>
#include <limits>


TEST_SUITE("Numeric") {
    TEST_CASE("Iota") {
        int result[10] = {0};
        int expected[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        wstl::Iota(std::begin(result), std::end(result), 0);

        bool equal = std::equal(std::begin(expected), std::end(expected), std::begin(result));
        CHECK(equal);
    }

    TEST_CASE("Accumulate") {
        int data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        int sum1 = wstl::Accumulate(std::begin(data), std::end(data), 0);
        int sum2 = std::accumulate(std::begin(data), std::end(data), 0);
        CHECK_EQ(sum1, sum2);

        int product1 = wstl::Accumulate(std::begin(data), std::end(data), 1, std::multiplies<int>());
        int product2 = std::accumulate(std::begin(data), std::end(data), 1, std::multiplies<int>());
        CHECK_EQ(product1, product2);
    }

    TEST_CASE("InnerProduct") {
        int data1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        int data2[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

        int result = wstl::InnerProduct(std::begin(data1), std::end(data1), std::begin(data2), 0);
        int expected = std::inner_product(std::begin(data1), std::end(data1), std::begin(data2), 0);
        CHECK_EQ(result, expected);

        // Number of pairwise matches
        result = wstl::InnerProduct(std::begin(data1), std::end(data1), std::begin(data2), 0, std::plus<int>(), std::equal_to<int>());
        expected = std::inner_product(std::begin(data1), std::end(data1), std::begin(data2), 0, std::plus<int>(), std::equal_to<int>());
        CHECK_EQ(result, expected);
    }

    TEST_CASE("AdjacentDifference") {
        int data[] = {4, 6, 9, 13, 18, 19, 19, 15, 10, 11};
        int result[10];
        int expected[10];

        int* resultPtr = wstl::AdjacentDifference(std::begin(data), std::end(data), std::begin(result));
        int* expectedPtr = std::adjacent_difference(std::begin(data), std::end(data), std::begin(expected));   
        
        ptrdiff_t d1 = std::distance(result, resultPtr);
        ptrdiff_t d2 = std::distance(expected, expectedPtr);
        CHECK_EQ(d1, d2);

        bool equal = std::equal(std::begin(expected), std::end(expected), std::begin(result));
        CHECK(equal);

        resultPtr = wstl::AdjacentDifference(std::begin(data), std::end(data), std::begin(result), std::plus<int>());
        expectedPtr = std::adjacent_difference(std::begin(data), std::end(data), std::begin(expected), std::plus<int>());   
        
        d1 = std::distance(result, resultPtr);
        d2 = std::distance(expected, expectedPtr);
        CHECK_EQ(d1, d2);

        equal = std::equal(std::begin(expected), std::end(expected), std::begin(result));
        CHECK(equal);
    }

    TEST_CASE("PartialSum") {
        int data[] = {4, 6, 9, 13, 18, 19, 19, 15, 10, 11};
        int result[10];
        int expected[10];

        int* resultPtr = wstl::PartialSum(std::begin(data), std::end(data), std::begin(result));
        int* expectedPtr = std::partial_sum(std::begin(data), std::end(data), std::begin(expected));   

        ptrdiff_t d1 = std::distance(result, resultPtr);
        ptrdiff_t d2 = std::distance(expected, expectedPtr);
        CHECK_EQ(d1, d2);

        bool equal = std::equal(std::begin(expected), std::end(expected), std::begin(result));
        CHECK(equal);

        resultPtr = wstl::PartialSum(std::begin(data), std::end(data), std::begin(result), std::multiplies<int>());
        expectedPtr = std::partial_sum(std::begin(data), std::end(data), std::begin(expected), std::multiplies<int>());   
        
        d1 = std::distance(result, resultPtr);
        d2 = std::distance(expected, expectedPtr);
        CHECK_EQ(d1, d2);

        equal = std::equal(std::begin(expected), std::end(expected), std::begin(result));
        CHECK(equal);
    }

    TEST_CASE("GCD") {
        constexpr int num1 = {2 * 2 * 3};
        constexpr int num2 = {2 * 3 * 3};

        // Unsigned
        CHECK_EQ(wstl::GCD((uint32_t) num1, (uint32_t) num2), 6U);
        CHECK_EQ(wstl::GCD(6U, 10U), 2U);
        CHECK_EQ(wstl::GCD(6U, 7U), 1U);
        CHECK_EQ(wstl::GCD(0U, 7U), 7U);
        CHECK_EQ(wstl::GCD(7U, 0U), 7U);
        CHECK_EQ(wstl::GCD(0U, 0U), 0U);

        // Signed
        CHECK_EQ(wstl::GCD(num1, num2), 6);
        CHECK_EQ(wstl::GCD(6, 10), 2);
        CHECK_EQ(wstl::GCD(6, -10), 2);
        CHECK_EQ(wstl::GCD(-6, -10), 2);
        CHECK_EQ(wstl::GCD(6, 7), 1);
        CHECK_EQ(wstl::GCD(0, 7), 7);
        CHECK_EQ(wstl::GCD(-7, 0), 7);
        CHECK_EQ(wstl::GCD(0, 0), 0);

        // More than two numbers
        CHECK_EQ(wstl::GCD(2, 3, 7, 14, 21), 1);
        CHECK_EQ(wstl::GCD(12, 18, 30, 42), 6);
        CHECK_EQ(wstl::GCD(-12, -18, 42), 6);
    }

    TEST_CASE("LCM") {
        constexpr int num1 = {2 * 2 * 3};
        constexpr int num2 = {2 * 3 * 3};

        // Unsigned
        CHECK_EQ(wstl::LCM((uint32_t) num1, (uint32_t) num2), 36U);
        CHECK_EQ(wstl::LCM(6U, 10U), 30U);
        CHECK_EQ(wstl::LCM(6U, 7U), 42U);
        CHECK_EQ(wstl::LCM(0U, 7U), 0U);
        CHECK_EQ(wstl::LCM(7U, 0U), 0U);
        CHECK_EQ(wstl::LCM(0U, 0U), 0U);

        // Signed
        CHECK_EQ(wstl::LCM(num1, num2), 36);
        CHECK_EQ(wstl::LCM(6, 10), 30);
        CHECK_EQ(wstl::LCM(6, -10), 30);
        CHECK_EQ(wstl::LCM(-6, -10), 30);
        CHECK_EQ(wstl::LCM(6, 7), 42);
        CHECK_EQ(wstl::LCM(0, 7), 0);
        CHECK_EQ(wstl::LCM(-7, 0), 0);
        CHECK_EQ(wstl::LCM(0, 0), 0);
        CHECK_EQ(wstl::LCM(50000, 50000), 50000);
        CHECK_EQ(wstl::LCM(12, 18), 36);
        CHECK_EQ(wstl::LCM(7, 5), 35);
        CHECK_EQ(wstl::LCM(21, 6), 42);

        // More than two numbers
        CHECK_EQ(wstl::LCM(2, 3, 7, 14, 21), 42);
        CHECK_EQ(wstl::LCM(12, 18, 30, 42), 1260);
        CHECK_EQ(wstl::LCM(-12, -18, 42), 252);
    }

    TEST_CASE("Midpoint") {
        // Unsigned
        CHECK_EQ(wstl::Midpoint(0U, 0U), 0U);
        CHECK_EQ(wstl::Midpoint(0U, 1U), 0U); // biased towards 0
        CHECK_EQ(wstl::Midpoint(1U, 0U), 1U); // biased towards 1

        CHECK_EQ(wstl::Midpoint(0U, std::numeric_limits<uint32_t>::max()), (std::numeric_limits<uint32_t>::max() / 2U));
        CHECK_EQ(wstl::Midpoint(std::numeric_limits<uint32_t>::max(), 0U), (std::numeric_limits<uint32_t>::max() / 2U) + 1U);

        CHECK_EQ(wstl::Midpoint(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()), (std::numeric_limits<uint32_t>::max() / 2U));
        CHECK_EQ(wstl::Midpoint(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::min()), (std::numeric_limits<uint32_t>::max() / 2U) + 1U);

        // Signed
        CHECK_EQ(wstl::Midpoint(0, 0), 0);
        CHECK_EQ(wstl::Midpoint(0, 1), 0);
        CHECK_EQ(wstl::Midpoint(1, 0), 1);
        CHECK_EQ(wstl::Midpoint(-3, 4), 0);
        CHECK_EQ(wstl::Midpoint(4, -3), 1);
        CHECK_EQ(wstl::Midpoint(-3, -4), -3);
        CHECK_EQ(wstl::Midpoint(-4, -3), -4);

        CHECK_EQ(wstl::Midpoint(0, std::numeric_limits<int32_t>::max()), (std::numeric_limits<int32_t>::max() / 2));
        CHECK_EQ(wstl::Midpoint(std::numeric_limits<int32_t>::max(), 0), (std::numeric_limits<int32_t>::max() / 2) + 1);

        CHECK_EQ(wstl::Midpoint(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()), -1);
        CHECK_EQ(wstl::Midpoint(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min()), 0);

        // Floating point
        CHECK_EQ(wstl::Midpoint(0.0, 0.0), doctest::Approx(0.0).epsilon(0.001));
        CHECK_EQ(wstl::Midpoint(0.0, 1.0), doctest::Approx(0.5).epsilon(0.001));
        CHECK_EQ(wstl::Midpoint(1.0, 0.0), doctest::Approx(0.5).epsilon(0.001));

        CHECK_EQ(wstl::Midpoint(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max()), doctest::Approx(0.0).epsilon(0.001));
        CHECK_EQ(wstl::Midpoint(std::numeric_limits<double>::max(), -std::numeric_limits<double>::max()), doctest::Approx(0.0).epsilon(0.001));

        // Pointer
        std::vector<int> data1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};

        CHECK_EQ(*wstl::Midpoint(data1.data(), data1.data() + data1.size()), data1[4]);
        CHECK_EQ(*wstl::Midpoint(data1.data() + data1.size(), data1.data()), data1[5]);

        // Random-access iterator
        std::deque<int> data2(data1.begin(), data1.end());

        CHECK_EQ(*wstl::Midpoint(data2.begin(), data2.end()), data2[4]);
        CHECK_EQ(*wstl::Midpoint(data2.end(), data2.begin()), data2[5]);

        // Bidirectional iterator
        std::list<int> data3(data1.begin(), data1.end());

        CHECK_EQ(*wstl::Midpoint(data3.begin(), data3.end()), *std::next(data3.begin(), 4));

        // Forward iterator
        std::forward_list<int> data4(data1.begin(), data1.end());

        CHECK_EQ(*wstl::Midpoint(data4.begin(), data4.end()), *std::next(data4.begin(), 4));
    }

    struct Hel {
        int Num;
    };

    TEST_CASE("GCD compile-time version") {
        constexpr int num1 = {2 * 2 * 3};
        constexpr int num2 = {2 * 3 * 3};

        // Unsigned
        CHECK_EQ(wstl::compile::GCD<uint32_t, num1, num2>::Value, 6U);
        CHECK_EQ(wstl::compile::GCD<uint32_t, 6U, 10U>::Value, 2U);
        CHECK_EQ(wstl::compile::GCD<uint32_t, 6U, 7U>::Value, 1U);
        CHECK_EQ(wstl::compile::GCD<uint32_t, 0U, 7U>::Value, 7U);
        CHECK_EQ(wstl::compile::GCD<uint32_t, 7U, 0U>::Value, 7U);
        CHECK_EQ(wstl::compile::GCD<uint32_t, 0U, 0U>::Value, 0U);

        // Signed
        CHECK_EQ(wstl::compile::GCD<int, num1, num2>::Value, 6);
        CHECK_EQ(wstl::compile::GCD<int, 6, 10>::Value, 2);
        CHECK_EQ(wstl::compile::GCD<int, 6, -10>::Value, 2);
        CHECK_EQ(wstl::compile::GCD<int, -6, -10>::Value, 2);
        CHECK_EQ(wstl::compile::GCD<int, 6, 7>::Value, 1);
        CHECK_EQ(wstl::compile::GCD<int, 0, 7>::Value, 7);
        CHECK_EQ(wstl::compile::GCD<int, -7, 0>::Value, 7);
        CHECK_EQ(wstl::compile::GCD<int, 0, 0>::Value, 0);

        // More than two numbers
        CHECK_EQ(wstl::compile::GCD<int, 2, 3, 7, 14, 21>::Value, 1);
        CHECK_EQ(wstl::compile::GCD<int, 12, 18, 30, 42>::Value, 6);
        CHECK_EQ(wstl::compile::GCD<int, -12, -18, 42>::Value, 6);
    }

    TEST_CASE("LCM compile-time version") {
        constexpr int num1 = {2 * 2 * 3};
        constexpr int num2 = {2 * 3 * 3};

        // Unsigned
        CHECK_EQ(wstl::compile::LCM<uint32_t, num1, num2>::Value, 36U);
        CHECK_EQ(wstl::compile::LCM<uint32_t, 6U, 10U>::Value, 30U);
        CHECK_EQ(wstl::compile::LCM<uint32_t, 6U, 7U>::Value, 42U);
        CHECK_EQ(wstl::compile::LCM<uint32_t, 0U, 7U>::Value, 0U);
        CHECK_EQ(wstl::compile::LCM<uint32_t, 7U, 0U>::Value, 0U);
        CHECK_EQ(wstl::compile::LCM<uint32_t, 0U, 0U>::Value, 0U);

        // Signed
        CHECK_EQ(wstl::compile::LCM<int, num1, num2>::Value, 36);
        CHECK_EQ(wstl::compile::LCM<int, 6, 10>::Value, 30);
        CHECK_EQ(wstl::compile::LCM<int, 6, -10>::Value, 30);
        CHECK_EQ(wstl::compile::LCM<int, -6, -10>::Value, 30);
        CHECK_EQ(wstl::compile::LCM<int, 6, 7>::Value, 42);
        CHECK_EQ(wstl::compile::LCM<int, 0, 7>::Value, 0);
        CHECK_EQ(wstl::compile::LCM<int, -7, 0>::Value, 0);
        CHECK_EQ(wstl::compile::LCM<int, 0, 0>::Value, 0);
        CHECK_EQ(wstl::compile::LCM<int, 50000, 50000>::Value, 50000);
        CHECK_EQ(wstl::compile::LCM<int, 12, 18>::Value, 36);
        CHECK_EQ(wstl::compile::LCM<int, 7, 5>::Value, 35);
        CHECK_EQ(wstl::compile::LCM<int, 21, 6>::Value, 42);

        // More than two numbers
        CHECK_EQ(wstl::compile::LCM<int, 2, 3, 7, 14, 21>::Value, 42);
        CHECK_EQ(wstl::compile::LCM<int, 12, 18, 30, 42>::Value, 1260);
        CHECK_EQ(wstl::compile::LCM<int, -12, -18, 42>::Value, 252);
    }

    TEST_CASE("Midpoint compile-time version") {
        // Unsigned
        CHECK_EQ(wstl::compile::Midpoint<uint32_t, 0U, 0U>::Value, 0U);
        CHECK_EQ(wstl::compile::Midpoint<uint32_t, 0U, 1U>::Value, 0U); // biased towards 0
        CHECK_EQ(wstl::compile::Midpoint<uint32_t, 1U, 0U>::Value, 1U); // biased towards 1

        CHECK_EQ(wstl::compile::Midpoint<uint32_t, 0U, std::numeric_limits<uint32_t>::max()>::Value, (std::numeric_limits<uint32_t>::max() / 2U));
        CHECK_EQ(wstl::compile::Midpoint<uint32_t, std::numeric_limits<uint32_t>::max(), 0U>::Value, (std::numeric_limits<uint32_t>::max() / 2U) + 1U);

        CHECK_EQ(wstl::compile::Midpoint<uint32_t, std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()>::Value, (std::numeric_limits<uint32_t>::max() / 2U));
        CHECK_EQ(wstl::compile::Midpoint<uint32_t, std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::min()>::Value, (std::numeric_limits<uint32_t>::max() / 2U) + 1U);

        // Signed
        CHECK_EQ(wstl::compile::Midpoint<int, 0, 0>::Value, 0);
        CHECK_EQ(wstl::compile::Midpoint<int, 0, 1>::Value, 0);
        CHECK_EQ(wstl::compile::Midpoint<int, 1, 0>::Value, 1);
        CHECK_EQ(wstl::compile::Midpoint<int, -3, 4>::Value, 0);
        CHECK_EQ(wstl::compile::Midpoint<int, 4, -3>::Value, 1);
        CHECK_EQ(wstl::compile::Midpoint<int, -3, -4>::Value, -3);
        CHECK_EQ(wstl::compile::Midpoint<int, -4, -3>::Value, -4);

        CHECK_EQ(wstl::compile::Midpoint<int, 0, std::numeric_limits<int32_t>::max()>::Value, (std::numeric_limits<int32_t>::max() / 2));
        CHECK_EQ(wstl::compile::Midpoint<int, std::numeric_limits<int32_t>::max(), 0>::Value, (std::numeric_limits<int32_t>::max() / 2) + 1);

        CHECK_EQ(wstl::compile::Midpoint<int, std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()>::Value, -1);
        CHECK_EQ(wstl::compile::Midpoint<int, std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min()>::Value, 0);
    }
}