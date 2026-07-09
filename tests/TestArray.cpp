// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s test_array.cpp
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// Some tests have been adapted and extended by Artem Bezruchko (WardenHD)
// to improve coverage and match WardenSTL's implementation.
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Array.hpp>
#include <array>

#include "Utils.hpp"


TEST_SUITE("Array") {    
    static const size_t SIZE = 10;

    using Data = wstl::Array<int, SIZE>;
    using CompareData = std::array<int, SIZE>;
    using ZeroData = wstl::Array<int, 0>;
    
    CompareData compareData = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    CompareData swapData = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    TEST_CASE("Constructor") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        
        CHECK_EQ(data.Size(), SIZE);
        CHECK_EQ(data.MaxSize(), SIZE);
    }

    TEST_CASE("Zero-sized constructor") {
        ZeroData data;

        CHECK(data.Empty());
        CHECK_EQ(data.Size(), 0UL);
        CHECK_EQ(data.MaxSize(), 0UL);
    }

    TEST_CASE("Assignment") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        Data otherData;

        data = otherData;

        bool equal = std::equal(data.Begin(), data.End(), otherData.Begin());
        CHECK(equal);
    }

    TEST_CASE("At") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        for(size_t i = 0; i < data.Size(); ++i) CHECK_EQ(data.At(i), compareData.at(i));

        CHECK_THROWS_AS({ int i = data.At(data.Size()); (void) i; }, wstl::OutOfRange);
    }

    TEST_CASE("Index operator") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        for(size_t i = 0; i < data.Size(); ++i) CHECK_EQ(data[i], compareData[i]);

        CHECK_THROWS_AS({ int i = data[data.Size()]; (void) i; }, wstl::OutOfRange);
    }

    TEST_CASE("Front") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        int& ref = data.Front();
        const int& constRef = data.Front();
        CHECK_EQ(ref, compareData.front());
        CHECK_EQ(constRef, compareData.front());

        ++ref;
        CHECK_EQ(ref, compareData.front() + 1);
    }

    TEST_CASE("Back") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        int& ref = data.Back();
        const int& constRef = data.Back();
        CHECK_EQ(ref, compareData.back());
        CHECK_EQ(constRef, compareData.back());

        ++ref;
        CHECK_EQ(ref, compareData.back() + 1);
    }

    TEST_CASE("Data") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        const Data constData = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        bool equal = std::equal(data.Begin(), data.End(), data.Data());
        CHECK(equal);

        equal = std::equal(constData.Begin(), constData.End(), constData.Data());
        CHECK(equal);
    }

    TEST_CASE("Begin") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        CHECK_EQ(data.Begin(), &data[0]);
    }

    TEST_CASE("ConstBegin") {
        const Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        CHECK_EQ(data.ConstBegin(), &data[0]);
    }

    TEST_CASE("End") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        CHECK_EQ(data.End(), data.Data() + SIZE);
    }

    TEST_CASE("ConstEnd") {
        const Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        CHECK_EQ(data.ConstEnd(), data.Data() + SIZE);
    }

    TEST_CASE("ReverseBegin") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        CHECK_EQ(data.ReverseBegin(), Data::ReverseIterator(data.Data() + SIZE));
    }

    TEST_CASE("ConstReverseBegin") {
        const Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        CHECK_EQ(data.ConstReverseBegin(), Data::ConstReverseIterator(data.Data() + SIZE));
    }

    TEST_CASE("ReverseEnd") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        CHECK_EQ(data.ReverseEnd(), Data::ReverseIterator(data.Data()));
    }

    TEST_CASE("ConstReverseEnd") {
        const Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        CHECK_EQ(data.ConstReverseEnd(), Data::ConstReverseIterator(data.Data()));
    }

    TEST_CASE("Range support") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        for(int& value : data) CHECK_EQ(value, compareData[size_t(&value - &data[0])]);
    }

    TEST_CASE("Iterator") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        bool result = wstl::Equal(data.Begin(), data.End(), compareData.begin());
        CHECK(result);
    }

    TEST_CASE("ConstIterator") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        bool result = wstl::Equal(data.ConstBegin(), data.ConstEnd(), compareData.cbegin());
        CHECK(result);
    }

    TEST_CASE("ReverseIterator") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        bool result = wstl::Equal(data.ReverseBegin(), data.ReverseEnd(), compareData.rbegin());
        CHECK(result);
    }

    TEST_CASE("ConstReverseIterator") {
        Data data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        bool result = wstl::Equal(data.ConstReverseBegin(), data.ConstReverseEnd(), compareData.crbegin());
        CHECK(result);
    }

    TEST_CASE("Empty") {
        Data data = {0};
        CHECK_FALSE(data.Empty());
    }

    TEST_CASE("Size") {
        Data data = {0};
        CHECK_EQ(data.Size(), SIZE);
    }

    TEST_CASE("MaxSize") {
        Data data = {0};
        CHECK_EQ(data.MaxSize(), SIZE);
    }

    TEST_CASE("Fill") {
        Data data = {0};
        data.Fill(1);

        CompareData compare;
        compare.fill(1);

        bool result = std::equal(data.Begin(), data.End(), compare.begin());
        CHECK(result);
    }

    TEST_CASE("Swap") {
        Data data1 = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        Data data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        wstl::Swap(data1, data2); // Effectively calls data1.Swap(data2);

        CHECK(std::equal(compareData.begin(), compareData.end(), data1.Begin()));
        CHECK(std::equal(swapData.begin(), swapData.end(), data2.Begin()));
    }

    TEST_CASE("Assign") {
        int initial[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        int source[] = {0, 1, 2, 3, 4};
        int check1[] = {0, 1, 2, 3, 4, -1, -1, -1, -1, -1};
        int check2[] = {0, 1, 2, 3, 4, 67, 67, 67, 67, 67};

        Data data = {0};

        // Initial data
        data.Assign(std::begin(initial), std::end(initial));
        bool equal = std::equal(data.Begin(), data.End(), std::begin(initial));
        CHECK(equal);

        // Smaller
        data.Assign(std::begin(source), std::end(source));
        equal = std::equal(data.Begin(), data.End(), std::begin(check1));
        CHECK(equal);

        // Smaller, default
        data.Assign(std::begin(source), std::end(source), 67);
        equal = std::equal(data.Begin(), data.End(), std::begin(check2));
        CHECK(equal);
    }

    TEST_CASE("Insert") {
        int initial[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        int range1[] = {10, 20, 30};
        int range2[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110};

        int checkSingle1[] = {67, 0, 1, 2, 3, 4, 5, 6, 7, 8};
        int checkSingle2[] = {0, 1, 2, 3, 4, 67, 5, 6, 7, 8};
        int checkSingle3[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 67};
        int checkRange1[] = {10, 20, 30, 0, 1, 2, 3, 4, 5, 6};
        int checkRange2[] = {0, 1, 2, 3, 4, 10, 20, 30, 5, 6};
        int checkRange3[] = {0, 1, 2, 3, 4, 5, 6, 10, 20, 30};
        int checkRange4[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        int checkRange5[] = {0, 1, 2, 3, 4, 10, 20, 30, 40, 50};

        Data data = {0};
        Data::Iterator result;

        // Single, begin
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Insert(data.Begin(), 67);
        CHECK_EQ(data[0], *result);

        bool equal = std::equal(data.Begin(), data.End(), std::begin(checkSingle1));
        CHECK(equal);

        // Single, middle
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Insert(data.Begin() + 5, 67);
        CHECK_EQ(data[5], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkSingle2));
        CHECK(equal);

        // Single, end
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Insert(data.End() - 1, 67);
        CHECK_EQ(data[9], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkSingle3));
        CHECK(equal);

        // Single, out of range
        CHECK_THROWS_AS({ result = data.Insert(data.End(), 67); }, wstl::OutOfRange);

        // Small range, begin
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Insert(data.Begin(), std::begin(range1), std::end(range1));
        CHECK_EQ(data[0], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkRange1));
        CHECK(equal);

        // Small range, middle
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Insert(data.Begin() + 5, std::begin(range1), std::end(range1));
        CHECK_EQ(data[5], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkRange2));
        CHECK(equal);

        // Small range, end
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Insert(data.Begin() + 7, std::begin(range1), std::end(range1));
        CHECK_EQ(data[7], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkRange3));
        CHECK(equal);

        // Large range, begin
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Insert(data.Begin(), std::begin(range2), std::end(range2));
        CHECK_EQ(data[0], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkRange4));
        CHECK(equal);

        // Large range, middle
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Insert(data.Begin() + 5, std::begin(range2), std::end(range2));
        CHECK_EQ(data[5], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkRange5));
        CHECK(equal);
    }

    TEST_CASE("Erase") {
        int initial[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        int checkSingle1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 9};
        int checkSingle2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 67};
        int checkSingle3[] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 9};
        int checkSingle4[] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 67};
        int checkSingle5[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        int checkSingle6[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 67};
        int checkRange1[] = {4, 5, 6, 7, 8, 9, 6, 7, 8, 9};
        int checkRange2[] = {4, 5, 6, 7, 8, 9, 67, 67, 67, 67};
        int checkRange3[] = {0, 1, 2, 3, 7, 8, 9, 7, 8, 9};
        int checkRange4[] = {0, 1, 2, 3, 7, 8, 9, 67, 67, 67};
        int checkRange5[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        int checkRange6[] = {0, 1, 2, 3, 4, 5, 6, 67, 67, 67};

        Data data = {0};
        Data::Iterator result;

        // Single, begin
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.Begin());
        CHECK_EQ(data[0], *result);

        bool equal = std::equal(data.Begin(), data.End(), std::begin(checkSingle1));
        CHECK(equal);

        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.Begin(), 67);
        CHECK_EQ(data[0], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkSingle2));
        CHECK(equal);

        // Single, middle
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.Begin() + 5);
        CHECK_EQ(data[5], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkSingle3));
        CHECK(equal);

        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.Begin() + 5, 67);
        CHECK_EQ(data[5], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkSingle4));
        CHECK(equal);

        // Single, end
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.End() - 1);
        CHECK_EQ(data[9], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkSingle5));
        CHECK(equal);

        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.End() - 1, 67);
        CHECK_EQ(data[9], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkSingle6));
        CHECK(equal);

        // Single, out of range
        CHECK_THROWS_AS({ result = data.Erase(data.End(), 67); }, wstl::OutOfRange);

        // Range, begin
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.Begin(), data.Begin() + 4);
        CHECK_EQ(data[0], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkRange1));
        CHECK(equal);

        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.Begin(), data.Begin() + 4, 67);
        CHECK_EQ(data[0], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkRange2));
        CHECK(equal);

        // Range, middle
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.Begin() + 4, data.Begin() + 7);
        CHECK_EQ(data[4], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkRange3));
        CHECK(equal);

        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.Begin() + 4, data.Begin() + 7, 67);
        CHECK_EQ(data[4], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkRange4));
        CHECK(equal);

        // Range, end
        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.End() - 3, data.End());
        CHECK_EQ(data[7], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkRange5));
        CHECK(equal);

        data.Assign(std::begin(initial), std::end(initial));
        result = data.Erase(data.End() - 3, data.End(), 67);
        CHECK_EQ(data[7], *result);

        equal = std::equal(data.Begin(), data.End(), std::begin(checkRange6));
        CHECK(equal);

        // Range, out of range
        CHECK_THROWS_AS({ result = data.Erase(data.Begin() + 5, data.End() + 1, 67); }, wstl::OutOfRange);

        // Range, first greater than last
        CHECK_THROWS_AS({ result = data.Erase(data.Begin() + 1, data.Begin(), 67); }, wstl::OutOfRange);
    }

    TEST_CASE("TupleElement") {
        CHECK(wstl::IsSame<wstl::TupleElementType<0, Data>, int>::Value);
        CHECK(wstl::IsSame<wstl::TupleElementType<3, Data>, int>::Value);
        CHECK(wstl::IsSame<wstl::TupleElementType<9, Data>, int>::Value);

        // The following line should fail with a compilation error
        // CHECK(wstl::IsSame<wstl::TupleElementType<10, Data>, int>::Value);
    }

    TEST_CASE("TupleSize") {
        CHECK_EQ(wstl::TupleSize<Data>::Value, SIZE);
        CHECK_EQ(wstl::TupleSize<wstl::Array<std::string, 5>>::Value, 5UL);
        CHECK_EQ(wstl::TupleSize<ZeroData>::Value, 0UL);
    }

    TEST_CASE("Get") {
        Data data1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        const Data data2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        CHECK_EQ(wstl::Get<3>(data1), data1[3]);
        CHECK_EQ(wstl::Get<5>(data2), data2[5]);

        // The following line should fail with a compilation error
        // int i = wstl::Get<11>(data1);
    }

    TEST_CASE("Equal") {
        Data data1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        Data data2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        CHECK_EQ(data1, data2);

        __WSTL_CONSTEXPR14__ Data data3 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        __WSTL_CONSTEXPR14__ Data data4 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        __WSTL_CONSTEXPR14__ bool result = (data3 == data4);
        CHECK(result);
    }

    TEST_CASE("Not equal") {
        Data data1 = {0, 1, 2, 3, 4, 5, 6, 6, 8, 9};
        Data data2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        CHECK(data1 != data2);

        __WSTL_CONSTEXPR14__ Data data3 = {0, 1, 2, 3, 4, 5, 6, 6, 8, 9};
        __WSTL_CONSTEXPR14__ Data data4 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        __WSTL_CONSTEXPR14__ bool result = (data3 != data4);
        CHECK(result);
    }

    TEST_CASE("Less than") {
        Data data1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        Data greater1 = {0, 1, 2, 3, 4, 5, 6, 8, 8, 9};
        Data less1 = {0, 1, 2, 3, 4, 4, 6, 7, 8, 9};

        CHECK(less1 < data1);
        CHECK(!(data1 < data1));
        CHECK(!(greater1 < data1));

        __WSTL_CONSTEXPR14__ Data data2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        __WSTL_CONSTEXPR14__ Data greater2 = {0, 1, 2, 3, 4, 5, 6, 8, 8, 9};
        __WSTL_CONSTEXPR14__ Data less2 = {0, 1, 2, 3, 4, 4, 6, 7, 8, 9};

        __WSTL_CONSTEXPR14__ bool result1 = (less2 < data2);
        __WSTL_CONSTEXPR14__ bool result2 = !(data2 < data2);
        __WSTL_CONSTEXPR14__ bool result3 = !(greater2 < data2);

        CHECK(result1);
        CHECK(result2);
        CHECK(result3);
    }

    TEST_CASE("Less than or equal") {
        Data data1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        Data greater1 = {0, 1, 2, 3, 4, 5, 6, 8, 8, 9};
        Data less1 = {0, 1, 2, 3, 4, 4, 6, 7, 8, 9};

        CHECK(less1 <= data1);
        CHECK(data1 <= data1);
        CHECK(!(greater1 <= data1));

        __WSTL_CONSTEXPR14__ Data data2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        __WSTL_CONSTEXPR14__ Data greater2 = {0, 1, 2, 3, 4, 5, 6, 8, 8, 9};
        __WSTL_CONSTEXPR14__ Data less2 = {0, 1, 2, 3, 4, 4, 6, 7, 8, 9};

        __WSTL_CONSTEXPR14__ bool result1 = (less2 <= data2);
        __WSTL_CONSTEXPR14__ bool result2 = (data2 <= data2);
        __WSTL_CONSTEXPR14__ bool result3 = !(greater2 <= data2);

        CHECK(result1);
        CHECK(result2);
        CHECK(result3);
    }

    TEST_CASE("Greater than") {
        Data data1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        Data greater1 = {0, 1, 2, 3, 4, 5, 6, 8, 8, 9};
        Data less1 = {0, 1, 2, 3, 4, 4, 6, 7, 8, 9};

        CHECK(!(less1 > data1));
        CHECK(!(data1 > data1));
        CHECK(greater1 > data1);

        __WSTL_CONSTEXPR14__ Data data2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        __WSTL_CONSTEXPR14__ Data greater2 = {0, 1, 2, 3, 4, 5, 6, 8, 8, 9};
        __WSTL_CONSTEXPR14__ Data less2 = {0, 1, 2, 3, 4, 4, 6, 7, 8, 9};

        __WSTL_CONSTEXPR14__ bool result1 = !(less2 > data2);
        __WSTL_CONSTEXPR14__ bool result2 = !(data2 > data2);
        __WSTL_CONSTEXPR14__ bool result3 = (greater2 > data2);

        CHECK(result1);
        CHECK(result2);
        CHECK(result3);
    }

    TEST_CASE("Greater than or equal") {
        Data data1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        Data greater1 = {0, 1, 2, 3, 4, 5, 6, 8, 8, 9};
        Data less1 = {0, 1, 2, 3, 4, 4, 6, 7, 8, 9};

        CHECK(!(less1 >= data1));
        CHECK(data1 >= data1);
        CHECK(greater1 >= data1);

        __WSTL_CONSTEXPR14__ Data data2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        __WSTL_CONSTEXPR14__ Data greater2 = {0, 1, 2, 3, 4, 5, 6, 8, 8, 9};
        __WSTL_CONSTEXPR14__ Data less2 = {0, 1, 2, 3, 4, 4, 6, 7, 8, 9};

        __WSTL_CONSTEXPR14__ bool result1 = !(less2 >= data2);
        __WSTL_CONSTEXPR14__ bool result2 = (data2 >= data2);
        __WSTL_CONSTEXPR14__ bool result3 = (greater2 >= data2);

        CHECK(result1);
        CHECK(result2);
        CHECK(result3);
    }

    TEST_CASE("ToArray") {
        auto data = wstl::ToArray({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

        bool equal = std::equal(compareData.begin(), compareData.end(), data.Begin());
        CHECK(equal);
    }

    TEST_CASE("ToArray movable") {
        typedef MovableData<int> Item;
        wstl::Array<Item, 5> compare = {Item(0), Item(1), Item(2), Item(3), Item(4)};
        
        auto data = wstl::ToArray({Item(0), Item(1), Item(2), Item(3), Item(4)});

        bool equal = std::equal(compare.Begin(), compare.End(), data.Begin());
        CHECK(equal);
    }

    #ifdef __WSTL_CXX14__
    typedef wstl::Array<int, 6> ArrayType;

    constexpr ArrayType FillArray(int i) noexcept {
        ArrayType a{};
        a.Fill(i);
        return a;
    }

    constexpr ArrayType SwapArrays(ArrayType a, ArrayType b) noexcept {
        a.Swap(b);
        return a;
    }

    TEST_CASE("Constexpr14") {
        constexpr ArrayType data{1, 2, 3, 4, 5, 6};

        // index operator
        constexpr int i0 = data[0];
        constexpr int i1 = data[1];
        constexpr int i2 = data[2];
        constexpr int i3 = data[3];
        constexpr int i4 = data[4];
        constexpr int i5 = data[5];
        CHECK_EQ(i0, data[0]);
        CHECK_EQ(i1, data[1]);
        CHECK_EQ(i2, data[2]);
        CHECK_EQ(i3, data[3]);
        CHECK_EQ(i4, data[4]);
        CHECK_EQ(i5, data[5]);

        // Front & Back
        constexpr int f0 = data.Front();
        constexpr int b5 = data.Back();
        CHECK_EQ(f0, data[0]);
        CHECK_EQ(b5, data[5]);

        // Begin, ConstBegin & End, ConstEnd
        constexpr int b0 = *data.Begin();
        constexpr int cb0 = *data.ConstBegin();
        CHECK_EQ(b0, data[0]);
        CHECK_EQ(cb0, data[0]);

        constexpr int e5 = *(data.End() - 1);
        constexpr int ce5 = *(data.ConstEnd() - 1);
        CHECK_EQ(e5, data[5]);
        CHECK_EQ(ce5, data[5]);

        // ReverseBegin, ConstReverseBegin & ReverseEnd, ConstReverseEnd
        constexpr int rb5 = *data.ReverseBegin();
        constexpr int crb5 = *data.ConstReverseBegin();
        CHECK_EQ(rb5, data[5]);
        CHECK_EQ(crb5, data[5]);

        constexpr int re0 = *(data.ReverseEnd() - 1);
        constexpr int cre0 = *(data.ConstReverseEnd() - 1);
        CHECK_EQ(re0, data[0]);
        CHECK_EQ(cre0, data[0]);

        // Data
        constexpr int d0 = *data.Data();
        CHECK_EQ(d0, data[0]);

        // Empty
        constexpr bool e = data.Empty();
        CHECK_FALSE(e);

        // Size
        constexpr size_t s = data.Size();
        CHECK_EQ(s, 6UL);

        // MaxSize
        constexpr size_t ms = data.MaxSize();
        CHECK_EQ(ms, 6UL);

        // Fill
        constexpr ArrayType a = FillArray(5);
        CHECK_EQ(a[0], 5);
        CHECK_EQ(a[1], 5);
        CHECK_EQ(a[2], 5);
        CHECK_EQ(a[3], 5);
        CHECK_EQ(a[4], 5);
        CHECK_EQ(a[5], 5);

        // Swap
        constexpr ArrayType data1{1, 2, 3, 4, 5, 6};
        constexpr ArrayType data2{6, 5, 4, 3, 2, 1};
        constexpr ArrayType data3 = SwapArrays(data1, data2);
        CHECK(std::equal(data2.Begin(), data2.End(), data3.Begin()));
    }
    #endif
}