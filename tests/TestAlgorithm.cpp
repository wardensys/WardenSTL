// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s test_algorithm.cpp
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// Some tests have been adapted and extended by Artem Bezruchko (WardenHD)
// to improve coverage and match WardenSTL's implementation.
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <algorithm>
#include <list>
#include <iterator>
#include <functional>
#include <forward_list>
#include <numeric>
#include <random>
#include <memory>

#include <wstl/Algorithm.hpp>
#include <wstl/Bit.hpp>

#include "Utils.hpp"


namespace {
    std::random_device rd;
    std::mt19937 urng(rd());

    constexpr size_t SIZE = 9;

    int dataA[SIZE] = {4, 5, 7, 1, 10, 6, 3, 7, 7};
    int dataB[SIZE] = {1, 60, 4, 3, 9, 10, 5, 4, 10};

    std::list<int> dataLA(std::begin(dataA), std::end(dataA));
    std::list<int> dataLB(std::begin(dataB), std::end(dataB));

    constexpr size_t NONTRIVIAL_SIZE = 7;

    NonTrivialData dataN[NONTRIVIAL_SIZE] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}};
}

TEST_SUITE("Algorithm") {
    TEST_CASE("Find") {
        int* it1 = std::find(std::begin(dataA), std::end(dataA), 3);
        int* it2 = wstl::Find(std::begin(dataA), std::end(dataA), 3);

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("FindIf") {
        auto predicate = [](int x) { return x == 5; };

        int* it1 = std::find_if(std::begin(dataA), std::end(dataA), predicate);
        int* it2 = wstl::FindIf(std::begin(dataA), std::end(dataA), predicate);

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("FindIfNot") {
        auto predicate = [](int x) { return x == 3; };

        int* it1 = std::find_if_not(std::begin(dataA), std::end(dataA), predicate);
        int* it2 = wstl::FindIfNot(std::begin(dataA), std::end(dataA), predicate);

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("AllOf") {
        auto truePredicate = [](int x) { return x > 0; };
        auto falsePredicate = [](int x) { return x > 3; };

        bool expected = std::all_of(std::begin(dataA), std::end(dataA), truePredicate);
        bool result = wstl::AllOf(std::begin(dataA), std::end(dataA), truePredicate);

        CHECK_EQ(expected, result);

        expected = std::all_of(std::begin(dataA), std::end(dataA), falsePredicate);
        result = wstl::AllOf(std::begin(dataA), std::end(dataA), falsePredicate);

        CHECK_EQ(expected, result);
    }

    TEST_CASE("AnyOf") {
        auto truePredicate = [](int x) { return x < 3; };
        auto falsePredicate = [](int x) { return x < 0; };

        bool expected = std::any_of(std::begin(dataA), std::end(dataA), truePredicate);
        bool result = wstl::AnyOf(std::begin(dataA), std::end(dataA), truePredicate);

        CHECK_EQ(expected, result);

        expected = std::any_of(std::begin(dataA), std::end(dataA), falsePredicate);
        result = wstl::AnyOf(std::begin(dataA), std::end(dataA), falsePredicate);

        CHECK_EQ(expected, result);
    }

    TEST_CASE("NoneOf") {
        auto truePredicate = [](int x) { return x < 0; };
        auto falsePredicate = [](int x) { return x < 3; };

        bool expected = std::none_of(std::begin(dataA), std::end(dataA), truePredicate);
        bool result = wstl::NoneOf(std::begin(dataA), std::end(dataA), truePredicate);

        CHECK_EQ(expected, result);

        expected = std::none_of(std::begin(dataA), std::end(dataA), falsePredicate);
        result = wstl::NoneOf(std::begin(dataA), std::end(dataA), falsePredicate);

        CHECK_EQ(expected, result);
    }

    TEST_CASE("ForEach") {
        int data[] = {4, 3, 1, 6, 7, 4};
        int expected[] = {8, 6, 2, 12, 14, 8};

        wstl::ForEach(std::begin(data), std::end(data), [](int& x) { x *= 2; });

        CHECK(std::equal(std::begin(data), std::end(data), std::begin(expected)));
    }

    TEST_CASE("ForEachInRange") {
        int data[] = {4, 3, 1, 6, 7, 4};
        int expected[] = {8, 6, 2, 12, 14, 8};

        wstl::ForEachInRange(std::begin(data), 6, [](int& x) { return x *= 2; });

        CHECK(std::equal(std::begin(data), std::end(data), std::begin(expected)));
    }

    TEST_CASE("Count") {
        ptrdiff_t c1 = wstl::Count(std::begin(dataA), std::end(dataA), 7);
        ptrdiff_t c2 = std::count(std::begin(dataA), std::end(dataA), 7);

        CHECK_EQ(c1, c2);
    }

    TEST_CASE("CountIf") {
        auto predicate = [](int x) { return x % 2 == 0; };

        ptrdiff_t c1 = wstl::CountIf(std::begin(dataA), std::end(dataA), predicate);
        ptrdiff_t c2 = std::count_if(std::begin(dataA), std::end(dataA), predicate);

        CHECK_EQ(c1, c2);
    }

    TEST_CASE("Mismatch") {
        int data1[] = {1, 2, 3, 4, 5, 7, 8};
        int data2[] = {1, 2, 3, 4, 5, 6, 7};
        int data3[] = {1, 2, 3, 4, 5, 6, 7, 8};
        auto predicate = [](int a, int b) { return a == b; };

        auto result = wstl::Mismatch(std::begin(data1), std::end(data1), std::begin(data2));
        auto expected = std::mismatch(std::begin(data1), std::end(data1), std::begin(data2));

        CHECK_EQ(result.First, expected.first);
        CHECK_EQ(result.Second, expected.second);

        result = wstl::Mismatch(std::begin(data1), std::end(data1), std::begin(data2), predicate);
        expected = std::mismatch(std::begin(data1), std::end(data1), std::begin(data2), predicate);

        CHECK_EQ(result.First, expected.first);
        CHECK_EQ(result.Second, expected.second);

        result = wstl::Mismatch(std::begin(data2), std::end(data2), std::begin(data3));
        expected = std::mismatch(std::begin(data2), std::end(data2), std::begin(data3));

        CHECK_EQ(result.First, expected.first);
        CHECK_EQ(result.Second, expected.second);

        #ifdef __WSTL_CXX14__
        result = wstl::Mismatch(std::begin(data2), std::end(data2), std::begin(data3), std::end(data3));
        expected = std::mismatch(std::begin(data2), std::end(data2), std::begin(data3), std::end(data3));

        CHECK_EQ(result.First, expected.first);
        CHECK_EQ(result.Second, expected.second);

        result = wstl::Mismatch(std::begin(data2), std::end(data2), std::begin(data3), std::end(data3), predicate);
        expected = std::mismatch(std::begin(data2), std::end(data2), std::begin(data3), std::end(data3), predicate);

        CHECK_EQ(result.First, expected.first);
        CHECK_EQ(result.Second, expected.second);
        #endif
    }

    TEST_CASE("Search") {
        std::string alphabet = "ABCDFEGHIJKLMNOPQRSTUVWXYZ";
        std::string sequence = "KLMNO";

        std::string::iterator it1 = wstl::Search(alphabet.begin(), alphabet.end(), sequence.begin(), sequence.end());
        std::string::iterator it2 = std::search(alphabet.begin(), alphabet.end(), sequence.begin(), sequence.end());

        CHECK_EQ(it1, it2);

        it1 = wstl::Search(alphabet.begin(), alphabet.end(), sequence.begin(), sequence.end(), std::equal_to<char>());
        it2 = std::search(alphabet.begin(), alphabet.end(), sequence.begin(), sequence.end(), std::equal_to<char>());

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("SearchInRange") {
        std::string str = "AABBBCCCDDDDDEFFGG";
        
        std::string::iterator it1 = wstl::SearchInRange(str.begin(), str.end(), 2, 'D');
        std::string::iterator it2 = std::search_n(str.begin(), str.end(), 2, 'D');

        CHECK_EQ(it1, it2);

        it1 = wstl::SearchInRange(str.begin(), str.end(), 2, 'E', std::equal_to<char>());
        it2 = std::search_n(str.begin(), str.end(), 2, 'E', std::equal_to<char>());

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("FindEnd") {
        std::string str = "AABBBCCCDDDDDEFFGG";
        std::string sequence = "DD";

        std::string::iterator it1 = wstl::FindEnd(str.begin(), str.end(), sequence.begin(), sequence.end());
        std::string::iterator it2 = std::find_end(str.begin(), str.end(), sequence.begin(), sequence.end());

        CHECK_EQ(it1, it2);

        it1 = wstl::FindEnd(str.begin(), str.end(), sequence.begin(), sequence.end(), std::equal_to<char>());
        it2 = std::find_end(str.begin(), str.end(), sequence.begin(), sequence.end(), std::equal_to<char>());

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("FindFirstOf") {
        std::string alphabet = "ABCDFEGHIJKLMNOPQRSTUVWXYZ";
        std::string sequence = "KEZ";

        std::string::iterator it1 = wstl::FindFirstOf(alphabet.begin(), alphabet.end(), sequence.begin(), sequence.end());
        std::string::iterator it2 = std::find_first_of(alphabet.begin(), alphabet.end(), sequence.begin(), sequence.end());

        CHECK_EQ(it1, it2);

        it1 = wstl::FindFirstOf(alphabet.begin(), alphabet.end(), sequence.begin(), sequence.end(), std::equal_to<char>());
        it2 = std::find_first_of(alphabet.begin(), alphabet.end(), sequence.begin(), sequence.end(), std::equal_to<char>());

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("AdjacentFind") {
        auto predicate = [](int a, int b) { return (a - b) == 1; };

        int* it1 = wstl::AdjacentFind(std::begin(dataA), std::end(dataA));
        int* it2 = std::adjacent_find(std::begin(dataA), std::end(dataA));

        CHECK_EQ(it1, it2);

        it1 = wstl::AdjacentFind(std::begin(dataA), std::end(dataA), predicate);
        it2 = std::adjacent_find(std::begin(dataA), std::end(dataA), predicate);

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("Copy") {
        // POD pointer
        int buffer1[SIZE] = {0};
        int buffer2[SIZE] = {0};

        int* p1 = wstl::Copy(std::begin(dataA), std::end(dataA), std::begin(buffer1));
        int* p2 = std::copy(std::begin(dataA), std::end(dataA), std::begin(buffer2));

        ptrdiff_t d1 = std::distance(buffer1, p1);
        ptrdiff_t d2 = std::distance(buffer2, p2);
        CHECK_EQ(d1, d2);

        bool result = std::equal(std::begin(buffer1), std::end(buffer1), std::begin(buffer2));
        CHECK(result);

        // Non-POD pointer
        NonTrivialData bufferN1[NONTRIVIAL_SIZE];
        NonTrivialData bufferN2[NONTRIVIAL_SIZE];

        NonTrivialData* pn1 = wstl::Copy(std::begin(dataN), std::end(dataN), std::begin(bufferN1));
        NonTrivialData* pn2 = std::copy(std::begin(dataN), std::end(dataN), std::begin(bufferN2));

        d1 = std::distance(bufferN1, pn1);
        d2 = std::distance(bufferN2, pn2);
        CHECK_EQ(d1, d2);

        result = std::equal(std::begin(bufferN1), std::end(bufferN1), std::begin(bufferN2));
        CHECK(result);

        // Non-random iterator
        std::list<int> list1(SIZE);
        std::list<int> list2(SIZE);

        std::list<int>::iterator pl1 = wstl::Copy(std::begin(dataLA), std::end(dataLA), std::begin(list1));
        std::list<int>::iterator pl2 = std::copy(std::begin(dataLA), std::end(dataLA), std::begin(list2));

        d1 = std::distance(list1.begin(), pl1);
        d2 = std::distance(list2.begin(), pl2);
        CHECK_EQ(d1, d2);

        result = std::equal(std::begin(list1), std::end(list1), std::begin(list2));
        CHECK(result);
    }

    TEST_CASE("CopyIf") {
        // POD pointer
        int buffer1[SIZE] = {0};
        int buffer2[SIZE] = {0};

        auto predicate = [](int x) { return x % 2 == 0; };

        int* p1 = wstl::CopyIf(std::begin(dataA), std::end(dataA), std::begin(buffer1), predicate);
        int* p2 = std::copy_if(std::begin(dataA), std::end(dataA), std::begin(buffer2), predicate);

        ptrdiff_t d1 = std::distance(buffer1, p1);
        ptrdiff_t d2 = std::distance(buffer2, p2);
        CHECK_EQ(d1, d2);

        bool result = std::equal(std::begin(buffer1), std::end(buffer1), std::begin(buffer2));
        CHECK(result);

        // Non-POD pointer
        NonTrivialData bufferN1[NONTRIVIAL_SIZE];
        NonTrivialData bufferN2[NONTRIVIAL_SIZE];

        auto predicateN = [](NonTrivialData x) { return x.A % 2 == 0; };

        NonTrivialData* pn1 = wstl::CopyIf(std::begin(dataN), std::end(dataN), std::begin(bufferN1), predicateN);
        NonTrivialData* pn2 = std::copy_if(std::begin(dataN), std::end(dataN), std::begin(bufferN2), predicateN);

        d1 = std::distance(bufferN1, pn1);
        d2 = std::distance(bufferN2, pn2);
        CHECK_EQ(d1, d2);

        result = std::equal(std::begin(bufferN1), std::end(bufferN1), std::begin(bufferN2));
        CHECK(result);

        // Non-random iterator
        std::list<int> list1(SIZE);
        std::list<int> list2(SIZE);

        std::list<int>::iterator pl1 = wstl::CopyIf(std::begin(dataLA), std::end(dataLA), std::begin(list1), predicate);
        std::list<int>::iterator pl2 = std::copy_if(std::begin(dataLA), std::end(dataLA), std::begin(list2), predicate);

        d1 = std::distance(list1.begin(), pl1);
        d2 = std::distance(list2.begin(), pl2);
        CHECK_EQ(d1, d2);

        result = std::equal(std::begin(list1), std::end(list1), std::begin(list2));
        CHECK(result);
    }

    TEST_CASE("CopyInRange") {
        // POD pointer
        int buffer1[SIZE] = {0};
        int buffer2[SIZE] = {0};

        int* p1 = wstl::CopyInRange(std::begin(dataA), SIZE, std::begin(buffer1));
        int* p2 = std::copy_n(std::begin(dataA), SIZE, std::begin(buffer2));

        ptrdiff_t d1 = std::distance(buffer1, p1);
        ptrdiff_t d2 = std::distance(buffer2, p2);
        CHECK_EQ(d1, d2);

        bool result = std::equal(std::begin(buffer1), std::end(buffer1), std::begin(buffer2));
        CHECK(result);

        // Non-POD pointer
        NonTrivialData bufferN1[NONTRIVIAL_SIZE];
        NonTrivialData bufferN2[NONTRIVIAL_SIZE];

        NonTrivialData* pn1 = wstl::CopyInRange(std::begin(dataN), NONTRIVIAL_SIZE, std::begin(bufferN1));
        NonTrivialData* pn2 = std::copy_n(std::begin(dataN), NONTRIVIAL_SIZE, std::begin(bufferN2));

        d1 = std::distance(bufferN1, pn1);
        d2 = std::distance(bufferN2, pn2);
        CHECK_EQ(d1, d2);

        result = std::equal(std::begin(bufferN1), std::end(bufferN1), std::begin(bufferN2));
        CHECK(result);

        // Non-random iterator
        std::list<int> list1(SIZE);
        std::list<int> list2(SIZE);

        std::list<int>::iterator pl1 = wstl::CopyInRange(std::begin(dataLA), SIZE, std::end(list1));
        std::list<int>::iterator pl2 = std::copy_n(std::begin(dataLA), SIZE, std::end(list2));

        d1 = std::distance(list1.begin(), pl1);
        d2 = std::distance(list2.begin(), pl2);
        CHECK_EQ(d1, d2);

        result = std::equal(std::begin(list1), std::end(list1), std::begin(list2));
        CHECK(result);
    }

    TEST_CASE("CopyBackward") {
        // POD pointer
        int buffer1[SIZE] = {0};
        int buffer2[SIZE] = {0};

        int* p1 = wstl::CopyBackward(std::begin(dataA), std::end(dataA), std::end(buffer1));
        int* p2 = std::copy_backward(std::begin(dataA), std::end(dataA), std::end(buffer2));

        ptrdiff_t d1 = std::distance(buffer1, p1);
        ptrdiff_t d2 = std::distance(buffer2, p2);
        CHECK_EQ(d1, d2);

        bool result = std::equal(std::begin(buffer1), std::end(buffer1), std::begin(buffer2));
        CHECK(result);

        // Non-POD pointer
        NonTrivialData bufferN1[NONTRIVIAL_SIZE];
        NonTrivialData bufferN2[NONTRIVIAL_SIZE];

        NonTrivialData* pn1 = wstl::CopyBackward(std::begin(dataN), std::end(dataN), std::end(bufferN1));
        NonTrivialData* pn2 = std::copy_backward(std::begin(dataN), std::end(dataN), std::end(bufferN2));

        d1 = std::distance(bufferN1, pn1);
        d2 = std::distance(bufferN2, pn2);
        CHECK_EQ(d1, d2);

        result = std::equal(std::begin(bufferN1), std::end(bufferN1), std::begin(bufferN2));
        CHECK(result);

        // Non-random iterator
        std::list<int> list1(SIZE);
        std::list<int> list2(SIZE);

        std::list<int>::iterator pl1 = wstl::CopyBackward(std::begin(dataLA), std::end(dataLA), std::end(list1));
        std::list<int>::iterator pl2 = std::copy_backward(std::begin(dataLA), std::end(dataLA), std::end(list2));

        d1 = std::distance(list1.begin(), pl1);
        d2 = std::distance(list2.begin(), pl2);
        CHECK_EQ(d1, d2);

        result = std::equal(std::begin(list1), std::end(list1), std::begin(list2));
        CHECK(result);
    }

    TEST_CASE("Move") {
        typedef std::vector<std::unique_ptr<uint32_t>> Data;
        Data data1;

        // Create some data
        std::unique_ptr<uint32_t> p1(new uint32_t(1U));
        std::unique_ptr<uint32_t> p2(new uint32_t(2U));
        std::unique_ptr<uint32_t> p3(new uint32_t(3U));
        std::unique_ptr<uint32_t> p4(new uint32_t(4U));
        std::unique_ptr<uint32_t> p5(new uint32_t(5U));

        // Push it
        data1.push_back(std::move(p1));
        data1.push_back(std::move(p2));
        data1.push_back(std::move(p3));
        data1.push_back(std::move(p4));
        data1.push_back(std::move(p5));

        Data data2;
        wstl::Move(data1.begin(), data1.end(), std::back_inserter(data2));

        CHECK(!p1);
        CHECK(!p2);
        CHECK(!p3);
        CHECK(!p4);
        CHECK(!p5);

        CHECK_EQ(*data2[0], 1U);
        CHECK_EQ(*data2[1], 2U);
        CHECK_EQ(*data2[2], 3U);
        CHECK_EQ(*data2[3], 4U);
        CHECK_EQ(*data2[4], 5U);
    }

    TEST_CASE("MoveBackward") {
        typedef std::vector<std::unique_ptr<uint32_t>> Data;
        Data data1;

        // Create some data
        std::unique_ptr<uint32_t> p1(new uint32_t(1U));
        std::unique_ptr<uint32_t> p2(new uint32_t(2U));
        std::unique_ptr<uint32_t> p3(new uint32_t(3U));
        std::unique_ptr<uint32_t> p4(new uint32_t(4U));
        std::unique_ptr<uint32_t> p5(new uint32_t(5U));

        // Push it
        data1.push_back(std::move(p1));
        data1.push_back(std::move(p2));
        data1.push_back(std::move(p3));
        data1.push_back(std::move(p4));
        data1.push_back(std::move(p5));

        Data data2(5);
        wstl::MoveBackward(data1.begin(), data1.end(), data2.end());

        CHECK(!p1);
        CHECK(!p2);
        CHECK(!p3);
        CHECK(!p4);
        CHECK(!p5);

        CHECK_EQ(*data2[0], 1U);
        CHECK_EQ(*data2[1], 2U);
        CHECK_EQ(*data2[2], 3U);
        CHECK_EQ(*data2[3], 4U);
        CHECK_EQ(*data2[4], 5U);
    }

    TEST_CASE("Fill") {
        int buffer[SIZE];

        wstl::Fill(std::begin(buffer), std::end(buffer), 5);
        
        bool result = std::all_of(std::begin(buffer), std::end(buffer), [](int x) { return x == 5; });
        CHECK(result);
    }

    TEST_CASE("FillInRange") {
        int buffer[SIZE];

        wstl::FillInRange(std::begin(buffer), SIZE, 5);
        
        bool result = std::all_of(std::begin(buffer), std::end(buffer), [](int x) { return x == 5; });
        CHECK(result);
    }

    TEST_CASE("Transform") {
        int buffer1[SIZE];
        int buffer2[SIZE];

        auto transform = [](int x) { return x * 2; };

        wstl::Transform(std::begin(dataA), std::end(dataA), buffer1, transform);
        std::transform(std::begin(dataA), std::end(dataA), buffer2, transform);

        bool result = std::equal(std::begin(buffer1), std::end(buffer1), std::begin(buffer2));
        CHECK(result);

        wstl::Transform(std::begin(dataA), std::end(dataA), std::begin(dataB), buffer1, std::plus<int>());
        std::transform(std::begin(dataA), std::end(dataA), std::begin(dataB), buffer2, std::plus<int>());

        result = std::equal(std::begin(buffer1), std::end(buffer1), std::begin(buffer2));
        CHECK(result);
    }

    TEST_CASE("Generate") {
        int buffer1[SIZE];
        int buffer2[SIZE];

        auto generator = []() { return 5; };

        wstl::Generate(std::begin(buffer1), std::end(buffer1), generator);
        std::generate(std::begin(buffer2), std::end(buffer2), generator);

        bool result = std::equal(std::begin(buffer1), std::end(buffer1), std::begin(buffer2));
        CHECK(result);
    }

    TEST_CASE("GenerateInRange") {
        int buffer1[SIZE];
        int buffer2[SIZE];

        auto generator = []() { return 5; };

        wstl::GenerateInRange(std::begin(buffer1), SIZE, generator);
        std::generate_n(std::begin(buffer2), SIZE, generator);

        bool result = std::equal(std::begin(buffer1), std::end(buffer1), std::begin(buffer2));
        CHECK(result);
    }

    TEST_CASE("Remove") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {1, 2, 3, 5, 6, 7};

        wstl::Remove(std::begin(data), std::end(data), 4);

        bool result = std::equal(std::begin(expected), std::end(expected), data);
        CHECK(result);
    }

    TEST_CASE("RemoveIf") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {2, 4, 4, 4, 6};

        auto predicate = [](int x) { return x % 2 != 0; };

        wstl::RemoveIf(std::begin(data), std::end(data), predicate);

        bool result = std::equal(std::begin(expected), std::end(expected), data);
        CHECK(result);
    }

    TEST_CASE("RemoveCopy") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {1, 2, 3, 5, 6, 7};
        int buffer[9];

        wstl::RemoveCopy(std::begin(data), std::end(data), buffer, 4);

        bool result = std::equal(std::begin(expected), std::end(expected), buffer);
        CHECK(result);
    }

    TEST_CASE("RemoveCopyIf") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {2, 4, 4, 4, 6};
        int buffer[9];

        auto predicate = [](int x) { return x % 2 != 0; };

        wstl::RemoveCopyIf(std::begin(data), std::end(data), buffer, predicate);

        bool result = std::equal(std::begin(expected), std::end(expected), buffer);
        CHECK(result);
    }

    TEST_CASE("Replace") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {1, 2, 3, 10, 10, 10, 5, 6, 7};

        wstl::Replace(std::begin(data), std::end(data), 4, 10);

        bool result = std::equal(std::begin(expected), std::end(expected), data);
        CHECK(result);
    }

    TEST_CASE("ReplaceIf") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {10, 2, 10, 4, 4, 4, 10, 6, 10};

        auto predicate = [](int x) { return x % 2 != 0; };

        wstl::ReplaceIf(std::begin(data), std::end(data), predicate, 10);

        bool result = std::equal(std::begin(expected), std::end(expected), data);
        CHECK(result);
    }

    TEST_CASE("ReplaceCopy") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {1, 2, 3, 10, 10, 10, 5, 6, 7};
        int buffer[9];

        wstl::ReplaceCopy(std::begin(data), std::end(data), buffer, 4, 10);

        bool result = std::equal(std::begin(expected), std::end(expected), buffer);
        CHECK(result);
    }

    TEST_CASE("ReplaceCopyIf") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {10, 2, 10, 4, 4, 4, 10, 6, 10};
        int buffer[9];

        auto predicate = [](int x) { return x % 2 != 0; };

        wstl::ReplaceCopyIf(std::begin(data), std::end(data), buffer, predicate, 10);

        bool result = std::equal(std::begin(expected), std::end(expected), buffer);
        CHECK(result);
    }

    TEST_CASE("IteratorSwap") {
        int a = 1;
        int b = 2;

        wstl::IteratorSwap(&a, &b);
        CHECK_EQ(a, 2);
        CHECK_EQ(b, 1);
    }

    TEST_CASE("SwapRanges") {
        int data1[] = {1, 2, 3};
        int data2[] = {4, 5, 6};

        wstl::SwapRanges(std::begin(data1), std::end(data1), std::begin(data2));

        CHECK_EQ(data1[0], 4);
        CHECK_EQ(data1[1], 5);
        CHECK_EQ(data1[2], 6);
        CHECK_EQ(data2[0], 1);
        CHECK_EQ(data2[1], 2);
        CHECK_EQ(data2[2], 3);
    }

    TEST_CASE("Reverse") {
        int data[] = {1, 2, 3, 4, 5};
        int expected[] = {5, 4, 3, 2, 1};

        wstl::Reverse(std::begin(data), std::end(data));

        bool result = std::equal(std::begin(expected), std::end(expected), data);
        CHECK(result);
    }

    TEST_CASE("ReverseCopy") {
        int result[SIZE];
        int expected[SIZE];

        int* ptr1 = wstl::ReverseCopy(std::begin(dataA), std::end(dataA), result);
        int* ptr2 = std::reverse_copy(std::begin(dataA), std::end(dataA), expected);
        
        ptrdiff_t d1 = std::distance(result, ptr1);
        ptrdiff_t d2 = std::distance(expected, ptr2);
        CHECK_EQ(d1, d2);

        bool equal = std::equal(std::begin(expected), std::end(expected), result);
        CHECK(equal);
    }

    TEST_CASE("Rotate") {
        // POD
        std::vector<int> initial = {1, 2, 3, 4, 5, 6, 7};

        for(size_t i = 0; i < initial.size(); ++i) {
            std::vector<int> data1(initial);
            std::vector<int> data2(initial);

            wstl::Rotate(data1.data(), data1.data() + i, data1.data() + data1.size());
            std::rotate(data2.data(), data2.data() + i, data2.data() + data2.size());

            bool result = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(result);
        }

        // Non-POD
        std::vector<NonTrivialData> initial2(std::begin(dataN), std::end(dataN));

        for(size_t i = 0; i < initial2.size(); ++i) {
            std::vector<NonTrivialData> data1(initial2);
            std::vector<NonTrivialData> data2(initial2);

            wstl::Rotate(data1.data(), data1.data() + i, data1.data() + data1.size());
            std::rotate(data2.data(), data2.data() + i, data2.data() + data2.size());

            bool result = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(result);
        }
    }

    TEST_CASE("RotateCopy") {
        // POD
        std::vector<int> initial = {1, 2, 3, 4, 5, 6, 7};

        for(size_t i = 0; i < initial.size(); ++i) {
            std::vector<int> data1(initial.size());
            std::vector<int> data2(initial.size());

            wstl::RotateCopy(initial.data(), initial.data() + i, initial.data() + initial.size(), data1.data());
            std::rotate_copy(initial.data(), initial.data() + i, initial.data() + initial.size(), data2.data());

            bool result = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(result);
        }

        // Non-POD
        std::vector<NonTrivialData> initial2(std::begin(dataN), std::end(dataN));
        
        for(size_t i = 0; i < initial2.size(); ++i) {
            std::vector<NonTrivialData> data1(initial2.size());
            std::vector<NonTrivialData> data2(initial2.size());

            wstl::RotateCopy(initial2.data(), initial2.data() + i, initial2.data() + initial2.size(), data1.data());
            std::rotate_copy(initial2.data(), initial2.data() + i, initial2.data() + initial2.size(), data2.data());

            bool result = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(result);
        }
    }

    TEST_CASE("Unique") {
        int data1[] = {1, 2, 1, 1, 3, 3, 3, 4, 5, 4};
        int data2[] = {1, 2, 1, 1, 3, 3, 3, 4, 5, 4};
        int expected[] = {1, 2, 1, 3, 4, 5, 4};

        wstl::Unique(std::begin(data1), std::end(data1));

        bool result = std::equal(std::begin(expected), std::end(expected), data1);
        CHECK(result);

        wstl::Unique(std::begin(data2), std::end(data2), std::equal_to<int>());

        result = std::equal(std::begin(expected), std::end(expected), data2);
        CHECK(result);
    }

    TEST_CASE("UniqueCopy") {
        int data[] = {1, 2, 1, 1, 3, 3, 3, 4, 5, 4};
        int expected[] = {1, 2, 1, 3, 4, 5, 4};
        int buffer1[10];
        int buffer2[10];

        wstl::UniqueCopy(std::begin(data), std::end(data), buffer1);

        bool result = std::equal(std::begin(expected), std::end(expected), buffer1);
        CHECK(result);

        wstl::UniqueCopy(std::begin(data), std::end(data), buffer2, std::equal_to<int>());

        result = std::equal(std::begin(expected), std::end(expected), buffer2);
        CHECK(result);
    }

    TEST_CASE("IsPartitioned") {
        int data[] = {1, 2, 3, 4, 5, 6, 7, 8};
        auto predicate = std::bind(std::greater<int>(), std::placeholders::_1, 4);

        bool expected = std::is_partitioned(std::begin(data), std::end(data), predicate);
        bool result = wstl::IsPartitioned(std::begin(data), std::end(data), predicate);
        CHECK_EQ(expected, result);

        std::partition(std::begin(data), std::end(data), predicate);

        expected = std::is_partitioned(std::begin(data), std::end(data), predicate);
        result = wstl::IsPartitioned(std::begin(data), std::end(data), predicate);
        CHECK_EQ(expected, result);
    }

    TEST_CASE("Partition") {
        // Forward iterator
        std::array<int, 6> initial = {1, 2, 3, 4, 5, 6};

        std::forward_list<int> expected1(std::begin(initial), std::end(initial));
        std::forward_list<int> data1(std::begin(initial), std::end(initial));

        bool complete = false;

        while(!complete) {
            auto pivot1 = wstl::Partition(data1.begin(), data1.end(), wstl::IsEven<int>);
            auto pivot2 = std::partition(expected1.begin(), expected1.end(), wstl::IsEven<int>);

            auto distance1 = std::distance(data1.begin(), pivot1);
            auto distance2 = std::distance(expected1.begin(), pivot2);

            CHECK_EQ(distance1, distance2);
            CHECK_EQ(*pivot1, *pivot2);

            bool result = std::equal(expected1.begin(), expected1.end(), data1.begin());
            CHECK(result);

            complete = !std::next_permutation(std::begin(initial), std::end(initial));

            expected1.assign(std::begin(initial), std::end(initial));
            data1.assign(std::begin(initial), std::end(initial));
        }

        // Bidirectional iterator
        std::array<int, 6> expected2 = initial;
        std::array<int, 6> data2 = initial;

        complete = false;

        while(!complete) {
            auto pivot1 = wstl::Partition(data2.begin(), data2.end(), wstl::IsEven<int>);
            auto pivot2 = std::partition(expected2.begin(), expected2.end(), wstl::IsEven<int>);

            auto distance1 = std::distance(data2.begin(), pivot1);
            auto distance2 = std::distance(expected2.begin(), pivot2);

            CHECK_EQ(distance1, distance2);
            CHECK_EQ(*pivot1, *pivot2);

            bool result = std::equal(expected2.begin(), expected2.end(), data2.begin());
            CHECK(result);

            complete = !std::next_permutation(std::begin(initial), std::end(initial));

            expected2 = initial;
            data2 = initial;
        }
    }

    TEST_CASE("PartitionCopy") {
        int data1False[SIZE] = {0};
        int data1True[SIZE] = {0};
        int data2False[SIZE] = {0};
        int data2True[SIZE] = {0};

        wstl::PartitionCopy(std::begin(dataA), std::end(dataA), data1True, data1False, std::bind(std::greater<int>(), std::placeholders::_1, 4));
        std::partition_copy(std::begin(dataA), std::end(dataA), data2True, data2False, std::bind(std::greater<int>(), std::placeholders::_1, 4));

        bool result = std::equal(std::begin(data2True), std::end(data2True), std::begin(data1True));
        CHECK(result);

        result = std::equal(std::begin(data2False), std::end(data2False), std::begin(data1False));
        CHECK(result);
    }

    TEST_CASE("StablePartition") {
        std::vector<NonTrivialData> initial = {{1, 1}, {2, 1}, {3, 1}, {2, 2}, {3, 2}, {4, 1}, {2, 3}, {3, 3}, {5, 1}};

        std::vector<NonTrivialData> data1(initial);
        std::vector<NonTrivialData> data2(initial);

        wstl::StablePartition(data1.begin(), data1.end(), [](NonTrivialData& x) { return x.A > 2 && x.B > 2; });
        std::stable_partition(data2.begin(), data2.end(), [](NonTrivialData& x) { return x.A > 2 && x.B > 2; });

        bool result = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(result);       
    }

    TEST_CASE("PartitionPoint") {
        int data[] = {1, 2, 3, 4, 5, 6, 7, 8};

        std::partition(std::begin(data), std::end(data), std::bind(std::greater<int>(), std::placeholders::_1, 4));

        int* p1 = wstl::PartitionPoint(std::begin(data), std::end(data), std::bind(std::greater<int>(), std::placeholders::_1, 4));
        int* p2 = std::partition_point(std::begin(data), std::end(data), std::bind(std::greater<int>(), std::placeholders::_1, 4));
        CHECK_EQ(p1, p2);

        std::partition(std::begin(data), std::end(data), std::bind(std::greater<int>(), std::placeholders::_1, 8));

        p1 = wstl::PartitionPoint(std::begin(data), std::end(data), std::bind(std::greater<int>(), std::placeholders::_1, 8));
        p2 = std::partition_point(std::begin(data), std::end(data), std::bind(std::greater<int>(), std::placeholders::_1, 8));
        CHECK_EQ(p1, p2);
    }

    TEST_CASE("MinElement") {
        // Normal
        int* expected = std::min_element(std::begin(dataA), std::end(dataA));
        int* result = wstl::MinElement(std::begin(dataA), std::end(dataA));
        CHECK_EQ(expected, result);

        // Compare
        expected = std::min_element(std::begin(dataA), std::end(dataA), std::greater<int>());
        result = wstl::MinElement(std::begin(dataA), std::end(dataA), std::greater<int>());
        CHECK_EQ(expected, result);

        // Empty
        std::array<int, 0> empty;

        expected = std::min_element(std::begin(empty), std::end(empty), std::greater<int>());
        result = wstl::MinElement(std::begin(empty), std::end(empty), std::greater<int>());
        CHECK_EQ(expected, result);
    }

    TEST_CASE("Min") {
        int a = 1;
        int b = 2;

        CHECK_EQ(wstl::Min(a, b), 1);
        CHECK_EQ(wstl::Min(a, b, std::greater<int>()), 2);
        CHECK_EQ(wstl::compile::Min<int, 1, 2>::Value, 1);
        CHECK_EQ(wstl::compile::Min<int, 1, 2, wstl::compile::Greater<int>>::Value, 2);
    }

    TEST_CASE("MaxElement") {
        // Normal
        int* expected = std::max_element(std::begin(dataA), std::end(dataA));
        int* result = wstl::MaxElement(std::begin(dataA), std::end(dataA));
        CHECK_EQ(expected, result);

        // Compare
        expected = std::max_element(std::begin(dataA), std::end(dataA), std::greater<int>());
        result = wstl::MaxElement(std::begin(dataA), std::end(dataA), std::greater<int>());
        CHECK_EQ(expected, result);

        // Empty
        std::array<int, 0> empty;

        expected = std::max_element(std::begin(empty), std::end(empty), std::greater<int>());
        result = wstl::MaxElement(std::begin(empty), std::end(empty), std::greater<int>());
        CHECK_EQ(expected, result);
    }

    TEST_CASE("Max") {
        int a = 1;
        int b = 2;

        CHECK_EQ(wstl::Max(a, b), 2);
        CHECK_EQ(wstl::Max(a, b, std::greater<int>()), 1);
        CHECK_EQ(wstl::compile::Max<int, 1, 2>::Value, 2);
        CHECK_EQ(wstl::compile::Max<int, 1, 2, wstl::compile::Greater<int>>::Value, 1);
    }

    TEST_CASE("MinMaxElement") {
        // Normal
        std::pair<int*, int*> expected = std::minmax_element(std::begin(dataA), std::end(dataA));
        wstl::Pair<int*, int*> result = wstl::MinMaxElement(std::begin(dataA), std::end(dataA));
        CHECK_EQ(expected.first, result.First);
        CHECK_EQ(expected.second, result.Second);

        // Compare
        expected = std::minmax_element(std::begin(dataA), std::end(dataA), std::greater<int>());
        result = wstl::MinMaxElement(std::begin(dataA), std::end(dataA), std::greater<int>());
        CHECK_EQ(expected.first, result.First);
        CHECK_EQ(expected.second, result.Second);

        // Empty
        std::array<int, 0> empty;

        expected = std::minmax_element(std::begin(empty), std::end(empty), std::greater<int>());
        result = wstl::MinMaxElement(std::begin(empty), std::end(empty), std::greater<int>());
        CHECK_EQ(expected.first, result.First);
        CHECK_EQ(expected.second, result.Second);
    }

    TEST_CASE("MinMax") {
        int a = 1;
        int b = 2;

        wstl::Pair<int, int> result = wstl::MinMax(a, b);
        int resultMin = wstl::compile::MinMax<int, 1, 2>::Min;
        int resultMax = wstl::compile::MinMax<int, 1, 2>::Max;

        CHECK_EQ(result.First, 1);
        CHECK_EQ(result.Second, 2);
        CHECK_EQ(resultMin, 1);
        CHECK_EQ(resultMax, 2);
    }

    TEST_CASE("Clamp") {
        // Runtime
        CHECK_EQ(wstl::Clamp(5, 0, 10), 5);
        CHECK_EQ(wstl::Clamp(-5, 0, 10), 0);
        CHECK_EQ(wstl::Clamp(15, 0, 10), 10);

        // Compile-time
        CHECK_EQ(wstl::compile::Clamp<int, 5, 0, 10>::Value, 5);
        CHECK_EQ(wstl::compile::Clamp<int, -5, 0, 10>::Value, 0);
        CHECK_EQ(wstl::compile::Clamp<int, 15, 0, 10>::Value, 10);

        // Constexpr
        constexpr int result1 = wstl::Clamp(5, 0, 10);
        constexpr int result2 = wstl::Clamp(-5, 0, 10);
        constexpr int result3 = wstl::Clamp(15, 0, 10);

        CHECK_EQ(result1, 5);
        CHECK_EQ(result2, 0);
        CHECK_EQ(result3, 10);
    }

    TEST_CASE("Equal") {
        int dataSmall[] = {1, 2, 3};

        CHECK(wstl::Equal(std::begin(dataA), std::end(dataA), std::begin(dataLA)));
        CHECK(!wstl::Equal(std::begin(dataA), std::end(dataA), std::begin(dataB)));
        CHECK(wstl::Equal(std::begin(dataA), std::end(dataA), std::begin(dataLA), std::end(dataLA)));
        CHECK(!wstl::Equal(std::begin(dataA), std::end(dataA), std::begin(dataSmall), std::end(dataSmall)));
    }

    TEST_CASE("LexicographicalCompare") {
        std::string text1("Hello World");
        std::string text2("Hello Xorld");

        bool expected = std::lexicographical_compare(text1.begin(), text1.end(), text2.begin(), text2.end());
        bool result = wstl::LexicographicalCompare(text1.begin(), text1.end(), text2.begin(), text2.end());
        CHECK_EQ(expected, result);

        expected = std::lexicographical_compare(text1.begin(), text1.end(), text2.begin(), text2.end(), std::greater<char>());
        result = wstl::LexicographicalCompare(text1.begin(), text1.end(), text2.begin(), text2.end(), std::greater<char>());
        CHECK_EQ(expected, result);
    }

    TEST_CASE("Heap") {
        std::vector<uint32_t> data1 = {1, 2, 3, 4, 5, 6, 7, 8};
        std::vector<uint32_t> data2 = {1, 2, 3, 4, 5, 6, 7, 8};

        wstl::MakeHeap(data1.begin(), data1.end());
        std::make_heap(data2.begin(), data2.end());

        CHECK(std::is_heap(data1.begin(), data1.end()));
        CHECK(std::is_heap(data2.begin(), data2.end()));
        CHECK_EQ(data1.size(), data2.size());

        bool equal = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(equal);

        wstl::PopHeap(data1.begin(), data1.end());
        std::pop_heap(data2.begin(), data2.end());

        data1.pop_back();
        data2.pop_back();

        CHECK(std::is_heap(data1.begin(), data1.end()));
        CHECK(std::is_heap(data2.begin(), data2.end()));

        equal = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(equal);

        data1.push_back(10);
        data2.push_back(10);

        wstl::PushHeap(data1.begin(), data1.end());
        std::push_heap(data2.begin(), data2.end());

        CHECK(std::is_heap(data1.begin(), data1.end()));
        CHECK(std::is_heap(data2.begin(), data2.end()));

        equal = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(equal);

        // Intentionally mess up the heap
        data1.push_back(20);
        data1.push_back(30);
        data2.push_back(20);
        data2.push_back(30);

        std::vector<uint32_t>::iterator result1 = wstl::IsHeapUntil(data1.begin(), data1.end());
        std::vector<uint32_t>::iterator result2 = std::is_heap_until(data2.begin(), data2.end());

        CHECK_EQ(std::distance(data1.begin(), result1), std::distance(data2.begin(), result2));

        equal = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(equal);

        data1.pop_back();
        data1.pop_back();
        data2.pop_back();
        data2.pop_back();

        wstl::SortHeap(data1.begin(), data1.end());
        std::sort_heap(data2.begin(), data2.end());

        equal = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(equal);
    }

    TEST_CASE("Heap movable") {
        typedef MovableData<uint32_t> Item;
        typedef std::vector<Item> Data;
        Data data1;

        // Create some data
        Item p1(1U), p2(2U), p3(3U), p4(4U), p5(5U), p6(6U), p7(7U), p8(8U);

        data1.push_back(std::move(p1));
        data1.push_back(std::move(p2));
        data1.push_back(std::move(p3));
        data1.push_back(std::move(p4));
        data1.push_back(std::move(p5));
        data1.push_back(std::move(p6));
        data1.push_back(std::move(p7));
        data1.push_back(std::move(p8));

        Data data2;
        data2.emplace_back(Item(1U));
        data2.emplace_back(Item(2U));
        data2.emplace_back(Item(3U));
        data2.emplace_back(Item(4U));
        data2.emplace_back(Item(5U));
        data2.emplace_back(Item(6U));
        data2.emplace_back(Item(7U));
        data2.emplace_back(Item(8U));

        wstl::MakeHeap(data1.begin(), data1.end());
        std::make_heap(data2.begin(), data2.end());

        CHECK(std::is_heap(data1.begin(), data1.end()));
        CHECK(std::is_heap(data2.begin(), data2.end()));
        CHECK_EQ(data1.size(), data2.size());

        bool equal = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(equal);

        wstl::PopHeap(data1.begin(), data1.end());
        std::pop_heap(data2.begin(), data2.end());

        data1.pop_back();
        data2.pop_back();

        CHECK(std::is_heap(data1.begin(), data1.end()));
        CHECK(std::is_heap(data2.begin(), data2.end()));

        equal = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(equal);

        data1.push_back(Item(10));
        data2.push_back(Item(10));

        wstl::PushHeap(data1.begin(), data1.end());
        std::push_heap(data2.begin(), data2.end());

        CHECK(std::is_heap(data1.begin(), data1.end()));
        CHECK(std::is_heap(data2.begin(), data2.end()));

        equal = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(equal);

        // Intentionally mess up the heap
        data1.push_back(Item(20));
        data1.push_back(Item(30));
        data2.push_back(Item(20));
        data2.push_back(Item(30));

        Data::iterator result1 = wstl::IsHeapUntil(data1.begin(), data1.end());
        Data::iterator result2 = std::is_heap_until(data2.begin(), data2.end());

        CHECK_EQ(std::distance(data1.begin(), result1), std::distance(data2.begin(), result2));

        equal = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(equal);

        data1.pop_back();
        data1.pop_back();
        data2.pop_back();
        data2.pop_back();

        wstl::SortHeap(data1.begin(), data1.end());
        std::sort_heap(data2.begin(), data2.end());

        equal = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(equal);
    }

    TEST_CASE("IsSortedUntil") {
        int data1[] = { 1, 2, 3, 4, 6, 5, 7, 8, 9, 10 };
        int data2[] = { 10, 9, 8, 7, 5, 6, 4, 3, 4, 2, 1 };

        int* p1 = wstl::IsSortedUntil(std::begin(data1), std::end(data1));
        int* p2 = std::is_sorted_until(std::begin(data1), std::end(data1));
        CHECK_EQ(p1, p2);

        p1 = wstl::IsSortedUntil(std::begin(data2), std::end(data2));
        p2 = std::is_sorted_until(std::begin(data2), std::end(data2));
        CHECK_EQ(p1, p2); 
    }

    TEST_CASE("IsSorted") {
        int data1[] = { 1, 2, 3, 4, 6, 5, 7, 8, 9, 10 };
        int data1Sorted[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int data2[] = { 10, 9, 8, 7, 5, 6, 4, 3, 2, 1 };
        int data2Sorted[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

        bool result = wstl::IsSorted(std::begin(data1), std::end(data1));
        CHECK(!result);

        bool result2 = wstl::IsSorted(std::begin(data1Sorted), std::end(data1Sorted));
        CHECK(result2);

        result = wstl::IsSorted(std::begin(data2), std::end(data2), std::greater<int>());
        CHECK(!result);

        result = wstl::IsSorted(std::begin(data2Sorted), std::end(data2Sorted), std::greater<int>());
        CHECK(result);
    }

    TEST_CASE("QuickSort") {
        std::vector<int> data(100, 0);
        std::iota(data.begin(), data.end(), 1);

        for(int i = 0; i < 100; ++i) {
            std::shuffle(data.begin(), data.end(), urng);

            std::vector<int> data1 = data;
            std::vector<int> data2 = data;

            wstl::QuickSort(data1.begin(), data1.end());
            std::sort(data2.begin(), data2.end());

            bool equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);

            data1 = data;
            data2 = data;

            wstl::QuickSort(data1.begin(), data1.end(), std::greater<int>());
            std::sort(data2.begin(), data2.end(), std::greater<int>());

            equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);
        }
    }

    TEST_CASE("PartialSort") {
        std::vector<int> initial = {5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

        for(size_t i = 0; i < initial.size(); ++i) {
            std::vector<int> data1 = initial;
            std::vector<int> data2 = initial;

            wstl::PartialSort(data1.begin(), data1.begin() + ptrdiff_t(i), data1.end());
            std::partial_sort(data2.begin(), data2.begin() + ptrdiff_t(i), data2.end());

            bool equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);

            data1 = initial;
            data2 = initial;

            wstl::PartialSort(data1.begin(), data1.begin() + ptrdiff_t(i), data1.end(), std::greater<int>());
            std::partial_sort(data2.begin(), data2.begin() + ptrdiff_t(i), data2.end(), std::greater<int>());

            equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);
        }
    }

    TEST_CASE("PartialSortCopy") {
        std::vector<int> initial = {5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

        for(size_t i = 0; i < initial.size(); ++i) {
            std::vector<int> data1(initial.size());
            std::vector<int> data2(initial.size());

            wstl::PartialSortCopy(initial.begin(), initial.begin() + ptrdiff_t(5), data1.begin(), data1.end());
            std::partial_sort_copy(initial.begin(), initial.begin() + ptrdiff_t(5), data2.begin(), data2.end());

            bool equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);

            data1.clear();
            data1.resize(initial.size());
            data2.clear();
            data2.resize(initial.size());

            wstl::PartialSortCopy(initial.begin(), initial.begin() + ptrdiff_t(5), data1.begin(), data1.end(), std::greater<int>());
            std::partial_sort_copy(initial.begin(), initial.begin() + ptrdiff_t(5), data2.begin(), data2.end(), std::greater<int>());

            equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);
        }
    }

    TEST_CASE("Merge") {
        int data1[] = {2, 6, 5, 7, 4, 2, 2, 6, 7, 0};
        int data2[] = {8, 3, 2, 5, 0, 1, 9, 6, 5, 0};

        std::vector<int> result;
        std::vector<int> expected;

        wstl::Merge(std::begin(data1), std::end(data1), std::begin(data2), std::end(data2), std::back_inserter(result));
        std::merge(std::begin(data1), std::end(data1), std::begin(data2), std::end(data2), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        result.clear();
        expected.clear();

        wstl::Merge(std::begin(data1), std::end(data1), std::begin(data2), std::end(data2), std::back_inserter(result), std::greater<int>());
        std::merge(std::begin(data1), std::end(data1), std::begin(data2), std::end(data2), std::back_inserter(expected), std::greater<int>());

        equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);
    }

    TEST_CASE("InplaceMerge") {
        std::vector<int> data1 = {2, 4, 6, 7, 9, 1, 3, 5, 8};
        std::vector<int> data2 = {9, 7, 6, 4, 1, 8, 5, 3, 2};

        std::vector<int> result(data1);
        std::vector<int> expected(data1);

        wstl::InplaceMerge(result.begin(), result.begin() + 5, result.end());
        std::inplace_merge(expected.begin(), expected.begin() + 5, expected.end());

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        result = data2;
        expected = data2;

        wstl::InplaceMerge(result.begin(), result.begin() + 5, result.end(), std::greater<int>());
        std::inplace_merge(expected.begin(), expected.begin() + 5, expected.end(), std::greater<int>());

        equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);
    }

    TEST_CASE("MergeSort") {
        std::vector<int> data(100, 0);
        std::iota(data.begin(), data.end(), 1);
        std::vector<int> buffer(100);

        for(int i = 0; i < 100; ++i) {
            std::shuffle(data.begin(), data.end(), urng);

            std::vector<int> data1 = data;
            std::vector<int> data2 = data;

            wstl::MergeSort(data1.begin(), data1.end(), buffer.begin());
            std::sort(data2.begin(), data2.end());

            bool equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);

            data1 = data;
            data2 = data;

            wstl::MergeSort(data1.begin(), data1.end(), buffer.begin(), std::greater<int>());
            std::sort(data2.begin(), data2.end(), std::greater<int>());

            equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);
        }
    }

    TEST_CASE("InplaceMergeSort") {
        std::vector<int> data(100, 0);
        std::iota(data.begin(), data.end(), 1);

        for(int i = 0; i < 100; ++i) {
            std::shuffle(data.begin(), data.end(), urng);

            // Random access iterator
            std::vector<int> data1 = data;
            std::vector<int> data2 = data;

            wstl::InplaceMergeSort(data1.begin(), data1.end());
            std::sort(data2.begin(), data2.end());

            bool equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);

            data1 = data;
            data2 = data;

            wstl::InplaceMergeSort(data1.begin(), data1.end(), std::greater<int>());
            std::sort(data2.begin(), data2.end(), std::greater<int>());

            equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);

            // Bidirectional iterator
            std::list<int> list1(data.begin(), data.end());
            std::list<int> list2(data.begin(), data.end());

            wstl::InplaceMergeSort(list1.begin(), list1.end());
            list2.sort();

            equal = std::equal(list1.begin(), list1.end(), list2.begin());
            CHECK(equal);

            list1.assign(data.begin(), data.end());
            list2.assign(data.begin(), data.end());

            wstl::InplaceMergeSort(list1.begin(), list1.end(), std::greater<int>());
            list2.sort(std::greater<int>());

            equal = std::equal(list1.begin(), list1.end(), list2.begin());
            CHECK(equal);
        }
    }

    TEST_CASE("HeapSort") {
        std::vector<int> data(100, 0);
        std::iota(data.begin(), data.end(), 1);

        for(int i = 0; i < 100; ++i) {
            std::shuffle(data.begin(), data.end(), urng);

            std::vector<int> data1 = data;
            std::vector<int> data2 = data;

            wstl::HeapSort(data1.begin(), data1.end());
            std::sort(data2.begin(), data2.end());

            bool equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);

            data1 = data;
            data2 = data;

            wstl::HeapSort(data1.begin(), data1.end(), std::greater<int>());
            std::sort(data2.begin(), data2.end(), std::greater<int>());

            equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);
        }
    }

    TEST_CASE("NthElement") {
        std::array<int, 8> initial = {1, 2, 3, 4, 5, 6};

        std::array<int, 8> compare = initial;
        std::array<int, 8> data = initial;

        bool complete = false;

        while (!complete) {
            for (size_t i = 0; i < initial.size(); ++i) {
                std::sort(compare.begin(), compare.end());
                wstl::NthElement(data.begin(), data.begin() + i, data.end());

                data = initial;

                std::sort(compare.begin(), compare.end(), std::greater<int>());
                wstl::NthElement(data.begin(), data.begin() + i, data.end(), std::greater<int>());

                CHECK_EQ(compare[i], data[i]);
            }

            complete = !std::next_permutation(initial.begin(), initial.end());

            compare = initial;
            data = initial;
        }
    }

    TEST_CASE("Sort") {
        std::vector<int> data(100, 0);
        std::iota(data.begin(), data.end(), 1);

        for(int i = 0; i < 100; ++i) {
            std::shuffle(data.begin(), data.end(), urng);

            std::vector<int> data1 = data;
            std::vector<int> data2 = data;

            wstl::Sort(data1.begin(), data1.end());
            std::sort(data2.begin(), data2.end());

            bool equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);

            data1 = data;
            data2 = data;

            wstl::Sort(data1.begin(), data1.end(), std::greater<int>());
            std::sort(data2.begin(), data2.end(), std::greater<int>());

            equal = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(equal);
        }
    }

    TEST_CASE("StableSort") {
        std::vector<NonTrivialData> initial = {{1, 1}, {2, 1}, {3, 1}, {2, 2}, {3, 2}, {4, 1}, {2, 3}, {3, 3}, {5, 1}};

        std::vector<NonTrivialData> data1(initial);
        std::vector<NonTrivialData> data2(initial);

        wstl::StableSort(data1.begin(), data1.end());
        std::stable_sort(data2.begin(), data2.end());

        bool result = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(result);

        data1 = initial;
        data2 = initial;

        wstl::StableSort(data1.begin(), data1.end(), std::greater<NonTrivialData>());
        std::stable_sort(data2.begin(), data2.end(), std::greater<NonTrivialData>());

        result = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(result);
    }

    TEST_CASE("LowerBound") {
        for(int i = 0; i < 9; ++i) {
            // Random access iterator
            int* expected1 = std::lower_bound(std::begin(dataA), std::end(dataA), i);
            int* result1 = wstl::LowerBound(std::begin(dataA), std::end(dataA), i);
            CHECK_EQ(expected1, result1);

            // Non-random access iterator
            std::list<int>::iterator expected2 = std::lower_bound(std::begin(dataLA), std::end(dataLA), i);
            std::list<int>::iterator result2 = wstl::LowerBound(std::begin(dataLA), std::end(dataLA), i);
            CHECK_EQ(expected2, result2);
        }
    }

    TEST_CASE("UpperBound") {
        for(int i = 0; i < 9; ++i) {
            // Random access iterator
            int* expected1 = std::upper_bound(std::begin(dataA), std::end(dataA), i);
            int* result1 = wstl::UpperBound(std::begin(dataA), std::end(dataA), i);
            CHECK_EQ(expected1, result1);

            // Non-random access iterator
            std::list<int>::iterator expected2 = std::upper_bound(std::begin(dataLA), std::end(dataLA), i);
            std::list<int>::iterator result2 = wstl::UpperBound(std::begin(dataLA), std::end(dataLA), i);
            CHECK_EQ(expected2, result2);
        }
    }

    TEST_CASE("BinarySearch") {
        int data1[] = {1, 3, 4, 5, 8, 9};
        int data2[] = {9, 8, 6, 5, 2, 0};

        bool expected = std::binary_search(std::begin(data1), std::end(data1), 5);
        bool result = wstl::BinarySearch(std::begin(data1), std::end(data1), 5);

        CHECK_EQ(expected, result);

        expected = std::binary_search(std::begin(data2), std::end(data2), 5, std::greater<int>());
        result = wstl::BinarySearch(std::begin(data2), std::end(data2), 5, std::greater<int>());

        CHECK_EQ(expected, result);
    }

    TEST_CASE("EqualRange") {
        for(int i = 0; i < 9; ++i) {
            // Random access iterator
            std::pair<int*, int*> expected1 = std::equal_range(std::begin(dataA), std::end(dataA), i);
            wstl::Pair<int*, int*> result1 = wstl::EqualRange(std::begin(dataA), std::end(dataA), i);

            CHECK_EQ(expected1.first, result1.First);
            CHECK_EQ(expected1.second, result1.Second);

            // Non-random access iterator
            typedef std::list<int>::iterator Iterator;
            std::pair<Iterator, Iterator> expected2 = std::equal_range(std::begin(dataLA), std::end(dataLA), i);
            wstl::Pair<Iterator, Iterator> result2 = wstl::EqualRange(std::begin(dataLA), std::end(dataLA), i);
            
            CHECK_EQ(expected2.first, result2.First);
            CHECK_EQ(expected2.second, result2.Second);
        }
    }

    TEST_CASE("BinaryFind") {
        int data1[] = {1, 3, 4, 5, 8, 9};
        int data2[] = {9, 8, 6, 5, 3, 0};

        int* it1 = std::find(std::begin(data1), std::end(data1), 3);
        int* it2 = wstl::BinaryFind(std::begin(data1), std::end(data1), 3);

        CHECK_EQ(it1, it2);

        it1 = std::find(std::begin(data2), std::end(data2), 5);
        it2 = wstl::BinaryFind(std::begin(data2), std::end(data2), 5, std::greater<int>(), std::less_equal<int>());

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("Includes") {
        int data1[] = {1, 2, 6, 7, 8, 10, 12};
        int sequence1True[] = {2, 8, 12};
        int sequence1False[] = {1, 6, 6, 7};
        int data2[] = {12, 10, 8, 7, 6, 2, 1};
        int sequence2True[] = {12, 8, 2};
        int sequence2False[] = {7, 6, 6, 1};

        bool result = wstl::Includes(std::begin(data1), std::end(data1), std::begin(sequence1True), std::end(sequence1True));
        CHECK(result);

        result = wstl::Includes(std::begin(data1), std::end(data1), std::begin(sequence1False), std::end(sequence1False));
        CHECK(!result);

        result = wstl::Includes(std::begin(data2), std::end(data2), std::begin(sequence2True), std::end(sequence2True), std::greater<int>());
        CHECK(result);

        result = wstl::Includes(std::begin(data2), std::end(data2), std::begin(sequence2False), std::end(sequence2False), std::greater<int>());
        CHECK(!result);
    }

    TEST_CASE("SetDifference") {
        int data1[] = {1, 2, 6, 7, 8, 10, 12};
        int sequence1[] = {2, 8, 12};
        int data2[] = {12, 10, 8, 7, 6, 2, 1};
        int sequence2[] = {12, 8, 2};

        std::vector<int> result;
        std::vector<int> expected;

        wstl::SetDifference(std::begin(data1), std::end(data1), std::begin(sequence1), std::end(sequence1), std::back_inserter(result));
        std::set_difference(std::begin(data1), std::end(data1), std::begin(sequence1), std::end(sequence1), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        wstl::SetDifference(std::begin(data2), std::end(data2), std::begin(sequence2), std::end(sequence2), std::back_inserter(result), std::greater<int>());
        std::set_difference(std::begin(data2), std::end(data2), std::begin(sequence2), std::end(sequence2), std::back_inserter(expected), std::greater<int>());

        equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);
    }

    TEST_CASE("SetIntersection") {
        int data1[] = {1, 2, 6, 7, 8, 10, 12};
        int sequence1[] = {2, 8, 8, 12};
        int data2[] = {12, 10, 8, 7, 6, 2, 1};
        int sequence2[] = {12, 8, 8, 2};

        std::vector<int> result;
        std::vector<int> expected;

        wstl::SetIntersection(std::begin(data1), std::end(data1), std::begin(sequence1), std::end(sequence1), std::back_inserter(result));
        std::set_intersection(std::begin(data1), std::end(data1), std::begin(sequence1), std::end(sequence1), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        wstl::SetIntersection(std::begin(data2), std::end(data2), std::begin(sequence2), std::end(sequence2), std::back_inserter(result), std::greater<int>());
        std::set_intersection(std::begin(data2), std::end(data2), std::begin(sequence2), std::end(sequence2), std::back_inserter(expected), std::greater<int>());

        equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);
    }

    TEST_CASE("SetDifference") {
        int data1[] = {1, 2, 6, 7, 8, 10, 12};
        int sequence1[] = {2, 8, 12};
        int data2[] = {12, 10, 8, 7, 6, 2, 1};
        int sequence2[] = {12, 8, 2};

        std::vector<int> result;
        std::vector<int> expected;

        wstl::SetDifference(std::begin(data1), std::end(data1), std::begin(sequence1), std::end(sequence1), std::back_inserter(result));
        std::set_difference(std::begin(data1), std::end(data1), std::begin(sequence1), std::end(sequence1), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        wstl::SetDifference(std::begin(data2), std::end(data2), std::begin(sequence2), std::end(sequence2), std::back_inserter(result), std::greater<int>());
        std::set_difference(std::begin(data2), std::end(data2), std::begin(sequence2), std::end(sequence2), std::back_inserter(expected), std::greater<int>());

        equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);
    }

    TEST_CASE("SetSymmetricDifference") {
        int data1[] = {1, 2, 6, 7, 8, 10, 12};
        int sequence1[] = {2, 8, 12};
        int data2[] = {12, 10, 8, 7, 6, 2, 1};
        int sequence2[] = {12, 8, 2};

        std::vector<int> result;
        std::vector<int> expected;

        wstl::SetSymmetricDifference(std::begin(data1), std::end(data1), std::begin(sequence1), std::end(sequence1), std::back_inserter(result));
        std::set_symmetric_difference(std::begin(data1), std::end(data1), std::begin(sequence1), std::end(sequence1), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        wstl::SetSymmetricDifference(std::begin(data2), std::end(data2), std::begin(sequence2), std::end(sequence2), std::back_inserter(result), std::greater<int>());
        std::set_symmetric_difference(std::begin(data2), std::end(data2), std::begin(sequence2), std::end(sequence2), std::back_inserter(expected), std::greater<int>());

        equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);
    }

    TEST_CASE("SetUnion") {
        int data1[] = {1, 2, 6, 7, 8, 10, 12};
        int sequence1[] = {2, 8, 8, 12};
        int data2[] = {12, 10, 8, 7, 6, 2, 1};
        int sequence2[] = {12, 8, 8, 2};

        std::vector<int> result;
        std::vector<int> expected;

        wstl::SetUnion(std::begin(data1), std::end(data1), std::begin(sequence1), std::end(sequence1), std::back_inserter(result));
        std::set_union(std::begin(data1), std::end(data1), std::begin(sequence1), std::end(sequence1), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        wstl::SetUnion(std::begin(data2), std::end(data2), std::begin(sequence2), std::end(sequence2), std::back_inserter(result), std::greater<int>());
        std::set_union(std::begin(data2), std::end(data2), std::begin(sequence2), std::end(sequence2), std::back_inserter(expected), std::greater<int>());

        equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);
    }

    TEST_CASE("IsPermutation") {
        int data[] = {1, 2, 3, 4, 5, 6, 7, 8};
        int permutation[] = {1, 3, 2, 4, 6, 5, 7, 8};
        int notPermutation[] = {1, 3, 2, 2, 6, 5, 7, 8};

        bool result = wstl::IsPermutation(std::begin(data), std::end(data), std::begin(permutation));
        CHECK(result);

        result = wstl::IsPermutation(std::begin(data), std::end(data), std::begin(notPermutation));
        CHECK(!result);

        result = wstl::IsPermutation(std::begin(data), std::end(data), std::begin(permutation), wstl::EqualTo<int>());
        CHECK(result);

        result = wstl::IsPermutation(std::begin(data), std::end(data), std::begin(notPermutation), wstl::EqualTo<int>());
        CHECK(!result);

        result = wstl::IsPermutation(std::begin(data), std::end(data), std::begin(permutation), std::end(permutation));
        CHECK(result);

        result = wstl::IsPermutation(std::begin(data), std::end(data), std::begin(notPermutation), std::end(notPermutation));
        CHECK(!result);

        result = wstl::IsPermutation(std::begin(data), std::end(data), std::begin(permutation), std::end(permutation), wstl::EqualTo<int>());
        CHECK(result);

        result = wstl::IsPermutation(std::begin(data), std::end(data), std::begin(notPermutation), std::end(notPermutation), wstl::EqualTo<int>());
        CHECK(!result);
    }

    TEST_CASE("NextPermutation") {
        std::vector<int> initial1 = {1, 2, 3, 4, 5, 6};
        std::vector<int> initial2 = {6, 5, 4, 3, 2, 1};

        std::vector<int> data1 = initial1;
        std::vector<int> data2 = initial1;
        std::vector<int> data3 = initial2;
        std::vector<int> data4 = initial2;

        bool complete1 = false;
        bool complete2 = false;
        bool complete3 = false;
        bool complete4 = false;

        while(complete1 && complete2 && complete3 && complete4) {
            complete1 = wstl::NextPermutation(data1.begin(), data1.end());
            complete2 = std::next_permutation(data2.begin(), data2.end());
            complete3 = wstl::NextPermutation(data3.begin(), data3.end(), std::greater<int>());
            complete4 = std::next_permutation(data4.begin(), data4.end(), std::greater<int>());

            bool equal = std::equal(data2.begin(), data2.end(), data1.begin());
            CHECK(equal);

            equal = std::equal(data4.begin(), data4.end(), data3.begin());
            CHECK(equal);
        }

        CHECK_EQ(complete1, complete2);
        CHECK_EQ(complete2, complete3);
        CHECK_EQ(complete3, complete4);
    }

    TEST_CASE("PreviousPermutation") {
        std::vector<int> initial1 = {1, 2, 3, 4, 5, 6};
        std::vector<int> initial2 = {6, 5, 4, 3, 2, 1};

        std::vector<int> data1 = initial1;
        std::vector<int> data2 = initial1;
        std::vector<int> data3 = initial2;
        std::vector<int> data4 = initial2;

        bool complete1 = false;
        bool complete2 = false;
        bool complete3 = false;
        bool complete4 = false;

        while(complete1 && complete2 && complete3 && complete4) {
            complete1 = wstl::PreviousPermutation(data1.begin(), data1.end());
            complete2 = std::prev_permutation(data2.begin(), data2.end());
            complete3 = wstl::PreviousPermutation(data3.begin(), data3.end(), std::greater<int>());
            complete4 = std::prev_permutation(data4.begin(), data4.end(), std::greater<int>());

            bool equal = std::equal(data2.begin(), data2.end(), data1.begin());
            CHECK(equal);

            equal = std::equal(data4.begin(), data4.end(), data3.begin());
            CHECK(equal);
        }

        CHECK_EQ(complete1, complete2);
        CHECK_EQ(complete2, complete3);
        CHECK_EQ(complete3, complete4);
    }

    TEST_CASE("CopySafe") {
        int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int data2[] = { 1, 2, 3, 4, 5 };
        std::list<int> data3(std::begin(data1), std::end(data1));
        std::list<int> data4(std::begin(data2), std::end(data2));

        int out1[10];
        int out2[5];

        int check1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int check2[] = { 1, 2, 3, 4, 5 };
        int check3[] = { 1, 2, 3, 4, 5, 0, 0, 0, 0, 0 };

        int* result;

        // Same size

        // Random access iterator
        std::fill(std::begin(out1), std::end(out1), 0);
        result = wstl::CopySafe(std::begin(data1), std::end(data1), std::begin(out1), std::end(out1));
        CHECK_EQ(std::end(out1), result);

        bool equal = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
        CHECK(equal);

        // Non-random access iterator
        std::fill(std::begin(out1), std::end(out1), 0);
        result = wstl::CopySafe(std::begin(data3), std::end(data3), std::begin(out1), std::end(out1));
        CHECK_EQ(std::end(out1), result);

        equal = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
        CHECK(equal);

        // Destination smaller

        // Random access iterator
        std::fill(std::begin(out2), std::end(out2), 0);
        result = wstl::CopySafe(std::begin(data1), std::end(data1), std::begin(out2), std::end(out2));
        CHECK_EQ(std::end(out2), result);

        equal = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
        CHECK(equal);

        // Non-random access iterator
        std::fill(std::begin(out2), std::end(out2), 0);
        result = wstl::CopySafe(std::begin(data3), std::end(data3), std::begin(out2), std::end(out2));
        CHECK_EQ(std::end(out2), result);

        equal = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
        CHECK(equal);

        // Source smaller

        // Random access iterator
        std::fill(std::begin(out1), std::end(out1), 0);
        result = wstl::CopySafe(std::begin(data4), std::end(data4), std::begin(out1), std::end(out1));
        CHECK_EQ(std::begin(out1) + 5, result);

        equal = std::equal(std::begin(out1), std::end(out1), std::begin(check3));
        CHECK(equal);

        // Non-random access iterator
        std::fill(std::begin(out1), std::end(out1), 0);
        result = wstl::CopySafe(std::begin(data4), std::end(data4), std::begin(out1), std::end(out1));
        CHECK_EQ(std::begin(out1) + 5, result);

        equal = std::equal(std::begin(out1), std::end(out1), std::begin(check3));
        CHECK(equal);
    }

    TEST_CASE("MoveSafe") {
        typedef MovableData<uint32_t> Item;
        typedef std::vector<Item> Data;
        Data data1;

        // Create some data
        Item p1(1U), p2(2U), p3(3U), p4(4U), p5(5U), p6(6U), p7(7U), p8(8U);

        data1.push_back(std::move(p1));
        data1.push_back(std::move(p2));
        data1.push_back(std::move(p3));
        data1.push_back(std::move(p4));
        data1.push_back(std::move(p5));
        data1.push_back(std::move(p6));
        data1.push_back(std::move(p7));
        data1.push_back(std::move(p8));

        Data data2;
        data2.emplace_back(Item(1U));
        data2.emplace_back(Item(2U));
        data2.emplace_back(Item(3U));
        data2.emplace_back(Item(4U));
        data2.emplace_back(Item(5U));

        std::list<Item> data3;
        data3.emplace_back(Item(1U));
        data3.emplace_back(Item(2U));
        data3.emplace_back(Item(3U));
        data3.emplace_back(Item(4U));
        data3.emplace_back(Item(5U));
        data3.emplace_back(Item(6U));
        data3.emplace_back(Item(7U));
        data3.emplace_back(Item(8U));

        std::list<Item> data4;
        data4.emplace_back(Item(1U));
        data4.emplace_back(Item(2U));
        data4.emplace_back(Item(3U));
        data4.emplace_back(Item(4U));
        data4.emplace_back(Item(5U));

        std::vector<Item> out1(10);
        std::vector<Item> out2(5);

        std::vector<Item> check1;
        check1.emplace_back(Item(1U));
        check1.emplace_back(Item(2U));
        check1.emplace_back(Item(3U));
        check1.emplace_back(Item(4U));
        check1.emplace_back(Item(5U));
        check1.emplace_back(Item(6U));
        check1.emplace_back(Item(7U));
        check1.emplace_back(Item(8U));

        std::vector<Item> check2;
        check2.emplace_back(Item(1U));
        check2.emplace_back(Item(2U));
        check2.emplace_back(Item(3U));
        check2.emplace_back(Item(4U));
        check2.emplace_back(Item(5U));

        std::vector<Item> check3;
        check3.emplace_back(Item(1U));
        check3.emplace_back(Item(2U));
        check3.emplace_back(Item(3U));
        check3.emplace_back(Item(4U));
        check3.emplace_back(Item(5U));
        check3.emplace_back(Item(0U));
        check3.emplace_back(Item(0U));
        check3.emplace_back(Item(0U));
        check3.emplace_back(Item(0U));

        // Same size

        // Random access iterator
        out1.clear();
        out1.resize(8);
        std::vector<Item>::iterator result = wstl::MoveSafe(std::make_move_iterator(data1.begin()), std::make_move_iterator(data1.end()), out1.begin(), out1.end());
        CHECK_EQ(out1.end(), result);

        bool equal = std::equal(out1.begin(), out1.end(), check1.begin());
        CHECK(equal);

        // Non-random access iterator
        out1.clear();
        out1.resize(8);
        result = wstl::MoveSafe(std::make_move_iterator(data3.begin()), std::make_move_iterator(data3.end()), out1.begin(), out1.end());
        CHECK_EQ(out1.end(), result);

        equal = std::equal(out1.begin(), out1.end(), check1.begin());
        CHECK(equal);

        // Destination smaller

        // Random access iterator
        out1.clear();
        out1.resize(5);
        result = wstl::MoveSafe(std::make_move_iterator(data1.begin()), std::make_move_iterator(data1.end()), out2.begin(), out2.end());
        CHECK_EQ(out2.end(), result);

        equal = std::equal(out2.begin(), out2.end(), check2.begin());
        CHECK(equal);

        // Non-random access iterator
        out1.clear();
        out1.resize(5);
        result = wstl::MoveSafe(std::make_move_iterator(data3.begin()), std::make_move_iterator(data3.end()), out2.begin(), out2.end());
        CHECK_EQ(out2.end(), result);

        equal = std::equal(out2.begin(), out2.end(), check2.begin());
        CHECK(equal);

        // Source smaller

        // Random access iterator
        out1.clear();
        out1.resize(10);
        result = wstl::MoveSafe(std::make_move_iterator(data4.begin()), std::make_move_iterator(data4.end()), out1.begin(), out1.end());
        CHECK_EQ(out1.begin() + 5, result);

        equal = std::equal(out1.begin(), out1.end(), check3.begin());
        CHECK(equal);

        // Non-random access iterator
        out1.clear();
        out1.resize(10);
        result = wstl::MoveSafe(std::make_move_iterator(data4.begin()), std::make_move_iterator(data4.end()), out1.begin(), out1.end());
        CHECK_EQ(out1.begin() + 5, result);

        equal = std::equal(out1.begin(), out1.end(), check3.begin());
        CHECK(equal);
    }
}