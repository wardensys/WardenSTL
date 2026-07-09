#include <doctest.h>
#include <wstl/Iterator.hpp>
#include <iterator>
#include <vector>
#include <wstl/Deque.hpp>
#include <wstl/Array.hpp>

#include "Utils.hpp"


template<typename T>
struct MoveOnlyIterator : wstl::Iterator<wstl::InputIteratorTag, T> {
    MoveOnlyIterator() : Ptr(nullptr) {}
    MoveOnlyIterator(T* ptr) : Ptr(ptr) {}

    MoveOnlyIterator(const MoveOnlyIterator&) = delete;

    MoveOnlyIterator(MoveOnlyIterator&& other) : Ptr(other.Ptr) {
        other.Ptr = nullptr;
    }

    MoveOnlyIterator& operator=(const MoveOnlyIterator&) = delete;

    MoveOnlyIterator& operator=(MoveOnlyIterator&& other) {
        if(this != &other) {
            Ptr = other.Ptr;
            other.Ptr = nullptr;
        }
        
        return *this;
    }

    MoveOnlyIterator& operator=(T* ptr) {
        Ptr = ptr;
        return *this;
    }

    T& operator*() const {
        return *Ptr;
    }

    T* operator->() const {
        return Ptr;
    }

    MoveOnlyIterator& operator++() {
        ++Ptr;
        return *this;
    }

    MoveOnlyIterator operator++(int) {
        T* temp = Ptr;
        ++Ptr;
        return MoveOnlyIterator(temp);
    }

    operator T*() const {
        return Ptr;
    }

    T* Ptr;
};

template<typename T>
bool operator==(const MoveOnlyIterator<T>& a, const MoveOnlyIterator<T>& b) {
    return a.Ptr == b.Ptr;
}

template<typename T>
bool operator!=(const MoveOnlyIterator<T>& a, const MoveOnlyIterator<T>& b) {
    return !(a == b);
}

template<typename T>
struct MoveOnlyIterator2 : std::iterator<std::input_iterator_tag, T> {
    MoveOnlyIterator2() : Ptr(nullptr) {}
    MoveOnlyIterator2(T* ptr) : Ptr(ptr) {}

    MoveOnlyIterator2(const MoveOnlyIterator2&) = delete;

    MoveOnlyIterator2(MoveOnlyIterator2&& other) : Ptr(other.Ptr) {
        other.Ptr = nullptr;
    }

    MoveOnlyIterator2& operator=(const MoveOnlyIterator2&) = delete;

    MoveOnlyIterator2& operator=(MoveOnlyIterator2&& other) {
        if(this != &other) {
            Ptr = other.Ptr;
            other.Ptr = nullptr;
        }
        
        return *this;
    }

    MoveOnlyIterator2& operator=(T* ptr) {
        Ptr = ptr;
        return *this;
    }

    T& operator*() const {
        return *Ptr;
    }

    T* operator->() const {
        return Ptr;
    }

    MoveOnlyIterator2& operator++() {
        ++Ptr;
        return *this;
    }

    MoveOnlyIterator2 operator++(int) {
        T* temp = Ptr;
        ++Ptr;
        return MoveOnlyIterator2(temp);
    }

    operator T*() const {
        return Ptr;
    }

    T* Ptr;
};

struct InputIterator : wstl::Iterator<wstl::InputIteratorTag, int> {};
struct OutputIterator : wstl::Iterator<wstl::OutputIteratorTag, int> {};

template<typename T>
struct ForwardIterator : wstl::Iterator<wstl::ForwardIteratorTag, T> {
    ForwardIterator() : Ptr(nullptr) {}
    ForwardIterator(T* ptr) : Ptr(ptr) {}
    ForwardIterator(const ForwardIterator& other) : Ptr(other.Ptr) {}

    ForwardIterator& operator=(const ForwardIterator& other) {
        Ptr = other.Ptr;
        return *this;
    }

    ForwardIterator& operator=(T* ptr) {
        Ptr = ptr;
        return *this;
    }

    T& operator*() const {
        return *Ptr;
    }

    T* operator->() const {
        return Ptr;
    }

    ForwardIterator& operator++() {
        ++Ptr;
        return *this;
    }

    ForwardIterator operator++(int) {
        T* temp = Ptr;
        ++Ptr;
        return ForwardIterator(temp);
    }

    operator T*() const {
        return Ptr;
    }

    T* Ptr;
};

template<typename T>
bool operator==(const ForwardIterator<T>& a, const ForwardIterator<T>& b) {
    return a.Ptr == b.Ptr;
}

template<typename T>
bool operator!=(const ForwardIterator<T>& a, const ForwardIterator<T>& b) {
    return !(a == b);
}

