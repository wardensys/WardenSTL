// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Utility.hpp>

#include "Utils.hpp"


TEST_SUITE("Utility") {
    TEST_CASE("AsConst") {
        Dummy d;
        const Dummy cd;
        const Dummy& ref = wstl::AsConst(d);

        CHECK(wstl::IsSame<decltype(wstl::AsConst(d)), const Dummy&>::Value);
        CHECK(wstl::IsSame<decltype(wstl::AsConst(cd)), const Dummy&>::Value);
        CHECK_EQ(&ref, &d);

        // The following line should not compile due to deleted overload for rvalue references
        // wstl::AsConst(Dummy{});
    }

    TEST_CASE("Exchange") {
        int a = 1;
        int b = 2;
        int c = wstl::Exchange(a, b);

        CHECK_EQ(a, 2);
        CHECK_EQ(b, 2);
        CHECK_EQ(c, 1);

        // Different type
        short d = 128;
        int e = wstl::Exchange(c, d);

        CHECK_EQ(c, 128);
        CHECK_EQ(d, 128);
        CHECK_EQ(e, 1);

        // Const
        const int f = 4;
        int g = wstl::Exchange(b, f);

        CHECK_EQ(b, 4);
        CHECK_EQ(f, 4);
        CHECK_EQ(g, 2);
    }

    TEST_CASE("Pair default constructor") {
        wstl::Pair<int, double> p;

        CHECK_EQ(p.First, int());
        CHECK_EQ(p.Second, double());
    }

    TEST_CASE("Pair parameterized constructor") {
        wstl::Pair<int, double> p(128, 6.6);

        CHECK_EQ(p.First, 128);
        CHECK_EQ(p.Second, 6.6);
    }

    TEST_CASE("Pair copy constructor") {
        wstl::Pair<int, double> p1(128, 6.6);
        wstl::Pair<int, double> p2(p1);

        CHECK_EQ(p1.First, p2.First);
        CHECK_EQ(p1.Second, p2.Second);
    }

    TEST_CASE("Pair move constructor from parameters") {
        wstl::Pair<MovableData<int>, MovableData<int>> p(10, 20);

        CHECK_EQ(p.First.Value, 10);
        CHECK_EQ(p.Second.Value, 20);
    }

    TEST_CASE("Pair move constructor") {
        wstl::Pair<MovableData<int>, MovableData<int>> p1(128, 6);
        wstl::Pair<MovableData<int>, MovableData<int>> p2(wstl::Move(p1));

        CHECK(!p1.First);
        CHECK(!p1.Second);

        CHECK_EQ(p2.First.Value, 128);
        CHECK_EQ(p2.Second.Value, 6);
    }

    TEST_CASE("Pair templated move constructor") {
        wstl::Pair<MovableData<int>, MovableData<int>> p1(128, 6);
        wstl::Pair<MovableData<long>, MovableData<long>> p2(wstl::Move(p1));

        CHECK(!p1.First);
        CHECK(!p1.Second);

        CHECK_EQ(p2.First.Value, 128);
        CHECK_EQ(p2.Second.Value, 6);
    }

    TEST_CASE("Pair piecewise construct") {
        wstl::Pair<NonTrivialData, NonTrivialData> p(wstl::PiecewiseConstruct, wstl::ForwardAsTuple(1, 10),
            wstl::ForwardAsTuple(30, 40));

        CHECK_EQ(p.First.A, 1);
        CHECK_EQ(p.First.B, 10);
        CHECK_EQ(p.Second.A, 30);
        CHECK_EQ(p.Second.B, 40);
    }

    TEST_CASE("Pair swap") {
        wstl::Pair<int, int> p1(1, 2);
        wstl::Pair<int, int> p2(3, 4);

        wstl::Swap(p1, p2); // Effectively calls p1.Swap(p2);

        CHECK_EQ(p1.First, 3);
        CHECK_EQ(p1.Second, 4);
        CHECK_EQ(p2.First, 1);
        CHECK_EQ(p2.Second, 2);
    }

    TEST_CASE("Pair copy assignment operator") {
        wstl::Pair<int, int> p1(1, 2);
        wstl::Pair<int, int> p2(3, 4);

        p1 = p2;

        CHECK_EQ(p1.First, 3);
        CHECK_EQ(p1.Second, 4);
    }

    TEST_CASE("Pair templated copy assignment operator") {
        wstl::Pair<long, long> p1(1, 2);
        wstl::Pair<long, int> p2(3, 4);

        p1 = p2;

        CHECK_EQ(p1.First, 3);
        CHECK_EQ(p1.Second, 4);
    }

    TEST_CASE("Pair move assignment operator") {
        wstl::Pair<MovableData<int>, MovableData<int>> p1(1, 2);
        wstl::Pair<MovableData<int>, MovableData<int>> p2(3, 4);

        p1 = wstl::Move(p2);

        CHECK(!p2.First);
        CHECK(!p2.Second);

        CHECK_EQ(p1.First.Value, 3);
        CHECK_EQ(p1.Second.Value, 4);
    }

    TEST_CASE("Pair templated move assignment operator") {
        wstl::Pair<MovableData<long>, MovableData<long>> p1(1, 2);
        wstl::Pair<MovableData<int>, MovableData<int>> p2(3, 4);

        p1 = wstl::Move(p2);

        CHECK(!p2.First);
        CHECK(!p2.Second);

        CHECK_EQ(p1.First.Value, 3);
        CHECK_EQ(p1.Second.Value, 4);
    }

    #ifdef __WSTL_CXX17__
    TEST_CASE("Pair template deduction guide") {
        wstl::Pair p(67, 6.7);

        CHECK(wstl::IsSameValue<decltype(p.First), int>);
        CHECK(wstl::IsSameValue<decltype(p.Second), double>);
        CHECK_EQ(p.First, 67);
        CHECK_EQ(p.Second, 6.7);
    }
    #endif

    TEST_CASE("Pair TupleSize specialization") {
        CHECK_EQ(wstl::TupleSize<wstl::Pair<int, double>>::Value, 2UL);
    }

    TEST_CASE("Pair TupleElement specialization") {
        CHECK(wstl::IsSame<wstl::TupleElementType<0, wstl::Pair<int, double>>, int>::Value);
        CHECK(wstl::IsSame<wstl::TupleElementType<1, wstl::Pair<int, double>>, double>::Value);

        // The following line should fail with a compilation error
        // wstl::TupleElementType<2, wstl::Pair<int, double>> er;
    }

    TEST_CASE("Pair Get specialization") {
        wstl::Pair<int, MovableData<int>> data(10, 64);
        const wstl::Pair<int, MovableData<int>> constData(20, 32);

        // Lvalue
        int i0 = wstl::Get<0>(data);
        int ci0 = wstl::Get<0>(constData);
        MovableData<int> m1 = wstl::Move(wstl::Get<1>(data));
        MovableData<int> cm1 = wstl::Move(wstl::Get<1>(constData));

        CHECK_EQ(i0, 10);
        CHECK_EQ(ci0, 20);
        CHECK_EQ(m1.Value, 64);
        CHECK_EQ(cm1.Value, 32);

        // The following lines should fail with a compilation error
        // auto el1 = wstl::Get<2>(data);
        // auto el2 = wstl::Get<2>(constData);

        // Rvalue
        int&& ri0 = wstl::Get<0>(wstl::Move(data));
        const int&& rci0 = wstl::Get<0>(wstl::Move(constData));
        MovableData<int>&& rm1 = wstl::Move(wstl::Get<1>(wstl::Move(data)));
        const MovableData<int>&& rcm1 = wstl::Move(wstl::Get<1>(wstl::Move(constData)));

        CHECK_EQ(ri0, 10);
        CHECK_EQ(rci0, 20);
        CHECK_EQ(rm1.Value, 64);
        CHECK_EQ(rcm1.Value, 32);

        // The following lines should fail with a compilation error
        // auto er1 = wstl::Get<2>(wstl::Move(data));
        // auto er2 = wstl::Get<2>(wstl::Move(constData));
    }

    TEST_CASE("Pair comparison operators") {
        // Equal pairs
        wstl::Pair<int, int> ea(1, 2);
        wstl::Pair<int, int> eb(1, 2);

        CHECK(ea == eb);
        CHECK_FALSE(ea != eb);
        CHECK_FALSE(ea < eb);
        CHECK(ea <= eb);
        CHECK_FALSE(ea > eb);
        CHECK(ea >= eb);

        // First element differs
        wstl::Pair<int, int> fa(1, 2);
        wstl::Pair<int, int> fb(2, 2);

        CHECK_FALSE(fa == fb);
        CHECK(fa != fb);
        CHECK(fa < fb);
        CHECK(fa <= fb);
        CHECK_FALSE(fa > fb);
        CHECK_FALSE(fa >= fb);

        // Second element differs
        wstl::Pair<int, int> sa(1, 3);
        wstl::Pair<int, int> sb(1, 4);

        CHECK_FALSE(sa == sb);
        CHECK(sa != sb);
        CHECK(sa < sb);
        CHECK(sa <= sb);
        CHECK_FALSE(sa > sb);
        CHECK_FALSE(sa >= sb);

        // Reverse ordering
        wstl::Pair<int, int> ra(5, 6);
        wstl::Pair<int, int> rb(1, 2);

        CHECK_FALSE(ra == rb);
        CHECK(ra != rb);
        CHECK_FALSE(ra < rb);
        CHECK_FALSE(ra <= rb);
        CHECK(ra > rb);
        CHECK(ra >= rb);

        // Cross-type pairs
        wstl::Pair<int, short> ca(1, short(2));
        wstl::Pair<long, int> cb(1L, 2);

        CHECK(ca == cb);
        CHECK_FALSE(ca != cb);
        CHECK_FALSE(ca < cb);
        CHECK(ca <= cb);
        CHECK_FALSE(ca > cb);
        CHECK(ca >= cb);

        // Lexicographical compare
        wstl::Pair<int, int> la(1, 9);
        wstl::Pair<int, int> lb(2, 0);
        CHECK(la < lb);

        wstl::Pair<int, int> lc(1, 2);
        wstl::Pair<int, int> ld(1, 3);
        CHECK(lc < ld);

        wstl::Pair<int, int> le(1, 3);
        wstl::Pair<int, int> lf(1, 2);
        CHECK(le > lf);
    }

    TEST_CASE("MakePair") {
        wstl::Pair<int, double> p1(67, 6.7);
        wstl::Pair<int, double> p2;

        p2 = wstl::MakePair(67, 6.7);

        CHECK_EQ(p1.First, p2.First);
        CHECK_EQ(p1.Second, p2.Second);

        // Movable data
        wstl::Pair<MovableData<int>, MovableData<int>> mp1(67, 69);
        wstl::Pair<MovableData<int>, MovableData<int>> mp2;

        mp2 = wstl::MakePair(wstl::Move(MovableData<int>(67)), wstl::Move(MovableData<int>(69)));

        CHECK_EQ(mp1.First, mp2.First);
        CHECK_EQ(mp1.Second, mp2.Second);
    }

    #if !defined(__WSTL_TYPETRAITS_NO_BUILTINS__) && defined(__WSTL_SUPPORTED_COMPILER__)
    TEST_CASE("ToUnderlying") {
        enum Enum0 : char {
            E0 = 'w',
            E1 = 's',
            E2 = 't',
            E3 = 'l'
        };

        enum class Enum1 : uint8_t {
            D0 = 100,
            D1 = 67,
            D2 = 11
        };

        enum Enum2 : int32_t {
            I0 = 12,
            I1 = -100
        };

        Enum0 e0 = Enum0::E0;
        Enum1 e1 = Enum1::D1;
        Enum2 e2 = Enum2::I1;

        CHECK_EQ(wstl::ToUnderlying(e0), 'w');
        CHECK_EQ(wstl::ToUnderlying(e1), 67);
        CHECK_EQ(wstl::ToUnderlying(e2), -100);
        CHECK_EQ(wstl::ToUnderlying(Enum0::E1), 's');
        CHECK_EQ(wstl::ToUnderlying(Enum0::E2), 't');
        CHECK_EQ(wstl::ToUnderlying(Enum0::E3), 'l');
        CHECK_EQ(wstl::ToUnderlying(Enum1::D0), 100);
        CHECK_EQ(wstl::ToUnderlying(Enum1::D2), 11);
        CHECK_EQ(wstl::ToUnderlying(Enum2::I0), 12);
    }
    #endif

    // Test cases for IntegerSequence, MakeIndexSequence and related functions 
    // that are defined in private TupleUtils.hpp header

    template<typename T, T... Integers>
    T IntegerSum(wstl::IntegerSequence<T, Integers...>) {
        T result = 0;

        T dummy[] = {(result += Integers)...};
        (void) dummy;

        return result;
    }

    TEST_CASE("IntegerSequence") {
        using Data = wstl::IntegerSequence<short, 10, 1, 33, 67, 29, 5>;

        // Type test
        CHECK(wstl::IsSame<Data::ValueType, short>::Value);

        // The following line should fail with a compilation error
        // wstl::IntegerSequence<float> er;

        // Size test
        CHECK_EQ(Data::Size(), 6UL);
        CHECK_EQ(wstl::IntegerSequence<int>::Size(), 0UL);

        // Practical test, sum of integers
        CHECK_EQ(IntegerSum(Data{}), 145);
        CHECK(wstl::IsSame<decltype(IntegerSum(Data{})), short>::Value);

        // MakeIntegerSequence
        using Data2 = wstl::MakeIntegerSequence<char, 10>;

        CHECK(wstl::IsSame<Data2::ValueType, char>::Value);
        CHECK_EQ(Data2::Size(), 10UL);
        
        CHECK_EQ(IntegerSum(Data2{}), 45);
        CHECK(wstl::IsSame<decltype(IntegerSum(Data2{})), char>::Value);
    }

    TEST_CASE("IndexSequence") {
        using Data = wstl::IndexSequence<10, 1, 33, 67, 29, 5>;

        CHECK(wstl::IsSame<Data::ValueType, size_t>::Value);
        CHECK_EQ(Data::Size(), 6UL);

        CHECK_EQ(IntegerSum(Data{}), 145UL);
        CHECK(wstl::IsSame<decltype(IntegerSum(Data{})), size_t>::Value);

        // MakeIndexSequence
        using Data2 = wstl::MakeIndexSequence<7>;

        CHECK(wstl::IsSame<Data2::ValueType, size_t>::Value);
        CHECK_EQ(Data2::Size(), 7UL);
        
        CHECK_EQ(IntegerSum(Data2{}), 21UL);
        CHECK(wstl::IsSame<decltype(IntegerSum(Data2{})), size_t>::Value);

        // IndexSequenceFor
        using Data3 = wstl::IndexSequenceFor<int, char, double, FakeInteger, long>;

        CHECK(wstl::IsSame<Data3::ValueType, size_t>::Value);
        CHECK_EQ(Data3::Size(), 5UL);
        
        CHECK_EQ(IntegerSum(Data3{}), 10UL);
        CHECK(wstl::IsSame<decltype(IntegerSum(Data3{})), size_t>::Value);
    }
}