// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
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
#include <array>

#include <wstl/Algorithm.hpp>
#include <wstl/Bit.hpp>

#include "Utils.hpp"


namespace {
    #ifdef __WSTL_CXX11__
    std::random_device rd;
    std::mt19937 urng(rd());
    #endif

    const std::size_t SIZE = 9;

    int dataA[SIZE] = {4, 5, 7, 1, 10, 6, 3, 7, 7};
    int dataB[SIZE] = {1, 60, 4, 3, 9, 10, 5, 4, 10};

    std::list<int> dataLA(BeginImpl(dataA), EndImpl(dataA));
    std::list<int> dataLB(BeginImpl(dataB), EndImpl(dataB));

    const std::size_t NONTRIVIAL_SIZE = 7;

    NonTrivialData dataN[NONTRIVIAL_SIZE] = {NonTrivialData(1, 2), NonTrivialData(3, 4), NonTrivialData(5, 6), 
        NonTrivialData(7, 8), NonTrivialData(9, 10), NonTrivialData(11, 12), NonTrivialData(13, 14)};

    template<typename T, T Value>
    struct ConstantGenerator {
        T operator()() const {
            return Value;
        }
    };

    bool StablePartitionPredicate(const NonTrivialData& x) {
        return x.A > 2 && x.B > 2; 
    }

    bool IsNotEven(int x) { 
        return x % 2 != 0; 
    }

    bool CopyIfPredicate2(const NonTrivialData& x) { 
        return x.A % 2 == 0; 
    }

    bool AdjacentFindPredicate(int a, int b) { 
        return (a - b) == 1; 
    }

    void DoubleFunction(int& x) { 
        x *= 2; 
    }
}

