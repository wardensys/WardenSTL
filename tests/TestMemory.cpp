// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Memory.hpp>
#include "Utils.hpp"
#include <memory>


struct NoAddress {
    NoAddress* operator&() {
        return wstl::NullPointer;
    }
};

template<typename T>
struct FancyPointer {
    typedef T ElementType;

    T* Raw;

    static FancyPointer PointerTo(ElementType& e) {
        FancyPointer p;
        p.Raw = wstl::AddressOf(e);
        return p;
    }
};

template<typename T, typename U>
struct RebindPointer {
    typedef U DifferenceType;

    template<typename V>
    struct Rebind { typedef RebindPointer<T, V> Other; };

    U* Raw;

    static RebindPointer PointerTo(T& e) {
        RebindPointer p;
        p.Raw = reinterpret_cast<U*>(wstl::AddressOf(e));
        return p;
    }
};

template<typename T, typename A1, typename A2>
struct TypePointer2; 

template<typename T, typename A1, typename A2, typename A3>
struct TypePointer3;

template<typename T, typename A1, typename A2, typename A3, typename A4>
struct TypePointer4;

template<typename T>
struct CustomDeleter {
    CustomDeleter() : ID('\0'), DeletedCount(0) {}
    explicit CustomDeleter(const char id) : ID(id), DeletedCount(0) {}

    #ifdef __WSTL_CXX11__
    CustomDeleter(CustomDeleter&& other) __WSTL_NOEXCEPT__ : ID(other.ID), DeletedCount(other.DeletedCount) {
        other.ID = '\0';
        other.DeletedCount = 0;
    }

    template<typename U, wstl::EnableIfType<wstl::IsConvertible<U*, T*>::Value, int> = 0>
    CustomDeleter(CustomDeleter<U>&& other) __WSTL_NOEXCEPT__ : ID(other.ID), DeletedCount(other.DeletedCount) {
        other.ID = '\0';
        other.DeletedCount = 0;
    }

    CustomDeleter& operator=(CustomDeleter&& other) __WSTL_NOEXCEPT__ {
        if(this != &other) {
            ID = other.ID;
            other.ID = '\0';
            DeletedCount = other.DeletedCount;
            other.DeletedCount = 0;
        }
        
        return *this;
    }

    template<typename U, wstl::EnableIfType<wstl::IsConvertible<U*, T*>::Value, int> = 0>
    CustomDeleter& operator=(CustomDeleter<U>&& other) __WSTL_NOEXCEPT__ {
        ID = other.ID;
        other.ID = '\0';
        DeletedCount = other.DeletedCount;
        other.DeletedCount = 0;
        return *this;
    }
    #else
    CustomDeleter(CustomDeleter& other) __WSTL_NOEXCEPT__ : ID(other.ID), DeletedCount(other.DeletedCount) {
        other.ID = '\0';
        other.DeletedCount = 0;
    }

    CustomDeleter& operator=(CustomDeleter& other) __WSTL_NOEXCEPT__ {
        if(this != &other) {
            ID = other.ID;
            other.ID = '\0';
            DeletedCount = other.DeletedCount;
            other.DeletedCount = 0;
        }

        return *this;
    }
    #endif

    void operator()(typename wstl::RemoveAllExtents<T>::Type* pointer) const __WSTL_NOEXCEPT__ {
        Deleter(pointer);
        ++DeletedCount;
    }

    char ID;
    mutable size_t DeletedCount;
    static const wstl::DefaultDelete<T> Deleter;

    CustomDeleter(const CustomDeleter&) __WSTL_DELETE__;
    CustomDeleter& operator=(const CustomDeleter&) __WSTL_DELETE__;
};

template<typename T>
const wstl::DefaultDelete<T> CustomDeleter<T>::Deleter;

template<typename T>
struct CustomDeleterCopyable {
    CustomDeleterCopyable() : ID('\0'), DeletedCount(0) {}
    explicit CustomDeleterCopyable(const char id) : ID(id), DeletedCount(0) {}

    CustomDeleterCopyable(const CustomDeleterCopyable& other) __WSTL_NOEXCEPT__ : ID(other.ID), DeletedCount(other.DeletedCount) {}

    CustomDeleterCopyable& operator=(const CustomDeleterCopyable& other) __WSTL_NOEXCEPT__ {
        ID = other.ID;
        DeletedCount = other.DeletedCount;
        return *this;
    }

    void operator()(typename wstl::RemoveAllExtents<T>::Type* pointer) const {
        Deleter(pointer);
        ++DeletedCount;
    }

    char ID;
    mutable size_t DeletedCount;
    static const wstl::DefaultDelete<T> Deleter;
};

struct Deletable {
    Deletable() : Valid(true), Counter(__WSTL_NULLPTR__) {}
    Deletable(size_t* counter) : Counter(counter) {}

    ~Deletable() {
        if(Counter) ++(*Counter);
    }

    void Delete() {
        Valid = false;
    }
    
    bool Valid;
    size_t* Counter;
};

struct DeletableDeleter {
    DeletableDeleter(size_t size) : Size(size) {}

    void operator()(Deletable* d) {
        for(size_t i = 0; i < Size; ++i) d[i].Delete();
    }

    size_t Size;
};

struct DummyDeleter {
    template<typename T>
    void operator()(T*) {}
};

template<typename T>
const wstl::DefaultDelete<T> CustomDeleterCopyable<T>::Deleter;

typedef wstl::AlignedStorage<sizeof(uint16_t), wstl::AlignmentOf<uint32_t>::Value> StorageType;

