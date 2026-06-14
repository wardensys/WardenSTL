// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
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


TEST_SUITE("Limits") {
    TEST_CASE("bool") {
        typedef std::numeric_limits<bool> Expected;
        typedef wstl::NumericLimits<bool> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("char") {
        typedef std::numeric_limits<char> Expected;
        typedef wstl::NumericLimits<char> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("signed char") {
        typedef std::numeric_limits<signed char> Expected;
        typedef wstl::NumericLimits<signed char> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("unsigned char") {
        typedef std::numeric_limits<unsigned char> Expected;
        typedef wstl::NumericLimits<unsigned char> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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
    TEST_CASE("char8_t") {
        typedef std::numeric_limits<char8_t> Expected;
        typedef wstl::NumericLimits<char8_t> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("char16_t") {
        typedef std::numeric_limits<char16_t> Expected;
        typedef wstl::NumericLimits<char16_t> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("char32_t") {
        typedef std::numeric_limits<char32_t> Expected;
        typedef wstl::NumericLimits<char32_t> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("wchar_t") {
        typedef std::numeric_limits<wchar_t> Expected;
        typedef wstl::NumericLimits<wchar_t> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("short") {
        typedef std::numeric_limits<short> Expected;
        typedef wstl::NumericLimits<short> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("unsigned short") {
        typedef std::numeric_limits<unsigned short> Expected;
        typedef wstl::NumericLimits<unsigned short> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("int") {
        typedef std::numeric_limits<int> Expected;
        typedef wstl::NumericLimits<int> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("unsigned int") {
        typedef std::numeric_limits<unsigned int> Expected;
        typedef wstl::NumericLimits<unsigned int> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("long") {
        typedef std::numeric_limits<long> Expected;
        typedef wstl::NumericLimits<long> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("unsigned long") {
        typedef std::numeric_limits<unsigned long> Expected;
        typedef wstl::NumericLimits<unsigned long> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("long long") {
        typedef std::numeric_limits<long long> Expected;
        typedef wstl::NumericLimits<long long> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("unsigned long long") {
        typedef std::numeric_limits<unsigned long long> Expected;
        typedef wstl::NumericLimits<unsigned long long> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(int(Result::HasDenormalization), int(Expected::has_denorm));
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
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
        CHECK_EQ(Result::Traps, Expected::traps);
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

    TEST_CASE("float") {
        typedef std::numeric_limits<float> Expected;
        typedef wstl::NumericLimits<float> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_EQ(Result::Traps, Expected::traps);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
    }

    TEST_CASE("double") {
        typedef std::numeric_limits<double> Expected;
        typedef wstl::NumericLimits<double> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_EQ(Result::Traps, Expected::traps);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
    }

    TEST_CASE("long double") {
        typedef std::numeric_limits<long double> Expected;
        typedef wstl::NumericLimits<long double> Result;

        CHECK_EQ(Result::IsSpecialized, Expected::is_specialized);
        CHECK_EQ(Result::IsSigned, Expected::is_signed);
        CHECK_EQ(Result::IsInteger, Expected::is_exact);
        CHECK_EQ(Result::HasInfinity, Expected::has_infinity);
        CHECK_EQ(Result::HasQuietNaN, Expected::has_quiet_NaN);
        CHECK_EQ(Result::HasSignalingNaN, Expected::has_signaling_NaN);
        CHECK_EQ(Result::HasDenormalizationLoss, Expected::has_denorm_loss);
        CHECK_EQ(Result::IsIEC559, Expected::is_iec559);
        CHECK_EQ(Result::IsBounded, Expected::is_bounded);
        CHECK_EQ(Result::IsModulo, Expected::is_modulo);
        CHECK_EQ(Result::Digits, Expected::digits);
        CHECK_EQ(Result::Digits10, Expected::digits10);
        CHECK_EQ(Result::MaxDigits10, Expected::max_digits10);
        CHECK_EQ(Result::Radix, Expected::radix);
        CHECK_EQ(Result::MinExponent, Expected::min_exponent);
        CHECK_EQ(Result::MinExponent10, Expected::min_exponent10);
        CHECK_EQ(Result::Traps, Expected::traps);
        CHECK_EQ(Result::Min(), Expected::min());
        CHECK_EQ(Result::Lowest(), Expected::lowest());
        CHECK_EQ(Result::Max(), Expected::max());
        CHECK_EQ(Result::Epsilon(), Expected::epsilon());
        CHECK_EQ(Result::RoundError(), Expected::round_error());
        CHECK_EQ(Result::Infinity(), Expected::infinity());
        CHECK_EQ(std::isnan(Result::QuietNaN()), std::isnan(Expected::quiet_NaN()));
        CHECK_EQ(std::isnan(Result::SignalingNaN()), std::isnan(Expected::signaling_NaN()));
    }
}