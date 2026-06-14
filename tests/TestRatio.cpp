// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s test_ratio.cpp
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// Some tests have been adapted and extended by Artem Bezruchko (WardenHD)
// to improve coverage and match WardenSTL's implementation.
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Ratio.hpp>
#include <ratio>


TEST_SUITE("Ratio") {
    TEST_CASE("Ratio definition") {
        constexpr intmax_t NUMERATOR = 20;
        constexpr intmax_t DENOMINATOR = 600;
        
        intmax_t N = NUMERATOR / wstl::compile::GCD<intmax_t, NUMERATOR, DENOMINATOR>::Value;
        intmax_t D = DENOMINATOR / wstl::compile::GCD<intmax_t, NUMERATOR, DENOMINATOR>::Value;

        using Data = wstl::Ratio<NUMERATOR, DENOMINATOR>;
        using RatioType = Data::Type;

        CHECK_FALSE(std::is_same<Data, RatioType>::value);

        CHECK_EQ(Data::Numerator, N);
        CHECK_EQ(Data::Denominator, D);

        CHECK_EQ(RatioType::Numerator, N);
        CHECK_EQ(RatioType::Denominator, D);
    }

    TEST_CASE("RatioAdd") {
        using Ratio1 = wstl::Ratio<2, 3>;
        using Ratio2 = wstl::Ratio<1, 6>;

        using Result = wstl::RatioAdd<Ratio1, Ratio2>;

        CHECK(wstl::IsSame<Result, wstl::Ratio<5, 6>>::Value);
        CHECK_EQ(Result::Numerator, 5);
        CHECK_EQ(Result::Denominator, 6);
    }

    TEST_CASE("RatioSubtract") {
        using Ratio1 = wstl::Ratio<2, 3>;
        using Ratio2 = wstl::Ratio<1, 6>;

        using Result1 = wstl::RatioSubtract<Ratio1, Ratio2>;

        CHECK(wstl::IsSame<Result1, wstl::Ratio<1, 2>>::Value);
        CHECK_EQ(Result1::Numerator, 1);
        CHECK_EQ(Result1::Denominator, 2);

        using Result2 = wstl::RatioSubtract<Ratio2, Ratio1>;

        CHECK(wstl::IsSame<Result2, wstl::Ratio<-1, 2>::Type>::Value);
        CHECK_EQ(Result2::Numerator, -1);
        CHECK_EQ(Result2::Denominator, 2);
    }

    TEST_CASE("RatioMultiply") {
        using Ratio1 = wstl::Ratio<2, 3>;
        using Ratio2 = wstl::Ratio<1, 6>;

        using Result = wstl::RatioMultiply<Ratio1, Ratio2>;

        CHECK(wstl::IsSame<Result, wstl::Ratio<1, 9>>::Value);
        CHECK_EQ(Result::Numerator, 1);
        CHECK_EQ(Result::Denominator, 9);
    }

    TEST_CASE("RatioDivide") {
        using Ratio1 = wstl::Ratio<2, 3>;
        using Ratio2 = wstl::Ratio<1, 6>;

        using Result1 = wstl::RatioDivide<Ratio1, Ratio2>;

        CHECK(wstl::IsSame<Result1, wstl::Ratio<4, 1>::Type>::Value);
        CHECK_EQ(Result1::Numerator, 4);
        CHECK_EQ(Result1::Denominator, 1);

        using Result2 = wstl::RatioDivide<Ratio2, Ratio1>;

        CHECK(wstl::IsSame<Result2, wstl::Ratio<1, 4>::Type>::Value);
        CHECK_EQ(Result2::Numerator, 1);
        CHECK_EQ(Result2::Denominator, 4);
    }

    TEST_CASE("RatioEqual") {
        using Ratio1 = wstl::Ratio<2, 3>;
        using Ratio2 = wstl::Ratio<1, 6>;

        CHECK(wstl::RatioEqual<Ratio1, Ratio1>::Value);
        CHECK_FALSE(wstl::RatioEqual<Ratio1, Ratio2>::Value);

        #ifdef __WSTL_CXX17__
        CHECK(wstl::RatioEqualValue<Ratio1, Ratio1>);
        CHECK_FALSE(wstl::RatioEqualValue<Ratio1, Ratio2>);
        #endif
    }

    TEST_CASE("RatioNotEqual") {
        using Ratio1 = wstl::Ratio<2, 3>;
        using Ratio2 = wstl::Ratio<1, 6>;

        CHECK(wstl::RatioNotEqual<Ratio1, Ratio2>::Value);
        CHECK_FALSE(wstl::RatioNotEqual<Ratio1, Ratio1>::Value);

        #ifdef __WSTL_CXX17__
        CHECK(wstl::RatioNotEqualValue<Ratio1, Ratio2>);
        CHECK_FALSE(wstl::RatioNotEqualValue<Ratio1, Ratio1>);
        #endif
    }

    TEST_CASE("RatioLess") {
        using Ratio1 = wstl::Ratio<1, 6>;
        using Ratio2 = wstl::Ratio<2, 3>;

        CHECK(wstl::RatioLess<Ratio1, Ratio2>::Value);
        CHECK_FALSE(wstl::RatioLess<Ratio1, Ratio1>::Value);
        CHECK_FALSE(wstl::RatioLess<Ratio2, Ratio1>::Value);

        #ifdef __WSTL_CXX17__
        CHECK(wstl::RatioLessValue<Ratio1, Ratio2>);
        CHECK_FALSE(wstl::RatioLessValue<Ratio1, Ratio1>);
        CHECK_FALSE(wstl::RatioLessValue<Ratio2, Ratio1>);
        #endif
    }

    TEST_CASE("RatioLessEqual") {
        using Ratio1 = wstl::Ratio<1, 6>;
        using Ratio2 = wstl::Ratio<2, 3>;

        CHECK(wstl::RatioLessEqual<Ratio1, Ratio2>::Value);
        CHECK(wstl::RatioLessEqual<Ratio1, Ratio1>::Value);
        CHECK_FALSE(wstl::RatioLess<Ratio2, Ratio1>::Value);

        #ifdef __WSTL_CXX17__
        CHECK(wstl::RatioLessEqualValue<Ratio1, Ratio2>);
        CHECK(wstl::RatioLessEqualValue<Ratio1, Ratio1>);
        CHECK_FALSE(wstl::RatioLessEqualValue<Ratio2, Ratio1>);
        #endif
    }

    TEST_CASE("RatioGreater") {
        using Ratio1 = wstl::Ratio<1, 6>;
        using Ratio2 = wstl::Ratio<2, 3>;

        CHECK(wstl::RatioGreater<Ratio2, Ratio1>::Value);
        CHECK_FALSE(wstl::RatioGreater<Ratio1, Ratio1>::Value);
        CHECK_FALSE(wstl::RatioGreater<Ratio1, Ratio2>::Value);

        #ifdef __WSTL_CXX17__
        CHECK(wstl::RatioGreaterValue<Ratio2, Ratio1>);
        CHECK_FALSE(wstl::RatioGreaterValue<Ratio1, Ratio1>);
        CHECK_FALSE(wstl::RatioGreaterValue<Ratio1, Ratio2>);
        #endif
    }

    TEST_CASE("RatioGreaterEqual") {
        using Ratio1 = wstl::Ratio<1, 6>;
        using Ratio2 = wstl::Ratio<2, 3>;

        CHECK(wstl::RatioGreaterEqual<Ratio2, Ratio1>::Value);
        CHECK(wstl::RatioGreaterEqual<Ratio1, Ratio1>::Value);
        CHECK_FALSE(wstl::RatioGreaterEqual<Ratio1, Ratio2>::Value);

        #ifdef __WSTL_CXX17__
        CHECK(wstl::RatioGreaterEqualValue<Ratio2, Ratio1>);
        CHECK(wstl::RatioGreaterEqualValue<Ratio1, Ratio1>);
        CHECK_FALSE(wstl::RatioGreaterEqualValue<Ratio1, Ratio2>);
        #endif
    }
}