TEST_SUITE("Memory") {
    TEST_CASE("AddressOf") {
        int i;
        CHECK(&i == wstl::AddressOf(i));

        NoAddress na;
        CHECK(&na != wstl::AddressOf(na));
        CHECK(reinterpret_cast<NoAddress*>(&reinterpret_cast<char&>(na)) == wstl::AddressOf(na));
    }

    TEST_CASE("PointerTraits int*") {
        typedef wstl::PointerTraits<int*> Traits;

        CHECK((wstl::IsSame<Traits::ElementType, int>::Value));
        CHECK((wstl::IsSame<Traits::PointerType, int*>::Value));
        CHECK((wstl::IsSame<Traits::DifferenceType, ptrdiff_t>::Value));
        CHECK((wstl::IsSame<Traits::Rebind<long>::Other, long*>::Value));

        int i = 10;
        int* p = Traits::PointerTo(i);
        CHECK_EQ(i, *p);
    }

    TEST_CASE("PointerTraits FancyPointer") {
        typedef wstl::PointerTraits<FancyPointer<char> > Traits;

        CHECK((wstl::IsSame<Traits::ElementType, char>::Value));
        CHECK((wstl::IsSame<Traits::PointerType, FancyPointer<char> >::Value));
        CHECK((wstl::IsSame<Traits::DifferenceType, ptrdiff_t>::Value));
        CHECK((wstl::IsSame<Traits::Rebind<long>::Other, FancyPointer<long> >::Value));

        char c = 'c';
        FancyPointer<char> p = Traits::PointerTo(c);
        CHECK_EQ(c, *p.Raw);
    }

    TEST_CASE("PointerTraits RebindPointer") {
        typedef wstl::PointerTraits<RebindPointer<int, long> > Traits;

        CHECK((wstl::IsSame<Traits::ElementType, int>::Value));
        CHECK((wstl::IsSame<Traits::PointerType, RebindPointer<int, long> >::Value));
        CHECK((wstl::IsSame<Traits::DifferenceType, long>::Value));
        CHECK((wstl::IsSame<Traits::Rebind<int>::Other, RebindPointer<int, int> >::Value));

        int i = 10;
        RebindPointer<int, long> p = Traits::PointerTo(i);
        CHECK_EQ(i, *reinterpret_cast<int*>(p.Raw));
    }

    TEST_CASE("PointerTraits TypePointer ElementType") {
        CHECK((wstl::IsSame<wstl::PointerTraits<TypePointer2<int, char, short> >::ElementType, int>::Value));
        CHECK((wstl::IsSame<wstl::PointerTraits<TypePointer3<long, char, short, float> >::ElementType, long>::Value));
        
        #ifdef __WSTL_CXX11__
        CHECK(wstl::IsSame<wstl::PointerTraits<TypePointer4<char, char, short, float, double>>::ElementType, char>::Value);
        #else
        // The following line should fail with a compilation error
        // CHECK((wstl::IsSame<wstl::PointerTraits<TypePointer4<char, char, short, float, double> >::ElementType, char>::Value));
        #endif
    }

    TEST_CASE("UniquePointer default constructor") {
        wstl::UniquePointer<int> dp;

        CHECK_EQ(dp.Get(), wstl::NullPointer);
        CHECK_FALSE((bool) dp);

        wstl::UniquePointer<int, CustomDeleter<int> > dp2;

        CHECK_EQ(dp2.Get(), wstl::NullPointer);
        CHECK_FALSE((bool) dp2);
        CHECK_EQ(dp2.GetDeleter().ID, '\0');

        // The following line should fail with a compilation error
        // wstl::UniquePointer<int, CustomDeleter<int>&> dp3;
    }

    TEST_CASE("UniquePointer pointer constructor") {
        wstl::UniquePointer<int> pp(new int(10));

        CHECK(pp.Get() != wstl::NullPointer);
        CHECK((bool) pp);
        CHECK_EQ(*pp, 10);

        wstl::UniquePointer<int, CustomDeleter<int> > pp2(new int(20));

        CHECK(pp2.Get() != wstl::NullPointer);
        CHECK((bool) pp2);
        CHECK_EQ(*pp2, 20);
        CHECK_EQ(pp2.GetDeleter().ID, '\0');

        // The following line should fail with a compilation error
        // wstl::UniquePointer<int, CustomDeleter<int>&> pp3(new int(30));
    }

    TEST_CASE("UniquePointer move constructor") {
        // Default parameters
        wstl::UniquePointer<int> up(new int(10));
        wstl::UniquePointer<int> mp(__WSTL_MOVE__(up));

        CHECK(up.Get() == wstl::NullPointer);
        CHECK_FALSE((bool) up);
        CHECK(mp.Get() != wstl::NullPointer);
        CHECK((bool) mp);
        CHECK_EQ(*mp, 10);

        #ifdef __WSTL_CXX11__
        wstl::UniquePointer<const int> mp2(wstl::Move(mp));

        CHECK(mp.Get() == wstl::NullPointer);
        CHECK_FALSE((bool) mp);
        CHECK(mp2.Get() != wstl::NullPointer);
        CHECK((bool) mp2);
        CHECK_EQ(*mp2, 10);
        #endif

        // Custom deleter
        CustomDeleter<int> deleter1('c');
        wstl::UniquePointer<int, CustomDeleter<int> > cup(new int(11), __WSTL_MOVE__(deleter1));
        wstl::UniquePointer<int, CustomDeleter<int> > cmp(__WSTL_MOVE__(cup));

        CHECK(cup.Get() == wstl::NullPointer);
        CHECK_FALSE((bool) cup);
        CHECK_EQ(cup.GetDeleter().ID, '\0');
        CHECK(cmp.Get() != wstl::NullPointer);
        CHECK((bool) cmp);
        CHECK_EQ(*cmp, 11);
        CHECK_EQ(cmp.GetDeleter().ID, 'c');

        #ifdef __WSTL_CXX11__
        wstl::UniquePointer<const int, const CustomDeleter<const int> > cmp2(wstl::Move(cmp));

        CHECK(cmp.Get() == wstl::NullPointer);
        CHECK_FALSE((bool) cmp);
        CHECK_EQ(cmp.GetDeleter().ID, '\0');
        CHECK(cmp2.Get() != wstl::NullPointer);
        CHECK((bool) cmp2);
        CHECK_EQ(*cmp2, 11);
        CHECK_EQ(cmp2.GetDeleter().ID, 'c');
        #endif

        // Custom deleter reference
        CustomDeleter<int> deleter2('e');

        wstl::UniquePointer<int, CustomDeleter<int>&> rup(new int(12), deleter2);
        wstl::UniquePointer<int, CustomDeleter<int>&> rmp(__WSTL_MOVE__(rup));

        CHECK(rup.Get() == wstl::NullPointer);
        CHECK_FALSE((bool) rup);
        CHECK_EQ(rup.GetDeleter().ID, 'e'); // deleter isn't moved because it's a reference
        CHECK(rmp.Get() != wstl::NullPointer);
        CHECK((bool) rmp);
        CHECK_EQ(*rmp, 12);
        CHECK_EQ(rmp.GetDeleter().ID, 'e');

        // The following line should fail with a compilation error
        // wstl::UniquePointer<const int, CustomDeleter<const int>&> rmp2(wstl::Move(rmp));
    }

    TEST_CASE("UniquePointer pointer and deleter constructor") {
        // Temporary deleter
        wstl::UniquePointer<int, CustomDeleterCopyable<int> > up1(new int(52), CustomDeleterCopyable<int>('c'));

        CHECK(up1.Get() != wstl::NullPointer);
        CHECK((bool) up1);
        CHECK_EQ(*up1, 52);
        CHECK_EQ(up1.GetDeleter().ID, 'c');

        // Moved deleter
        CustomDeleter<int> deleter1('e');
        wstl::UniquePointer<int, CustomDeleter<int> > up2(new int(50), __WSTL_MOVE__(deleter1));

        CHECK(up2.Get() != wstl::NullPointer);
        CHECK((bool) up2);
        CHECK_EQ(*up2, 50);
        CHECK_EQ(up2.GetDeleter().ID, 'e');
        CHECK_EQ(deleter1.ID, '\0');

        // Copied deleter
        CustomDeleterCopyable<int> deleter2('f');
        wstl::UniquePointer<int, CustomDeleterCopyable<int> > up3(new int(67), deleter2);

        CHECK(up3.Get() != wstl::NullPointer);
        CHECK((bool) up3);
        CHECK_EQ(*up3, 67);
        CHECK_EQ(up3.GetDeleter().ID, 'f');

        up3.GetDeleter().ID = 'g';

        CHECK_EQ(up3.GetDeleter().ID, 'g');
        CHECK_EQ(deleter2.ID, 'f'); // ID hasn't changed because it's up3 holds its own copy

        // Reference deleter
        wstl::UniquePointer<int, CustomDeleterCopyable<int>&> up4(new int(14), deleter2);

        CHECK(up4.Get() != wstl::NullPointer);
        CHECK((bool) up4);
        CHECK_EQ(*up4, 14);
        CHECK_EQ(up4.GetDeleter().ID, 'f');

        up4.GetDeleter().ID = 'd';

        CHECK_EQ(up4.GetDeleter().ID, 'd');
        CHECK_EQ(deleter2.ID, 'd'); // ID should change because it's a reference
    }

    TEST_CASE("UniquePointer destructor") {
        Deletable deletable;

        {
            wstl::UniquePointer<Deletable, DeletableDeleter> up(&deletable, DeletableDeleter(1));
        }

        CHECK_FALSE(deletable.Valid);

        CustomDeleterCopyable<int> deleter;

        {
            wstl::UniquePointer<int, CustomDeleterCopyable<int>&> up(new int(1), deleter);
        }

        CHECK_EQ(deleter.DeletedCount, 1);
    }

    TEST_CASE("UniquePointer assignment operators") {
        // Move assignment, default deleter
        wstl::UniquePointer<int> up1(new int(1));
        wstl::UniquePointer<int> up2(new int(2));

        up1 = __WSTL_MOVE__(up2);
        
        CHECK(!(bool) up2);
        CHECK_EQ(*up1, 2);

        #ifdef __WSTL_CXX11__
        wstl::UniquePointer<const int> up3(new int(3));

        up3 = wstl::Move(up1);

        CHECK(!(bool) up1);
        CHECK_EQ(*up3, 2);

        up3 = wstl::NullPointer;
        CHECK(!(bool) up3);
        #else
        up1 = wstl::NullPointer;
        CHECK(!(bool) up1);
        #endif

        // Move assignment, custom deleter
        CustomDeleter<int> deleter1('1');
        CustomDeleter<int> deleter2('2');
        wstl::UniquePointer<int, CustomDeleter<int> > cup1(new int(11), __WSTL_MOVE__(deleter1));
        wstl::UniquePointer<int, CustomDeleter<int> > cup2(new int(21), __WSTL_MOVE__(deleter2));

        cup1 = __WSTL_MOVE__(cup2);

        CHECK(!(bool) cup2);
        CHECK_EQ(cup2.GetDeleter().ID, '\0');
        CHECK_EQ(*cup1, 21);
        CHECK_EQ(cup1.GetDeleter().ID, '2');

        #ifdef __WSTL_CXX11__
        wstl::UniquePointer<const int, CustomDeleter<const int> > cup3(new int(31), CustomDeleter<const int>('3'));

        cup3 = wstl::Move(cup1);

        CHECK(!(bool) cup1);
        CHECK_EQ(cup1.GetDeleter().ID, '\0');
        CHECK_EQ(*cup3, 21);
        CHECK_EQ(cup3.GetDeleter().ID, '2');

        cup3 = wstl::NullPointer;
        CHECK(!(bool) cup3);
        #else
        cup1 = wstl::NullPointer;
        CHECK(!(bool) cup1);
        #endif

        CustomDeleterCopyable<int> deleter3('d');
        CustomDeleterCopyable<int> deleter4('e');
        wstl::UniquePointer<int, CustomDeleterCopyable<int>&> rup1(new int(11), deleter3);
        wstl::UniquePointer<int, CustomDeleterCopyable<int>&> rup2(new int(21), deleter4);

        rup1 = __WSTL_MOVE__(rup2);

        CHECK(!(bool) rup2);
        CHECK_EQ(rup2.GetDeleter().ID, 'e');
        CHECK_EQ(*rup1, 21);
        CHECK_EQ(rup1.GetDeleter().ID, 'e');

        CHECK_EQ(deleter3.ID, 'e');
        CHECK_EQ(deleter4.ID, 'e');

        rup1 = wstl::NullPointer;
        CHECK(!(bool) rup1);

        // The following lines should fail with a compilation error
        // CustomDeleter<int> deleter5('d');
        // CustomDeleter<int> deleter6('e');
        // wstl::UniquePointer<int, CustomDeleter<int>&> rup1(new int(11), deleter5);
        // wstl::UniquePointer<int, CustomDeleter<int>&> rup2(new int(21), deleter6);
        // rup1 = __WSTL_MOVE__(rup2);
        // CustomDeleterCopyable<const int> deleter7('g');
        // wstl::UniquePointer<const int, CustomDeleterCopyable<const int>&> rup3(new int(31), deleter7);
        // rup3 = wstl::Move(rup1);
    }

    TEST_CASE("UniquePointer Release") {
        int* buffer = new int;

        wstl::UniquePointer<int> up1(buffer);

        CHECK_EQ(up1.Release(), buffer);
        CHECK(!(bool) up1);

        wstl::UniquePointer<int, CustomDeleter<int> > up2(buffer);

        CHECK_EQ(up2.Release(), buffer);
        CHECK(!(bool) up2);

        CustomDeleter<int> deleter;
        wstl::UniquePointer<int, CustomDeleter<int>&> up3(buffer, deleter);

        CHECK_EQ(up3.Release(), buffer);
        CHECK(!(bool) up3);

        delete buffer;
    }

    TEST_CASE("UniquePointer Reset") {
        wstl::UniquePointer<int> up1(new int(1));
        int* p1 = new int(2);

        CHECK_EQ(*up1, 1);
        up1.Reset(p1);
        CHECK_EQ(*up1, 2);

        wstl::UniquePointer<int, CustomDeleter<int> > up2(new int(1));
        int* p2 = new int(3);

        CHECK_EQ(*up2, 1);
        up2.Reset(p2);
        CHECK_EQ(*up2, 3);
        CHECK_EQ(up2.GetDeleter().DeletedCount, 1);

        CustomDeleter<int> deleter;
        wstl::UniquePointer<int, CustomDeleter<int>&> up3(new int(1), deleter);
        int* p3 = new int(4);

        CHECK_EQ(*up3, 1);
        up3.Reset(p3);
        CHECK_EQ(*up3, 4);
        CHECK_EQ(up3.GetDeleter().DeletedCount, 1);
    }

    TEST_CASE("UniquePointer Swap") {
        wstl::UniquePointer<int> up1(new int(1));
        wstl::UniquePointer<int> up2(new int(2));

        up1.Swap(up2);

        CHECK_EQ(*up1, 2);
        CHECK_EQ(*up2, 1);

        CustomDeleter<int> deleter1('1');
        CustomDeleter<int> deleter2('2');
        wstl::UniquePointer<int, CustomDeleter<int> > up3(new int(3), __WSTL_MOVE__(deleter1));
        wstl::UniquePointer<int, CustomDeleter<int> > up4(new int(4), __WSTL_MOVE__(deleter2));

        up3.Swap(up4);

        CHECK_EQ(*up3, 4);
        CHECK_EQ(*up4, 3);
        CHECK_EQ(up3.GetDeleter().ID, '2');
        CHECK_EQ(up4.GetDeleter().ID, '1');

        CustomDeleterCopyable<int> deleter3('z');
        CustomDeleterCopyable<int> deleter4('x');
        wstl::UniquePointer<int, CustomDeleterCopyable<int>&> rup1(new int(11), deleter3);
        wstl::UniquePointer<int, CustomDeleterCopyable<int>&> rup2(new int(21), deleter4);

        rup1.Swap(rup2);

        CHECK_EQ(*rup1, 21);
        CHECK_EQ(*rup2, 11);
        CHECK_EQ(rup1.GetDeleter().ID, 'x');
        CHECK_EQ(rup2.GetDeleter().ID, 'z');

        CHECK_EQ(deleter3.ID, 'x');
        CHECK_EQ(deleter4.ID, 'z');
    }

    TEST_CASE("UniquePointer Get") {
        wstl::UniquePointer<int> up1(new int(1));
        CHECK_EQ(*up1.Get(), 1);

        up1.Reset();
        CHECK_EQ(up1.Get(), wstl::NullPointer);

        CustomDeleterCopyable<int> deleter('d');
        wstl::UniquePointer<int, CustomDeleterCopyable<int>&> up2(new int(2), deleter);
        CHECK_EQ(*up2.Get(), 2);

        up2.Reset();
        CHECK_EQ(up2.Get(), wstl::NullPointer);
    }

    TEST_CASE("UniquePointer GetDeleter") {
        CustomDeleter<int> deleter1('a');
        CustomDeleter<int> deleter2('b');
        wstl::UniquePointer<int, CustomDeleter<int> > up1(new int(1), __WSTL_MOVE__(deleter1));
        const wstl::UniquePointer<int, CustomDeleter<int> > up2(new int(2), __WSTL_MOVE__(deleter2));

        CHECK_EQ(up1.GetDeleter().ID, 'a');
        CHECK_EQ(up2.GetDeleter().ID, 'b');

        CustomDeleterCopyable<int> deleter3('z');
        CustomDeleterCopyable<int> deleter4('x');
        wstl::UniquePointer<int, CustomDeleterCopyable<int>&> up3(new int(11), deleter3);
        wstl::UniquePointer<int, CustomDeleterCopyable<int>&> up4(new int(21), deleter4);

        CHECK_EQ(up3.GetDeleter().ID, 'z');
        CHECK_EQ(up4.GetDeleter().ID, 'x');
    }

    TEST_CASE("UniquePointer operator bool") {
        wstl::UniquePointer<int> up1;

        CHECK(!(bool) up1);
        up1.Reset(new int(1));
        CHECK((bool) up1);

        wstl::UniquePointer<int, CustomDeleter<int> > up2;

        CHECK(!(bool) up2);
        up2.Reset(new int(2));
        CHECK((bool) up2);

        CustomDeleterCopyable<int> deleter('3');
        wstl::UniquePointer<int, CustomDeleterCopyable<int>&> up3(new int(3), deleter);

        CHECK((bool) up3);
        up3.Reset();
        CHECK(!(bool) up3);
    }

    TEST_CASE("UniquePointer dereference operator") {
        wstl::UniquePointer<int> up1(new int(1));
        CHECK_EQ(*up1, 1);

        wstl::UniquePointer<int, CustomDeleter<int> > up2(new int(2));
        CHECK_EQ(*up2, 2);

        CustomDeleterCopyable<int> deleter('3');
        wstl::UniquePointer<int, CustomDeleterCopyable<int>&> up3(new int(3), deleter);
        CHECK_EQ(*up3, 3);
    }

    TEST_CASE("UniquePointer arrow operator") {
        wstl::UniquePointer<From> up1(new From(67));
        CHECK_EQ(up1->Value, 67);

        wstl::UniquePointer<From, CustomDeleter<From> > up2(new From(68));
        CHECK_EQ(up2->Value, 68);

        CustomDeleterCopyable<From> deleter('3');
        wstl::UniquePointer<From, CustomDeleterCopyable<From>&> up3(new From(69), deleter);
        CHECK_EQ(up3->Value, 69);
    }

    TEST_CASE("UniquePointer subscript operator") {
        int* p1 = new int(1);
        wstl::UniquePointer<int> up1(p1);
        CHECK_EQ(&up1[2], p1 + 2);

        int* p2 = new int(2);
        wstl::UniquePointer<int, CustomDeleter<int> > up2(p2);
        CHECK_EQ(&up2[2], p2 + 2);

        int* p3 = new int(3);
        CustomDeleterCopyable<int> deleter('3');
        wstl::UniquePointer<int, CustomDeleterCopyable<int>&> up3(p3, deleter);
        CHECK_EQ(&up3[3], p3 + 3);
    }

    TEST_CASE("UniquePointer comparison operators") {
        int* p1 = (int*) 1;
        int* p2 = (int*) 2;
        wstl::UniquePointer<int, DummyDeleter> up1(p1);
        wstl::UniquePointer<int, DummyDeleter> up2(p1);
        wstl::UniquePointer<int, DummyDeleter> up3(p2);

        CHECK(up1 == up2);
        CHECK(up1 != up3);
        CHECK(!(up1 < up2));
        CHECK(up1 < up3);
        CHECK(!(up3 <= up1));
        CHECK(up1 <= up2);
        CHECK(up1 <= up3);
        CHECK(!(up1 > up2));
        CHECK(up3 > up1);
        CHECK(!(up1 >= up3));
        CHECK(up2 >= up1);
        CHECK(up3 >= up1);
    }

    TEST_CASE("UniquePointer array default constructor") {
        wstl::UniquePointer<int[]> dp;

        CHECK_EQ(dp.Get(), wstl::NullPointer);
        CHECK_FALSE((bool) dp);

        wstl::UniquePointer<int[], CustomDeleter<int[]> > dp2;

        CHECK_EQ(dp2.Get(), wstl::NullPointer);
        CHECK_FALSE((bool) dp2);
        CHECK_EQ(dp2.GetDeleter().ID, '\0');

        // The following line should fail with a compilation error
        // wstl::UniquePointer<int, CustomDeleter&> dp3;
    }

    TEST_CASE("UniquePointer array pointer constructor") {
        wstl::UniquePointer<int[]> pp(new int[4]);
        CustomIota(&pp[0], &pp[4], 0);

        CHECK(pp.Get() != wstl::NullPointer);
        CHECK((bool) pp);

        CHECK_EQ(pp[0], 0);
        CHECK_EQ(pp[1], 1);
        CHECK_EQ(pp[2], 2);
        CHECK_EQ(pp[3], 3);

        wstl::UniquePointer<int[], CustomDeleter<int[]> > pp2(new int[4]);
        CustomIota(&pp2[0], &pp2[4], 0);

        CHECK(pp2.Get() != wstl::NullPointer);
        CHECK((bool) pp2);
        CHECK_EQ(pp2.GetDeleter().ID, '\0');

        CHECK_EQ(pp2[0], 0);
        CHECK_EQ(pp2[1], 1);
        CHECK_EQ(pp2[2], 2);
        CHECK_EQ(pp2[3], 3);

        // The following line should fail with a compilation error
        // wstl::UniquePointer<int[], CustomDeleter<int[]>&> pp3(new int[4]);
    }

    TEST_CASE("UniquePointer array move constructor") {
        // Default parameters
        wstl::UniquePointer<int[]> up(new int[4]);
        CustomIota(&up[0], &up[4], 0);

        wstl::UniquePointer<int[]> mp(__WSTL_MOVE__(up));

        CHECK(up.Get() == wstl::NullPointer);
        CHECK_FALSE((bool) up);
        CHECK(mp.Get() != wstl::NullPointer);
        CHECK((bool) mp);
        
        CHECK_EQ(mp[0], 0);
        CHECK_EQ(mp[1], 1);
        CHECK_EQ(mp[2], 2);
        CHECK_EQ(mp[3], 3);

        #ifdef __WSTL_CXX11__
        wstl::UniquePointer<const int[]> mp2(wstl::Move(mp));

        CHECK(mp.Get() == wstl::NullPointer);
        CHECK_FALSE((bool) mp);
        CHECK(mp2.Get() != wstl::NullPointer);
        CHECK((bool) mp2);
        
        CHECK_EQ(mp2[0], 0);
        CHECK_EQ(mp2[1], 1);
        CHECK_EQ(mp2[2], 2);
        CHECK_EQ(mp2[3], 3);
        #endif

        // Custom deleter
        CustomDeleter<int[]> deleter1('c');
        wstl::UniquePointer<int[], CustomDeleter<int[]> > cup(new int[4], __WSTL_MOVE__(deleter1));
        CustomIota(&cup[0], &cup[4], 0);

        wstl::UniquePointer<int[], CustomDeleter<int[]> > cmp(__WSTL_MOVE__(cup));

        CHECK(cup.Get() == wstl::NullPointer);
        CHECK_FALSE((bool) cup);
        CHECK_EQ(cup.GetDeleter().ID, '\0');
        CHECK(cmp.Get() != wstl::NullPointer);
        CHECK((bool) cmp);
        CHECK_EQ(cmp.GetDeleter().ID, 'c');

        CHECK_EQ(cmp[0], 0);
        CHECK_EQ(cmp[1], 1);
        CHECK_EQ(cmp[2], 2);
        CHECK_EQ(cmp[3], 3);

        #ifdef __WSTL_CXX11__
        wstl::UniquePointer<const int[], const CustomDeleter<const int[]> > cmp2(wstl::Move(cmp));

        CHECK(cmp.Get() == wstl::NullPointer);
        CHECK_FALSE((bool) cmp);
        CHECK_EQ(cmp.GetDeleter().ID, '\0');
        CHECK(cmp2.Get() != wstl::NullPointer);
        CHECK((bool) cmp2);
        CHECK_EQ(cmp2.GetDeleter().ID, 'c');

        CHECK_EQ(cmp2[0], 0);
        CHECK_EQ(cmp2[1], 1);
        CHECK_EQ(cmp2[2], 2);
        CHECK_EQ(cmp2[3], 3);
        #endif

        // Custom deleter reference
        CustomDeleter<int[]> deleter2('e');

        wstl::UniquePointer<int[], CustomDeleter<int[]>&> rup(new int[4], deleter2);
        CustomIota(&rup[0], &rup[4], 0);

        wstl::UniquePointer<int[], CustomDeleter<int[]>&> rmp(__WSTL_MOVE__(rup));

        CHECK(rup.Get() == wstl::NullPointer);
        CHECK_FALSE((bool) rup);
        CHECK_EQ(rup.GetDeleter().ID, 'e'); // deleter isn't moved because it's a reference
        CHECK(rmp.Get() != wstl::NullPointer);
        CHECK((bool) rmp);
        CHECK_EQ(rmp.GetDeleter().ID, 'e');

        CHECK_EQ(rmp[0], 0);
        CHECK_EQ(rmp[1], 1);
        CHECK_EQ(rmp[2], 2);
        CHECK_EQ(rmp[3], 3);

        // The following line should fail with a compilation error
        // wstl::UniquePointer<const int[], CustomDeleter<const int>&> rmp2(wstl::Move(rmp));
    }

    TEST_CASE("UniquePointer array pointer and deleter constructor") {
        // Temporary deleter
        wstl::UniquePointer<int[], CustomDeleterCopyable<int> > up1(new int[4], CustomDeleterCopyable<int>('c'));
        CustomIota(&up1[0], &up1[4], 0);

        CHECK(up1.Get() != wstl::NullPointer);
        CHECK((bool) up1);
        CHECK_EQ(up1.GetDeleter().ID, 'c');
        
        CHECK_EQ(up1[0], 0);
        CHECK_EQ(up1[1], 1);
        CHECK_EQ(up1[2], 2);
        CHECK_EQ(up1[3], 3);

        // Moved deleter
        CustomDeleter<int[]> deleter1('e');
        wstl::UniquePointer<int, CustomDeleter<int[]> > up2(new int[4], __WSTL_MOVE__(deleter1));
        CustomIota(&up2[0], &up2[4], 4);

        CHECK(up2.Get() != wstl::NullPointer);
        CHECK((bool) up2);
        CHECK_EQ(up2.GetDeleter().ID, 'e');
        CHECK_EQ(deleter1.ID, '\0');

        CHECK_EQ(up2[0], 4);
        CHECK_EQ(up2[1], 5);
        CHECK_EQ(up2[2], 6);
        CHECK_EQ(up2[3], 7);

        // Copied deleter
        CustomDeleterCopyable<int> deleter2('f');
        wstl::UniquePointer<int, CustomDeleterCopyable<int> > up3(new int[4], deleter2);
        CustomIota(&up3[0], &up3[4], 8);

        CHECK(up3.Get() != wstl::NullPointer);
        CHECK((bool) up3);
        CHECK_EQ(up3.GetDeleter().ID, 'f');

        CHECK_EQ(up3[0], 8);
        CHECK_EQ(up3[1], 9);
        CHECK_EQ(up3[2], 10);
        CHECK_EQ(up3[3], 11);

        up3.GetDeleter().ID = 'g';

        CHECK_EQ(up3.GetDeleter().ID, 'g');
        CHECK_EQ(deleter2.ID, 'f'); // ID hasn't changed because it's up3 holds its own copy

        // Reference deleter
        wstl::UniquePointer<int[], CustomDeleterCopyable<int>&> up4(new int[4], deleter2);
        CustomIota(&up4[0], &up4[4], 12);

        CHECK(up4.Get() != wstl::NullPointer);
        CHECK((bool) up4);
        CHECK_EQ(up4.GetDeleter().ID, 'f');

        CHECK_EQ(up4[0], 12);
        CHECK_EQ(up4[1], 13);
        CHECK_EQ(up4[2], 14);
        CHECK_EQ(up4[3], 15);

        up4.GetDeleter().ID = 'd';

        CHECK_EQ(up4.GetDeleter().ID, 'd');
        CHECK_EQ(deleter2.ID, 'd'); // ID should change because it's a reference
    }

    TEST_CASE("UniquePointer array destructor") {
        Deletable deletable[4];

        {
            wstl::UniquePointer<Deletable[], DeletableDeleter> up(deletable, DeletableDeleter(4));
        }

        CHECK_FALSE(deletable[0].Valid);
        CHECK_FALSE(deletable[1].Valid);
        CHECK_FALSE(deletable[2].Valid);
        CHECK_FALSE(deletable[3].Valid);

        CustomDeleterCopyable<int[]> deleter;

        {
            wstl::UniquePointer<int[], CustomDeleterCopyable<int[]>&> up(new int[4], deleter);
        }

        CHECK_EQ(deleter.DeletedCount, 1);
    }

    TEST_CASE("UniquePointer array assignment operators") {
        // Move assignment, default deleter
        wstl::UniquePointer<int[]> up1(new int[4]);
        CustomIota(&up1[0], &up1[4], 0);

        wstl::UniquePointer<int[]> up2(new int[4]);
        CustomIota(&up2[0], &up2[4], 4);

        up1 = __WSTL_MOVE__(up2);
        
        CHECK(!(bool) up2);
        
        CHECK_EQ(up1[0], 4);
        CHECK_EQ(up1[1], 5);
        CHECK_EQ(up1[2], 6);
        CHECK_EQ(up1[3], 7);

        #ifdef __WSTL_CXX11__
        int* p1 = new int[4];
        CustomIota(&p1[0], &p1[4], 8);
        wstl::UniquePointer<const int[]> up3(p1);

        up3 = wstl::Move(up1);

        CHECK(!(bool) up1);
        
        CHECK_EQ(up3[0], 4);
        CHECK_EQ(up3[1], 5);
        CHECK_EQ(up3[2], 6);
        CHECK_EQ(up3[3], 7);

        up3 = wstl::NullPointer;
        CHECK(!(bool) up3);
        #else
        up1 = wstl::NullPointer;
        CHECK(!(bool) up1);
        #endif

        // Move assignment, custom deleter
        CustomDeleter<int[]> deleter1('1');
        CustomDeleter<int[]> deleter2('2');
        wstl::UniquePointer<int[], CustomDeleter<int[]> > cup1(new int[4], __WSTL_MOVE__(deleter1));
        CustomIota(&cup1[0], &cup1[4], 0);

        wstl::UniquePointer<int[], CustomDeleter<int[]> > cup2(new int[4], __WSTL_MOVE__(deleter2));
        CustomIota(&cup2[0], &cup2[4], 4);

        cup1 = __WSTL_MOVE__(cup2);

        CHECK(!(bool) cup2);
        CHECK_EQ(cup2.GetDeleter().ID, '\0');
        CHECK_EQ(cup1.GetDeleter().ID, '2');

        CHECK_EQ(cup1[0], 4);
        CHECK_EQ(cup1[1], 5);
        CHECK_EQ(cup1[2], 6);
        CHECK_EQ(cup1[3], 7);

        #ifdef __WSTL_CXX11__
        int* p2 = new int[4];
        CustomIota(&p2[0], &p2[4], 8);
        wstl::UniquePointer<const int[], CustomDeleter<const int[]> > cup3(p2, CustomDeleter<const int[]>('3'));
        
        cup3 = wstl::Move(cup1);

        CHECK(!(bool) cup1);
        CHECK_EQ(cup1.GetDeleter().ID, '\0');
        CHECK_EQ(cup3.GetDeleter().ID, '2');

        CHECK_EQ(cup3[0], 4);
        CHECK_EQ(cup3[1], 5);
        CHECK_EQ(cup3[2], 6);
        CHECK_EQ(cup3[3], 7);

        cup3 = wstl::NullPointer;
        CHECK(!(bool) cup3);
        #else
        cup1 = wstl::NullPointer;
        CHECK(!(bool) cup1);
        #endif

        CustomDeleterCopyable<int[]> deleter3('d');
        CustomDeleterCopyable<int[]> deleter4('e');
        wstl::UniquePointer<int[], CustomDeleterCopyable<int[]>&> rup1(new int[4], deleter3);
        CustomIota(&rup1[0], &rup1[4], 0);
        
        wstl::UniquePointer<int[], CustomDeleterCopyable<int[]>&> rup2(new int[4], deleter4);
        CustomIota(&rup2[0], &rup2[4], 4);

        rup1 = __WSTL_MOVE__(rup2);

        CHECK(!(bool) rup2);
        CHECK_EQ(rup2.GetDeleter().ID, 'e');
        CHECK_EQ(rup1.GetDeleter().ID, 'e');

        CHECK_EQ(rup1[0], 4);
        CHECK_EQ(rup1[1], 5);
        CHECK_EQ(rup1[2], 6);
        CHECK_EQ(rup1[3], 7);

        CHECK_EQ(deleter3.ID, 'e');
        CHECK_EQ(deleter4.ID, 'e');

        rup1 = wstl::NullPointer;
        CHECK(!(bool) rup1);

        // The following lines should fail with a compilation error
        // CustomDeleter<int[]> deleter5('d');
        // CustomDeleter<int[]> deleter6('e');
        // wstl::UniquePointer<int[], CustomDeleter<int[]>&> rup1(new int[4], deleter5);
        // wstl::UniquePointer<int[], CustomDeleter<int[]>&> rup2(new int[4], deleter6);
        // rup1 = __WSTL_MOVE__(rup2);
        // CustomDeleterCopyable<const int> deleter7('g');
        // wstl::UniquePointer<const int[], CustomDeleterCopyable<const int>&> rup3(new int[4], deleter7);
        // rup3 = wstl::Move(rup1);
    }

    TEST_CASE("UniquePointer array Release") {
        int* buffer = new int[4];

        wstl::UniquePointer<int[]> up1(buffer);

        CHECK_EQ(up1.Release(), buffer);
        CHECK(!(bool) up1);

        wstl::UniquePointer<int[], CustomDeleter<int[]> > up2(buffer);

        CHECK_EQ(up2.Release(), buffer);
        CHECK(!(bool) up2);

        CustomDeleter<int[]> deleter;
        wstl::UniquePointer<int[], CustomDeleter<int[]>&> up3(buffer, deleter);

        CHECK_EQ(up3.Release(), buffer);
        CHECK(!(bool) up3);

        delete[] buffer;
    }

    TEST_CASE("UniquePointer array Reset") {
        wstl::UniquePointer<int[]> up1(new int[4]);
        CustomIota(&up1[0], &up1[4], 0);

        int* p1 = new int[4];
        CustomIota(&p1[0], &p1[4], 4);

        CHECK_EQ(up1[0], 0);
        CHECK_EQ(up1[1], 1);
        CHECK_EQ(up1[2], 2);
        CHECK_EQ(up1[3], 3);

        up1.Reset(p1);
        
        CHECK_EQ(up1[0], 4);
        CHECK_EQ(up1[1], 5);
        CHECK_EQ(up1[2], 6);
        CHECK_EQ(up1[3], 7);

        wstl::UniquePointer<int[], CustomDeleter<int[]> > up2(new int[4]);
        CustomIota(&up2[0], &up2[4], 0);

        int* p2 = new int[4];
        CustomIota(&p2[0], &p2[4], 4);

        CHECK_EQ(up2[0], 0);
        CHECK_EQ(up2[1], 1);
        CHECK_EQ(up2[2], 2);
        CHECK_EQ(up2[3], 3);

        up2.Reset(p2);
        
        CHECK_EQ(up2[0], 4);
        CHECK_EQ(up2[1], 5);
        CHECK_EQ(up2[2], 6);
        CHECK_EQ(up2[3], 7);
        
        CHECK_EQ(up2.GetDeleter().DeletedCount, 1);

        CustomDeleter<int[]> deleter;
        wstl::UniquePointer<int, CustomDeleter<int[]>&> up3(new int[4], deleter);
        CustomIota(&up3[0], &up3[4], 0);

        int* p3 = new int[4];
        CustomIota(&p3[0], &p3[4], 4);

        CHECK_EQ(up3[0], 0);
        CHECK_EQ(up3[1], 1);
        CHECK_EQ(up3[2], 2);
        CHECK_EQ(up3[3], 3);

        up3.Reset(p3);

        CHECK_EQ(up3[0], 4);
        CHECK_EQ(up3[1], 5);
        CHECK_EQ(up3[2], 6);
        CHECK_EQ(up3[3], 7);

        CHECK_EQ(up3.GetDeleter().DeletedCount, 1);
    }

    TEST_CASE("UniquePointer array Swap") {
        wstl::UniquePointer<int[]> up1(new int[4]);
        CustomIota(&up1[0], &up1[4], 0);

        wstl::UniquePointer<int[]> up2(new int[4]);
        CustomIota(&up2[0], &up2[4], 4);

        up1.Swap(up2);

        CHECK_EQ(up1[0], 4);
        CHECK_EQ(up1[1], 5);
        CHECK_EQ(up1[2], 6);
        CHECK_EQ(up1[3], 7);

        CHECK_EQ(up2[0], 0);
        CHECK_EQ(up2[1], 1);
        CHECK_EQ(up2[2], 2);
        CHECK_EQ(up2[3], 3);

        CustomDeleter<int[]> deleter1('1');
        CustomDeleter<int[]> deleter2('2');

        wstl::UniquePointer<int[], CustomDeleter<int[]> > up3(new int[4], __WSTL_MOVE__(deleter1));
        CustomIota(&up3[0], &up3[4], 0);

        wstl::UniquePointer<int[], CustomDeleter<int[]> > up4(new int[4], __WSTL_MOVE__(deleter2));
        CustomIota(&up4[0], &up4[4], 4);

        up3.Swap(up4);

        CHECK_EQ(up3[0], 4);
        CHECK_EQ(up3[1], 5);
        CHECK_EQ(up3[2], 6);
        CHECK_EQ(up3[3], 7);

        CHECK_EQ(up4[0], 0);
        CHECK_EQ(up4[1], 1);
        CHECK_EQ(up4[2], 2);
        CHECK_EQ(up4[3], 3);

        CHECK_EQ(up3.GetDeleter().ID, '2');
        CHECK_EQ(up4.GetDeleter().ID, '1');

        CustomDeleterCopyable<int[]> deleter3('z');
        CustomDeleterCopyable<int[]> deleter4('x');
        
        wstl::UniquePointer<int[], CustomDeleterCopyable<int[]>&> rup1(new int[4], deleter3);
        CustomIota(&rup1[0], &rup1[4], 8);

        wstl::UniquePointer<int[], CustomDeleterCopyable<int[]>&> rup2(new int[4], deleter4);
        CustomIota(&rup2[0], &rup2[4], 12);

        rup1.Swap(rup2);

        CHECK_EQ(rup1[0], 12);
        CHECK_EQ(rup1[1], 13);
        CHECK_EQ(rup1[2], 14);
        CHECK_EQ(rup1[3], 15);

        CHECK_EQ(rup2[0], 8);
        CHECK_EQ(rup2[1], 9);
        CHECK_EQ(rup2[2], 10);
        CHECK_EQ(rup2[3], 11);

        CHECK_EQ(rup1.GetDeleter().ID, 'x');
        CHECK_EQ(rup2.GetDeleter().ID, 'z');

        CHECK_EQ(deleter3.ID, 'x');
        CHECK_EQ(deleter4.ID, 'z');
    }

    TEST_CASE("UniquePointer array Get") {
        wstl::UniquePointer<int[]> up1(new int[2]);
        up1[0] = 1;

        CHECK_EQ(*up1.Get(), 1);

        up1.Reset();
        CHECK_EQ(up1.Get(), wstl::NullPointer);

        CustomDeleterCopyable<int[]> deleter('d');
        wstl::UniquePointer<int[], CustomDeleterCopyable<int[]>&> up2(new int[2], deleter);
        up2[0] = 2;
        CHECK_EQ(*up2.Get(), 2);

        up2.Reset();
        CHECK_EQ(up2.Get(), wstl::NullPointer);
    }

    TEST_CASE("UniquePointer array GetDeleter") {
        CustomDeleter<int[]> deleter1('a');
        CustomDeleter<int[]> deleter2('b');
        wstl::UniquePointer<int[], CustomDeleter<int[]> > up1(new int[1], __WSTL_MOVE__(deleter1));
        const wstl::UniquePointer<int[], CustomDeleter<int[]> > up2(new int[1], __WSTL_MOVE__(deleter2));

        CHECK_EQ(up1.GetDeleter().ID, 'a');
        CHECK_EQ(up2.GetDeleter().ID, 'b');

        CustomDeleterCopyable<int[]> deleter3('z');
        CustomDeleterCopyable<int[]> deleter4('x');
        wstl::UniquePointer<int[], CustomDeleterCopyable<int[]>&> up3(new int[1], deleter3);
        wstl::UniquePointer<int[], CustomDeleterCopyable<int[]>&> up4(new int[1], deleter4);

        CHECK_EQ(up3.GetDeleter().ID, 'z');
        CHECK_EQ(up4.GetDeleter().ID, 'x');
    }

    TEST_CASE("UniquePointer array operator bool") {
        wstl::UniquePointer<int[]> up1;

        CHECK(!(bool) up1);
        up1.Reset(new int[2]);
        CHECK((bool) up1);

        wstl::UniquePointer<int[], CustomDeleter<int[]> > up2;

        CHECK(!(bool) up2);
        up2.Reset(new int[2]);
        CHECK((bool) up2);

        CustomDeleterCopyable<int[]> deleter('3');
        wstl::UniquePointer<int[], CustomDeleterCopyable<int[]>&> up3(new int[2], deleter);

        CHECK((bool) up3);
        up3.Reset();
        CHECK(!(bool) up3);
    }

    TEST_CASE("UniquePointer array dereference operator") {
        wstl::UniquePointer<int[]> up1(new int[2]);
        up1[0] = 1;
        CHECK_EQ(*up1, 1);

        wstl::UniquePointer<int[], CustomDeleter<int[]> > up2(new int[2]);
        up2[0] = 2;
        CHECK_EQ(*up2, 2);

        CustomDeleterCopyable<int[]> deleter('3');
        wstl::UniquePointer<int, CustomDeleterCopyable<int[]>&> up3(new int[2], deleter);
        up3[0] = 3;
        CHECK_EQ(*up3, 3);
    }

    TEST_CASE("UniquePointer array arrow operator") {
        NonTrivialData* p1 = new NonTrivialData[2];
        p1->A = 67;
        p1->B = 68;

        wstl::UniquePointer<NonTrivialData[]> up1(p1);
        CHECK_EQ(up1->A, 67);
        CHECK_EQ(up1->B, 68);

        NonTrivialData* p2 = new NonTrivialData[2];
        p2->A = 69;
        p2->B = 70;

        wstl::UniquePointer<NonTrivialData[], CustomDeleter<NonTrivialData[]> > up2(p2);
        CHECK_EQ(up2->A, 69);
        CHECK_EQ(up2->B, 70);

        NonTrivialData* p3 = new NonTrivialData[2];
        p3->A = 71;
        p3->B = 72;

        CustomDeleterCopyable<NonTrivialData[]> deleter('3');
        wstl::UniquePointer<NonTrivialData[], CustomDeleterCopyable<NonTrivialData[]>&> up3(p3, deleter);
        CHECK_EQ(up3->A, 71);
        CHECK_EQ(up3->B, 72);
    }

    TEST_CASE("UniquePointer array subscript operator") {
        int* p1 = new int[4];
        p1[0] = 10;
        p1[1] = 5;
        p1[2] = 18;
        p1[3] = 60;
        
        wstl::UniquePointer<int> up1(p1);
        
        CHECK_EQ(up1[0], 10);
        CHECK_EQ(up1[1], 5);
        CHECK_EQ(up1[2], 18);
        CHECK_EQ(up1[3], 60);

        int* p2 = new int[4];
        p2[0] = 3;
        p2[1] = 7;
        p2[2] = 15;
        p2[3] = 30;
        
        wstl::UniquePointer<int, CustomDeleter<int[]> > up2(p2);
        
        CHECK_EQ(up2[0], 3);
        CHECK_EQ(up2[1], 7);
        CHECK_EQ(up2[2], 15);
        CHECK_EQ(up2[3], 30);

        int* p3 = new int[4];
        p3[0] = 80;
        p3[1] = 1;
        p3[2] = 28;
        p3[3] = 50;

        CustomDeleterCopyable<int[]> deleter('3');
        wstl::UniquePointer<int, CustomDeleterCopyable<int[]>&> up3(p3, deleter);
       
        CHECK_EQ(up3[0], 80);
        CHECK_EQ(up3[1], 1);
        CHECK_EQ(up3[2], 28);
        CHECK_EQ(up3[3], 50);
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("MakeUnique") {
        wstl::UniquePointer<NonTrivialData> up1 = wstl::MakeUnique<NonTrivialData>(12, 14);

        CHECK_EQ(up1->A, 12);
        CHECK_EQ(up1->B, 14);

        wstl::UniquePointer<int[]> up2 = wstl::MakeUnique<int[]>(4);
        CustomIota(&up2[0], &up2[4], 0);

        CHECK_EQ(up2[0], 0);
        CHECK_EQ(up2[1], 1);
        CHECK_EQ(up2[2], 2);
        CHECK_EQ(up2[3], 3);
    }
    #endif

    TEST_CASE("UninitializedCopy") {
        // Trivial type
        int source1[4] = {12, 34, 56, 78};
        int* memory1 = new int[4];

        int* result1 = wstl::UninitializedCopy(&source1[0], &source1[4], memory1);
        CHECK_EQ(result1, memory1 + 4);

        CHECK_EQ(memory1[0], 12);
        CHECK_EQ(memory1[1], 34);
        CHECK_EQ(memory1[2], 56);
        CHECK_EQ(memory1[3], 78);

        delete[] memory1;

        // Non-trivial type
        NonTrivialData source2[4] = { NonTrivialData(1, 2), NonTrivialData(3, 4), 
            NonTrivialData(5, 6), NonTrivialData(7, 8) };

        NonTrivialData* memory2 = new NonTrivialData[4];

        NonTrivialData* result2 = wstl::UninitializedCopy(&source2[0], &source2[4], memory2);
        CHECK_EQ(result2, memory2 + 4);

        CHECK_EQ(memory2[0].A, 1);
        CHECK_EQ(memory2[1].A, 3);
        CHECK_EQ(memory2[2].A, 5);
        CHECK_EQ(memory2[3].A, 7);
        CHECK_EQ(memory2[0].B, 2);
        CHECK_EQ(memory2[1].B, 4);
        CHECK_EQ(memory2[2].B, 6);
        CHECK_EQ(memory2[3].B, 8);

        delete[] memory2;
    }

    TEST_CASE("UninitializedCopyInRange") {
        // Trivial type
        int source1[4] = {12, 34, 56, 78};
        int* memory1 = new int[4];

        int* result1 = wstl::UninitializedCopyInRange(&source1[0], 4, memory1);
        CHECK_EQ(result1, memory1 + 4);

        CHECK_EQ(memory1[0], 12);
        CHECK_EQ(memory1[1], 34);
        CHECK_EQ(memory1[2], 56);
        CHECK_EQ(memory1[3], 78);

        delete[] memory1;

        // Non-trivial type
        NonTrivialData source2[4] = { NonTrivialData(1, 2), NonTrivialData(3, 4), 
            NonTrivialData(5, 6), NonTrivialData(7, 8) };

        NonTrivialData* memory2 = new NonTrivialData[4];

        NonTrivialData* result2 = wstl::UninitializedCopyInRange(&source2[0], 4, memory2);
        CHECK_EQ(result2, memory2 + 4);

        CHECK_EQ(memory2[0].A, 1);
        CHECK_EQ(memory2[1].A, 3);
        CHECK_EQ(memory2[2].A, 5);
        CHECK_EQ(memory2[3].A, 7);
        CHECK_EQ(memory2[0].B, 2);
        CHECK_EQ(memory2[1].B, 4);
        CHECK_EQ(memory2[2].B, 6);
        CHECK_EQ(memory2[3].B, 8);

        delete[] memory2;
    }

    TEST_CASE("UninitializedFill") {
        // Trivial type
        int* memory1 = new int[4];

        wstl::UninitializedFill(memory1, memory1 + 4, 42);

        CHECK_EQ(memory1[0], 42);
        CHECK_EQ(memory1[1], 42);
        CHECK_EQ(memory1[2], 42);
        CHECK_EQ(memory1[3], 42);

        delete[] memory1;

        // Non-trivial type
        NonTrivialData* memory2 = new NonTrivialData[4];

        wstl::UninitializedFill(memory2, memory2 + 4, NonTrivialData(3, 7));

        CHECK_EQ(memory2[0].A, 3);
        CHECK_EQ(memory2[1].A, 3);
        CHECK_EQ(memory2[2].A, 3);
        CHECK_EQ(memory2[3].A, 3);
        CHECK_EQ(memory2[0].B, 7);
        CHECK_EQ(memory2[1].B, 7);
        CHECK_EQ(memory2[2].B, 7);
        CHECK_EQ(memory2[3].B, 7);

        delete[] memory2;
    }

    TEST_CASE("UninitializedFillInRange") {
        // Trivial type
        int* memory1 = new int[4];

        int* result1 = wstl::UninitializedFillInRange(memory1, 4, 42);
        CHECK_EQ(result1, memory1 + 4);

        CHECK_EQ(memory1[0], 42);
        CHECK_EQ(memory1[1], 42);
        CHECK_EQ(memory1[2], 42);
        CHECK_EQ(memory1[3], 42);

        delete[] memory1;

        // Non-trivial type
        NonTrivialData* memory2 = new NonTrivialData[4];

        NonTrivialData* result2 = wstl::UninitializedFillInRange(memory2, 4, NonTrivialData(3, 7));
        CHECK_EQ(result2, memory2 + 4);

        CHECK_EQ(memory2[0].A, 3);
        CHECK_EQ(memory2[1].A, 3);
        CHECK_EQ(memory2[2].A, 3);
        CHECK_EQ(memory2[3].A, 3);
        CHECK_EQ(memory2[0].B, 7);
        CHECK_EQ(memory2[1].B, 7);
        CHECK_EQ(memory2[2].B, 7);
        CHECK_EQ(memory2[3].B, 7);

        delete[] memory2;
    }

    TEST_CASE("UninitializedMove") {
        // Copyable type
        int source1[4] = {12, 34, 56, 78};
        int* memory1 = new int[4];

        int* result1 = wstl::UninitializedMove(&source1[0], &source1[4], memory1);
        CHECK_EQ(result1, memory1 + 4);

        CHECK_EQ(memory1[0], 12);
        CHECK_EQ(memory1[1], 34);
        CHECK_EQ(memory1[2], 56);
        CHECK_EQ(memory1[3], 78);

        delete[] memory1;

        // Movable type
        #ifdef __WSTL_CXX11__
        MovableData<int> source2[4] = { MovableData<int>(1), MovableData<int>(2), 
            MovableData<int>(3), MovableData<int>(4) };

        MovableData<int>* memory2 = new MovableData<int>[4];

        MovableData<int>* result = wstl::UninitializedMove(&source2[0], &source2[4], memory2);
        CHECK_EQ(result, memory2 + 4);

        CHECK(!source2[0]);
        CHECK(!source2[1]);
        CHECK(!source2[2]);
        CHECK(!source2[3]);

        CHECK_EQ(memory2[0].Value, 1U);
        CHECK_EQ(memory2[1].Value, 2U);
        CHECK_EQ(memory2[2].Value, 3U);
        CHECK_EQ(memory2[3].Value, 4U);
        #endif
    }

    TEST_CASE("UninitializedMoveInRange") {
        // Copyable type
        int source1[4] = {12, 34, 56, 78};
        int* memory1 = new int[4];

        int* result1 = wstl::UninitializedMoveInRange(&source1[0], 4, memory1);
        CHECK_EQ(result1, memory1 + 4);

        CHECK_EQ(memory1[0], 12);
        CHECK_EQ(memory1[1], 34);
        CHECK_EQ(memory1[2], 56);
        CHECK_EQ(memory1[3], 78);

        delete[] memory1;

        // Movable type
        #ifdef __WSTL_CXX11__
        MovableData<int> source2[4] = { MovableData<int>(1), MovableData<int>(2), 
            MovableData<int>(3), MovableData<int>(4) };

        MovableData<int>* memory2 = new MovableData<int>[4];

        MovableData<int>* result = wstl::UninitializedMoveInRange(&source2[0], 4, memory2);
        CHECK_EQ(result, memory2 + 4);

        CHECK(!source2[0]);
        CHECK(!source2[1]);
        CHECK(!source2[2]);
        CHECK(!source2[3]);

        CHECK_EQ(memory2[0].Value, 1U);
        CHECK_EQ(memory2[1].Value, 2U);
        CHECK_EQ(memory2[2].Value, 3U);
        CHECK_EQ(memory2[3].Value, 4U);
        #endif
    }

    TEST_CASE("UninitializedDefaultConstruct") {
        // Trivial type - does nothing

        // Non-trivial type
        NonTrivialData* memory2 = new NonTrivialData[4];

        wstl::UninitializedDefaultConstruct(memory2, memory2 + 4);

        CHECK_EQ(memory2[0].A, 0);
        CHECK_EQ(memory2[1].A, 0);
        CHECK_EQ(memory2[2].A, 0);
        CHECK_EQ(memory2[3].A, 0);
        CHECK_EQ(memory2[0].B, 0);
        CHECK_EQ(memory2[1].B, 0);
        CHECK_EQ(memory2[2].B, 0);
        CHECK_EQ(memory2[3].B, 0);

        delete[] memory2;
    }

    TEST_CASE("UninitializedDefaultConstructInRange") {
        // Trivial type - does nothing
        int* memory1 = new int[4];

        int* result1 = wstl::UninitializedDefaultConstructInRange(memory1, 4);
        CHECK_EQ(result1, memory1 + 4);

        // Non-trivial type
        NonTrivialData* memory2 = new NonTrivialData[4];

        NonTrivialData* result2 = wstl::UninitializedDefaultConstructInRange(memory2, 4);
        CHECK_EQ(result2, memory2 + 4);

        CHECK_EQ(memory2[0].A, 0);
        CHECK_EQ(memory2[1].A, 0);
        CHECK_EQ(memory2[2].A, 0);
        CHECK_EQ(memory2[3].A, 0);
        CHECK_EQ(memory2[0].B, 0);
        CHECK_EQ(memory2[1].B, 0);
        CHECK_EQ(memory2[2].B, 0);
        CHECK_EQ(memory2[3].B, 0);

        delete[] memory2;
    }

    TEST_CASE("UninitializedValueConstruct") {
        // Trivial type
        int* memory1 = new int[4];

        wstl::UninitializedValueConstruct(memory1, memory1 + 4);

        CHECK_EQ(memory1[0], 0);
        CHECK_EQ(memory1[1], 0);
        CHECK_EQ(memory1[2], 0);
        CHECK_EQ(memory1[3], 0);

        delete[] memory1;

        // Non-trivial type
        NonTrivialData* memory2 = new NonTrivialData[4];

        wstl::UninitializedValueConstruct(memory2, memory2 + 4);

        CHECK_EQ(memory2[0].A, 0);
        CHECK_EQ(memory2[1].A, 0);
        CHECK_EQ(memory2[2].A, 0);
        CHECK_EQ(memory2[3].A, 0);
        CHECK_EQ(memory2[0].B, 0);
        CHECK_EQ(memory2[1].B, 0);
        CHECK_EQ(memory2[2].B, 0);
        CHECK_EQ(memory2[3].B, 0);

        delete[] memory2;
    }

    TEST_CASE("UninitializedValueConstructInRange") {
        // Trivial type
        int* memory1 = new int[4];

        int* result1 = wstl::UninitializedValueConstructInRange(memory1, 4);
        CHECK_EQ(result1, memory1 + 4);

        CHECK_EQ(memory1[0], 0);
        CHECK_EQ(memory1[1], 0);
        CHECK_EQ(memory1[2], 0);
        CHECK_EQ(memory1[3], 0);

        delete[] memory1;

        // Non-trivial type
        NonTrivialData* memory2 = new NonTrivialData[4];

        NonTrivialData* result2 = wstl::UninitializedValueConstructInRange(memory2, 4);
        CHECK_EQ(result2, memory2 + 4);

        CHECK_EQ(memory2[0].A, 0);
        CHECK_EQ(memory2[1].A, 0);
        CHECK_EQ(memory2[2].A, 0);
        CHECK_EQ(memory2[3].A, 0);
        CHECK_EQ(memory2[0].B, 0);
        CHECK_EQ(memory2[1].B, 0);
        CHECK_EQ(memory2[2].B, 0);
        CHECK_EQ(memory2[3].B, 0);

        delete[] memory2;
    }

    TEST_CASE("ConstructAt") {
        // Trivial type
        void* memory1 = ::operator new(sizeof(int));

        int* result1 = wstl::ConstructAt(static_cast<int*>(memory1));
        CHECK_EQ(*result1, 0);

        ::operator delete(memory1);

        // Non-trivial types
        void* memory2 = ::operator new(sizeof(From));

        From* result2 = wstl::ConstructAt(static_cast<From*>(memory2), 10);
        CHECK_EQ(result2->Value, 10);

        result2->~From();
        ::operator delete(memory2);

        void* memory3 = ::operator new(sizeof(NonTrivialData));

        NonTrivialData* result3 = wstl::ConstructAt(static_cast<NonTrivialData*>(memory3), 10, 20);
        CHECK_EQ(result3->A, 10);
        CHECK_EQ(result3->B, 20);

        result3->~NonTrivialData();
        ::operator delete(memory3);
    }

    TEST_CASE("DestroyAt") {
        // Trivial type
        void* memory1 = ::operator new(sizeof(int));
        int* result1 = wstl::ConstructAt(static_cast<int*>(memory1), 10);

        wstl::DestroyAt(result1);
        CHECK_EQ(*result1, 10); // does nothing

        ::operator delete(memory1);

        // Non-trivial types
        size_t counter = 0;
        void* memory2 = ::operator new(sizeof(Deletable));
        Deletable* result2 = wstl::ConstructAt(static_cast<Deletable*>(memory2), &counter);

        wstl::DestroyAt(result2);
        CHECK_EQ(counter, 1);

        ::operator delete(memory2);
    }

    TEST_CASE("DestroyInRange") {
        // Trivial type
        int* memory1 = new int[4];
        CustomIota(&memory1[0], &memory1[4], 0);

        int* result = wstl::DestroyInRange(memory1, 4); // Nothing happens
        CHECK_EQ(result, memory1 + 4);

        CHECK_EQ(memory1[0], 0);
        CHECK_EQ(memory1[1], 1);
        CHECK_EQ(memory1[2], 2);
        CHECK_EQ(memory1[3], 3);

        delete[] memory1;

        // Non-trivial types
        size_t counter = 0;
        void* memory2 = ::operator new(sizeof(Deletable) * 4);
        
        for(size_t i = 0; i < 4; ++i) 
            wstl::ConstructAt<Deletable>(static_cast<Deletable*>(memory2) + i, &counter);

        Deletable* result2 = wstl::DestroyInRange(static_cast<Deletable*>(memory2), 4);
        CHECK_EQ(result2, static_cast<Deletable*>(memory2) + 4);
        CHECK_EQ(counter, 4);

        ::operator delete(memory2);
    }

    TEST_CASE("AlignedStorage") {
        StorageType data[10];

        size_t alignment = wstl::AlignmentOf<StorageType>::Value;

        #ifdef __WSTL_CXX11__
        size_t expected = std::alignment_of<uint32_t>::value;
        #else
        size_t expected = 4UL;
        #endif

        CHECK_EQ(alignment, expected);

        // Test alignment of each element
        for(int i = 0; i < 10; ++i) CHECK_EQ((size_t(&data[i]) % expected), 0UL);
    }

    TEST_CASE("AlignedStorage conversion operators") {
        StorageType data;
        void* ptrData = &data.Data;

        uint32_t& ref = data;
        const uint32_t& cref = data;
        CHECK_EQ(&ref, ptrData);
        CHECK_EQ(&cref, ptrData);

        uint32_t* ptr = data;
        const uint32_t* cptr = data;
        CHECK_EQ(ptr, ptrData);
        CHECK_EQ(cptr, ptrData);

        uint32_t& ref2 = data.GetReference<uint32_t>();
        const uint32_t& cref2 = data.GetReference<uint32_t>();
        CHECK_EQ(&ref2, ptrData);
        CHECK_EQ(&cref2, ptrData);

        uint32_t* ptr2 = data.GetPointer<uint32_t>();
        const uint32_t* cptr2 = data.GetPointer<uint32_t>();
        CHECK_EQ(ptr2, ptrData);
        CHECK_EQ(cptr2, ptrData);
    }

    TEST_CASE("UninitializedBuffer") {
        typedef wstl::UninitializedBuffer<uint32_t, 3> BufferType;

        const size_t alignment = wstl::AlignmentOf<BufferType>::Value;

        #ifdef __WSTL_CXX11__
        const size_t expected = std::alignment_of<uint32_t>::value;
        #else
        const size_t expected = 4UL;
        #endif

        CHECK_EQ(BufferType::Alignment, alignment);
        CHECK_EQ(alignment, expected);

        CHECK_EQ(BufferType::ObjectSize, sizeof(uint32_t));
        CHECK_EQ(BufferType::StaticSize, 3);

        BufferType buffer;
        const BufferType& refbuffer = buffer;
        buffer[0] = 0U;
        buffer[1] = 1U;
        buffer[2] = 2U;

        // Pointer conversion
        uint32_t* ptr = buffer;
        const uint32_t* cptr = refbuffer;

        CHECK_EQ(*ptr, 0);
        CHECK_EQ(*cptr, 0);

        // Reference conversion
        uint32_t& ref = buffer;
        const uint32_t& cref = refbuffer;

        CHECK_EQ(ref, 0);
        CHECK_EQ(cref, 0);

        // Data
        CHECK_EQ(*buffer.Data(), 0);
        CHECK_EQ(*refbuffer.Data(), 0);

        // Begin
        CHECK_EQ(buffer.Begin(), ptr);
        CHECK_EQ(refbuffer.Begin(), cptr);

        // End
        CHECK_EQ(buffer.End(), ptr + 3);
        CHECK_EQ(refbuffer.End(), cptr + 3);

        // Subscript operator
        CHECK_EQ(buffer[0], 0U);
        CHECK_EQ(buffer[1], 1U);
        CHECK_EQ(buffer[2], 2U);

        CHECK_EQ(refbuffer[0], 0U);
        CHECK_EQ(refbuffer[1], 1U);
        CHECK_EQ(refbuffer[2], 2U);
    }

    TEST_CASE("memory Copy") {
        // Basic copy
        uint8_t source1[6] = {1, 2, 3, 4, 5, 6};
        uint8_t destination1[6] = {0};

        CHECK_EQ(wstl::memory::Copy(destination1, source1, sizeof(source1)), destination1);

        CHECK_EQ(destination1[0], 1);
        CHECK_EQ(destination1[1], 2);
        CHECK_EQ(destination1[2], 3);
        CHECK_EQ(destination1[3], 4);
        CHECK_EQ(destination1[4], 5);
        CHECK_EQ(destination1[5], 6);

        // Zero bytes copied
        uint8_t source2[4] = {10, 20, 30, 40};
        uint8_t destination2[4] = {0};

        CHECK_EQ(wstl::memory::Copy(destination2, source2, 0), destination2);

        CHECK_EQ(destination2[0], 0);
        CHECK_EQ(destination2[1], 0);
        CHECK_EQ(destination2[2], 0);
        CHECK_EQ(destination2[3], 0);

        // Single byte copied
        uint8_t source3[1] = {0xAB};
        uint8_t destination3[1] = {0};

        CHECK_EQ(wstl::memory::Copy(destination3, source3, 1), destination3);
        CHECK_EQ(destination3[0], 0xAB);

        // Partial copy, surrounding bytes must stay untouched
        uint8_t source4[4] = {0xAA, 0xBB, 0xCC, 0xDD};
        uint8_t destination4[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        uint8_t expected4[8] = {1, 1, 0xAA, 0xBB, 0xCC, 0xDD, 1, 1};

        CHECK_EQ(wstl::memory::Copy(destination4 + 2, source4, 4), destination4 + 2);
        CHECK(std::equal(expected4, expected4 + 8, destination4));

        // Larger buffer with an odd, non-power-of-two size
        uint8_t source5[33];
        uint8_t destination5[33];

        for(size_t i = 0; i < sizeof(source5); ++i) source5[i] = static_cast<uint8_t>(i + 1);

        CHECK_EQ(wstl::memory::Copy(destination5, source5, sizeof(source5)), destination5);

        for(size_t i = 0; i < sizeof(source5); ++i) CHECK_EQ(destination5[i], static_cast<uint8_t>(i + 1));

        // Copy of a structure
        NonTrivialData structSource(12, 34);
        NonTrivialData structDestination(0, 0);

        wstl::memory::Copy(&structDestination, &structSource, sizeof(NonTrivialData));

        CHECK_EQ(structDestination.A, 12);
        CHECK_EQ(structDestination.B, 34);
    }

    TEST_CASE("memory Set") {
        // Basic set
        uint8_t buffer1[6];

        CHECK_EQ(wstl::memory::Set(buffer1, 0x5A, sizeof(buffer1)), buffer1);

        for(size_t i = 0; i < sizeof(buffer1); ++i) CHECK_EQ(buffer1[i], 0x5A);

        // Zero bytes set
        uint8_t buffer2[4] = {1, 2, 3, 4};

        CHECK_EQ(wstl::memory::Set(buffer2, 0, 0), buffer2);

        CHECK_EQ(buffer2[0], 1);
        CHECK_EQ(buffer2[1], 2);
        CHECK_EQ(buffer2[2], 3);
        CHECK_EQ(buffer2[3], 4);

        // Partial set, surrounding bytes must stay untouched
        uint8_t buffer3[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        uint8_t expected3[8] = {1, 1, 0xCC, 0xCC, 0xCC, 1, 1, 1};

        CHECK_EQ(wstl::memory::Set(buffer3 + 2, 0xCC, 3), buffer3 + 2);
        CHECK(std::equal(expected3, expected3 + 8, buffer3));

        // Value is converted to unsigned char
        uint8_t buffer4[2];

        wstl::memory::Set(buffer4, 0x1FF, sizeof(buffer4));

        CHECK_EQ(buffer4[0], 0xFF);
        CHECK_EQ(buffer4[1], 0xFF);
    }

    TEST_CASE("memory Move") {
        // Non-overlapping move
        uint8_t source1[6] = {1, 2, 3, 4, 5, 6};
        uint8_t destination1[6] = {0};

        CHECK_EQ(wstl::memory::Move(destination1, source1, sizeof(source1)), destination1);

        for(size_t i = 0; i < sizeof(source1); ++i) CHECK_EQ(destination1[i], i + 1);

        // Zero bytes moved
        uint8_t destination2[4] = {1, 2, 3, 4};

        CHECK_EQ(wstl::memory::Move(destination2, destination2, 0), destination2);

        CHECK_EQ(destination2[0], 1);
        CHECK_EQ(destination2[1], 2);
        CHECK_EQ(destination2[2], 3);
        CHECK_EQ(destination2[3], 4);

        // Overlapping move, destination before source
        uint8_t buffer3[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        uint8_t expected3[8] = {3, 4, 5, 6, 7, 8, 7, 8};

        CHECK_EQ(wstl::memory::Move(buffer3, buffer3 + 2, 6), buffer3);
        CHECK(std::equal(expected3, expected3 + 8, buffer3));

        // Overlapping move, destination after source
        uint8_t buffer4[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        uint8_t expected4[8] = {1, 2, 1, 2, 3, 4, 5, 6};

        CHECK_EQ(wstl::memory::Move(buffer4 + 2, buffer4, 6), buffer4 + 2);
        CHECK(std::equal(expected4, expected4 + 8, buffer4));
    }

    TEST_CASE("memory Compare") {
        // Equal ranges
        uint8_t buffer1[6] = {1, 2, 3, 4, 5, 6};
        uint8_t buffer2[6] = {1, 2, 3, 4, 5, 6};

        CHECK_EQ(wstl::memory::Compare(buffer1, buffer2, sizeof(buffer1)), 0);

        // Zero bytes compared
        uint8_t buffer3[4] = {1, 2, 3, 4};
        uint8_t buffer4[4] = {5, 6, 7, 8};

        CHECK_EQ(wstl::memory::Compare(buffer3, buffer4, 0), 0);

        // First range is smaller
        uint8_t buffer5[4] = {1, 2, 3, 4};
        uint8_t buffer6[4] = {1, 2, 3, 5};

        CHECK(wstl::memory::Compare(buffer5, buffer6, sizeof(buffer5)) < 0);
        CHECK(wstl::memory::Compare(buffer6, buffer5, sizeof(buffer5)) > 0);

        // Difference in the last byte
        uint8_t buffer7[4] = {1, 2, 3, 4};
        uint8_t buffer8[4] = {1, 2, 3, 255};

        CHECK(wstl::memory::Compare(buffer7, buffer8, sizeof(buffer7)) < 0);
        CHECK(wstl::memory::Compare(buffer8, buffer7, sizeof(buffer7)) > 0);

        // Difference in the first byte, rest must not matter
        uint8_t buffer9[4] = {1, 2, 9, 9};
        uint8_t buffer10[4] = {2, 0, 0, 0};

        CHECK(wstl::memory::Compare(buffer9, buffer10, sizeof(buffer9)) < 0);
        CHECK(wstl::memory::Compare(buffer10, buffer9, sizeof(buffer9)) > 0);
    }

    TEST_CASE("memory Locate") {
        // Byte found in the middle
        uint8_t buffer1[6] = {1, 2, 3, 4, 5, 6};

        CHECK_EQ(wstl::memory::Locate(buffer1, 4, sizeof(buffer1)), buffer1 + 3);

        // Byte found at the boundaries
        CHECK_EQ(wstl::memory::Locate(buffer1, 1, sizeof(buffer1)), buffer1);
        CHECK_EQ(wstl::memory::Locate(buffer1, 6, sizeof(buffer1)), buffer1 + 5);

        // Byte not found
        CHECK_EQ(wstl::memory::Locate(buffer1, 42, sizeof(buffer1)), wstl::NullPointer);

        // Zero bytes searched
        CHECK_EQ(wstl::memory::Locate(buffer1, 1, 0), wstl::NullPointer);

        // Search stops at the first occurrence
        uint8_t buffer2[5] = {1, 2, 3, 2, 1};

        CHECK_EQ(wstl::memory::Locate(buffer2, 2, sizeof(buffer2)), buffer2 + 1);
    }
}
