#include <doctest.h>
#include <wstl/TypeList.hpp>


typedef wstl::TypeList<int, char, double, std::string, short, long> Data;

TEST_SUITE("TypeList") {
    TEST_CASE("TupleElement") {
        CHECK((wstl::IsSame<wstl::TupleElement<0, Data>::Type, int>::Value));
        CHECK((wstl::IsSame<wstl::TupleElement<1, Data>::Type, char>::Value));
        CHECK((wstl::IsSame<wstl::TupleElement<2, Data>::Type, double>::Value));
        CHECK((wstl::IsSame<wstl::TupleElement<3, Data>::Type, std::string>::Value));
        CHECK((wstl::IsSame<wstl::TupleElement<4, Data>::Type, short>::Value));
        CHECK((wstl::IsSame<wstl::TupleElement<5, Data>::Type, long>::Value));
    }

    TEST_CASE("TupleFront") {
        CHECK((wstl::IsSame<wstl::TupleFront<Data>::Type, int>::Value));
    }

    TEST_CASE("TupleBack") {
        CHECK((wstl::IsSame<wstl::TupleBack<Data>::Type, long>::Value));
    }

    TEST_CASE("TupleSize") {
        CHECK_EQ(wstl::TupleSize<Data>::Value, 6);
        CHECK_EQ(wstl::TupleSize<wstl::TypeList<> >::Value, 0);
    }

    TEST_CASE("TupleIsEmpty") {
        CHECK_FALSE(wstl::TupleIsEmpty<Data>::Value);
        CHECK(wstl::TupleIsEmpty<wstl::TypeList<> >::Value);
    }

    TEST_CASE("TypeListConcatenate") {
        CHECK((wstl::IsSame<wstl::TypeListConcatenate<wstl::TypeList<>, wstl::TypeList<> >::Type, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListConcatenate<wstl::TypeList<int>, wstl::TypeList<> >::Type, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListConcatenate<wstl::TypeList<int>, wstl::TypeList<char> >::Type, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListConcatenate<wstl::TypeList<int, double>, wstl::TypeList<char, long> >::Type, wstl::TypeList<int, double, char, long> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListConcatenate<wstl::TypeList<int, char, char>, wstl::TypeList<int, short, short> >::Type, wstl::TypeList<int, char, char, int, short, short> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListConcatenate<wstl::TypeList<int, char>, wstl::TypeList<char, int, short, short> >::Type, wstl::TypeList<int, char, char, int, short, short> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListConcatenate<wstl::TypeList<int>, wstl::TypeList<char, char, int, short, short> >::Type, wstl::TypeList<int, char, char, int, short, short> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListConcatenate<wstl::TypeList<int, char, char, int>, wstl::TypeList<short, short> >::Type, wstl::TypeList<int, char, char, int, short, short> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListConcatenate<wstl::TypeList<int, char, char, int, short>, wstl::TypeList<short> >::Type, wstl::TypeList<int, char, char, int, short, short> >::Value));

        #ifdef __WSTL_CXX11__
        CHECK(wstl::IsSame<wstl::TypeListConcatenate<wstl::TypeList<int>, wstl::TypeList<char>, wstl::TypeList<double> >::Type, wstl::TypeList<int, char, double> >::Value);
        #endif
    }

    TEST_CASE("TypeListPushFront") {
        CHECK((wstl::IsSame<wstl::TypeListPushFront<wstl::TypeList<>, int>::Type, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPushFront<wstl::TypeList<char, double>, int>::Type, wstl::TypeList<int, char, double> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPushFront<wstl::TypeList<char, double, short, long, float>, int>::Type, wstl::TypeList<int, char, double, short, long, float> >::Value));
    }

    TEST_CASE("TypeListPushBack") {
        CHECK((wstl::IsSame<wstl::TypeListPushBack<wstl::TypeList<>, int>::Type, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPushBack<wstl::TypeList<char, double>, int>::Type, wstl::TypeList<char, double, int> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPushBack<wstl::TypeList<int, char, double, short, long>, float>::Type, wstl::TypeList<int, char, double, short, long, float> >::Value));
    }

    TEST_CASE("TypeListPopFront") {
        CHECK((wstl::IsSame<wstl::TypeListPopFront<wstl::TypeList<> >::Type, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopFront<wstl::TypeList<int> >::Type, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopFront<wstl::TypeList<int, char> >::Type, wstl::TypeList<char> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopFront<wstl::TypeList<int, char, double> >::Type, wstl::TypeList<char, double> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopFront<wstl::TypeList<int, char, double, long> >::Type, wstl::TypeList<char, double, long> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopFront<wstl::TypeList<int, char, double, long, short> >::Type, wstl::TypeList<char, double, long, short> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopFront<Data>::Type, wstl::TypeList<char, double, std::string, short, long> >::Value));
    }

    TEST_CASE("TypeListPopBack") {
        CHECK((wstl::IsSame<wstl::TypeListPopBack<wstl::TypeList<> >::Type, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopBack<wstl::TypeList<int> >::Type, wstl::TypeList<> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopBack<wstl::TypeList<int, char> >::Type, wstl::TypeList<int> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopBack<wstl::TypeList<int, char, double> >::Type, wstl::TypeList<int, char> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopBack<wstl::TypeList<int, char, double, long> >::Type, wstl::TypeList<int, char, double> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopBack<wstl::TypeList<int, char, double, long, short> >::Type, wstl::TypeList<int, char, double, long> >::Value));
        CHECK((wstl::IsSame<wstl::TypeListPopBack<Data>::Type, wstl::TypeList<int, char, double, std::string, short> >::Value));
    }
}