TEST_SUITE("Algorithm") {
    TEST_CASE("Find") {
        int* it1 = std::find(BeginImpl(dataA), EndImpl(dataA), 3);
        int* it2 = wstl::Find(BeginImpl(dataA), EndImpl(dataA), 3);

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("FindIf") {
        #ifdef __WSTL_CXX11__
        auto predicate = [](int x) { return x == 5; };
        #else
        std::binder2nd<std::equal_to<int> > predicate(std::equal_to<int>(), 5);
        #endif

        int* it1 = std::find_if(BeginImpl(dataA), EndImpl(dataA), predicate);
        int* it2 = wstl::FindIf(BeginImpl(dataA), EndImpl(dataA), predicate);

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("FindIfNot") {
        #ifdef __WSTL_CXX11__
        auto predicate = [](int x) { return x == 3; };
        #else
        std::binder2nd<std::equal_to<int> > predicate(std::equal_to<int>(), 3);
        #endif

        int* it1 = std::find_if_not(BeginImpl(dataA), EndImpl(dataA), predicate);
        int* it2 = wstl::FindIfNot(BeginImpl(dataA), EndImpl(dataA), predicate);

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("AllOf") {
        #ifdef __WSTL_CXX11__
        auto truePredicate = [](int x) { return x > 0; };
        auto falsePredicate = [](int x) { return x > 3; };
        #else
        std::binder2nd<std::greater<int> > truePredicate(std::greater<int>(), 0);
        std::binder2nd<std::greater<int> > falsePredicate(std::greater<int>(), 3);
        #endif

        bool expected = std::all_of(BeginImpl(dataA), EndImpl(dataA), truePredicate);
        bool result = wstl::AllOf(BeginImpl(dataA), EndImpl(dataA), truePredicate);

        CHECK_EQ(expected, result);

        expected = std::all_of(BeginImpl(dataA), EndImpl(dataA), falsePredicate);
        result = wstl::AllOf(BeginImpl(dataA), EndImpl(dataA), falsePredicate);

        CHECK_EQ(expected, result);
    }

    TEST_CASE("AnyOf") {
        #ifdef __WSTL_CXX11__
        auto truePredicate = [](int x) { return x < 3; };
        auto falsePredicate = [](int x) { return x < 0; };
        #else
        std::binder2nd<std::less<int> > truePredicate(std::less<int>(), 0);
        std::binder2nd<std::less<int> > falsePredicate(std::less<int>(), 3);
        #endif

        bool expected = std::any_of(BeginImpl(dataA), EndImpl(dataA), truePredicate);
        bool result = wstl::AnyOf(BeginImpl(dataA), EndImpl(dataA), truePredicate);

        CHECK_EQ(expected, result);

        expected = std::any_of(BeginImpl(dataA), EndImpl(dataA), falsePredicate);
        result = wstl::AnyOf(BeginImpl(dataA), EndImpl(dataA), falsePredicate);

        CHECK_EQ(expected, result);
    }

    TEST_CASE("NoneOf") {
        #ifdef __WSTL_CXX11__
        auto truePredicate = [](int x) { return x < 0; };
        auto falsePredicate = [](int x) { return x < 3; };
        #else
        std::binder2nd<std::less<int> > truePredicate(std::less<int>(), 0);
        std::binder2nd<std::less<int> > falsePredicate(std::less<int>(), 3);
        #endif

        bool expected = std::none_of(BeginImpl(dataA), EndImpl(dataA), truePredicate);
        bool result = wstl::NoneOf(BeginImpl(dataA), EndImpl(dataA), truePredicate);

        CHECK_EQ(expected, result);

        expected = std::none_of(BeginImpl(dataA), EndImpl(dataA), falsePredicate);
        result = wstl::NoneOf(BeginImpl(dataA), EndImpl(dataA), falsePredicate);

        CHECK_EQ(expected, result);
    }

    TEST_CASE("ForEach") {
        int data[] = {4, 3, 1, 6, 7, 4};
        int expected[] = {8, 6, 2, 12, 14, 8};

        wstl::ForEach(BeginImpl(data), EndImpl(data), &DoubleFunction);

        CHECK(std::equal(BeginImpl(data), EndImpl(data), BeginImpl(expected)));
    }

    TEST_CASE("ForEachInRange") {
        int data[] = {4, 3, 1, 6, 7, 4};
        int expected[] = {8, 6, 2, 12, 14, 8};
        
        wstl::ForEachInRange(BeginImpl(data), 6, &DoubleFunction);

        CHECK(std::equal(BeginImpl(data), EndImpl(data), BeginImpl(expected)));
    }

    TEST_CASE("Count") {
        ptrdiff_t c1 = wstl::Count(BeginImpl(dataA), EndImpl(dataA), 7);
        ptrdiff_t c2 = std::count(BeginImpl(dataA), EndImpl(dataA), 7);

        CHECK_EQ(c1, c2);
    }

    TEST_CASE("CountIf") {
        ptrdiff_t c1 = wstl::CountIf(BeginImpl(dataA), EndImpl(dataA), &wstl::IsEven<int>);
        ptrdiff_t c2 = std::count_if(BeginImpl(dataA), EndImpl(dataA), &wstl::IsEven<int>);

        CHECK_EQ(c1, c2);
    }

    TEST_CASE("Mismatch") {
        int data1[] = {1, 2, 3, 4, 5, 7, 8};
        int data2[] = {1, 2, 3, 4, 5, 6, 7};
        int data3[] = {1, 2, 3, 4, 5, 6, 7, 8};

        wstl::Pair<int*, int*> result = wstl::Mismatch(BeginImpl(data1), EndImpl(data1), BeginImpl(data2));
        std::pair<int*, int*> expected = std::mismatch(BeginImpl(data1), EndImpl(data1), BeginImpl(data2));

        CHECK_EQ(result.First, expected.first);
        CHECK_EQ(result.Second, expected.second);

        result = wstl::Mismatch(BeginImpl(data1), EndImpl(data1), BeginImpl(data2), std::equal_to<int>());
        expected = std::mismatch(BeginImpl(data1), EndImpl(data1), BeginImpl(data2), std::equal_to<int>());

        CHECK_EQ(result.First, expected.first);
        CHECK_EQ(result.Second, expected.second);

        result = wstl::Mismatch(BeginImpl(data2), EndImpl(data2), BeginImpl(data3));
        expected = std::mismatch(BeginImpl(data2), EndImpl(data2), BeginImpl(data3));

        CHECK_EQ(result.First, expected.first);
        CHECK_EQ(result.Second, expected.second);

        #ifdef __WSTL_CXX14__
        result = wstl::Mismatch(BeginImpl(data2), EndImpl(data2), BeginImpl(data3), EndImpl(data3));
        expected = std::mismatch(BeginImpl(data2), EndImpl(data2), BeginImpl(data3), EndImpl(data3));

        CHECK_EQ(result.First, expected.first);
        CHECK_EQ(result.Second, expected.second);

        result = wstl::Mismatch(BeginImpl(data2), EndImpl(data2), BeginImpl(data3), EndImpl(data3), std::equal_to<int>());
        expected = std::mismatch(BeginImpl(data2), EndImpl(data2), BeginImpl(data3), EndImpl(data3), std::equal_to<int>());

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
        int* it1 = wstl::AdjacentFind(BeginImpl(dataA), EndImpl(dataA));
        int* it2 = std::adjacent_find(BeginImpl(dataA), EndImpl(dataA));

        CHECK_EQ(it1, it2);

        it1 = wstl::AdjacentFind(BeginImpl(dataA), EndImpl(dataA), &AdjacentFindPredicate);
        it2 = std::adjacent_find(BeginImpl(dataA), EndImpl(dataA), &AdjacentFindPredicate);

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("Copy") {
        // POD pointer
        int buffer1[SIZE] = {0};
        int buffer2[SIZE] = {0};

        int* p1 = wstl::Copy(BeginImpl(dataA), EndImpl(dataA), BeginImpl(buffer1));
        int* p2 = std::copy(BeginImpl(dataA), EndImpl(dataA), BeginImpl(buffer2));

        ptrdiff_t d1 = std::distance(buffer1, p1);
        ptrdiff_t d2 = std::distance(buffer2, p2);
        CHECK_EQ(d1, d2);

        bool result = std::equal(BeginImpl(buffer1), EndImpl(buffer1), BeginImpl(buffer2));
        CHECK(result);

        // Non-POD pointer
        NonTrivialData bufferN1[NONTRIVIAL_SIZE];
        NonTrivialData bufferN2[NONTRIVIAL_SIZE];

        NonTrivialData* pn1 = wstl::Copy(BeginImpl(dataN), EndImpl(dataN), BeginImpl(bufferN1));
        NonTrivialData* pn2 = std::copy(BeginImpl(dataN), EndImpl(dataN), BeginImpl(bufferN2));

        d1 = std::distance(bufferN1, pn1);
        d2 = std::distance(bufferN2, pn2);
        CHECK_EQ(d1, d2);

        result = std::equal(BeginImpl(bufferN1), EndImpl(bufferN1), BeginImpl(bufferN2));
        CHECK(result);

        // Non-random iterator
        std::list<int> list1(SIZE);
        std::list<int> list2(SIZE);

        std::list<int>::iterator pl1 = wstl::Copy(BeginImpl(dataLA), EndImpl(dataLA), BeginImpl(list1));
        std::list<int>::iterator pl2 = std::copy(BeginImpl(dataLA), EndImpl(dataLA), BeginImpl(list2));

        d1 = std::distance(list1.begin(), pl1);
        d2 = std::distance(list2.begin(), pl2);
        CHECK_EQ(d1, d2);

        result = std::equal(BeginImpl(list1), EndImpl(list1), BeginImpl(list2));
        CHECK(result);
    }

    TEST_CASE("CopyIf") {
        // POD pointer
        int buffer1[SIZE] = {0};
        int buffer2[SIZE] = {0};

        int* p1 = wstl::CopyIf(BeginImpl(dataA), EndImpl(dataA), BeginImpl(buffer1), &wstl::IsEven<int>);
        int* p2 = std::copy_if(BeginImpl(dataA), EndImpl(dataA), BeginImpl(buffer2), &wstl::IsEven<int>);

        ptrdiff_t d1 = std::distance(buffer1, p1);
        ptrdiff_t d2 = std::distance(buffer2, p2);
        CHECK_EQ(d1, d2);

        bool result = std::equal(BeginImpl(buffer1), EndImpl(buffer1), BeginImpl(buffer2));
        CHECK(result);

        // Non-POD pointer
        NonTrivialData bufferN1[NONTRIVIAL_SIZE];
        NonTrivialData bufferN2[NONTRIVIAL_SIZE];

        NonTrivialData* pn1 = wstl::CopyIf(BeginImpl(dataN), EndImpl(dataN), BeginImpl(bufferN1), &CopyIfPredicate2);
        NonTrivialData* pn2 = std::copy_if(BeginImpl(dataN), EndImpl(dataN), BeginImpl(bufferN2), &CopyIfPredicate2);

        d1 = std::distance(bufferN1, pn1);
        d2 = std::distance(bufferN2, pn2);
        CHECK_EQ(d1, d2);

        result = std::equal(BeginImpl(bufferN1), EndImpl(bufferN1), BeginImpl(bufferN2));
        CHECK(result);

        // Non-random iterator
        std::list<int> list1(SIZE);
        std::list<int> list2(SIZE);

        std::list<int>::iterator pl1 = wstl::CopyIf(BeginImpl(dataLA), EndImpl(dataLA), BeginImpl(list1), &wstl::IsEven<int>);
        std::list<int>::iterator pl2 = std::copy_if(BeginImpl(dataLA), EndImpl(dataLA), BeginImpl(list2), &wstl::IsEven<int>);

        d1 = std::distance(list1.begin(), pl1);
        d2 = std::distance(list2.begin(), pl2);
        CHECK_EQ(d1, d2);

        result = std::equal(BeginImpl(list1), EndImpl(list1), BeginImpl(list2));
        CHECK(result);
    }

    TEST_CASE("CopyInRange") {
        // POD pointer
        int buffer1[SIZE] = {0};
        int buffer2[SIZE] = {0};

        int* p1 = wstl::CopyInRange(BeginImpl(dataA), SIZE, BeginImpl(buffer1));
        int* p2 = std::copy_n(BeginImpl(dataA), SIZE, BeginImpl(buffer2));

        ptrdiff_t d1 = std::distance(buffer1, p1);
        ptrdiff_t d2 = std::distance(buffer2, p2);
        CHECK_EQ(d1, d2);

        bool result = std::equal(BeginImpl(buffer1), EndImpl(buffer1), BeginImpl(buffer2));
        CHECK(result);

        // Non-POD pointer
        NonTrivialData bufferN1[NONTRIVIAL_SIZE];
        NonTrivialData bufferN2[NONTRIVIAL_SIZE];

        NonTrivialData* pn1 = wstl::CopyInRange(BeginImpl(dataN), NONTRIVIAL_SIZE, BeginImpl(bufferN1));
        NonTrivialData* pn2 = std::copy_n(BeginImpl(dataN), NONTRIVIAL_SIZE, BeginImpl(bufferN2));

        d1 = std::distance(bufferN1, pn1);
        d2 = std::distance(bufferN2, pn2);
        CHECK_EQ(d1, d2);

        result = std::equal(BeginImpl(bufferN1), EndImpl(bufferN1), BeginImpl(bufferN2));
        CHECK(result);

        // Non-random iterator
        std::list<int> list1(SIZE);
        std::list<int> list2(SIZE);

        std::list<int>::iterator pl1 = wstl::CopyInRange(BeginImpl(dataLA), SIZE, EndImpl(list1));
        std::list<int>::iterator pl2 = std::copy_n(BeginImpl(dataLA), SIZE, EndImpl(list2));

        d1 = std::distance(list1.begin(), pl1);
        d2 = std::distance(list2.begin(), pl2);
        CHECK_EQ(d1, d2);

        result = std::equal(BeginImpl(list1), EndImpl(list1), BeginImpl(list2));
        CHECK(result);
    }

    TEST_CASE("CopyBackward") {
        // POD pointer
        int buffer1[SIZE] = {0};
        int buffer2[SIZE] = {0};

        int* p1 = wstl::CopyBackward(BeginImpl(dataA), EndImpl(dataA), EndImpl(buffer1));
        int* p2 = std::copy_backward(BeginImpl(dataA), EndImpl(dataA), EndImpl(buffer2));

        ptrdiff_t d1 = std::distance(buffer1, p1);
        ptrdiff_t d2 = std::distance(buffer2, p2);
        CHECK_EQ(d1, d2);

        bool result = std::equal(BeginImpl(buffer1), EndImpl(buffer1), BeginImpl(buffer2));
        CHECK(result);

        // Non-POD pointer
        NonTrivialData bufferN1[NONTRIVIAL_SIZE];
        NonTrivialData bufferN2[NONTRIVIAL_SIZE];

        NonTrivialData* pn1 = wstl::CopyBackward(BeginImpl(dataN), EndImpl(dataN), EndImpl(bufferN1));
        NonTrivialData* pn2 = std::copy_backward(BeginImpl(dataN), EndImpl(dataN), EndImpl(bufferN2));

        d1 = std::distance(bufferN1, pn1);
        d2 = std::distance(bufferN2, pn2);
        CHECK_EQ(d1, d2);

        result = std::equal(BeginImpl(bufferN1), EndImpl(bufferN1), BeginImpl(bufferN2));
        CHECK(result);

        // Non-random iterator
        std::list<int> list1(SIZE);
        std::list<int> list2(SIZE);

        std::list<int>::iterator pl1 = wstl::CopyBackward(BeginImpl(dataLA), EndImpl(dataLA), EndImpl(list1));
        std::list<int>::iterator pl2 = std::copy_backward(BeginImpl(dataLA), EndImpl(dataLA), EndImpl(list2));

        d1 = std::distance(list1.begin(), pl1);
        d2 = std::distance(list2.begin(), pl2);
        CHECK_EQ(d1, d2);

        result = std::equal(BeginImpl(list1), EndImpl(list1), BeginImpl(list2));
        CHECK(result);
    }

    TEST_CASE("Move") {
        #ifdef __WSTL_CXX11__
        typedef std::vector<std::unique_ptr<uint32_t> > Data;
        #else
        typedef std::vector<uint32_t> Data;
        #endif

        Data data1;

        #ifdef __WSTL_CXX11__
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
        #else
        // Create data and push it
        data1.push_back(1U);
        data1.push_back(2U);
        data1.push_back(3U);
        data1.push_back(4U);
        data1.push_back(5U);
        #endif

        Data data2;
        wstl::Move(data1.begin(), data1.end(), std::back_inserter(data2));

        #ifdef __WSTL_CXX11__
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
        #else
        CHECK_EQ(data2[0], 1U);
        CHECK_EQ(data2[1], 2U);
        CHECK_EQ(data2[2], 3U);
        CHECK_EQ(data2[3], 4U);
        CHECK_EQ(data2[4], 5U);
        #endif
    }

    TEST_CASE("MoveBackward") {
        #ifdef __WSTL_CXX11__
        typedef std::vector<std::unique_ptr<uint32_t> > Data;
        #else
        typedef std::vector<uint32_t> Data;
        #endif

        Data data1;

        #ifdef __WSTL_CXX11__
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
        #else
        // Create data and push it
        data1.push_back(1U);
        data1.push_back(2U);
        data1.push_back(3U);
        data1.push_back(4U);
        data1.push_back(5U);
        #endif

        Data data2(5);
        wstl::MoveBackward(data1.begin(), data1.end(), data2.end());

        #ifdef __WSTL_CXX11__
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
        #else
        CHECK_EQ(data2[0], 1U);
        CHECK_EQ(data2[1], 2U);
        CHECK_EQ(data2[2], 3U);
        CHECK_EQ(data2[3], 4U);
        CHECK_EQ(data2[4], 5U);
        #endif
    }

    TEST_CASE("Fill") {
        int buffer[SIZE];

        #ifdef __WSTL_CXX11__
        auto predicate = [](int x) { return x == 5; };
        #else
        std::binder2nd<std::equal_to<int> > predicate(std::equal_to<int>(), 5);
        #endif

        wstl::Fill(BeginImpl(buffer), EndImpl(buffer), 5);
        
        bool result = std::all_of(BeginImpl(buffer), EndImpl(buffer), predicate);
        CHECK(result);
    }

    TEST_CASE("FillInRange") {
        int buffer[SIZE];

        #ifdef __WSTL_CXX11__
        auto predicate = [](int x) { return x == 5; };
        #else
        std::binder2nd<std::equal_to<int> > predicate(std::equal_to<int>(), 5);
        #endif

        wstl::FillInRange(BeginImpl(buffer), SIZE, 5);
        
        bool result = std::all_of(BeginImpl(buffer), EndImpl(buffer), predicate);
        CHECK(result);
    }

    TEST_CASE("Transform") {
        int buffer1[SIZE];
        int buffer2[SIZE];

        #ifdef __WSTL_CXX11__
        auto transform = [](int x) { return x * 2; };
        #else
        std::binder2nd<std::multiplies<int> > transform(std::multiplies<int>(), 2);
        #endif

        wstl::Transform(BeginImpl(dataA), EndImpl(dataA), buffer1, transform);
        std::transform(BeginImpl(dataA), EndImpl(dataA), buffer2, transform);

        bool result = std::equal(BeginImpl(buffer1), EndImpl(buffer1), BeginImpl(buffer2));
        CHECK(result);

        wstl::Transform(BeginImpl(dataA), EndImpl(dataA), BeginImpl(dataB), buffer1, std::plus<int>());
        std::transform(BeginImpl(dataA), EndImpl(dataA), BeginImpl(dataB), buffer2, std::plus<int>());

        result = std::equal(BeginImpl(buffer1), EndImpl(buffer1), BeginImpl(buffer2));
        CHECK(result);
    }

    TEST_CASE("Generate") {
        int buffer1[SIZE];
        int buffer2[SIZE];

        wstl::Generate(BeginImpl(buffer1), EndImpl(buffer1), ConstantGenerator<int, 5>());
        std::generate(BeginImpl(buffer2), EndImpl(buffer2), ConstantGenerator<int, 5>());

        bool result = std::equal(BeginImpl(buffer1), EndImpl(buffer1), BeginImpl(buffer2));
        CHECK(result);
    }

    TEST_CASE("GenerateInRange") {
        int buffer1[SIZE];
        int buffer2[SIZE];

        wstl::GenerateInRange(BeginImpl(buffer1), SIZE, ConstantGenerator<int, 5>());
        std::generate_n(BeginImpl(buffer2), SIZE, ConstantGenerator<int, 5>());

        bool result = std::equal(BeginImpl(buffer1), EndImpl(buffer1), BeginImpl(buffer2));
        CHECK(result);
    }

    TEST_CASE("Remove") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {1, 2, 3, 5, 6, 7};

        wstl::Remove(BeginImpl(data), EndImpl(data), 4);

        bool result = std::equal(BeginImpl(expected), EndImpl(expected), data);
        CHECK(result);
    }

    TEST_CASE("RemoveIf") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {2, 4, 4, 4, 6};

        wstl::RemoveIf(BeginImpl(data), EndImpl(data), &IsNotEven);

        bool result = std::equal(BeginImpl(expected), EndImpl(expected), data);
        CHECK(result);
    }

    TEST_CASE("RemoveCopy") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {1, 2, 3, 5, 6, 7};
        int buffer[9];

        wstl::RemoveCopy(BeginImpl(data), EndImpl(data), buffer, 4);

        bool result = std::equal(BeginImpl(expected), EndImpl(expected), buffer);
        CHECK(result);
    }

    TEST_CASE("RemoveCopyIf") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {2, 4, 4, 4, 6};
        int buffer[9];

        wstl::RemoveCopyIf(BeginImpl(data), EndImpl(data), buffer, &IsNotEven);

        bool result = std::equal(BeginImpl(expected), EndImpl(expected), buffer);
        CHECK(result);
    }

    TEST_CASE("Replace") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {1, 2, 3, 10, 10, 10, 5, 6, 7};

        wstl::Replace(BeginImpl(data), EndImpl(data), 4, 10);

        bool result = std::equal(BeginImpl(expected), EndImpl(expected), data);
        CHECK(result);
    }

    TEST_CASE("ReplaceIf") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {10, 2, 10, 4, 4, 4, 10, 6, 10};

        wstl::ReplaceIf(BeginImpl(data), EndImpl(data), &IsNotEven, 10);

        bool result = std::equal(BeginImpl(expected), EndImpl(expected), data);
        CHECK(result);
    }

    TEST_CASE("ReplaceCopy") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {1, 2, 3, 10, 10, 10, 5, 6, 7};
        int buffer[9];

        wstl::ReplaceCopy(BeginImpl(data), EndImpl(data), buffer, 4, 10);

        bool result = std::equal(BeginImpl(expected), EndImpl(expected), buffer);
        CHECK(result);
    }

    TEST_CASE("ReplaceCopyIf") {
        int data[] = {1, 2, 3, 4, 4, 4, 5, 6, 7};
        int expected[] = {10, 2, 10, 4, 4, 4, 10, 6, 10};
        int buffer[9];

        wstl::ReplaceCopyIf(BeginImpl(data), EndImpl(data), buffer, &IsNotEven, 10);

        bool result = std::equal(BeginImpl(expected), EndImpl(expected), buffer);
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

        wstl::SwapRanges(BeginImpl(data1), EndImpl(data1), BeginImpl(data2));

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

        wstl::Reverse(BeginImpl(data), EndImpl(data));

        bool result = std::equal(BeginImpl(expected), EndImpl(expected), data);
        CHECK(result);
    }

    TEST_CASE("ReverseCopy") {
        int result[SIZE];
        int expected[SIZE];

        int* ptr1 = wstl::ReverseCopy(BeginImpl(dataA), EndImpl(dataA), result);
        int* ptr2 = std::reverse_copy(BeginImpl(dataA), EndImpl(dataA), expected);
        
        ptrdiff_t d1 = std::distance(result, ptr1);
        ptrdiff_t d2 = std::distance(expected, ptr2);
        CHECK_EQ(d1, d2);

        bool equal = std::equal(BeginImpl(expected), EndImpl(expected), result);
        CHECK(equal);
    }

    TEST_CASE("Rotate") {
        // POD
        std::array<int, 7> initial = {1, 2, 3, 4, 5, 6, 7};

        for(size_t i = 0; i < initial.size(); ++i) {
            std::array<int, 7> data1(initial);
            std::array<int, 7> data2(initial);

            wstl::Rotate(data1.data(), data1.data() + i, data1.data() + data1.size());
            std::rotate(data2.data(), data2.data() + i, data2.data() + data2.size());

            bool result = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(result);
        }

        // Non-POD
        std::vector<NonTrivialData> initial2(BeginImpl(dataN), EndImpl(dataN));

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
        std::array<int, 7> initial = {1, 2, 3, 4, 5, 6, 7};

        for(size_t i = 0; i < initial.size(); ++i) {
            std::array<int, 7> data1, data2;

            wstl::RotateCopy(initial.data(), initial.data() + i, initial.data() + initial.size(), data1.data());
            std::rotate_copy(initial.data(), initial.data() + i, initial.data() + initial.size(), data2.data());

            bool result = std::equal(data1.begin(), data1.end(), data2.begin());
            CHECK(result);
        }

        // Non-POD
        std::vector<NonTrivialData> initial2(BeginImpl(dataN), EndImpl(dataN));
        
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

        wstl::Unique(BeginImpl(data1), EndImpl(data1));

        bool result = std::equal(BeginImpl(expected), EndImpl(expected), data1);
        CHECK(result);

        wstl::Unique(BeginImpl(data2), EndImpl(data2), std::equal_to<int>());

        result = std::equal(BeginImpl(expected), EndImpl(expected), data2);
        CHECK(result);
    }

    TEST_CASE("UniqueCopy") {
        int data[] = {1, 2, 1, 1, 3, 3, 3, 4, 5, 4};
        int expected[] = {1, 2, 1, 3, 4, 5, 4};
        int buffer1[10];
        int buffer2[10];

        wstl::UniqueCopy(BeginImpl(data), EndImpl(data), buffer1);

        bool result = std::equal(BeginImpl(expected), EndImpl(expected), buffer1);
        CHECK(result);

        wstl::UniqueCopy(BeginImpl(data), EndImpl(data), buffer2, std::equal_to<int>());

        result = std::equal(BeginImpl(expected), EndImpl(expected), buffer2);
        CHECK(result);
    }

    TEST_CASE("IsPartitioned") {
        int data[] = {1, 2, 3, 4, 5, 6, 7, 8};

        #ifdef __WSTL_CXX11__
        auto predicate = std::bind(std::greater<int>(), std::placeholders::_1, 4);
        #else
        std::binder2nd<std::greater<int> > predicate(std::greater<int>(), 4);
        #endif

        bool expected = std::is_partitioned(BeginImpl(data), EndImpl(data), predicate);
        bool result = wstl::IsPartitioned(BeginImpl(data), EndImpl(data), predicate);
        CHECK_EQ(expected, result);

        std::partition(BeginImpl(data), EndImpl(data), predicate);

        expected = std::is_partitioned(BeginImpl(data), EndImpl(data), predicate);
        result = wstl::IsPartitioned(BeginImpl(data), EndImpl(data), predicate);
        CHECK_EQ(expected, result);
    }

    TEST_CASE("Partition") {
        // Forward iterator
        std::array<int, 6> initial = {1, 2, 3, 4, 5, 6};

        std::forward_list<int> expected1(BeginImpl(initial), EndImpl(initial));
        std::forward_list<int> data1(BeginImpl(initial), EndImpl(initial));

        bool complete = false;

        while(!complete) {
            std::forward_list<int>::iterator pivot1 = wstl::Partition(data1.begin(), data1.end(), wstl::IsEven<int>);
            std::forward_list<int>::iterator pivot2 = std::partition(expected1.begin(), expected1.end(), wstl::IsEven<int>);

            ptrdiff_t distance1 = std::distance(data1.begin(), pivot1);
            ptrdiff_t distance2 = std::distance(expected1.begin(), pivot2);

            CHECK_EQ(distance1, distance2);
            CHECK_EQ(*pivot1, *pivot2);

            bool result = std::equal(expected1.begin(), expected1.end(), data1.begin());
            CHECK(result);

            complete = !std::next_permutation(BeginImpl(initial), EndImpl(initial));

            expected1.assign(BeginImpl(initial), EndImpl(initial));
            data1.assign(BeginImpl(initial), EndImpl(initial));
        }

        // Bidirectional iterator
        std::array<int, 6> expected2 = initial;
        std::array<int, 6> data2 = initial;

        complete = false;

        while(!complete) {
            std::array<int, 6>::iterator pivot1 = wstl::Partition(data2.begin(), data2.end(), wstl::IsEven<int>);
            std::array<int, 6>::iterator pivot2 = std::partition(expected2.begin(), expected2.end(), wstl::IsEven<int>);

            ptrdiff_t distance1 = std::distance(data2.begin(), pivot1);
            ptrdiff_t distance2 = std::distance(expected2.begin(), pivot2);

            CHECK_EQ(distance1, distance2);
            CHECK_EQ(*pivot1, *pivot2);

            bool result = std::equal(expected2.begin(), expected2.end(), data2.begin());
            CHECK(result);

            complete = !std::next_permutation(BeginImpl(initial), EndImpl(initial));

            expected2 = initial;
            data2 = initial;
        }
    }

    TEST_CASE("PartitionCopy") {
        int data1False[SIZE] = {0};
        int data1True[SIZE] = {0};
        int data2False[SIZE] = {0};
        int data2True[SIZE] = {0};

        #ifdef __WSTL_CXX11__
        auto predicate = std::bind(std::greater<int>(), std::placeholders::_1, 4);
        #else
        std::binder2nd<std::greater<int> > predicate(std::greater<int>(), 4);
        #endif

        wstl::PartitionCopy(BeginImpl(dataA), EndImpl(dataA), data1True, data1False, predicate);
        std::partition_copy(BeginImpl(dataA), EndImpl(dataA), data2True, data2False, predicate);

        bool result = std::equal(BeginImpl(data2True), EndImpl(data2True), BeginImpl(data1True));
        CHECK(result);

        result = std::equal(BeginImpl(data2False), EndImpl(data2False), BeginImpl(data1False));
        CHECK(result);
    }

    TEST_CASE("StablePartition") {
        std::vector<NonTrivialData> initial;

        initial.push_back(NonTrivialData(1, 1));
        initial.push_back(NonTrivialData(2, 1));
        initial.push_back(NonTrivialData(3, 1));
        initial.push_back(NonTrivialData(2, 2));
        initial.push_back(NonTrivialData(3, 2));
        initial.push_back(NonTrivialData(4, 1));
        initial.push_back(NonTrivialData(2, 3));
        initial.push_back(NonTrivialData(3, 3));
        initial.push_back(NonTrivialData(5, 1));

        std::vector<NonTrivialData> data1(initial);
        std::vector<NonTrivialData> data2(initial);

        wstl::StablePartition(data1.begin(), data1.end(), &StablePartitionPredicate);
        std::stable_partition(data2.begin(), data2.end(), &StablePartitionPredicate);

        bool result = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(result);       
    }

    TEST_CASE("PartitionPoint") {
        int data[] = {1, 2, 3, 4, 5, 6, 7, 8};

        #ifdef __WSTL_CXX11__
        auto predicate1 = std::bind(std::greater<int>(), std::placeholders::_1, 4);
        auto predicate2 = std::bind(std::greater<int>(), std::placeholders::_1, 8);
        #else
        std::binder2nd<std::greater<int> > predicate1(std::greater<int>(), 4);
        std::binder2nd<std::greater<int> > predicate2(std::greater<int>(), 8);
        #endif

        std::partition(BeginImpl(data), EndImpl(data), predicate1);

        int* p1 = wstl::PartitionPoint(BeginImpl(data), EndImpl(data), predicate1);
        int* p2 = std::partition_point(BeginImpl(data), EndImpl(data), predicate1);
        CHECK_EQ(p1, p2);

        std::partition(BeginImpl(data), EndImpl(data), predicate2);

        p1 = wstl::PartitionPoint(BeginImpl(data), EndImpl(data), predicate2);
        p2 = std::partition_point(BeginImpl(data), EndImpl(data), predicate2);
        CHECK_EQ(p1, p2);
    }

    TEST_CASE("MinElement") {
        // Normal
        int* expected = std::min_element(BeginImpl(dataA), EndImpl(dataA));
        int* result = wstl::MinElement(BeginImpl(dataA), EndImpl(dataA));
        CHECK_EQ(expected, result);

        // Compare
        expected = std::min_element(BeginImpl(dataA), EndImpl(dataA), std::greater<int>());
        result = wstl::MinElement(BeginImpl(dataA), EndImpl(dataA), std::greater<int>());
        CHECK_EQ(expected, result);

        // Empty
        std::array<int, 0> empty;

        std::array<int, 0>::iterator expectedEmpty = std::min_element(BeginImpl(empty), EndImpl(empty), std::greater<int>());
        std::array<int, 0>::iterator resultEmpty = wstl::MinElement(BeginImpl(empty), EndImpl(empty), std::greater<int>());
        CHECK_EQ(expectedEmpty, resultEmpty);
    }

    TEST_CASE("Min") {
        int a = 1;
        int b = 2;

        CHECK_EQ(wstl::Min(a, b), 1);
        CHECK_EQ(wstl::Min(a, b, std::greater<int>()), 2);
        CHECK_EQ((wstl::compile::Min<int, 1, 2>::Value), 1);
        CHECK_EQ((wstl::compile::Min<int, 1, 2, wstl::compile::Greater<int> >::Value), 2);
    }

    TEST_CASE("MaxElement") {
        // Normal
        int* expected = std::max_element(BeginImpl(dataA), EndImpl(dataA));
        int* result = wstl::MaxElement(BeginImpl(dataA), EndImpl(dataA));
        CHECK_EQ(expected, result);

        // Compare
        expected = std::max_element(BeginImpl(dataA), EndImpl(dataA), std::greater<int>());
        result = wstl::MaxElement(BeginImpl(dataA), EndImpl(dataA), std::greater<int>());
        CHECK_EQ(expected, result);

        // Empty
        std::array<int, 0> empty;

        std::array<int, 0>::iterator expectedEmpty = std::max_element(BeginImpl(empty), EndImpl(empty), std::greater<int>());
        std::array<int, 0>::iterator resultEmpty = wstl::MaxElement(BeginImpl(empty), EndImpl(empty), std::greater<int>());
        CHECK_EQ(expectedEmpty, resultEmpty);
    }

    TEST_CASE("Max") {
        int a = 1;
        int b = 2;

        CHECK_EQ(wstl::Max(a, b), 2);
        CHECK_EQ(wstl::Max(a, b, std::greater<int>()), 1);
        CHECK_EQ((wstl::compile::Max<int, 1, 2>::Value), 2);
        CHECK_EQ((wstl::compile::Max<int, 1, 2, wstl::compile::Greater<int> >::Value), 1);
    }

    TEST_CASE("MinMaxElement") {
        // Normal
        std::pair<int*, int*> expected = std::minmax_element(BeginImpl(dataA), EndImpl(dataA));
        wstl::Pair<int*, int*> result = wstl::MinMaxElement(BeginImpl(dataA), EndImpl(dataA));
        CHECK_EQ(expected.first, result.First);
        CHECK_EQ(expected.second, result.Second);

        // Compare
        expected = std::minmax_element(BeginImpl(dataA), EndImpl(dataA), std::greater<int>());
        result = wstl::MinMaxElement(BeginImpl(dataA), EndImpl(dataA), std::greater<int>());
        CHECK_EQ(expected.first, result.First);
        CHECK_EQ(expected.second, result.Second);

        // Empty
        typedef std::array<int, 0> EmptyArray;
        EmptyArray empty;

        std::pair<EmptyArray::iterator, EmptyArray::iterator> expectedEmpty = std::minmax_element(BeginImpl(empty), EndImpl(empty), std::greater<int>());
        wstl::Pair<EmptyArray::iterator, EmptyArray::iterator> resultEmpty = wstl::MinMaxElement(BeginImpl(empty), EndImpl(empty), std::greater<int>());
        CHECK_EQ(expectedEmpty.first, resultEmpty.First);
        CHECK_EQ(expectedEmpty.second, resultEmpty.Second);
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
        CHECK_EQ((wstl::compile::Clamp<int, 5, 0, 10>::Value), 5);
        CHECK_EQ((wstl::compile::Clamp<int, -5, 0, 10>::Value), 0);
        CHECK_EQ((wstl::compile::Clamp<int, 15, 0, 10>::Value), 10);

        // Constexpr
        #ifdef __WSTL_CXX11__
        constexpr int result1 = wstl::Clamp(5, 0, 10);
        constexpr int result2 = wstl::Clamp(-5, 0, 10);
        constexpr int result3 = wstl::Clamp(15, 0, 10);

        CHECK_EQ(result1, 5);
        CHECK_EQ(result2, 0);
        CHECK_EQ(result3, 10);
        #endif
    }

    TEST_CASE("Equal") {
        int dataSmall[] = {1, 2, 3};

        CHECK(wstl::Equal(BeginImpl(dataA), EndImpl(dataA), BeginImpl(dataLA)));
        CHECK(!wstl::Equal(BeginImpl(dataA), EndImpl(dataA), BeginImpl(dataB)));
        CHECK(wstl::Equal(BeginImpl(dataA), EndImpl(dataA), BeginImpl(dataLA), EndImpl(dataLA)));
        CHECK(!wstl::Equal(BeginImpl(dataA), EndImpl(dataA), BeginImpl(dataSmall), EndImpl(dataSmall)));
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
        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
        std::vector<uint32_t> data1(BeginImpl(arr), EndImpl(arr));
        std::vector<uint32_t> data2(BeginImpl(arr), EndImpl(arr));

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
        #ifdef __WSTL_CXX11__
        typedef MovableData<uint32_t> Item;
        #else
        typedef uint32_t Item;
        #endif

        typedef std::vector<Item> Data;
        Data data1;

        // Create some data
        Item p1(1U), p2(2U), p3(3U), p4(4U), p5(5U), p6(6U), p7(7U), p8(8U);

        data1.push_back(__WSTL_MOVE__(p1));
        data1.push_back(__WSTL_MOVE__(p2));
        data1.push_back(__WSTL_MOVE__(p3));
        data1.push_back(__WSTL_MOVE__(p4));
        data1.push_back(__WSTL_MOVE__(p5));
        data1.push_back(__WSTL_MOVE__(p6));
        data1.push_back(__WSTL_MOVE__(p7));
        data1.push_back(__WSTL_MOVE__(p8));

        Data data2;
        data2.push_back(Item(1U));
        data2.push_back(Item(2U));
        data2.push_back(Item(3U));
        data2.push_back(Item(4U));
        data2.push_back(Item(5U));
        data2.push_back(Item(6U));
        data2.push_back(Item(7U));
        data2.push_back(Item(8U));

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

        int* p1 = wstl::IsSortedUntil(BeginImpl(data1), EndImpl(data1));
        int* p2 = std::is_sorted_until(BeginImpl(data1), EndImpl(data1));
        CHECK_EQ(p1, p2);

        p1 = wstl::IsSortedUntil(BeginImpl(data2), EndImpl(data2));
        p2 = std::is_sorted_until(BeginImpl(data2), EndImpl(data2));
        CHECK_EQ(p1, p2); 
    }

    TEST_CASE("IsSorted") {
        int data1[] = { 1, 2, 3, 4, 6, 5, 7, 8, 9, 10 };
        int data1Sorted[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int data2[] = { 10, 9, 8, 7, 5, 6, 4, 3, 2, 1 };
        int data2Sorted[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

        bool result = wstl::IsSorted(BeginImpl(data1), EndImpl(data1));
        CHECK(!result);

        bool result2 = wstl::IsSorted(BeginImpl(data1Sorted), EndImpl(data1Sorted));
        CHECK(result2);

        result = wstl::IsSorted(BeginImpl(data2), EndImpl(data2), std::greater<int>());
        CHECK(!result);

        result = wstl::IsSorted(BeginImpl(data2Sorted), EndImpl(data2Sorted), std::greater<int>());
        CHECK(result);
    }

    TEST_CASE("QuickSort") {
        std::vector<int> data(100, 0);
        std::iota(data.begin(), data.end(), 1);

        for(int i = 0; i < 100; ++i) {
            #ifdef __WSTL_CXX11__
            std::shuffle(data.begin(), data.end(), urng);
            #else
            std::random_shuffle(data.begin(), data.end());
            #endif

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
        typedef std::array<int, 10> Array;
        Array initial = {5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

        for(size_t i = 0; i < initial.size(); ++i) {
            Array data1 = initial;
            Array data2 = initial;

            wstl::PartialSort(data1.begin(), data1.begin() + ptrdiff_t(i), data1.end());
            std::partial_sort(data2.begin(), data2.begin() + ptrdiff_t(i), data2.end());

            bool equal = std::equal(data1.begin(), data1.begin() + ptrdiff_t(i), data2.begin());

            CAPTURE(i);

            if (!equal) {
                for (Array::iterator x = data1.begin(); x != data1.end(); ++x) std::cout << *x << ' ';
                std::cout << '\n';

                for (Array::iterator x = data2.begin(); x != data2.end(); ++x) std::cout << *x << ' ';
                std::cout << '\n';
            }

            CHECK(equal);

            data1 = initial;
            data2 = initial;

            wstl::PartialSort(data1.begin(), data1.begin() + ptrdiff_t(i), data1.end(), std::greater<int>());
            std::partial_sort(data2.begin(), data2.begin() + ptrdiff_t(i), data2.end(), std::greater<int>());

            equal = std::equal(data1.begin(), data1.begin() + ptrdiff_t(i), data2.begin());

            CAPTURE(i);

            if (!equal) {
                for (Array::iterator x = data1.begin(); x != data1.end(); ++x) std::cout << *x << ' ';
                std::cout << '\n';

                for (Array::iterator x = data2.begin(); x != data2.end(); ++x) std::cout << *x << ' ';
                std::cout << '\n';
            }

            CHECK(equal);
        }
    }

    TEST_CASE("PartialSortCopy") {
        std::array<int, 10> initial = {5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

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

        wstl::Merge(BeginImpl(data1), EndImpl(data1), BeginImpl(data2), EndImpl(data2), std::back_inserter(result));
        std::merge(BeginImpl(data1), EndImpl(data1), BeginImpl(data2), EndImpl(data2), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        result.clear();
        expected.clear();

        wstl::Merge(BeginImpl(data1), EndImpl(data1), BeginImpl(data2), EndImpl(data2), std::back_inserter(result), std::greater<int>());
        std::merge(BeginImpl(data1), EndImpl(data1), BeginImpl(data2), EndImpl(data2), std::back_inserter(expected), std::greater<int>());

        equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);
    }

    TEST_CASE("InplaceMerge") {
        std::array<int, 9> data1 = {2, 4, 6, 7, 9, 1, 3, 5, 8};
        std::array<int, 9> data2 = {9, 7, 6, 4, 1, 8, 5, 3, 2};

        std::array<int, 9> result(data1);
        std::array<int, 9> expected(data1);

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
            #ifdef __WSTL_CXX11__
            std::shuffle(data.begin(), data.end(), urng);
            #else
            std::random_shuffle(data.begin(), data.end());
            #endif

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
            #ifdef __WSTL_CXX11__
            std::shuffle(data.begin(), data.end(), urng);
            #else
            std::random_shuffle(data.begin(), data.end());
            #endif

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
            #ifdef __WSTL_CXX11__
            std::shuffle(data.begin(), data.end(), urng);
            #else
            std::random_shuffle(data.begin(), data.end());
            #endif

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
            #ifdef __WSTL_CXX11__
            std::shuffle(data.begin(), data.end(), urng);
            #else
            std::random_shuffle(data.begin(), data.end());
            #endif

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
        std::vector<NonTrivialData> initial;

        initial.push_back(NonTrivialData(1, 1));
        initial.push_back(NonTrivialData(2, 1));
        initial.push_back(NonTrivialData(3, 1));
        initial.push_back(NonTrivialData(2, 2));
        initial.push_back(NonTrivialData(3, 2));
        initial.push_back(NonTrivialData(4, 1));
        initial.push_back(NonTrivialData(2, 3));
        initial.push_back(NonTrivialData(3, 3));
        initial.push_back(NonTrivialData(5, 1));

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
            int* expected1 = std::lower_bound(BeginImpl(dataA), EndImpl(dataA), i);
            int* result1 = wstl::LowerBound(BeginImpl(dataA), EndImpl(dataA), i);
            CHECK_EQ(expected1, result1);

            // Non-random access iterator
            std::list<int>::iterator expected2 = std::lower_bound(BeginImpl(dataLA), EndImpl(dataLA), i);
            std::list<int>::iterator result2 = wstl::LowerBound(BeginImpl(dataLA), EndImpl(dataLA), i);
            CHECK_EQ(expected2, result2);
        }
    }

    TEST_CASE("UpperBound") {
        for(int i = 0; i < 9; ++i) {
            // Random access iterator
            int* expected1 = std::upper_bound(BeginImpl(dataA), EndImpl(dataA), i);
            int* result1 = wstl::UpperBound(BeginImpl(dataA), EndImpl(dataA), i);
            CHECK_EQ(expected1, result1);

            // Non-random access iterator
            std::list<int>::iterator expected2 = std::upper_bound(BeginImpl(dataLA), EndImpl(dataLA), i);
            std::list<int>::iterator result2 = wstl::UpperBound(BeginImpl(dataLA), EndImpl(dataLA), i);
            CHECK_EQ(expected2, result2);
        }
    }

    TEST_CASE("BinarySearch") {
        int data1[] = {1, 3, 4, 5, 8, 9};
        int data2[] = {9, 8, 6, 5, 2, 0};

        bool expected = std::binary_search(BeginImpl(data1), EndImpl(data1), 5);
        bool result = wstl::BinarySearch(BeginImpl(data1), EndImpl(data1), 5);

        CHECK_EQ(expected, result);

        expected = std::binary_search(BeginImpl(data2), EndImpl(data2), 5, std::greater<int>());
        result = wstl::BinarySearch(BeginImpl(data2), EndImpl(data2), 5, std::greater<int>());

        CHECK_EQ(expected, result);
    }

    TEST_CASE("EqualRange") {
        for(int i = 0; i < 9; ++i) {
            // Random access iterator
            std::pair<int*, int*> expected1 = std::equal_range(BeginImpl(dataA), EndImpl(dataA), i);
            wstl::Pair<int*, int*> result1 = wstl::EqualRange(BeginImpl(dataA), EndImpl(dataA), i);

            CHECK_EQ(expected1.first, result1.First);
            CHECK_EQ(expected1.second, result1.Second);

            // Non-random access iterator
            typedef std::list<int>::iterator Iterator;
            std::pair<Iterator, Iterator> expected2 = std::equal_range(BeginImpl(dataLA), EndImpl(dataLA), i);
            wstl::Pair<Iterator, Iterator> result2 = wstl::EqualRange(BeginImpl(dataLA), EndImpl(dataLA), i);
            
            CHECK_EQ(expected2.first, result2.First);
            CHECK_EQ(expected2.second, result2.Second);
        }
    }

    TEST_CASE("BinaryFind") {
        int data1[] = {1, 3, 4, 5, 8, 9};
        int data2[] = {9, 8, 6, 5, 3, 0};

        int* it1 = std::find(BeginImpl(data1), EndImpl(data1), 3);
        int* it2 = wstl::BinaryFind(BeginImpl(data1), EndImpl(data1), 3);

        CHECK_EQ(it1, it2);

        it1 = std::find(BeginImpl(data2), EndImpl(data2), 5);
        it2 = wstl::BinaryFind(BeginImpl(data2), EndImpl(data2), 5, std::greater<int>(), std::less_equal<int>());

        CHECK_EQ(it1, it2);
    }

    TEST_CASE("Includes") {
        int data1[] = {1, 2, 6, 7, 8, 10, 12};
        int sequence1True[] = {2, 8, 12};
        int sequence1False[] = {1, 6, 6, 7};
        int data2[] = {12, 10, 8, 7, 6, 2, 1};
        int sequence2True[] = {12, 8, 2};
        int sequence2False[] = {7, 6, 6, 1};

        bool result = wstl::Includes(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1True), EndImpl(sequence1True));
        CHECK(result);

        result = wstl::Includes(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1False), EndImpl(sequence1False));
        CHECK(!result);

        result = wstl::Includes(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2True), EndImpl(sequence2True), std::greater<int>());
        CHECK(result);

        result = wstl::Includes(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2False), EndImpl(sequence2False), std::greater<int>());
        CHECK(!result);
    }

    TEST_CASE("SetDifference") {
        int data1[] = {1, 2, 6, 7, 8, 10, 12};
        int sequence1[] = {2, 8, 12};
        int data2[] = {12, 10, 8, 7, 6, 2, 1};
        int sequence2[] = {12, 8, 2};

        std::vector<int> result;
        std::vector<int> expected;

        wstl::SetDifference(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1), EndImpl(sequence1), std::back_inserter(result));
        std::set_difference(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1), EndImpl(sequence1), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        wstl::SetDifference(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2), EndImpl(sequence2), std::back_inserter(result), std::greater<int>());
        std::set_difference(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2), EndImpl(sequence2), std::back_inserter(expected), std::greater<int>());

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

        wstl::SetIntersection(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1), EndImpl(sequence1), std::back_inserter(result));
        std::set_intersection(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1), EndImpl(sequence1), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        wstl::SetIntersection(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2), EndImpl(sequence2), std::back_inserter(result), std::greater<int>());
        std::set_intersection(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2), EndImpl(sequence2), std::back_inserter(expected), std::greater<int>());

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

        wstl::SetDifference(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1), EndImpl(sequence1), std::back_inserter(result));
        std::set_difference(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1), EndImpl(sequence1), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        wstl::SetDifference(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2), EndImpl(sequence2), std::back_inserter(result), std::greater<int>());
        std::set_difference(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2), EndImpl(sequence2), std::back_inserter(expected), std::greater<int>());

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

        wstl::SetSymmetricDifference(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1), EndImpl(sequence1), std::back_inserter(result));
        std::set_symmetric_difference(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1), EndImpl(sequence1), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        wstl::SetSymmetricDifference(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2), EndImpl(sequence2), std::back_inserter(result), std::greater<int>());
        std::set_symmetric_difference(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2), EndImpl(sequence2), std::back_inserter(expected), std::greater<int>());

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

        wstl::SetUnion(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1), EndImpl(sequence1), std::back_inserter(result));
        std::set_union(BeginImpl(data1), EndImpl(data1), BeginImpl(sequence1), EndImpl(sequence1), std::back_inserter(expected));

        bool equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);

        wstl::SetUnion(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2), EndImpl(sequence2), std::back_inserter(result), std::greater<int>());
        std::set_union(BeginImpl(data2), EndImpl(data2), BeginImpl(sequence2), EndImpl(sequence2), std::back_inserter(expected), std::greater<int>());

        equal = std::equal(expected.begin(), expected.end(), result.begin());
        CHECK(equal);
    }

    TEST_CASE("IsPermutation") {
        int data[] = {1, 2, 3, 4, 5, 6, 7, 8};
        int permutation[] = {1, 3, 2, 4, 6, 5, 7, 8};
        int notPermutation[] = {1, 3, 2, 2, 6, 5, 7, 8};

        bool result = wstl::IsPermutation(BeginImpl(data), EndImpl(data), BeginImpl(permutation));
        CHECK(result);

        result = wstl::IsPermutation(BeginImpl(data), EndImpl(data), BeginImpl(notPermutation));
        CHECK(!result);

        result = wstl::IsPermutation(BeginImpl(data), EndImpl(data), BeginImpl(permutation), wstl::EqualTo<int>());
        CHECK(result);

        result = wstl::IsPermutation(BeginImpl(data), EndImpl(data), BeginImpl(notPermutation), wstl::EqualTo<int>());
        CHECK(!result);

        result = wstl::IsPermutation(BeginImpl(data), EndImpl(data), BeginImpl(permutation), EndImpl(permutation));
        CHECK(result);

        result = wstl::IsPermutation(BeginImpl(data), EndImpl(data), BeginImpl(notPermutation), EndImpl(notPermutation));
        CHECK(!result);

        result = wstl::IsPermutation(BeginImpl(data), EndImpl(data), BeginImpl(permutation), EndImpl(permutation), wstl::EqualTo<int>());
        CHECK(result);

        result = wstl::IsPermutation(BeginImpl(data), EndImpl(data), BeginImpl(notPermutation), EndImpl(notPermutation), wstl::EqualTo<int>());
        CHECK(!result);
    }

    TEST_CASE("NextPermutation") {
        std::vector<int> initial1;
        initial1.push_back(1);
        initial1.push_back(2);
        initial1.push_back(3);
        initial1.push_back(4);
        initial1.push_back(5);
        initial1.push_back(6);

        std::vector<int> initial2;
        initial2.push_back(6);
        initial2.push_back(5);
        initial2.push_back(4);
        initial2.push_back(3);
        initial2.push_back(2);
        initial2.push_back(1);

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
        std::vector<int> initial1;
        initial1.push_back(1);
        initial1.push_back(2);
        initial1.push_back(3);
        initial1.push_back(4);
        initial1.push_back(5);
        initial1.push_back(6);

        std::vector<int> initial2;
        initial2.push_back(6);
        initial2.push_back(5);
        initial2.push_back(4);
        initial2.push_back(3);
        initial2.push_back(2);
        initial2.push_back(1);

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
        std::list<int> data3(BeginImpl(data1), EndImpl(data1));
        std::list<int> data4(BeginImpl(data2), EndImpl(data2));

        int out1[10];
        int out2[5];

        int check1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        int check2[] = { 1, 2, 3, 4, 5 };
        int check3[] = { 1, 2, 3, 4, 5, 0, 0, 0, 0, 0 };

        int* result;

        // Same size

        // Random access iterator
        std::fill(BeginImpl(out1), EndImpl(out1), 0);
        result = wstl::CopySafe(BeginImpl(data1), EndImpl(data1), BeginImpl(out1), EndImpl(out1));
        CHECK_EQ(EndImpl(out1), result);

        bool equal = std::equal(BeginImpl(out1), EndImpl(out1), BeginImpl(check1));
        CHECK(equal);

        // Non-random access iterator
        std::fill(BeginImpl(out1), EndImpl(out1), 0);
        result = wstl::CopySafe(BeginImpl(data3), EndImpl(data3), BeginImpl(out1), EndImpl(out1));
        CHECK_EQ(EndImpl(out1), result);

        equal = std::equal(BeginImpl(out1), EndImpl(out1), BeginImpl(check1));
        CHECK(equal);

        // Destination smaller

        // Random access iterator
        std::fill(BeginImpl(out2), EndImpl(out2), 0);
        result = wstl::CopySafe(BeginImpl(data1), EndImpl(data1), BeginImpl(out2), EndImpl(out2));
        CHECK_EQ(EndImpl(out2), result);

        equal = std::equal(BeginImpl(out2), EndImpl(out2), BeginImpl(check2));
        CHECK(equal);

        // Non-random access iterator
        std::fill(BeginImpl(out2), EndImpl(out2), 0);
        result = wstl::CopySafe(BeginImpl(data3), EndImpl(data3), BeginImpl(out2), EndImpl(out2));
        CHECK_EQ(EndImpl(out2), result);

        equal = std::equal(BeginImpl(out2), EndImpl(out2), BeginImpl(check2));
        CHECK(equal);

        // Source smaller

        // Random access iterator
        std::fill(BeginImpl(out1), EndImpl(out1), 0);
        result = wstl::CopySafe(BeginImpl(data4), EndImpl(data4), BeginImpl(out1), EndImpl(out1));
        CHECK_EQ(BeginImpl(out1) + 5, result);

        equal = std::equal(BeginImpl(out1), EndImpl(out1), BeginImpl(check3));
        CHECK(equal);

        // Non-random access iterator
        std::fill(BeginImpl(out1), EndImpl(out1), 0);
        result = wstl::CopySafe(BeginImpl(data4), EndImpl(data4), BeginImpl(out1), EndImpl(out1));
        CHECK_EQ(BeginImpl(out1) + 5, result);

        equal = std::equal(BeginImpl(out1), EndImpl(out1), BeginImpl(check3));
        CHECK(equal);
    }

    TEST_CASE("MoveSafe") {
        #ifdef __WSTL_CXX11__
        typedef MovableData<uint32_t> Item;
        #else
        typedef uint32_t Item;
        #endif

        typedef std::vector<Item> Data;
        Data data1;

        // Create some data
        Item p1(1U), p2(2U), p3(3U), p4(4U), p5(5U), p6(6U), p7(7U), p8(8U);

        data1.push_back(__WSTL_MOVE__(p1));
        data1.push_back(__WSTL_MOVE__(p2));
        data1.push_back(__WSTL_MOVE__(p3));
        data1.push_back(__WSTL_MOVE__(p4));
        data1.push_back(__WSTL_MOVE__(p5));
        data1.push_back(__WSTL_MOVE__(p6));
        data1.push_back(__WSTL_MOVE__(p7));
        data1.push_back(__WSTL_MOVE__(p8));

        Data data2;
        data2.push_back(Item(1U));
        data2.push_back(Item(2U));
        data2.push_back(Item(3U));
        data2.push_back(Item(4U));
        data2.push_back(Item(5U));

        std::list<Item> data3;
        data3.push_back(Item(1U));
        data3.push_back(Item(2U));
        data3.push_back(Item(3U));
        data3.push_back(Item(4U));
        data3.push_back(Item(5U));
        data3.push_back(Item(6U));
        data3.push_back(Item(7U));
        data3.push_back(Item(8U));

        std::list<Item> data4;
        data4.push_back(Item(1U));
        data4.push_back(Item(2U));
        data4.push_back(Item(3U));
        data4.push_back(Item(4U));
        data4.push_back(Item(5U));

        std::vector<Item> out1(10);
        std::vector<Item> out2(5);

        std::vector<Item> check1;
        check1.push_back(Item(1U));
        check1.push_back(Item(2U));
        check1.push_back(Item(3U));
        check1.push_back(Item(4U));
        check1.push_back(Item(5U));
        check1.push_back(Item(6U));
        check1.push_back(Item(7U));
        check1.push_back(Item(8U));

        std::vector<Item> check2;
        check2.push_back(Item(1U));
        check2.push_back(Item(2U));
        check2.push_back(Item(3U));
        check2.push_back(Item(4U));
        check2.push_back(Item(5U));

        std::vector<Item> check3;
        check3.push_back(Item(1U));
        check3.push_back(Item(2U));
        check3.push_back(Item(3U));
        check3.push_back(Item(4U));
        check3.push_back(Item(5U));
        check3.push_back(Item(0U));
        check3.push_back(Item(0U));
        check3.push_back(Item(0U));
        check3.push_back(Item(0U));
        check3.push_back(Item(0U));

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
