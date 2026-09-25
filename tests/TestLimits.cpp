// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s test_limits.cpp
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// Some tests have been adapted and extended by Artem Bezruchko (WardenHD)
// to improve coverage and match WardenSTL's implementation.
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Limits.hpp>
#include <limits>
#include <cmath>


TEST_SUITE("Limits") {
    TEST_CASE("NumericLimits bool") {
        typedef std::numeric_limits<bool> Expected;
        typedef wstl::NumericLimits<bool> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits char") {
        typedef std::numeric_limits<char> Expected;
        typedef wstl::NumericLimits<char> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits signed char") {
        typedef std::numeric_limits<signed char> Expected;
        typedef wstl::NumericLimits<signed char> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits unsigned char") {
        typedef std::numeric_limits<unsigned char> Expected;
        typedef wstl::NumericLimits<unsigned char> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    #ifdef __WSTL_CXX20__
    TEST_CASE("NumericLimits char8_t") {
        typedef std::numeric_limits<char8_t> Expected;
        typedef wstl::NumericLimits<char8_t> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }
    #endif

    #ifdef __WSTL_CXX11__
    TEST_CASE("NumericLimits char16_t") {
        typedef std::numeric_limits<char16_t> Expected;
        typedef wstl::NumericLimits<char16_t> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits char32_t") {
        typedef std::numeric_limits<char32_t> Expected;
        typedef wstl::NumericLimits<char32_t> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }
    #endif

    TEST_CASE("NumericLimits wchar_t") {
        typedef std::numeric_limits<wchar_t> Expected;
        typedef wstl::NumericLimits<wchar_t> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits short") {
        typedef std::numeric_limits<short> Expected;
        typedef wstl::NumericLimits<short> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits unsigned short") {
        typedef std::numeric_limits<unsigned short> Expected;
        typedef wstl::NumericLimits<unsigned short> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits int") {
        typedef std::numeric_limits<int> Expected;
        typedef wstl::NumericLimits<int> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits unsigned int") {
        typedef std::numeric_limits<unsigned int> Expected;
        typedef wstl::NumericLimits<unsigned int> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits long") {
        typedef std::numeric_limits<long> Expected;
        typedef wstl::NumericLimits<long> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits unsigned long") {
        typedef std::numeric_limits<unsigned long> Expected;
        typedef wstl::NumericLimits<unsigned long> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits long long") {
        typedef std::numeric_limits<long long> Expected;
        typedef wstl::NumericLimits<long long> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits unsigned long long") {
        typedef std::numeric_limits<unsigned long long> Expected;
        typedef wstl::NumericLimits<unsigned long long> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(int(Result::RoundStyle), int(Expected::round_style));
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::TinynessBefore, Expected::tinyness_before);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
        CHECK_EQ(Result::DenormalizedMin(), Expected::denorm_min());
    }

    TEST_CASE("NumericLimits float") {
        typedef std::numeric_limits<float> Expected;
        typedef wstl::NumericLimits<float> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
    }

    TEST_CASE("NumericLimits double") {
        typedef std::numeric_limits<double> Expected;
        typedef wstl::NumericLimits<double> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
    }

    TEST_CASE("NumericLimits long double") {
        typedef std::numeric_limits<long double> Expected;
        typedef wstl::NumericLimits<long double> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        #ifndef __WSTL_CXX23__
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        #endif
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_FALSE(Result::Traps);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
    }

    TEST_CASE("IntegralLimits bool") {
        typedef bool Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits char") {
        typedef char Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits unsigned char") {
        typedef unsigned char Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits signed char") {
        typedef signed char Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    #ifdef __WSTL_CXX20__
    TEST_CASE("IntegralLimits char8_t") {
        typedef char8_t Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }
    #endif

    #ifdef __WSTL_CXX11__
    TEST_CASE("IntegralLimits char16_t") {
        typedef char16_t Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits char32_t") {
        typedef char32_t Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }
    #endif

    TEST_CASE("IntegralLimits wchar_t") {
        typedef wchar_t Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits short") {
        typedef short Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits unsigned short") {
        typedef unsigned short Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits int") {
        typedef int Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits unsigned int") {
        typedef unsigned int Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits long") {
        typedef long Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits unsigned long") {
        typedef unsigned long Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits long long") {
        typedef long long Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }

    TEST_CASE("IntegralLimits unsigned long long") {
        typedef unsigned long long Type;
        typedef wstl::IntegralLimits<Type> Result;
        typedef std::numeric_limits<Type> Expected;

        CHECK_EQ(Result::Min, Expected::min());
        CHECK_EQ(Result::Max, Expected::max());
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::Bits, sizeof(Type) * 8);
    }
}