template<typename T>
struct BidirectionalIterator : wstl::Iterator<wstl::BidirectionalIteratorTag, T> {
    BidirectionalIterator() : Ptr(nullptr) {}
    BidirectionalIterator(T* ptr) : Ptr(ptr) {}
    BidirectionalIterator(const BidirectionalIterator& other) : Ptr(other.Ptr) {}

    BidirectionalIterator& operator=(const BidirectionalIterator& other) {
        Ptr = other.Ptr;
        return *this;
    }

    BidirectionalIterator& operator=(T* ptr) {
        Ptr = ptr;
        return *this;
    }

    T& operator*() const {
        return *Ptr;
    }

    T* operator->() const {
        return Ptr;
    }

    BidirectionalIterator& operator++() {
        ++Ptr;
        return *this;
    }

    BidirectionalIterator operator++(int) {
        T* temp = Ptr;
        ++Ptr;
        return BidirectionalIterator(temp);
    }

    BidirectionalIterator& operator--() {
        --Ptr;
        return *this;
    }

    BidirectionalIterator operator--(int) {
        T* temp = Ptr;
        --Ptr;
        return BidirectionalIterator(temp);
    }

    operator T*() const {
        return Ptr;
    }

    T* Ptr;
};

template<typename T>
struct RandomAccessIterator : wstl::Iterator<wstl::RandomAccessIteratorTag, T> {
    RandomAccessIterator() : Ptr(nullptr) {}
    RandomAccessIterator(T* ptr) : Ptr(ptr) {}
    RandomAccessIterator(const RandomAccessIterator& other) : Ptr(other.Ptr) {}

    RandomAccessIterator& operator=(const RandomAccessIterator& other) {
        Ptr = other.Ptr;
        return *this;
    }

    RandomAccessIterator& operator=(T* ptr) {
        Ptr = ptr;
        return *this;
    }

    T& operator*() const {
        return *Ptr;
    }

    T* operator->() const {
        return Ptr;
    }

    RandomAccessIterator& operator++() {
        ++Ptr;
        return *this;
    }

    RandomAccessIterator operator++(int) {
        T* temp = Ptr;
        ++Ptr;
        return RandomAccessIterator(temp);
    }

    RandomAccessIterator& operator--() {
        --Ptr;
        return *this;
    }

    RandomAccessIterator operator--(int) {
        T* temp = Ptr;
        --Ptr;
        return RandomAccessIterator(temp);
    }

    RandomAccessIterator operator+=(int count) {
        Ptr += count;
        return *this;
    }

    RandomAccessIterator operator-=(int count) {
        Ptr -= count;
        return *this;
    }

    operator T*() const {
        return Ptr;
    }

    T* Ptr;
};

template<typename T>
ptrdiff_t operator-(const RandomAccessIterator<T>& a, const RandomAccessIterator<T>& b) {
    return a.Ptr - b.Ptr;
}

namespace {
    constexpr size_t SIZE = 9;
    int dataA[SIZE] = {4, 5, 9, 1, 10, 6, 3, 7, 2};
}

