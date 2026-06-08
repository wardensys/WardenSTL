// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Tuple.hpp>
#include <wstl/Utility.hpp>
#include <tuple>

#include "Utils.hpp"


TEST_SUITE("Tuple") {
    using Data = wstl::Tuple<int, double, int, std::string>;
    using ExplicitData = wstl::Tuple<int, double, int, ExplicitConstructible>;

    TEST_CASE("Default constructor") {
        Data t1 = {};
        ExplicitData t2;

        int i0 = wstl::Get<0>(t1);
        double d1 = wstl::Get<1>(t1);
        int i2 = wstl::Get<2>(t1);
        std::string& s3 = wstl::Get<3>(t1);

        CHECK_EQ(i0, 0);
        CHECK_EQ(d1, 0);
        CHECK_EQ(i2, 0);
        CHECK(s3.empty());

        int i4 = wstl::Get<0>(t2);
        double d5 = wstl::Get<1>(t2);
        int i6 = wstl::Get<2>(t2);
        ExplicitConstructible& e7 = wstl::Get<3>(t2);

        CHECK_EQ(i4, 0);
        CHECK_EQ(d5, 0);
        CHECK_EQ(i6, 0);
        CHECK_EQ(e7.Value, 0);

        // The following lines should fail with a compilation error
        // wstl::Tuple<int, double, int, NonDefaultConstructible> t3;
        // wstl::Tuple<int, double, int, ExplicitConstructible> t4 = {};
    }

    TEST_CASE("Copy constructor") {
        Data t1 = {10, 10.5, 67, "Hello World"};
        Data t2(t1);

        CHECK_EQ(wstl::Get<0>(t1), wstl::Get<0>(t2));
        CHECK_EQ(wstl::Get<1>(t1), wstl::Get<1>(t2));
        CHECK_EQ(wstl::Get<2>(t1), wstl::Get<2>(t2));
        CHECK_EQ(wstl::Get<3>(t1), wstl::Get<3>(t2));

        // Const
        const Data ct1(10, 10.5, 67, "Hello World");
        Data ct2(ct1);

        CHECK_EQ(wstl::Get<0>(ct1), wstl::Get<0>(ct2));
        CHECK_EQ(wstl::Get<1>(ct1), wstl::Get<1>(ct2));
        CHECK_EQ(wstl::Get<2>(ct1), wstl::Get<2>(ct2));
        CHECK_EQ(wstl::Get<3>(ct1), wstl::Get<3>(ct2));

        // From implicitly convertible
        wstl::Tuple<int, double, short, std::string> it1(10, 10.5, short(67), "Hello World");
        wstl::Tuple<int, double, int, std::string> it2 = it1;

        CHECK_EQ(wstl::Get<0>(it1), wstl::Get<0>(it2));
        CHECK_EQ(wstl::Get<1>(it1), wstl::Get<1>(it2));
        CHECK_EQ(wstl::Get<2>(it1), wstl::Get<2>(it2));
        CHECK_EQ(wstl::Get<3>(it1), wstl::Get<3>(it2));

        // From const implicitly convertible
        const wstl::Tuple<int, double, short, std::string> cit1(10, 10.5, short(67), "Hello World");
        wstl::Tuple<int, double, int, std::string> cit2 = cit1;

        CHECK_EQ(wstl::Get<0>(cit1), wstl::Get<0>(cit2));
        CHECK_EQ(wstl::Get<1>(cit1), wstl::Get<1>(cit2));
        CHECK_EQ(wstl::Get<2>(cit1), wstl::Get<2>(cit2));
        CHECK_EQ(wstl::Get<3>(cit1), wstl::Get<3>(cit2));

        // From explicitly convertible
        wstl::Tuple<From> et1(15);
        wstl::Tuple<ToExplicit> et2(et1);

        CHECK_EQ(wstl::Get<0>(et1).Value, wstl::Get<0>(et2).Value);

        // The following line should fail with a compilation error
        // wstl::Tuple<ToExplicit> et3 = et1;

        // From const explicitly convertible
        const wstl::Tuple<From> cet1(15);
        wstl::Tuple<ToExplicit> cet2(et1);

        CHECK_EQ(wstl::Get<0>(cet1).Value, wstl::Get<0>(cet2).Value);

        // The following line should fail with a compilation error
        // wstl::Tuple<ToExplicit> cet3 = cet1;
    }

    TEST_CASE("Move constructor") {
        wstl::Tuple<int, double, int, MovableData<int>> t1(10, 10.5, 67, 128);
        wstl::Tuple<int, double, int, MovableData<int>> t2(wstl::Move(t1));

        CHECK_EQ(wstl::Get<0>(t1), wstl::Get<0>(t2));
        CHECK_EQ(wstl::Get<1>(t1), wstl::Get<1>(t2));
        CHECK_EQ(wstl::Get<2>(t1), wstl::Get<2>(t2));
        CHECK_EQ(wstl::Get<3>(t1), wstl::Get<3>(t2));

        // Const
        const wstl::Tuple<int, double, int, MovableData<int>> ct1(10, 10.5, 67, 128);
        wstl::Tuple<int, double, int, MovableData<int>> ct2(Move(ct1));

        CHECK_EQ(wstl::Get<0>(ct1), wstl::Get<0>(ct2));
        CHECK_EQ(wstl::Get<1>(ct1), wstl::Get<1>(ct2));
        CHECK_EQ(wstl::Get<2>(ct1), wstl::Get<2>(ct2));
        CHECK_EQ(wstl::Get<3>(ct1), wstl::Get<3>(ct2));

        // From implicitly convertible
        wstl::Tuple<int, double, short, MovableData<int>> it1(10, 10.5, short(67), 128);
        wstl::Tuple<int, double, int, MovableData<int>> it2 = Move(it1);

        CHECK_EQ(wstl::Get<0>(it1), wstl::Get<0>(it2));
        CHECK_EQ(wstl::Get<1>(it1), wstl::Get<1>(it2));
        CHECK_EQ(wstl::Get<2>(it1), wstl::Get<2>(it2));
        CHECK_EQ(wstl::Get<3>(it1), wstl::Get<3>(it2));

        // From const implicitly convertible
        const wstl::Tuple<int, double, short, MovableData<int>> cit1(10, 10.5, short(67), 128);
        wstl::Tuple<int, double, int, MovableData<int>> cit2 = Move(cit1);

        CHECK_EQ(wstl::Get<0>(cit1), wstl::Get<0>(cit2));
        CHECK_EQ(wstl::Get<1>(cit1), wstl::Get<1>(cit2));
        CHECK_EQ(wstl::Get<2>(cit1), wstl::Get<2>(cit2));
        CHECK_EQ(wstl::Get<3>(cit1), wstl::Get<3>(cit2));

        // From explicitly convertible
        wstl::Tuple<From> et1(15);
        wstl::Tuple<ToExplicit> et2(Move(et1));

        CHECK_EQ(wstl::Get<0>(et1).Value, wstl::Get<0>(et2).Value);

        // The following line should fail with a compilation error
        // wstl::Tuple<ToExplicit> et3 = Move(et1);

        // From const explicitly convertible
        const wstl::Tuple<From> cet1(15);
        wstl::Tuple<ToExplicit> cet2(Move(cet1));

        CHECK_EQ(wstl::Get<0>(cet1).Value, wstl::Get<0>(cet2).Value);

        // The following line should fail with a compilation error
        // wstl::Tuple<ToExplicit> cet3 = Move(cet1);
    }

    TEST_CASE("Parameterized constructor") {
        Data t1 = {10, 10.5, 67, "Hello World"};
        CHECK_EQ(wstl::Get<0>(t1), 10);
        CHECK_EQ(wstl::Get<1>(t1), 10.5);
        CHECK_EQ(wstl::Get<2>(t1), 67);
        CHECK_EQ(wstl::Get<3>(t1), "Hello World");

        wstl::Tuple<int> t2 = 17;
        CHECK_EQ(wstl::Get<0>(t2), 17);

        ExplicitData t3(10, 10.5, 67, 128);
        CHECK_EQ(wstl::Get<0>(t3), 10);
        CHECK_EQ(wstl::Get<1>(t3), 10.5);
        CHECK_EQ(wstl::Get<2>(t3), 67);
        CHECK_EQ(wstl::Get<3>(t3).Value, 128);

        // The following line should fail with a compilation error
        // ExplicitData t4 = {10, 10.5, 67, 128};
        // wstl::Tuple<ExplicitConstructible> t5 = 128;
        
        #ifdef __WSTL_CXX17__
        // Template deduction guide
        wstl::Tuple gt(10, std::string("Hello"), 0.5);
        CHECK_EQ(wstl::Get<0>(gt), 10);
        CHECK_EQ(wstl::Get<1>(gt), "Hello");
        CHECK_EQ(wstl::Get<2>(gt), 0.5);
        #endif
    }

    TEST_CASE("Constructor from pair") {
        wstl::Pair<int, std::string> p(69, std::string("Hell"));
        wstl::Tuple<int, std::string> t1 = p;

        CHECK_EQ(wstl::Get<0>(t1), 69);
        CHECK_EQ(wstl::Get<1>(t1), "Hell");

        // Const
        const wstl::Pair<int, std::string> cp(69, "Hell");
        wstl::Tuple<int, std::string> ct1 = cp;

        CHECK_EQ(wstl::Get<0>(ct1), 69);
        CHECK_EQ(wstl::Get<1>(ct1), "Hell");

        // Movable only
        wstl::Pair<int, MovableData<int>> mp(69, wstl::Move(MovableData<int>(128)));
        wstl::Tuple<int, MovableData<int>> mt1 = wstl::Move(mp);

        CHECK_EQ(wstl::Get<0>(mt1), 69);
        CHECK_EQ(wstl::Get<1>(mt1).Value, 128);

        // Rvalue, implicit conversion
        wstl::Tuple<long, std::string> rit1 = wstl::Move(p);

        CHECK_EQ(wstl::Get<0>(rit1), 69);
        CHECK_EQ(wstl::Get<1>(rit1), "Hell");

        // Rvalue, explicit conversion
        wstl::Pair<From, From> rep(228, 69);
        wstl::Tuple<ToExplicit, ToExplicit> ret1(wstl::Move(rep));

        CHECK_EQ(wstl::Get<0>(ret1).Value, 228);
        CHECK_EQ(wstl::Get<1>(ret1).Value, 69);

        // The following line should fail with a compilation error
        // wstl::Tuple<ToExplicit, ToExplicit> ret2 = wstl::Move(rep);

        // Const rvalue, implicit conversion
        wstl::Tuple<int, std::string> crit1 = wstl::Move(cp);

        CHECK_EQ(wstl::Get<0>(crit1), 69);
        CHECK_EQ(wstl::Get<1>(crit1), "Hell");

        // Const rvalue, explicit conversion
        const wstl::Pair<From, From> crep(228, 69);
        wstl::Tuple<ToExplicit, ToExplicit> cret1(wstl::Move(rep));

        CHECK_EQ(wstl::Get<0>(cret1).Value, 228);
        CHECK_EQ(wstl::Get<1>(cret1).Value, 69);

        // The following line should fail with a compilation error
        // wstl::Tuple<ToExplicit, ToExplicit> cret2 = wstl::Move(crep);

        #ifdef __WSTL_CXX17__
        // Template deduction guide
        wstl::Pair gp(69, std::string("Hell"));
        wstl::Tuple gt(gp);

        CHECK_EQ(wstl::Get<0>(gt), 69);
        CHECK_EQ(wstl::Get<1>(gt), "Hell");
        #endif
    }

    TEST_CASE("Copy assignment operator") {
        Data t1 = {10, 10.5, 67, "Hello World"};
        Data t2 = {8, 9.1, 52, "Bye"};

        t2 = t1;

        CHECK_EQ(wstl::Get<0>(t1), wstl::Get<0>(t2));
        CHECK_EQ(wstl::Get<1>(t1), wstl::Get<1>(t2));
        CHECK_EQ(wstl::Get<2>(t1), wstl::Get<2>(t2));
        CHECK_EQ(wstl::Get<3>(t1), wstl::Get<3>(t2));
        
        // The following lines should fail with a compilation error
        // wstl::Tuple<MovableData<int>> t3(123);
        // wstl::Tuple<MovableData<int>> t4(595);
        // t3 = t4;

        // Templated
        wstl::Tuple<int, double, int, std::string> tt1(10, 10.5, 67, "Hello World");
        wstl::Tuple<long, long double, long, std::string> tt2(8, 9.1, 52, "Bye");

        tt2 = tt1;

        CHECK_EQ(wstl::Get<0>(tt1), wstl::Get<0>(tt2));
        CHECK_EQ(wstl::Get<1>(tt1), wstl::Get<1>(tt2));
        CHECK_EQ(wstl::Get<2>(tt1), wstl::Get<2>(tt2));
        CHECK_EQ(wstl::Get<3>(tt1), wstl::Get<3>(tt2));

        // The following lines should fail with a compilation error
        // wstl::Tuple<From> t3(123);
        // wstl::Tuple<To> t4(595);
        // t3 = t4;
    }

    TEST_CASE("Move assignment operator") {
        wstl::Tuple<MovableData<int>> t1(wstl::Move(MovableData<int>(123)));
        wstl::Tuple<MovableData<int>> t2(wstl::Move(MovableData<int>(595)));

        t2 = wstl::Move(t1);

        CHECK_EQ(wstl::Get<0>(t1), wstl::Get<0>(t2));

        // Templated
        wstl::Tuple<int, MovableData<int>> tt1(12, wstl::Move(MovableData<int>(123)));
        wstl::Tuple<long, MovableData<int>> tt2(34, wstl::Move(MovableData<int>(595)));

        tt2 = wstl::Move(tt1);

        CHECK_EQ(wstl::Get<0>(tt1), wstl::Get<0>(tt2));
        CHECK_EQ(wstl::Get<1>(tt1), wstl::Get<1>(tt2));

        // The following lines should fail with a compilation error
        // wstl::Tuple<From, MovableData<int>> t3(12, 123);
        // wstl::Tuple<To, MovableData<int>> t4(12, 595);
        // t3 = wstl::Move(t4);
    }

    TEST_CASE("Assignment operator from pair") {
        wstl::Pair<int, std::string> p(69, "Hell");
        wstl::Tuple<int, std::string> t1(10, "Word");

        t1 = p;

        CHECK_EQ(wstl::Get<0>(t1), 69);
        CHECK_EQ(wstl::Get<1>(t1), "Hell");

        // The following lines should fail with a compilation error
        // wstl::Tuple<int, std::string, double> t2(10, "Word", 4.5);
        // t2 = p;
        // wstl::Tuple<int> t3(10);
        // t3 = p;
        // wstl::Pair<From, char> p2(120, 10);
        // wstl::Tuple<ToExplicit, char> t4(100, 5);
        // t4 = p2;

        // Movable
        wstl::Pair<int, MovableData<int>> mp(69, 128);
        wstl::Tuple<int, MovableData<int>> mt1(100, 11);

        mt1 = wstl::Move(mp);

        CHECK_EQ(wstl::Get<0>(mt1), 69);
        CHECK_EQ(wstl::Get<1>(mt1).Value, 128);

        // The following line should fail with a compilation error
        // wstl::Tuple<int, MovableData<int>, double> mt2(10, 111, 4.5);
        // mt2 = wstl::Move(mp);
        // wstl::Tuple<int> mt3(10);
        // mt3 = wstl::Move(mp);
        // wstl::Pair<From, MovableData<int>> mp2(120, 10);
        // wstl::Tuple<ToExplicit, MovableData<int>> mt4(100, 5);
        // mt4 = wstl::Move(mp2);
    }

    TEST_CASE("Swap") {
        wstl::Tuple<int, int> t1(1, 2);
        wstl::Tuple<int, int> t2(3, 4);

        wstl::Swap(t1, t2); // Effectively calls t1.Swap(t2);

        CHECK_EQ(wstl::Get<0>(t1), 3);
        CHECK_EQ(wstl::Get<1>(t1), 4);
        CHECK_EQ(wstl::Get<0>(t2), 1);
        CHECK_EQ(wstl::Get<1>(t2), 2);
    }

    TEST_CASE("TupleElement") {
        CHECK(wstl::IsSame<wstl::TupleElementType<0, Data>, int>::Value);
        CHECK(wstl::IsSame<wstl::TupleElementType<1, Data>, double>::Value);
        CHECK(wstl::IsSame<wstl::TupleElementType<2, Data>, int>::Value);
        CHECK(wstl::IsSame<wstl::TupleElementType<3, Data>, std::string>::Value);
    }

    TEST_CASE("TupleSize") {
        CHECK_EQ(wstl::TupleSize<Data>::Value, 4UL);
        CHECK_EQ(wstl::TupleSize<wstl::Tuple<>>::Value, 0UL);
    }

    TEST_CASE("Tuple Get specialization") {
        wstl::Tuple<int, MovableData<int>> data(10, 64);
        const wstl::Tuple<int, MovableData<int>> constData(20, 32);

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
        // auto el1 = wstl::Get<4>(data);
        // auto el2 = wstl::Get<4>(constData);

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
        // auto er1 = wstl::Get<4>(wstl::Move(data));
        // auto er2 = wstl::Get<4>(wstl::Move(constData));
    }

    TEST_CASE("Tuple comparison operators") {
        // Empty tuples
        wstl::Tuple<> a;
        wstl::Tuple<> b;

        CHECK(a == b);
        CHECK_FALSE(a != b);
        CHECK_FALSE(a < b);
        CHECK(a <= b);
        CHECK_FALSE(a > b);
        CHECK(a >= b);

        // Equal tuples
        wstl::Tuple<int, int, int> ea(1, 2, 3);
        wstl::Tuple<int, int, int> eb(1, 2, 3);

        CHECK(ea == eb);
        CHECK_FALSE(ea != eb);
        CHECK_FALSE(ea < eb);
        CHECK(ea <= eb);
        CHECK_FALSE(ea > eb);
        CHECK(ea >= eb);

        // First element differs
        wstl::Tuple<int, int, int> fa(1, 2, 3);
        wstl::Tuple<int, int, int> fb(2, 2, 3);

        CHECK_FALSE(fa == fb);
        CHECK(fa != fb);
        CHECK(fa < fb);
        CHECK(fa <= fb);
        CHECK_FALSE(fa > fb);
        CHECK_FALSE(fa >= fb);

        // Middle element differs
        wstl::Tuple<int, int, int> ma(1, 2, 3);
        wstl::Tuple<int, int, int> mb(1, 3, 3);

        CHECK_FALSE(ma == mb);
        CHECK(ma != mb);
        CHECK(ma < mb);
        CHECK(ma <= mb);
        CHECK_FALSE(ma > mb);
        CHECK_FALSE(ma >= mb);

        // Last element differs
        wstl::Tuple<int, int, int> da(1, 2, 3);
        wstl::Tuple<int, int, int> db(1, 2, 4);

        CHECK_FALSE(da == db);
        CHECK(da != db);
        CHECK(da < db);
        CHECK(da <= db);
        CHECK_FALSE(da > db);
        CHECK_FALSE(da >= db);

        // Reverse ordering
        wstl::Tuple<int, int, int> ra(5, 6, 7);
        wstl::Tuple<int, int, int> rb(1, 2, 3);

        CHECK_FALSE(ra == rb);
        CHECK(ra != rb);
        CHECK_FALSE(ra < rb);
        CHECK_FALSE(ra <= rb);
        CHECK(ra > rb);
        CHECK(ra >= rb);

        // Cross-type tuples
        wstl::Tuple<int, short, char> ca(1, short(2), char(3));
        wstl::Tuple<long, int, int> cb(1L, 2, 3);

        CHECK(ca == cb);
        CHECK_FALSE(ca != cb);
        CHECK_FALSE(ca < cb);
        CHECK(ca <= cb);
        CHECK_FALSE(ca > cb);
        CHECK(ca >= cb);

        // Single-element tuples
        wstl::Tuple<int> sa(10);
        wstl::Tuple<int> sb(20);

        CHECK(sa < sb);
        CHECK(sb > sa);
        CHECK(sa <= sb);
        CHECK(sb >= sa);

        // Nested tuples
        wstl::Tuple<int, wstl::Tuple<int, int>> na(1, wstl::Tuple<int, int>(2, 3));
        wstl::Tuple<int, wstl::Tuple<int, int>> nb(1, wstl::Tuple<int, int>(2, 4));

        CHECK(na < nb);
        CHECK(nb > na);
        CHECK(na != nb);

        // Lexicographical compare
        wstl::Tuple<int, int, int> la(1, 9, 9);
        wstl::Tuple<int, int, int> lb(2, 0, 0);
        CHECK(la < lb);

        wstl::Tuple<int, int, int> lc(1, 2, 9);
        wstl::Tuple<int, int, int> ld(1, 3, 0);
        CHECK(lc < ld);

        wstl::Tuple<int, int, int> le(1, 2, 3);
        wstl::Tuple<int, int, int> lf(1, 2, 4);
        CHECK(le < lf);

        // The following lines should fail with a compilation error
        // wstl::Tuple<int, int> ert1(1, 2);
        // wstl::Tuple<int, int, int> ert2(3, 4, 5);
        // CHECK(ert1 == ert2);
        // CHECK(ert1 < ert2);
    }

    TEST_CASE("MakeTuple") {
        auto t1 = wstl::MakeTuple(10, 10.5, 67, std::string("Hello World"));

        CHECK_EQ(wstl::Get<0>(t1), 10);
        CHECK_EQ(wstl::Get<1>(t1), 10.5);
        CHECK_EQ(wstl::Get<2>(t1), 67);
        CHECK_EQ(wstl::Get<3>(t1), std::string("Hello World"));
    }

    TEST_CASE("Tie") {
        int i;
        double d;
        std::string s;

        wstl::Tie(i, d, s) = wstl::MakeTuple(10, 10.5, std::string("Hello World"));

        CHECK_EQ(i, 10);
        CHECK_EQ(d, 10.5);
        CHECK_EQ(s, "Hello World");

        // Movable
        wstl::Tuple<int, MovableData<int>> t(20, MovableData<int>(128));
        int i2;
        MovableData<int> m;
        
        wstl::Tie(i2, m) = wstl::Move(t);

        CHECK_EQ(i2, 20);
        CHECK_EQ(m.Value, 128);

        // Convertible types
        wstl::Tuple<int, float, std::string> ct(30, 15.5f, "Hello");
        long l;
        double f;
        std::string s2;

        wstl::Tie(l, f, s2) = ct;

        CHECK_EQ(l, 30);
        CHECK_EQ(f, 15.5f);
        CHECK_EQ(s2, "Hello");
    }

    TEST_CASE("ForwardAsTuple") {
        char v1 = 1;
        int v2 = 2;
        std::string v3 = "Hello";

        auto t = wstl::ForwardAsTuple(v1, v2, v3);

        CHECK_EQ(v1, wstl::Get<0>(t));
        CHECK_EQ(v2, wstl::Get<1>(t));
        CHECK_EQ(v3, wstl::Get<2>(t));
    }

    __WSTL_NODISCARD__ bool Get() { return true; } 

    TEST_CASE("IgnoreType") {
        // Should not trigger compilation error
        wstl::Ignore = Get();
    }

    TEST_CASE("TupleConcatenate") {
        // 1 tuple
        wstl::Tuple<int, double> t1{1, 2.3};

        auto ct1 = wstl::TupleConcatenate(t1);
        static_assert(wstl::IsSame<decltype(ct1), wstl::Tuple<int, double>>::Value, "ct1 Type mismatch");

        CHECK_EQ(wstl::Get<0>(ct1), wstl::Get<0>(t1));
        CHECK_EQ(wstl::Get<1>(ct1), wstl::Get<1>(t1));

        // 2 tuples
        wstl::Tuple<int, std::string> t2{4, "Hey"};

        auto ct2 = wstl::TupleConcatenate(t1, t2);
        static_assert(wstl::IsSame<decltype(ct2), wstl::Tuple<int, double, int, std::string>>::Value, "ct2 Type mismatch");

        CHECK_EQ(wstl::Get<0>(ct2), wstl::Get<0>(t1));
        CHECK_EQ(wstl::Get<1>(ct2), wstl::Get<1>(t1));
        CHECK_EQ(wstl::Get<2>(ct2), wstl::Get<0>(t2));
        CHECK_EQ(wstl::Get<3>(ct2), wstl::Get<1>(t2));

        // 3 tuples
        wstl::Tuple<bool, int> t3{true, 5};

        auto ct3 = wstl::TupleConcatenate(t1, t2, t3);
        static_assert(wstl::IsSame<decltype(ct3), wstl::Tuple<int, double, int, std::string, bool, int>>::Value, "ct3 Type mismatch");

        CHECK_EQ(wstl::Get<0>(ct3), wstl::Get<0>(t1));
        CHECK_EQ(wstl::Get<1>(ct3), wstl::Get<1>(t1));
        CHECK_EQ(wstl::Get<2>(ct3), wstl::Get<0>(t2));
        CHECK_EQ(wstl::Get<3>(ct3), wstl::Get<1>(t2));
        CHECK_EQ(wstl::Get<4>(ct3), wstl::Get<0>(t3));
        CHECK_EQ(wstl::Get<5>(ct3), wstl::Get<1>(t3));

        // 4 tuples
        wstl::Tuple<double, int, bool> t4{1.01, 6, false};

        auto ct4 = wstl::TupleConcatenate(t1, t2, t3, t4);
        static_assert(wstl::IsSame<decltype(ct4), wstl::Tuple<int, double, int, std::string, bool, int, double, int, bool>>::Value, "ct4 Type mismatch");

        CHECK_EQ(wstl::Get<0>(ct4), wstl::Get<0>(t1));
        CHECK_EQ(wstl::Get<1>(ct4), wstl::Get<1>(t1));
        CHECK_EQ(wstl::Get<2>(ct4), wstl::Get<0>(t2));
        CHECK_EQ(wstl::Get<3>(ct4), wstl::Get<1>(t2));
        CHECK_EQ(wstl::Get<4>(ct4), wstl::Get<0>(t3));
        CHECK_EQ(wstl::Get<5>(ct4), wstl::Get<1>(t3));
        CHECK_EQ(wstl::Get<6>(ct4), wstl::Get<0>(t4));
        CHECK_EQ(wstl::Get<7>(ct4), wstl::Get<1>(t4));
        CHECK_EQ(wstl::Get<8>(ct4), wstl::Get<2>(t4));
    }

    TEST_CASE("MakeFromTuple") {
        wstl::Tuple<int, int> t(10, 20);

        NonTrivialData obj = wstl::MakeFromTuple<NonTrivialData>(wstl::Move(t));

        CHECK_EQ(obj.A, 10);
        CHECK_EQ(obj.B, 20);
    }

    TEST_CASE("Apply") {
        auto function = [](int a, int b, int c) { return a + b * c; };

        wstl::Tuple<int, int, int> t(10, 20, 30);
        int result = wstl::Apply(function, t);

        CHECK_EQ(result, 610);
    }
}
