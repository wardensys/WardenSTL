// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_TEST_UTILS_HPP__
#define __WSTL_TEST_UTILS_HPP__

#include <iostream>
#include <algorithm>
#include <wstl/NullPointer.hpp>

#ifndef __WSTL_CXX11__
namespace doctest {
    inline String toString(const wstl::NullPointerType&) {
        return "NullPointer";
    }
}

namespace wstl {
    inline std::ostream& operator<<(std::ostream& stream, const NullPointerType&) {
        return stream << "NullPointer";
    }
}
#endif

template<typename C>
inline typename C::iterator BeginImpl(C& c) {
    return c.begin();
}

template<typename C>
inline typename C::const_iterator BeginImpl(const C& c) {
    return c.begin();
}

template<typename C>
inline typename C::const_iterator ConstBeginImpl(const C& c) {
    return c.cbegin();
}

template<typename C>
inline typename C::iterator EndImpl(C& c) {
    return c.end();
}

template<typename C>
inline typename C::const_iterator EndImpl(const C& c) {
    return c.end();
}

template<typename C>
inline typename C::const_iterator ConstEndImpl(const C& c) {
    return c.cend();
}

template<typename T, size_t N>
inline T* BeginImpl(T (&array)[N]) {
    return &array[0];
}

template<typename T, size_t N>
inline const T* BeginImpl(const T (&array)[N]) {
    return &array[0];
}

template<typename T, size_t N>
inline const T* ConstBeginImpl(const T (&array)[N]) {
    return &array[0];
}

template<typename T, size_t N>
inline T* EndImpl(T (&array)[N]) {
    return &array[N];
}

template<typename T, size_t N>
inline const T* EndImpl(const T (&array)[N]) {
    return &array[N];
}

template<typename T, size_t N>
inline const T* ConstEndImpl(const T (&array)[N]) {
    return &array[N];
}

struct NonDefaultConstructible {
    NonDefaultConstructible(int value) : Value(value) {}

    int Value;
};

struct ExplicitConstructible {
    explicit ExplicitConstructible() : Value(0) {}
    explicit ExplicitConstructible(int value) : Value(value) {}
    
    int Value;
};

struct NonTrivialData {
    NonTrivialData() : A(0), B(0) {}
    NonTrivialData(int a, int b) : A(a), B(b) {}

    int A;
    int B;
};

struct Dummy {};
struct Dummy2 {};

struct FakeInteger {
    int Value;
};

struct From {
    From(int x) : Value(x) {};

    int Value;
};

struct To {
    To(const From& from) : Value(from.Value) {};
    
    To& operator=(const From& from) {
        Value = from.Value;
        return *this;
    }

    int Value;
};

struct ToExplicit {
    explicit ToExplicit(const From& from) : Value(from.Value) {};
    
    ToExplicit& operator=(const From& from) {
        Value = from.Value;
        return *this;
    }

    int Value;
};

inline bool operator==(const NonTrivialData& a, const NonTrivialData& b) {
    return a.A == b.A && a.B == b.B;
}

inline bool operator!=(const NonTrivialData& a, const NonTrivialData& b) {
    return !(a == b);
}

inline bool operator<(const NonTrivialData& a, const NonTrivialData& b) {
    return a.A < b.A || (a.A == b.A && a.B < b.B);
}

inline bool operator>(const NonTrivialData& a, const NonTrivialData& b) {
    return b < a;
}

inline bool operator<=(const NonTrivialData& a, const NonTrivialData& b) {
    return !(b < a);
}

inline bool operator>=(const NonTrivialData& a, const NonTrivialData& b) {
    return !(a < b);
}

#ifdef __WSTL_CXX11__
template<typename T>
struct MovableData {
    MovableData() : Value(), Valid(true) {}

    explicit MovableData(const T& value) : Value(value), Valid(true) {}
    explicit MovableData(T&& value) : Value(std::move(value)), Valid(true) {}

    MovableData(MovableData&& other) noexcept : Value(std::move(other.Value)), Valid(true) {
        other.Valid = false;
    }

    template<typename U>
    MovableData(MovableData<U>&& other) noexcept : Value(std::move(other.Value)), Valid(true) {
        other.Valid = false;
    }

    MovableData(const MovableData&& other) noexcept : Value(std::move(other.Value)), Valid(true) {
        other.Valid = false;
    }

    template<typename U>
    MovableData(const MovableData<U>&& other) noexcept : Value(std::move(other.Value)), Valid(true) {
        other.Valid = false;
    }

    virtual ~MovableData() {
        Valid = false;
    }

    MovableData& operator=(MovableData&& other) noexcept {
        Value = std::move(other.Value);
        Valid = true;

        other.Valid = false;
        return *this;
    }

    bool operator<(const MovableData& other) const {
        return Value < other.Value;
    }

    bool operator>(const MovableData& other) const {
        return other.Value < Value;
    }

    bool operator<=(const MovableData& other) const {
        return !(other.Value < Value);
    }

    bool operator>=(const MovableData& other) const {
        return !(Value < other.Value);
    }

    operator bool() const {
        return Valid;
    }

    T Value;
    mutable bool Valid;

private:
    MovableData(const MovableData&) = delete;
    MovableData& operator=(const MovableData&) = delete;
};

template<typename T>
bool operator==(const MovableData<T>& a, const MovableData<T>& b) {
    return a.Value == b.Value;
}

template<typename T>
bool operator!=(const MovableData<T>& a, const MovableData<T>& b) {
    return !(a == b);
}

#endif
#endif