TEST_SUITE("Iterator") {
    TEST_CASE("Advance") {
        // Forward iterator
        ForwardIterator<int> fit1 = std::begin(dataA);
        int* fit2 = std::begin(dataA);

        wstl::Advance(fit1, 4);
        std::advance(fit2, 4);
        CHECK_EQ(*fit1, *fit2);

        wstl::Advance(fit1, -3);
        CHECK_EQ(*fit1, *fit2);

        // Bidirectionl iterator
        BidirectionalIterator<int> bit1 = std::begin(dataA);
        int* bit2 = std::begin(dataA);

        wstl::Advance(bit1, 4);
        std::advance(bit2, 4);
        CHECK_EQ(*bit1, *bit2);

        wstl::Advance(bit1, -3);
        std::advance(bit2, -3);
        CHECK_EQ(*bit1, *bit2);

        // Random access iterator
        RandomAccessIterator<int> rit1 = std::begin(dataA);
        int* rit2 = std::begin(dataA);
        
        wstl::Advance(rit1, 4);
        std::advance(rit2, 4);
        CHECK_EQ(*rit1, *rit2);

        wstl::Advance(rit1, -3);
        std::advance(rit2, -3);
        CHECK_EQ(*rit1, *rit2);
    }

    TEST_CASE("Distance") {
        // Forward iterator
        CHECK_EQ(wstl::Distance(ForwardIterator<int>(dataA), ForwardIterator<int>(dataA + SIZE)), ptrdiff_t(SIZE));

        // Bidirectionl iterator
        CHECK_EQ(wstl::Distance(BidirectionalIterator<int>(dataA), BidirectionalIterator<int>(dataA + SIZE)), ptrdiff_t(SIZE));

        // Random access iterator
        CHECK_EQ(wstl::Distance(RandomAccessIterator<int>(dataA), RandomAccessIterator<int>(dataA + SIZE)), ptrdiff_t(SIZE));
    }

    TEST_CASE("Next") {
        int* it = dataA;

        for(size_t i = 1; i < SIZE; ++i) 
            CHECK_EQ(dataA[i], *wstl::Next(it, ptrdiff_t(i)));
    }

    TEST_CASE("Previous") {
        int* it = dataA + SIZE;

        for(size_t i = 2; i < SIZE; ++i) 
            CHECK_EQ(dataA[SIZE - i], *wstl::Previous(it, ptrdiff_t(i)));
    }

    TEST_CASE("IsInputIterator") {
        CHECK(wstl::IsInputIterator<InputIterator>::Value);
        CHECK_FALSE(wstl::IsInputIterator<OutputIterator>::Value);
        CHECK(wstl::IsInputIterator<ForwardIterator<int>>::Value);
        CHECK(wstl::IsInputIterator<BidirectionalIterator<int>>::Value);
        CHECK(wstl::IsInputIterator<RandomAccessIterator<int>>::Value);
        CHECK(wstl::IsInputIterator<int*>::Value);
        CHECK(wstl::IsInputIterator<const int*>::Value);
    }

    TEST_CASE("IsOutputIterator") {
        CHECK_FALSE(wstl::IsOutputIterator<InputIterator>::Value);
        CHECK(wstl::IsOutputIterator<OutputIterator>::Value);
        CHECK_FALSE(wstl::IsOutputIterator<ForwardIterator<int>>::Value);
        CHECK_FALSE(wstl::IsOutputIterator<BidirectionalIterator<int>>::Value);
        CHECK_FALSE(wstl::IsOutputIterator<RandomAccessIterator<int>>::Value);
        CHECK_FALSE(wstl::IsOutputIterator<int*>::Value);
        CHECK_FALSE(wstl::IsOutputIterator<const int*>::Value);
    }

    TEST_CASE("IsForwardIterator") {
        CHECK_FALSE(wstl::IsForwardIterator<InputIterator>::Value);
        CHECK_FALSE(wstl::IsForwardIterator<OutputIterator>::Value);
        CHECK(wstl::IsForwardIterator<ForwardIterator<int>>::Value);
        CHECK(wstl::IsForwardIterator<BidirectionalIterator<int>>::Value);
        CHECK(wstl::IsForwardIterator<RandomAccessIterator<int>>::Value);
        CHECK(wstl::IsForwardIterator<int*>::Value);
        CHECK(wstl::IsForwardIterator<const int*>::Value);
    }

    TEST_CASE("IsBidirectionalIterator") {
        CHECK_FALSE(wstl::IsBidirectionalIterator<InputIterator>::Value);
        CHECK_FALSE(wstl::IsBidirectionalIterator<OutputIterator>::Value);
        CHECK_FALSE(wstl::IsBidirectionalIterator<ForwardIterator<int>>::Value);
        CHECK(wstl::IsBidirectionalIterator<BidirectionalIterator<int>>::Value);
        CHECK(wstl::IsBidirectionalIterator<RandomAccessIterator<int>>::Value);
        CHECK(wstl::IsBidirectionalIterator<int*>::Value);
        CHECK(wstl::IsBidirectionalIterator<const int*>::Value);
    }

    TEST_CASE("IsRandomAccessIterator") {
        CHECK_FALSE(wstl::IsRandomAccessIterator<InputIterator>::Value);
        CHECK_FALSE(wstl::IsRandomAccessIterator<OutputIterator>::Value);
        CHECK_FALSE(wstl::IsRandomAccessIterator<ForwardIterator<int>>::Value);
        CHECK_FALSE(wstl::IsRandomAccessIterator<BidirectionalIterator<int>>::Value);
        CHECK(wstl::IsRandomAccessIterator<RandomAccessIterator<int>>::Value);
        CHECK(wstl::IsRandomAccessIterator<int*>::Value);
        CHECK(wstl::IsRandomAccessIterator<const int*>::Value);
    }

    TEST_CASE("ReverseIterator constructors") {
        // Default
        wstl::ReverseIterator<int*> dri1;
        std::reverse_iterator<int*> dri2;
        CHECK_EQ(dri1.Base(), dri2.base());

        // Iterator
        wstl::ReverseIterator<int*> iri1(dataA + 6);
        std::reverse_iterator<int*> iri2(dataA + 6);
        CHECK_EQ(iri1.Base(), iri2.base());

        wstl::ReverseIterator<BidirectionalIterator<int>> iri3(dataA + 6);
        CHECK_EQ(iri3.Base(), dataA + 6);

        // The following line should fail with a compilation error
        // wstl::ReverseIterator<ForwardIterator<int>> err1(dataA + 6);

        // Copy constructor
        wstl::ReverseIterator<int*> cri1(iri1);
        CHECK_EQ(cri1.Base(), iri1.Base());

        wstl::ReverseIterator<int*> cri3(iri3);
        CHECK_EQ(cri3.Base(), iri3.Base());
    }

    TEST_CASE("ReverseIterator copy assignment operator") {
        wstl::ReverseIterator<int*> it1(dataA + 6);
        wstl::ReverseIterator<BidirectionalIterator<int>> it2(dataA + 6);
        wstl::ReverseIterator<int*> it3(dataA + 2);
        wstl::ReverseIterator<int*> it4(dataA + 2);

        it3 = it1;
        CHECK_EQ(*it3, *it1);

        it4 = it2;
        CHECK_EQ(*it4, *it2);
    }

    TEST_CASE("ReverseIterator Base function") {
        wstl::ReverseIterator<int*> it1(dataA + 6);
        wstl::ReverseIterator<int*> it2;

        CHECK_EQ(it1.Base(), dataA + 6);
        CHECK_EQ(it2.Base(), nullptr);
    }

    TEST_CASE("ReverseIterator dereference operator") {
        wstl::ReverseIterator<int*> it(dataA + 6);
        std::reverse_iterator<int*> it2(dataA + 6);

        CHECK_EQ(*it, *it2);
    }

    TEST_CASE("ReverseIterator arrow operator") {
        FakeInteger data[] = {{1}, {2}, {3}, {4}, {5}};
        
        wstl::ReverseIterator<FakeInteger*> it1(data + 2);
        std::reverse_iterator<FakeInteger*> it2(data + 2);

        CHECK_EQ(it1->Value, it2->Value);
    }

    TEST_CASE("ReverseIterator increment and decrement") {
        wstl::ReverseIterator<int*> it1(dataA + 6);
        std::reverse_iterator<int*> it2(dataA + 6);

        CHECK_EQ(*++it1, *++it2);
        CHECK_EQ(*it1++, *it2++);
        CHECK_EQ(*it1, *it2);
        CHECK_EQ(*--it1, *--it2);
        CHECK_EQ(*it1--, *it2--);
        CHECK_EQ(*it1, *it2);
        CHECK_EQ(*(it1 + 2), *(it2 + 2));
        CHECK_EQ(*(it1 + -2), *(it2 + -2));
        CHECK_EQ(*(it1 - 2), *(it2 - 2));
        CHECK_EQ(*(it1 - -2), *(it2 - -2));

        it1 += 2;
        it2 += 2;
        CHECK_EQ(*it1, *it2);

        it1 -= 2;
        it2 -= 2;
        CHECK_EQ(*it1, *it2);
    }

    TEST_CASE("ReverseIterator subscript operator") {
        wstl::ReverseIterator<int*> it1(dataA + 2);
        std::reverse_iterator<int*> it2(dataA + 2);

        CHECK_EQ(it1[2], it2[2]);
    }

    TEST_CASE("ReverseIterator comparison operators") {
        int data[] = {1, 2, 3};

        // Equal
        wstl::ReverseIterator<int*> ea(data + 2);
        wstl::ReverseIterator<int*> eb(data + 2);

        CHECK(ea == eb);
        CHECK_FALSE(ea != eb);
        CHECK_FALSE(ea < eb);
        CHECK(ea <= eb);
        CHECK_FALSE(ea > eb);
        CHECK(ea >= eb);

        // Less
        wstl::ReverseIterator<int*> la(data + 2);
        wstl::ReverseIterator<int*> lb(data + 1);

        CHECK_FALSE(la == lb);
        CHECK(la != lb);
        CHECK(la < lb);
        CHECK(la <= lb);
        CHECK_FALSE(la > lb);
        CHECK_FALSE(la >= lb);

        // Greater
        wstl::ReverseIterator<int*> ga(data + 1);
        wstl::ReverseIterator<int*> gb(data + 2);

        CHECK_FALSE(ga == gb);
        CHECK(ga != gb);
        CHECK_FALSE(ga < gb);
        CHECK_FALSE(ga <= gb);
        CHECK(ga > gb);
        CHECK(ga >= gb);
    }

    TEST_CASE("ReverseIterator additional operators") {
        // Addition operator
        wstl::ReverseIterator<int*> it1(dataA + 4);
        std::reverse_iterator<int*> it2(dataA + 4);

        CHECK_EQ(*(2 + it1), *(2 + it2));
        CHECK_EQ(*(-2 + it1), *(-2 + it2));
        
        // Difference operator
        wstl::ReverseIterator<int*> it3(dataA + 2);
        std::reverse_iterator<int*> it4(dataA + 2);

        CHECK_EQ(it3 - it1, it4 - it2);
        CHECK_EQ(it1 - it3, it2 - it4);
    }

    TEST_CASE("MakeReverseIterator") {
        wstl::ReverseIterator<int*> it1(dataA + 4);
        wstl::ReverseIterator<int*> it2 = wstl::MakeReverseIterator(dataA + 4);

        CHECK_EQ(*it1, *it2);

        // Practical test
        std::vector<int> data = {1, 2, 3, 4, 5};

        CHECK(wstl::Equal(wstl::MakeReverseIterator(data.end()), wstl::MakeReverseIterator(data.begin()), data.rbegin()));
    }






    TEST_CASE("MoveIterator constructors") {
        MovableData<int> data[] = {MovableData<int>(1), MovableData<int>(2), MovableData<int>(3)};

        // Default
        wstl::MoveIterator<MovableData<int>*> dri1;
        std::move_iterator<MovableData<int>*> dri2;
        CHECK_EQ(dri1.Base(), dri2.base());

        // Iterator
        wstl::MoveIterator<MovableData<int>*> iri1(data + 1);
        CHECK_EQ(iri1.Base(), data + 1);

        // Move-only iterator
        MoveOnlyIterator<MovableData<int>> mit(data + 2);
        CHECK_EQ(mit.Ptr, data + 2);

        wstl::MoveIterator<MoveOnlyIterator<MovableData<int>>> iri3(wstl::Move(mit));
        CHECK_EQ(mit.Ptr, nullptr);
        CHECK_EQ(iri3.Base().Ptr, data + 2);

        // The following line should fail with a compilator error
        // wstl::MoveIterator<MoveOnlyIterator<MovableData<int>>> iri3(mit);

        // Copy constructor
        wstl::MoveIterator<MovableData<int>*> cri1(iri1);
        CHECK_EQ(cri1.Base(), iri1.Base());

        wstl::MoveIterator<MovableData<int>*> cri3(iri3);
        CHECK_EQ(cri3.Base(), iri3.Base());
    }

    TEST_CASE("MoveIterator copy assignment operator") {
        MovableData<int> data[] = {MovableData<int>(1), MovableData<int>(2), MovableData<int>(3)};

        wstl::MoveIterator<MovableData<int>*> it1(data + 1);
        wstl::MoveIterator<BidirectionalIterator<MovableData<int>>> it2(data + 1);
        wstl::MoveIterator<MovableData<int>*> it3(data + 2);
        wstl::MoveIterator<MovableData<int>*> it4(data + 2);

        it3 = it1;
        CHECK_EQ(it3.Base(), it1.Base());

        it4 = it2;
        CHECK_EQ(it4.Base(), it2.Base());
    }

    TEST_CASE("MoveIterator Base function") {
        MovableData<int> data[] = {MovableData<int>(1), MovableData<int>(2), MovableData<int>(3)};
        wstl::MoveIterator<MovableData<int>*> it1(data + 1);
        wstl::MoveIterator<MoveOnlyIterator<MovableData<int>>> it2(wstl::Move(MoveOnlyIterator<MovableData<int>>(data + 2)));

        MovableData<int>* const& b1 = it1.Base();
        CHECK_EQ(b1, data + 1);

        CHECK_EQ(it2.Base().Ptr, data + 2);
        MoveOnlyIterator<MovableData<int>> mb1 = wstl::Move(it2).Base();
        CHECK_EQ(mb1, data + 2);
        CHECK_EQ(it2.Base().Ptr, nullptr);
    }

    TEST_CASE("MoveIterator dereference operator") {
        MovableData<int> data[] = {MovableData<int>(1), MovableData<int>(2), MovableData<int>(3)};
        wstl::MoveIterator<MovableData<int>*> it(data + 1);

        CHECK_EQ((*it).Value, 2);
        CHECK(data[1].Valid); // object must still be valid

        MovableData<int> i = *it;

        CHECK_EQ(i.Value, 2);
        CHECK_FALSE(data[1].Valid);
    }

    TEST_CASE("MoveIterator arrow operator") {
        MovableData<int> data[] = {MovableData<int>(1), MovableData<int>(2), MovableData<int>(3)};
        wstl::MoveIterator<MovableData<int>*> it(data + 1);
        
        CHECK_EQ(it->Value, 2);
        CHECK(data[1].Valid); // object must still be valid
    }

    TEST_CASE("MoveIterator increment and decrement") {
        MovableData<int> data[] = {MovableData<int>(1), MovableData<int>(2), MovableData<int>(3), MovableData<int>(4), MovableData<int>(5)};
        wstl::MoveIterator<MovableData<int>*> it1(data + 2);
        std::move_iterator<MovableData<int>*> it2(data + 2);

        CHECK_EQ((++it1).Base(), (++it2).base());
        CHECK_EQ((it1++).Base(), (it2++).base());
        CHECK_EQ(it1.Base(), it2.base());
        CHECK_EQ((--it1).Base(), (--it2).base());
        CHECK_EQ((it1--).Base(), (it2--).base());
        CHECK_EQ(it1.Base(), it2.base());
        CHECK_EQ((it1 + 2).Base(), (it2 + 2).base());
        CHECK_EQ((it1 + -2).Base(), (it2 + -2).base());
        CHECK_EQ((it1 - 2).Base(), (it2 - 2).base());
        CHECK_EQ((it1 - -2).Base(), (it2 - -2).base());

        it1 += 2;
        it2 += 2;
        CHECK_EQ(it1.Base(), it2.base());

        it1 -= 2;
        it2 -= 2;
        CHECK_EQ(it1.Base(), it2.base());
    }

    TEST_CASE("MoveIterator subscript operator") {
        MovableData<int> data[] = {MovableData<int>(1), MovableData<int>(2), MovableData<int>(3)};
        wstl::MoveIterator<MovableData<int>*> it(data);
        
        CHECK_EQ(it[2].Value, 3);
        CHECK(data[2].Valid);

        MovableData<int> i = it[1];

        CHECK_EQ(i.Value, 2);
        CHECK_FALSE(data[1].Valid);
    }

    TEST_CASE("MoveIterator comparison operators") {
        MovableData<int> data[] = {MovableData<int>(1), MovableData<int>(2), MovableData<int>(3)};

        // Equal
        wstl::MoveIterator<MovableData<int>*> ea(data + 2);
        wstl::MoveIterator<MovableData<int>*> eb(data + 2);

        CHECK(ea == eb);
        CHECK_FALSE(ea != eb);
        CHECK_FALSE(ea < eb);
        CHECK(ea <= eb);
        CHECK_FALSE(ea > eb);
        CHECK(ea >= eb);

        // Less
        wstl::MoveIterator<MovableData<int>*> la(data + 1);
        wstl::MoveIterator<MovableData<int>*> lb(data + 2);

        CHECK_FALSE(la == lb);
        CHECK(la != lb);
        CHECK(la < lb);
        CHECK(la <= lb);
        CHECK_FALSE(la > lb);
        CHECK_FALSE(la >= lb);

        // Greater
        wstl::MoveIterator<MovableData<int>*> ga(data + 2);
        wstl::MoveIterator<MovableData<int>*> gb(data + 1);

        CHECK_FALSE(ga == gb);
        CHECK(ga != gb);
        CHECK_FALSE(ga < gb);
        CHECK_FALSE(ga <= gb);
        CHECK(ga > gb);
        CHECK(ga >= gb);
    }

    TEST_CASE("MoveIterator additional operators") {
        MovableData<int> data[] = {MovableData<int>(1), MovableData<int>(2), MovableData<int>(3), MovableData<int>(4), MovableData<int>(5)};

        // Addition operator
        wstl::MoveIterator<MovableData<int>*> it1(data + 2);
        std::move_iterator<MovableData<int>*> it2(data + 2);

        CHECK_EQ(*(2 + it1), *(2 + it2));
        CHECK_EQ(*(-2 + it1), *(-2 + it2));
        
        // Difference operator
        wstl::MoveIterator<MovableData<int>*> it3(data);
        std::move_iterator<MovableData<int>*> it4(data);

        CHECK_EQ(it3 - it1, it4 - it2);
        CHECK_EQ(it1 - it3, it2 - it4);
    }

    TEST_CASE("MakeMoveIterator") {
        MovableData<int> data[] = {MovableData<int>(1), MovableData<int>(2), MovableData<int>(3)};

        wstl::MoveIterator<MovableData<int>*> it1(data);
        wstl::MoveIterator<MovableData<int>*> it2 = wstl::MakeMoveIterator(data);

        CHECK_EQ(it1.Base(), it2.Base());
    }

    TEST_CASE("InsertIterator") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        std::vector<int> expected = {67, 1, 2, 3, 4, 5, 67};
        wstl::Deque<int, 7> result(2, 67);
        
        std::copy(data.begin(), data.end(), wstl::Inserter(result, result.Begin() + 1));

        CHECK(wstl::Equal(expected.begin(), expected.end(), result.Begin()));
    }

    TEST_CASE("FrontInsertIterator") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        std::vector<int> expected = {5, 4, 3, 2, 1, 67, 67};
        wstl::Deque<int, 7> result(2, 67);
        
        std::copy(data.begin(), data.end(), wstl::FrontInserter(result));

        CHECK(wstl::Equal(expected.begin(), expected.end(), result.Begin()));
    }

    TEST_CASE("BackInsertIterator") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        std::vector<int> expected = {67, 67, 1, 2, 3, 4, 5};
        wstl::Deque<int, 7> result(2, 67);
        
        std::copy(data.begin(), data.end(), wstl::BackInserter(result));

        CHECK(wstl::Equal(expected.begin(), expected.end(), result.Begin()));
    }

    TEST_CASE("Range access functions") {
        int arr[] = {1, 2, 3};
        const int carr[] = {1, 2, 3};

        wstl::Deque<int, 3> deq(std::begin(arr), std::end(arr));
        const wstl::Deque<int, 3> cdeq(std::begin(arr), std::end(arr));

        // Raw arrays
        CHECK_EQ(wstl::Begin(arr), arr);
        CHECK_EQ(wstl::End(arr), arr + 3);
        CHECK(wstl::IsSame<decltype(wstl::Begin(arr)), int*>::Value);
        CHECK(wstl::IsSame<decltype(wstl::End(arr)), int*>::Value);

        CHECK_EQ(wstl::Begin(carr), carr);
        CHECK_EQ(wstl::End(carr), carr + 3);
        CHECK(wstl::IsSame<decltype(wstl::Begin(carr)), const int*>::Value);
        CHECK(wstl::IsSame<decltype(wstl::End(carr)), const int*>::Value);

        CHECK_EQ(wstl::ConstBegin(arr), arr);
        CHECK_EQ(wstl::ConstEnd(arr), arr + 3);
        CHECK_EQ(wstl::ConstBegin(carr), carr);
        CHECK_EQ(wstl::ConstEnd(carr), carr + 3);
        CHECK(wstl::IsSame<decltype(wstl::ConstBegin(arr)), const int*>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstEnd(arr)), const int*>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstBegin(carr)), const int*>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstEnd(carr)), const int*>::Value);

        CHECK_EQ(wstl::ReverseBegin(arr).Base(), arr + 3);
        CHECK_EQ(wstl::ReverseEnd(arr).Base(), arr);
        CHECK(wstl::IsSame<decltype(wstl::ReverseBegin(arr)), wstl::ReverseIterator<int*>>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ReverseEnd(arr)), wstl::ReverseIterator<int*>>::Value);

        CHECK_EQ(wstl::ReverseBegin(carr).Base(), carr + 3);
        CHECK_EQ(wstl::ReverseEnd(carr).Base(), carr);
        CHECK(wstl::IsSame<decltype(wstl::ReverseBegin(carr)), wstl::ReverseIterator<const int*>>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ReverseEnd(carr)), wstl::ReverseIterator<const int*>>::Value);

        CHECK_EQ(wstl::ConstReverseBegin(arr).Base(), arr + 3);
        CHECK_EQ(wstl::ConstReverseEnd(arr).Base(), arr);
        CHECK_EQ(wstl::ConstReverseBegin(carr).Base(), carr + 3);
        CHECK_EQ(wstl::ConstReverseEnd(carr).Base(), carr);
        CHECK(wstl::IsSame<decltype(wstl::ConstReverseBegin(arr)), wstl::ReverseIterator<const int*>>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstReverseEnd(arr)), wstl::ReverseIterator<const int*>>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstReverseBegin(carr)), wstl::ReverseIterator<const int*>>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstReverseEnd(carr)), wstl::ReverseIterator<const int*>>::Value);

        // Containers
        CHECK_EQ(wstl::Begin(deq), deq.Begin());
        CHECK_EQ(wstl::End(deq), deq.End());
        CHECK(wstl::IsSame<decltype(wstl::Begin(deq)), wstl::Deque<int, 3>::Iterator>::Value);
        CHECK(wstl::IsSame<decltype(wstl::End(deq)), wstl::Deque<int, 3>::Iterator>::Value);

        CHECK_EQ(wstl::Begin(cdeq), cdeq.Begin());
        CHECK_EQ(wstl::End(cdeq), cdeq.End());
        CHECK(wstl::IsSame<decltype(wstl::Begin(cdeq)), wstl::Deque<int, 3>::ConstIterator>::Value);
        CHECK(wstl::IsSame<decltype(wstl::End(cdeq)), wstl::Deque<int, 3>::ConstIterator>::Value);

        CHECK_EQ(wstl::ConstBegin(deq), deq.Begin());
        CHECK_EQ(wstl::ConstEnd(deq), deq.End());
        CHECK_EQ(wstl::ConstBegin(cdeq), cdeq.Begin());
        CHECK_EQ(wstl::ConstEnd(cdeq), cdeq.End());
        CHECK(wstl::IsSame<decltype(wstl::ConstBegin(deq)), wstl::Deque<int, 3>::ConstIterator>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstEnd(deq)), wstl::Deque<int, 3>::ConstIterator>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstBegin(cdeq)), wstl::Deque<int, 3>::ConstIterator>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstEnd(cdeq)), wstl::Deque<int, 3>::ConstIterator>::Value);

        CHECK_EQ(wstl::ReverseBegin(deq), deq.ReverseBegin());
        CHECK_EQ(wstl::ReverseEnd(deq), deq.ReverseEnd());
        CHECK(wstl::IsSame<decltype(wstl::ReverseBegin(deq)), wstl::Deque<int, 3>::ReverseIterator>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ReverseEnd(deq)), wstl::Deque<int, 3>::ReverseIterator>::Value);

        CHECK_EQ(wstl::ReverseBegin(cdeq), cdeq.ReverseBegin());
        CHECK_EQ(wstl::ReverseEnd(cdeq), cdeq.ReverseEnd());
        CHECK(wstl::IsSame<decltype(wstl::ReverseBegin(cdeq)), wstl::Deque<int, 3>::ConstReverseIterator>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ReverseEnd(cdeq)), wstl::Deque<int, 3>::ConstReverseIterator>::Value);

        CHECK_EQ(wstl::ConstReverseBegin(deq), deq.ConstReverseBegin());
        CHECK_EQ(wstl::ConstReverseEnd(deq), deq.ConstReverseEnd());
        CHECK_EQ(wstl::ConstReverseBegin(cdeq), cdeq.ConstReverseBegin());
        CHECK_EQ(wstl::ConstReverseEnd(cdeq), cdeq.ConstReverseEnd());
        CHECK(wstl::IsSame<decltype(wstl::ConstReverseBegin(deq)), wstl::Deque<int, 3>::ConstReverseIterator>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstReverseEnd(deq)), wstl::Deque<int, 3>::ConstReverseIterator>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstReverseBegin(cdeq)), wstl::Deque<int, 3>::ConstReverseIterator>::Value);
        CHECK(wstl::IsSame<decltype(wstl::ConstReverseEnd(cdeq)), wstl::Deque<int, 3>::ConstReverseIterator>::Value);
    }

    TEST_CASE("ArraySize") {
        int arr[] = {1, 2, 3, 4, 5};
        const int carr[] = {1, 2, 3, 4, 5};

        CHECK_EQ(ARRAY_SIZE(arr), 5UL);
        CHECK_EQ(ARRAY_SIZE(carr), 5UL);
    }

    TEST_CASE("Size") {
        int arr[] = {1, 2, 3, 4, 5};
        const int carr[] = {1, 2, 3, 4, 5};
        wstl::Deque<int, 5> deq(std::begin(arr), std::end(arr));
        const wstl::Deque<int, 5> cdeq(std::begin(arr), std::end(arr));
        
        wstl::Deque<int, 2> zero;

        CHECK_EQ(wstl::Size(arr), 5UL);
        CHECK_EQ(wstl::Size(carr), 5UL);
        CHECK_EQ(wstl::Size(deq), 5UL);
        CHECK_EQ(wstl::Size(cdeq), 5UL);
        CHECK_EQ(wstl::Size(zero), 0UL);
    }

    TEST_CASE("Empty") {
        int arr[] = {1, 2, 3, 4, 5};
        const int carr[] = {1, 2, 3, 4, 5};
        wstl::Deque<int, 5> deq(std::begin(arr), std::end(arr));
        const wstl::Deque<int, 5> cdeq(std::begin(arr), std::end(arr));
        
        wstl::Deque<int, 2> zero;

        CHECK_FALSE(wstl::Empty(arr));
        CHECK_FALSE(wstl::Empty(carr));
        CHECK_FALSE(wstl::Empty(deq));
        CHECK_FALSE(wstl::Empty(cdeq));
        CHECK(wstl::Empty(zero));
    }

    TEST_CASE("Data") {
        int arr1[] = {1, 2, 3, 4, 5};
        const int carr1[] = {1, 2, 3, 4, 5};
        wstl::Array<int, 5> arr2 = {1, 2, 3, 4, 5};
        const wstl::Array<int, 5> carr2 = {1, 2, 3, 4, 5};

        CHECK_EQ(wstl::Data(arr1), arr1);
        CHECK_EQ(wstl::Data(carr1), carr1);
        CHECK_EQ(wstl::Data(arr2), arr2.Begin());
        CHECK_EQ(wstl::Data(carr2), carr2.Begin());

        // The following lines should fail with a compilation error
        // wstl::Deque<int, 5> deq;
        // wstl::Data(deq);
    }
}