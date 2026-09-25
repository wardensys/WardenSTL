// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Functional.hpp>
#include <functional>
#include <array>
#include <vector>

#include "Utils.hpp"


namespace {
    static bool functionCalled = false;
    static std::array<bool, 3> paramsCorrect = {false, false, false};

    static const __WSTL_CONSTEXPR__ int VALUE1 = 67;
    static const __WSTL_CONSTEXPR__ char VALUE2 = 'x';
    static const __WSTL_CONSTEXPR__ FakeInteger VALUE3 = {69};

    struct SetupFixture {
        SetupFixture() {
            functionCalled = false;
            paramsCorrect[0] = false;
            paramsCorrect[1] = false;
            paramsCorrect[2] = false;
        }
    };

    #ifdef __WSTL_CXX11__
    template<typename Return, typename... Args, typename... CallArgs>
    Return Call(wstl::FunctionBase<Return(Args...)>& function, CallArgs&&... args) {
        return function(wstl::Forward<CallArgs>(args)...);
    }

    template<typename Return, typename... Args, typename... CallArgs>
    Return Call(const wstl::FunctionBase<Return(Args...)>& function, CallArgs&&... args) {
        return function(wstl::Forward<CallArgs>(args)...);
    }
    #else
    template<typename Return, typename Arg1, typename Arg2>
    Return Call(const wstl::FunctionBase<Return(Arg1, Arg2)>& function, Arg1 arg1, Arg2 arg2) {
        return function(arg1, arg2);
    }

    template<typename Return, typename Arg>
    Return Call(const wstl::FunctionBase<Return(Arg)>& function, Arg arg) {
        return function(arg);
    }

    template<typename Return>
    Return Call(const wstl::FunctionBase<Return()>& function) {
        return function();
    }
    #endif

    bool FreeBool(int a) __WSTL_NOEXCEPT__ {
        functionCalled = true;
        paramsCorrect[0] = (a == VALUE1);
        return a == VALUE1;
    }

    bool FreeBool2(int a, char b) {
        return a == b;
    }
    
    bool FreeBool0() {
        return functionCalled;
    }

    int FreeAdd(int a, int b) {
        return a + b;
    }

    void FreeVoid() {
        functionCalled = true;
    }

    void Free1(int a) {
        functionCalled = true;
        paramsCorrect[0] = (a == VALUE1);
    }

    void Free2(int a, char b) {
        functionCalled = true;
        paramsCorrect[0] = (a == VALUE1);
        paramsCorrect[1] = (b == VALUE2);
    }

    #ifdef __WSTL_CXX11__
    void Free3(int a, char b, FakeInteger c) {
        functionCalled = true;
        paramsCorrect[0] = (a == VALUE1);
        paramsCorrect[1] = (b == VALUE2);
        paramsCorrect[2] = (c.Value == VALUE3.Value);
    }

    int Free3Add(int a, char b, FakeInteger c) {
        return a + b + c.Value;
    }
    #endif

    int FreeReturn() {
        functionCalled = true;
        return VALUE1;
    }

    class TestObject {
    public:
        void MemberNoexcept() __WSTL_NOEXCEPT__ {
            functionCalled = true;
        }

        void MemberVoid() {
            functionCalled = true;
        }

        void Member1(int a) {
            functionCalled = true;
            paramsCorrect[0] = (a == VALUE1);
        }

        void Member2(int a, char b) {
            functionCalled = true;
            paramsCorrect[0] = (a == VALUE1);
            paramsCorrect[1] = (b == VALUE2);
        }

        void Member3(int a, char b, FakeInteger c) {
            functionCalled = true;
            paramsCorrect[0] = (a == VALUE1);
            paramsCorrect[1] = (b == VALUE2);
            paramsCorrect[2] = (c.Value == VALUE3.Value);
        }

        int MemberReturn() {
            functionCalled = true;
            return VALUE1;
        }

        void ConstMemberVoid() const {
            functionCalled = true;
        }

        void ConstMember1(int a) const {
            functionCalled = true;
            paramsCorrect[0] = (a == VALUE1);
        }

        void ConstMember2(int a, char b) const {
            functionCalled = true;
            paramsCorrect[0] = (a == VALUE1);
            paramsCorrect[1] = (b == VALUE2);
        }

        void ConstMember3(int a, char b, FakeInteger c) const {
            functionCalled = true;
            paramsCorrect[0] = (a == VALUE1);
            paramsCorrect[1] = (b == VALUE2);
            paramsCorrect[2] = (c.Value == VALUE3.Value);
        }

        int ConstMemberReturn() const {
            functionCalled = true;
            return VALUE1;
        }

        bool MemberBool(int a) {
            return a == VALUE1;
        }

        bool MemberBool3(int a, char b, FakeInteger c) const {
            return (a == VALUE1) && (b == VALUE2) && (c.Value == VALUE3.Value);
        }
    };

    TestObject testObject;
    const TestObject constTestObject;

    struct TestObject2 {
        explicit TestObject2(int n) : Value(n) {}

        int Sum(int x) const { return x + Value; }

        int Value;
    };

    struct Functor {
        typedef int ResultType;
        typedef int ArgumentType;

        explicit Functor(int n) : Value(n) {}

        int operator()(int x) const { return x + Value; }

        int Value;
    };

    struct HeterogeneousFunctor {
        typedef void IsTransparent;

        template<typename T, typename U>
        bool operator()(T t, U u) { return t == u; }
    };

    struct BoolMember {
        bool Value;
    };

    bool BoolVariable;

    struct GetBoolFunctor : wstl::NullaryFunction<bool> {
        bool operator()() const {
            return BoolVariable;
        }
    };

    struct InvalidFunctor {
        bool operator()(int a) const {
            return a == VALUE1;
        }
    };

    struct Functor2 : wstl::BinaryFunction<int, char, bool> {
        bool operator()(int a, char b) const {
            return (a == VALUE1) && (b == VALUE2);
        }
    };

    struct BoolFunctor1 : wstl::UnaryFunction<FakeInteger, bool> {
        bool operator()(FakeInteger a) const {
            return a.Value == VALUE3.Value;
        }
    };

    #ifdef __WSTL_CXX11__
    struct CVRefCallable {
        int operator()(int&) const {
            return 1;
        }

        int operator()(const int&) const {
            return 2;
        }

        int operator()(int&&) const {
            return 3;
        }

        int operator()(const int&&) const {
            return 4;
        }
    };

    struct BoolCVRefCallable {
        bool operator()(int) & {
            return false;
        }

        bool operator()(int) const& {
            return false;
        }

        bool operator()(int) && {
            return true;
        }

        bool operator()(int) const&& {
            return true;
        }
    };
    #endif
}

TEST_SUITE("Functional") {
    TEST_CASE_FIXTURE(SetupFixture, "Function free void") {
        // Constructors
        wstl::Function<void()> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<void()> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(), wstl::BadFunctionCall);

        wstl::Function<void()> func2(FreeVoid);
        CHECK(func2);
        CHECK_EQ(func2.Target(), FreeVoid);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<void()> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), FreeVoid);
        
        #ifdef __WSTL_CXX11__
        wstl::Function<void()> func4(wstl::Move(func2));
        CHECK_FALSE(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), FreeVoid);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), FreeVoid);

        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_FALSE(func4);
        CHECK(func2);
        CHECK_EQ(func2.Target(), FreeVoid);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);

        func0 = FreeVoid;
        CHECK_EQ(func0.Target(), FreeVoid);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), FreeVoid);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(FreeVoid);
        CHECK_EQ(func3.Target(), FreeVoid);

        // Function call
        Call(func3);
        CHECK(functionCalled);
    }


    TEST_CASE_FIXTURE(SetupFixture, "Function const free void") {
        // Constructors
        const wstl::Function<void()> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<void()> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(), wstl::BadFunctionCall);

        const wstl::Function<void()> func2(FreeVoid);
        CHECK(func2);
        CHECK_EQ(func2.Target(), FreeVoid);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<void()> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), FreeVoid);

        #ifdef __WSTL_CXX11__
        const wstl::Function<void()> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), FreeVoid);
        #endif

        // Function call
        Call(func3);
        CHECK(functionCalled);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function free 1 parameter") {
        // Constructors
        wstl::Function<void(int)> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<void(int)> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0), wstl::BadFunctionCall);

        wstl::Function<void(int)> func2(Free1);
        CHECK(func2);
        CHECK_EQ(func2.Target(), Free1);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<void(int)> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), Free1);

        #ifdef __WSTL_CXX11__
        wstl::Function<void(int)> func4(wstl::Move(func2));
        CHECK_FALSE(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), Free1);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), Free1);
        
        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_FALSE(func4);
        CHECK(func2);
        CHECK_EQ(func2.Target(), Free1);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);

        func0 = Free1;
        CHECK_EQ(func0.Target(), Free1);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), Free1);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(Free1);
        CHECK_EQ(func3.Target(), Free1);

        Call(func3, VALUE1);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const free 1 parameter") {
        // Constructors
        const wstl::Function<void(int)> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<void(int)> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0), wstl::BadFunctionCall);

        const wstl::Function<void(int)> func2(Free1);
        CHECK(func2);
        CHECK_EQ(func2.Target(), Free1);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<void(int)> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), Free1);

        #ifdef __WSTL_CXX11__
        const wstl::Function<void(int)> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), Free1);
        #endif

        // Function call
        Call(func3, VALUE1);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function free 2 parameters") {
        // Constructors
        wstl::Function<void(int, char)> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0, '\0'), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<void(int, char)> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0, '\0'), wstl::BadFunctionCall);

        wstl::Function<void(int, char)> func2(Free2);
        CHECK(func2);
        CHECK_EQ(func2.Target(), Free2);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<void(int, char)> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), Free2);

        #ifdef __WSTL_CXX11__
        wstl::Function<void(int, char)> func4(wstl::Move(func2));
        CHECK_FALSE(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), Free2);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), Free2);

        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_FALSE(func4);
        CHECK(func2);
        CHECK_EQ(func2.Target(), Free2);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);

        func0 = Free2;
        CHECK_EQ(func0.Target(), Free2);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), Free2);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(Free2);
        CHECK_EQ(func3.Target(), Free2);

        // Function call
        Call(func3, VALUE1, VALUE2);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const free 2 parameters") {
        // Constructors
        const wstl::Function<void(int, char)> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0, '\0'), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<void(int, char)> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0, '\0'), wstl::BadFunctionCall);

        const wstl::Function<void(int, char)> func2(Free2);
        CHECK(func2);
        CHECK_EQ(func2.Target(), Free2);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<void(int, char)> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), Free2);

        #ifdef __WSTL_CXX11__
        const wstl::Function<void(int, char)> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), Free2);
        #endif

        // Function call
        Call(func3, VALUE1, VALUE2);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE_FIXTURE(SetupFixture, "Function free 3 parameters") {
        // Constructors
        wstl::Function<void(int, char, FakeInteger)> func0(nullptr);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), nullptr);
        CHECK_THROWS_AS(func0(0, '\0', {0}), wstl::BadFunctionCall);
        CHECK(func0 == nullptr);
        CHECK_FALSE(func0 != nullptr);

        wstl::Function<void(int, char, FakeInteger)> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), nullptr);
        CHECK_THROWS_AS(func1(0, '\0', {0}), wstl::BadFunctionCall);

        wstl::Function<void(int, char, FakeInteger)> func2(Free3);
        CHECK(func2);
        CHECK_EQ(func2.Target(), Free3);
        CHECK_FALSE(func2 == nullptr);
        CHECK(func2 != nullptr);

        wstl::Function<void(int, char, FakeInteger)> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), Free3);

        wstl::Function<void(int, char, FakeInteger)> func4(wstl::Move(func2));
        CHECK_FALSE(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), Free3);
        
        // Assignment operators
        func1 = func4;
        CHECK(func4);
        CHECK(func1);
        CHECK_EQ(func1.Target(), Free3);

        func2 = wstl::Move(func3);
        CHECK_FALSE(func3);
        CHECK(func2);
        CHECK_EQ(func2.Target(), Free3);

        func4 = nullptr;
        CHECK_EQ(func4.Target(), nullptr);

        func4 = Free3;
        CHECK_EQ(func4.Target(), Free3);

        // Swap
        func3.Swap(func2);
        CHECK_EQ(func3.Target(), Free3);
        CHECK_EQ(func2.Target(), nullptr);

        // Reset
        func2.Reset(Free3);
        CHECK_EQ(func2.Target(), Free3);

        // Function call
        Call(func3, VALUE1, VALUE2, VALUE3);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const free 3 parameters") {
        // Constructors
        const wstl::Function<void(int, char, FakeInteger)> func0(nullptr);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), nullptr);
        CHECK_THROWS_AS(func0(0, '\0', {0}), wstl::BadFunctionCall);
        CHECK(func0 == nullptr);
        CHECK_FALSE(func0 != nullptr);

        const wstl::Function<void(int, char, FakeInteger)> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), nullptr);
        CHECK_THROWS_AS(func1(0, '\0', {0}), wstl::BadFunctionCall);

        const wstl::Function<void(int, char, FakeInteger)> func2(Free3);
        CHECK(func2);
        CHECK_EQ(func2.Target(), Free3);
        CHECK_FALSE(func2 == nullptr);
        CHECK(func2 != nullptr);

        const wstl::Function<void(int, char, FakeInteger)> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), Free3);

        const wstl::Function<void(int, char, FakeInteger)> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), Free3);

        // Function call
        Call(func3, VALUE1, VALUE2, VALUE3);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
    }
    #endif

    TEST_CASE_FIXTURE(SetupFixture, "Function free return") {
        // Constructors
        wstl::Function<int()> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func0(); (void) i; }, wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<int()> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func1(); (void) i; }, wstl::BadFunctionCall);

        wstl::Function<int()> func2(FreeReturn);
        CHECK(func2);
        CHECK_EQ(func2.Target(), FreeReturn);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<int()> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), FreeReturn);

        #ifdef __WSTL_CXX11__
        wstl::Function<int()> func4(wstl::Move(func2));
        CHECK_FALSE(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), FreeReturn);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), FreeReturn);

        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_FALSE(func4);
        CHECK(func2);
        CHECK_EQ(func2.Target(), FreeReturn);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);

        func0 = FreeReturn;
        CHECK_EQ(func0.Target(), FreeReturn);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), FreeReturn);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(FreeReturn);
        CHECK_EQ(func3.Target(), FreeReturn);

        // Function call
        int result = Call(func3);
        CHECK(functionCalled);
        CHECK_EQ(result, VALUE1);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const free return") {
        // Constructors
        const wstl::Function<int()> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func0(); (void) i; }, wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<int()> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func1(); (void) i; }, wstl::BadFunctionCall);

        const wstl::Function<int()> func2(FreeReturn);
        CHECK(func2);
        CHECK_EQ(func2.Target(), FreeReturn);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<int()> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), FreeReturn);

        #ifdef __WSTL_CXX11__
        const wstl::Function<int()> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), FreeReturn);
        #endif

        // Function call
        int result = Call(func3);
        CHECK(functionCalled);
        CHECK_EQ(result, VALUE1);
    }

    // Member, void

    TEST_CASE_FIXTURE(SetupFixture, "Function member void") {
        // Constructors
        wstl::Function<void(), TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<void(), TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(), wstl::BadFunctionCall);

        wstl::Function<void(), TestObject> func2(&testObject, &TestObject::MemberVoid);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::MemberVoid);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<void(), TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::MemberVoid);
        CHECK_EQ(func3.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        wstl::Function<void(), TestObject> func4(wstl::Move(func2));
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::MemberVoid);
        CHECK_EQ(func4.TargetObject(), &testObject);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), &TestObject::MemberVoid);
        CHECK_EQ(func1.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_EQ(func4.Target(), wstl::NullPointer);
        CHECK_EQ(func4.TargetObject(), wstl::NullPointer);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::MemberVoid);
        CHECK_EQ(func2.TargetObject(), &testObject);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), &TestObject::MemberVoid);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_EQ(func3.Target(), wstl::NullPointer);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(&testObject, &TestObject::MemberVoid);
        CHECK_EQ(func3.Target(), &TestObject::MemberVoid);
        CHECK_EQ(func3.TargetObject(), &testObject);

        // Function call
        Call(func3);
        CHECK(functionCalled);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const member void") {
        // Constructors
        const wstl::Function<void(), TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<void(), TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(), wstl::BadFunctionCall);

        const wstl::Function<void(), TestObject> func2(&testObject, &TestObject::MemberVoid);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::MemberVoid);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<void(), TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::MemberVoid);
        CHECK_EQ(func3.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        const wstl::Function<void(), TestObject> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::MemberVoid);
        CHECK_EQ(func4.TargetObject(), &testObject);
        #endif

        // Function call
        Call(func3);
        CHECK(functionCalled);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function member const void") {
        // Constructors
        wstl::Function<void(), const TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<void(), const TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(), wstl::BadFunctionCall);

        wstl::Function<void(), const TestObject> func2(&constTestObject, &TestObject::ConstMemberVoid);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMemberVoid);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<void(), const TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::ConstMemberVoid);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        wstl::Function<void(), const TestObject> func4(wstl::Move(func2));
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::ConstMemberVoid);
        CHECK_EQ(func4.TargetObject(), &constTestObject);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), &TestObject::ConstMemberVoid);
        CHECK_EQ(func1.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_EQ(func4.Target(), wstl::NullPointer);
        CHECK_EQ(func4.TargetObject(), wstl::NullPointer);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMemberVoid);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), &TestObject::ConstMemberVoid);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_EQ(func3.Target(), wstl::NullPointer);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(&constTestObject, &TestObject::ConstMemberVoid);
        CHECK_EQ(func3.Target(), &TestObject::ConstMemberVoid);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        // Function call
        Call(func3);
        CHECK(functionCalled);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const member const void") {
        // Constructors
        const wstl::Function<void(), const TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<void(), const TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(), wstl::BadFunctionCall);

        const wstl::Function<void(), const TestObject> func2(&constTestObject, &TestObject::ConstMemberVoid);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMemberVoid);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<void(), const TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::ConstMemberVoid);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        const wstl::Function<void(), const TestObject> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::ConstMemberVoid);
        CHECK_EQ(func4.TargetObject(), &constTestObject);
        #endif

        // Function call
        Call(func3);
        CHECK(functionCalled);
    }

    // Member, 1 parameter

    TEST_CASE_FIXTURE(SetupFixture, "Function member 1 parameter") {
        // Constructors
        wstl::Function<void(int), TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<void(int), TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0), wstl::BadFunctionCall);

        wstl::Function<void(int), TestObject> func2(&testObject, &TestObject::Member1);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::Member1);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<void(int), TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::Member1);
        CHECK_EQ(func3.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        wstl::Function<void(int), TestObject> func4(wstl::Move(func2));
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::Member1);
        CHECK_EQ(func4.TargetObject(), &testObject);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), &TestObject::Member1);
        CHECK_EQ(func1.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_EQ(func4.Target(), wstl::NullPointer);
        CHECK_EQ(func4.TargetObject(), wstl::NullPointer);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::Member1);
        CHECK_EQ(func2.TargetObject(), &testObject);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), &TestObject::Member1);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_EQ(func3.Target(), wstl::NullPointer);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(&testObject, &TestObject::Member1);
        CHECK_EQ(func3.Target(), &TestObject::Member1);
        CHECK_EQ(func3.TargetObject(), &testObject);

        // Function call
        Call(func3, VALUE1);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const member 1 parameter") {
        // Constructors
        const wstl::Function<void(int), TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<void(int), TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0), wstl::BadFunctionCall);

        const wstl::Function<void(int), TestObject> func2(&testObject, &TestObject::Member1);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::Member1);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<void(int), TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::Member1);
        CHECK_EQ(func3.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        const wstl::Function<void(int), TestObject> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::Member1);
        CHECK_EQ(func4.TargetObject(), &testObject);
        #endif

        // Function call
        Call(func3, VALUE1);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function member const 1 parameter") {
        // Constructors
        wstl::Function<void(int), const TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<void(int), const TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0), wstl::BadFunctionCall);

        wstl::Function<void(int), const TestObject> func2(&constTestObject, &TestObject::ConstMember1);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember1);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<void(int), const TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::ConstMember1);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        wstl::Function<void(int), const TestObject> func4(wstl::Move(func2));
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::ConstMember1);
        CHECK_EQ(func4.TargetObject(), &constTestObject);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), &TestObject::ConstMember1);
        CHECK_EQ(func1.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_EQ(func4.Target(), wstl::NullPointer);
        CHECK_EQ(func4.TargetObject(), wstl::NullPointer);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember1);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember1);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_EQ(func3.Target(), wstl::NullPointer);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(&constTestObject, &TestObject::ConstMember1);
        CHECK_EQ(func3.Target(), &TestObject::ConstMember1);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        // Function call
        Call(func3, VALUE1);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const member const 1 parameter") {
        // Constructors
        const wstl::Function<void(int), const TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<void(int), const TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0), wstl::BadFunctionCall);

        const wstl::Function<void(int), const TestObject> func2(&constTestObject, &TestObject::ConstMember1);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember1);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<void(int), const TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::ConstMember1);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        const wstl::Function<void(int), const TestObject> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::ConstMember1);
        CHECK_EQ(func4.TargetObject(), &constTestObject);
        #endif

        // Function call
        Call(func3, VALUE1);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
    }

    // Member, 2 parameters

    TEST_CASE_FIXTURE(SetupFixture, "Function member 2 parameters") {
        // Constructors
        wstl::Function<void(int, char), TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0, '\0'), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<void(int, char), TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0, '\0'), wstl::BadFunctionCall);

        wstl::Function<void(int, char), TestObject> func2(&testObject, &TestObject::Member2);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::Member2);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<void(int, char), TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::Member2);
        CHECK_EQ(func3.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        wstl::Function<void(int, char), TestObject> func4(wstl::Move(func2));
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::Member2);
        CHECK_EQ(func4.TargetObject(), &testObject);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), &TestObject::Member2);
        CHECK_EQ(func1.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_EQ(func4.Target(), wstl::NullPointer);
        CHECK_EQ(func4.TargetObject(), wstl::NullPointer);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::Member2);
        CHECK_EQ(func2.TargetObject(), &testObject);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), &TestObject::Member2);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_EQ(func3.Target(), wstl::NullPointer);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(&testObject, &TestObject::Member2);
        CHECK_EQ(func3.Target(), &TestObject::Member2);
        CHECK_EQ(func3.TargetObject(), &testObject);

        // Function call
        Call(func3, VALUE1, VALUE2);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const member 2 parameters") {
        // Constructors
        const wstl::Function<void(int, char), TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0, '\0'), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<void(int, char), TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0, '\0'), wstl::BadFunctionCall);

        const wstl::Function<void(int, char), TestObject> func2(&testObject, &TestObject::Member2);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::Member2);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<void(int, char), TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::Member2);
        CHECK_EQ(func3.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        const wstl::Function<void(int, char), TestObject> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::Member2);
        CHECK_EQ(func4.TargetObject(), &testObject);
        #endif

        // Function call
        Call(func3, VALUE1, VALUE2);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function member const 2 parameters") {
        // Constructors
        wstl::Function<void(int, char), const TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0, '\0'), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<void(int, char), const TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0, '\0'), wstl::BadFunctionCall);

        wstl::Function<void(int, char), const TestObject> func2(&constTestObject, &TestObject::ConstMember2);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember2);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<void(int, char), const TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::ConstMember2);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        wstl::Function<void(int, char), const TestObject> func4(wstl::Move(func2));
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::ConstMember2);
        CHECK_EQ(func4.TargetObject(), &constTestObject);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), &TestObject::ConstMember2);
        CHECK_EQ(func1.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_EQ(func4.Target(), wstl::NullPointer);
        CHECK_EQ(func4.TargetObject(), wstl::NullPointer);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember2);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember2);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_EQ(func3.Target(), wstl::NullPointer);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(&constTestObject, &TestObject::ConstMember2);
        CHECK_EQ(func3.Target(), &TestObject::ConstMember2);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        // Function call
        Call(func3, VALUE1, VALUE2);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const member const 2 parameters") {
        // Constructors
        const wstl::Function<void(int, char), const TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func0(0, '\0'), wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<void(int, char), const TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS(func1(0, '\0'), wstl::BadFunctionCall);

        const wstl::Function<void(int, char), const TestObject> func2(&constTestObject, &TestObject::ConstMember2);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember2);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<void(int, char), const TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::ConstMember2);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        const wstl::Function<void(int, char), const TestObject> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::ConstMember2);
        CHECK_EQ(func4.TargetObject(), &constTestObject);
        #endif

        // Function call
        Call(func3, VALUE1, VALUE2);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
    }

    // Member, 3 parameters

    #ifdef __WSTL_CXX11__
    TEST_CASE_FIXTURE(SetupFixture, "Function member 3 parameters") {
        // Constructors
        wstl::Function<void(int, char, FakeInteger), TestObject> func0(nullptr);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), nullptr);
        CHECK_EQ(func0.TargetObject(), nullptr);
        CHECK_THROWS_AS(func0(0, '\0', {0}), wstl::BadFunctionCall);
        CHECK(func0 == nullptr);
        CHECK_FALSE(func0 != nullptr);

        wstl::Function<void(int, char, FakeInteger), TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), nullptr);
        CHECK_EQ(func1.TargetObject(), nullptr);
        CHECK_THROWS_AS(func1(0, '\0', {0}), wstl::BadFunctionCall);

        wstl::Function<void(int, char, FakeInteger), TestObject> func2(&testObject, &TestObject::Member3);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::Member3);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_FALSE(func2 == nullptr);
        CHECK(func2 != nullptr);

        wstl::Function<void(int, char, FakeInteger), TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::Member3);
        CHECK_EQ(func3.TargetObject(), &testObject);

        wstl::Function<void(int, char, FakeInteger), TestObject> func4(wstl::Move(func2));
        CHECK_EQ(func2.Target(), nullptr);
        CHECK_EQ(func2.TargetObject(), nullptr);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::Member3);
        CHECK_EQ(func4.TargetObject(), &testObject);
        
        // Assignment operators
        func1 = func4;
        CHECK(func4);
        CHECK(func1);
        CHECK_EQ(func1.Target(), &TestObject::Member3);
        CHECK_EQ(func1.TargetObject(), &testObject);

        func2 = wstl::Move(func3);
        CHECK_EQ(func3.Target(), nullptr);
        CHECK_EQ(func3.TargetObject(), nullptr);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::Member3);
        CHECK_EQ(func2.TargetObject(), &testObject);

        // Swap
        func3.Swap(func2);
        CHECK_EQ(func3.Target(), &TestObject::Member3);
        CHECK_EQ(func3.TargetObject(), &testObject);
        CHECK_EQ(func2.Target(), nullptr);
        CHECK_EQ(func2.Target(), nullptr);

        // Reset
        func2.Reset(&testObject, &TestObject::Member3);
        CHECK_EQ(func2.Target(), &TestObject::Member3);
        CHECK_EQ(func2.TargetObject(), &testObject);

        // Function call
        Call(func3, VALUE1, VALUE2, VALUE3);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const member 3 parameters") {
        // Constructors
        const wstl::Function<void(int, char, FakeInteger), TestObject> func0(nullptr);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), nullptr);
        CHECK_EQ(func0.TargetObject(), nullptr);
        CHECK_THROWS_AS(func0(0, '\0', {0}), wstl::BadFunctionCall);
        CHECK(func0 == nullptr);
        CHECK_FALSE(func0 != nullptr);

        const wstl::Function<void(int, char, FakeInteger), TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), nullptr);
        CHECK_EQ(func1.TargetObject(), nullptr);
        CHECK_THROWS_AS(func1(0, '\0', {0}), wstl::BadFunctionCall);

        const wstl::Function<void(int, char, FakeInteger), TestObject> func2(&testObject, &TestObject::Member3);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::Member3);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_FALSE(func2 == nullptr);
        CHECK(func2 != nullptr);

        const wstl::Function<void(int, char, FakeInteger), TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::Member3);
        CHECK_EQ(func3.TargetObject(), &testObject);

        const wstl::Function<void(int, char, FakeInteger), TestObject> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::Member3);
        CHECK_EQ(func4.TargetObject(), &testObject);

        // Function call
        Call(func3, VALUE1, VALUE2, VALUE3);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function member const 3 parameters") {
        // Constructors
        wstl::Function<void(int, char, FakeInteger), const TestObject> func0(nullptr);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), nullptr);
        CHECK_EQ(func0.TargetObject(), nullptr);
        CHECK_THROWS_AS(func0(0, '\0', {0}), wstl::BadFunctionCall);
        CHECK(func0 == nullptr);
        CHECK_FALSE(func0 != nullptr);

        wstl::Function<void(int, char, FakeInteger), const TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), nullptr);
        CHECK_EQ(func1.TargetObject(), nullptr);
        CHECK_THROWS_AS(func1(0, '\0', {0}), wstl::BadFunctionCall);

        wstl::Function<void(int, char, FakeInteger), const TestObject> func2(&constTestObject, &TestObject::ConstMember3);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember3);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_FALSE(func2 == nullptr);
        CHECK(func2 != nullptr);

        wstl::Function<void(int, char, FakeInteger), const TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::ConstMember3);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        wstl::Function<void(int, char, FakeInteger), const TestObject> func4(wstl::Move(func2));
        CHECK_EQ(func2.Target(), nullptr);
        CHECK_EQ(func2.TargetObject(), nullptr);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::ConstMember3);
        CHECK_EQ(func4.TargetObject(), &constTestObject);
        
        // Assignment operators
        func1 = func4;
        CHECK(func4);
        CHECK(func1);
        CHECK_EQ(func1.Target(), &TestObject::ConstMember3);
        CHECK_EQ(func1.TargetObject(), &constTestObject);

        func2 = wstl::Move(func3);
        CHECK_EQ(func3.Target(), nullptr);
        CHECK_EQ(func3.TargetObject(), nullptr);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember3);
        CHECK_EQ(func2.TargetObject(), &constTestObject);

        // Swap
        func3.Swap(func2);
        CHECK_EQ(func3.Target(), &TestObject::ConstMember3);
        CHECK_EQ(func3.TargetObject(), &constTestObject);
        CHECK_EQ(func2.Target(), nullptr);
        CHECK_EQ(func2.Target(), nullptr);

        // Reset
        func2.Reset(&constTestObject, &TestObject::ConstMember3);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember3);
        CHECK_EQ(func2.TargetObject(), &constTestObject);

        // Function call
        Call(func3, VALUE1, VALUE2, VALUE3);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const member const 3 parameters") {
        // Constructors
        const wstl::Function<void(int, char, FakeInteger), const TestObject> func0(nullptr);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), nullptr);
        CHECK_EQ(func0.TargetObject(), nullptr);
        CHECK_THROWS_AS(func0(0, '\0', {0}), wstl::BadFunctionCall);
        CHECK(func0 == nullptr);
        CHECK_FALSE(func0 != nullptr);

        const wstl::Function<void(int, char, FakeInteger), const TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), nullptr);
        CHECK_EQ(func1.TargetObject(), nullptr);
        CHECK_THROWS_AS(func1(0, '\0', {0}), wstl::BadFunctionCall);

        const wstl::Function<void(int, char, FakeInteger), const TestObject> func2(&constTestObject, &TestObject::ConstMember3);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMember3);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_FALSE(func2 == nullptr);
        CHECK(func2 != nullptr);

        const wstl::Function<void(int, char, FakeInteger), const TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::ConstMember3);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        const wstl::Function<void(int, char, FakeInteger), const TestObject> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::ConstMember3);
        CHECK_EQ(func4.TargetObject(), &constTestObject);

        // Function call
        Call(func3, VALUE1, VALUE2, VALUE3);
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
    }
    #endif

    // Member, return

    TEST_CASE_FIXTURE(SetupFixture, "Function member return") {
        // Constructors
        wstl::Function<int(), TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func0(); (void) i; }, wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<int(), TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func1(); (void) i; }, wstl::BadFunctionCall);

        wstl::Function<int(), TestObject> func2(&testObject, &TestObject::MemberReturn);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::MemberReturn);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<int(), TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::MemberReturn);
        CHECK_EQ(func3.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        wstl::Function<int(), TestObject> func4(wstl::Move(func2));
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::MemberReturn);
        CHECK_EQ(func4.TargetObject(), &testObject);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), &TestObject::MemberReturn);
        CHECK_EQ(func1.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_EQ(func4.Target(), wstl::NullPointer);
        CHECK_EQ(func4.TargetObject(), wstl::NullPointer);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::MemberReturn);
        CHECK_EQ(func2.TargetObject(), &testObject);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), &TestObject::MemberReturn);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_EQ(func3.Target(), wstl::NullPointer);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(&testObject, &TestObject::MemberReturn);
        CHECK_EQ(func3.Target(), &TestObject::MemberReturn);
        CHECK_EQ(func3.TargetObject(), &testObject);

        // Function call
        int result = Call(func3);
        CHECK(functionCalled);
        CHECK_EQ(result, VALUE1);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const member return") {
        // Constructors
        const wstl::Function<int(), TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func0(); (void) i; }, wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<int(), TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func1(); (void) i; }, wstl::BadFunctionCall);

        const wstl::Function<int(), TestObject> func2(&testObject, &TestObject::MemberReturn);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::MemberReturn);
        CHECK_EQ(func2.TargetObject(), &testObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<int(), TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::MemberReturn);
        CHECK_EQ(func3.TargetObject(), &testObject);

        #ifdef __WSTL_CXX11__
        const wstl::Function<int(), TestObject> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::MemberReturn);
        CHECK_EQ(func4.TargetObject(), &testObject);
        #endif

        // Function call
        int result = Call(func3);
        CHECK(functionCalled);
        CHECK_EQ(result, VALUE1);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function member const return") {
        // Constructors
        wstl::Function<int(), const TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func0(); (void) i; }, wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        wstl::Function<int(), const TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func1(); (void) i; }, wstl::BadFunctionCall);

        wstl::Function<int(), const TestObject> func2(&constTestObject, &TestObject::ConstMemberReturn);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMemberReturn);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        wstl::Function<int(), const TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::ConstMemberReturn);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        wstl::Function<int(), const TestObject> func4(wstl::Move(func2));
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::ConstMemberReturn);
        CHECK_EQ(func4.TargetObject(), &constTestObject);
        #endif
        
        // Assignment operators
        func1 = func3;
        CHECK(func3);
        CHECK(func1);
        CHECK_EQ(func1.Target(), &TestObject::ConstMemberReturn);
        CHECK_EQ(func1.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        func2 = wstl::Move(func4);
        CHECK_EQ(func4.Target(), wstl::NullPointer);
        CHECK_EQ(func4.TargetObject(), wstl::NullPointer);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMemberReturn);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        #endif

        func2 = wstl::NullPointer;
        CHECK_EQ(func2.Target(), wstl::NullPointer);
        CHECK_EQ(func2.TargetObject(), wstl::NullPointer);

        // Swap
        func2.Swap(func3);
        CHECK_EQ(func2.Target(), &TestObject::ConstMemberReturn);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_EQ(func3.Target(), wstl::NullPointer);
        CHECK_EQ(func3.Target(), wstl::NullPointer);

        // Reset
        func3.Reset(&constTestObject, &TestObject::ConstMemberReturn);
        CHECK_EQ(func3.Target(), &TestObject::ConstMemberReturn);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        // Function call
        int result = Call(func3);
        CHECK(functionCalled);
        CHECK_EQ(result, VALUE1);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Function const member const return") {
        // Constructors
        const wstl::Function<int(), const TestObject> func0(wstl::NullPointer);
        CHECK_FALSE(func0);
        CHECK_EQ(func0.Target(), wstl::NullPointer);
        CHECK_EQ(func0.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func0(); (void) i; }, wstl::BadFunctionCall);
        CHECK(func0 == wstl::NullPointer);
        CHECK_FALSE(func0 != wstl::NullPointer);

        const wstl::Function<int(), const TestObject> func1;
        CHECK_FALSE(func1);
        CHECK_EQ(func1.Target(), wstl::NullPointer);
        CHECK_EQ(func1.TargetObject(), wstl::NullPointer);
        CHECK_THROWS_AS({ int i = func1(); (void) i; }, wstl::BadFunctionCall);

        const wstl::Function<int(), const TestObject> func2(&constTestObject, &TestObject::ConstMemberReturn);
        CHECK(func2);
        CHECK_EQ(func2.Target(), &TestObject::ConstMemberReturn);
        CHECK_EQ(func2.TargetObject(), &constTestObject);
        CHECK_FALSE(func2 == wstl::NullPointer);
        CHECK(func2 != wstl::NullPointer);

        const wstl::Function<int(), const TestObject> func3(func2);
        CHECK(func2);
        CHECK(func3);
        CHECK_EQ(func3.Target(), &TestObject::ConstMemberReturn);
        CHECK_EQ(func3.TargetObject(), &constTestObject);

        #ifdef __WSTL_CXX11__
        const wstl::Function<int(), const TestObject> func4(wstl::Move(func2));
        CHECK(func2);
        CHECK(func4);
        CHECK_EQ(func4.Target(), &TestObject::ConstMemberReturn);
        CHECK_EQ(func4.TargetObject(), &constTestObject);
        #endif

        // Function call
        int result = Call(func3);
        CHECK(functionCalled);
        CHECK_EQ(result, VALUE1);
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("Invoke") {
        // Free function
        int (*fp)(int, int) = &FreeAdd;
        int (*const cfp)(int, int) = &FreeAdd;

        CHECK_EQ(wstl::Invoke(FreeAdd, 60, 7), 67);
        CHECK_EQ(wstl::Invoke(fp, 61, 8), 69);
        CHECK_EQ(wstl::Invoke(cfp, 59, 8), 67);

        // Lambda
        CHECK_EQ(wstl::Invoke([](int i) { return i * 2; }, 10), 20);

        // Functor
        Functor functor(100);
        const Functor cfunctor(128);

        CHECK_EQ(wstl::Invoke(functor, 100), 200);
        CHECK_EQ(wstl::Invoke(cfunctor, 20), 148);

        // Member object pointer and function pointer
        TestObject2 base(67);
        const TestObject2 cbase(228);

        CHECK_EQ(wstl::Invoke(&TestObject2::Value, base), 67);
        CHECK_EQ(wstl::Invoke(&TestObject2::Value, &base), 67);
        CHECK_EQ(wstl::Invoke(&TestObject2::Value, wstl::Reference(base)), 67);
        CHECK_EQ(wstl::Invoke(&TestObject2::Value, cbase), 228);
        CHECK_EQ(wstl::Invoke(&TestObject2::Value, &cbase), 228);
        CHECK_EQ(wstl::Invoke(&TestObject2::Value, wstl::Reference(cbase)), 228);

        CHECK_EQ(wstl::Invoke(&TestObject2::Sum, base, 3), 70);
        CHECK_EQ(wstl::Invoke(&TestObject2::Sum, &base, 4), 71);
        CHECK_EQ(wstl::Invoke(&TestObject2::Sum, wstl::Reference(base), 5), 72);
        CHECK_EQ(wstl::Invoke(&TestObject2::Sum, cbase, 2), 230);
        CHECK_EQ(wstl::Invoke(&TestObject2::Sum, &cbase, 3), 231);
        CHECK_EQ(wstl::Invoke(&TestObject2::Sum, wstl::Reference(cbase), 4), 232);
    }

    TEST_CASE("InvokeReturn") {
        // Free function
        int (*fp)(int, int) = &FreeAdd;
        int (*const cfp)(int, int) = &FreeAdd;

        CHECK_EQ(wstl::InvokeReturn<char>(FreeAdd, 60, 7), 'C');
        CHECK_EQ(wstl::InvokeReturn<float>(fp, 61, 8), 69.0F);
        CHECK_EQ(wstl::InvokeReturn<unsigned int>(cfp, 59, 8), 67U);

        functionCalled = false;
        wstl::InvokeReturn<void>(FreeReturn);
        CHECK(functionCalled);

        // Lambda
        CHECK_EQ(wstl::InvokeReturn<unsigned long>([](int i) { return i * 2; }, 10), 20UL);

        // Functor
        Functor functor(100);
        const Functor cfunctor(128);

        CHECK_EQ(wstl::InvokeReturn<float>(functor, 100), 200.0F);
        CHECK_EQ(wstl::InvokeReturn<float>(cfunctor, 20), 148.0F);

        // Member object pointer and function pointer
        TestObject2 base(67);
        const TestObject2 cbase(228);

        CHECK_EQ(wstl::InvokeReturn<double>(&TestObject2::Value, base), 67.0);
        CHECK_EQ(wstl::InvokeReturn<double>(&TestObject2::Value, &base), 67.0);
        CHECK_EQ(wstl::InvokeReturn<double>(&TestObject2::Value, wstl::Reference(base)), 67.0);
        CHECK_EQ(wstl::InvokeReturn<double>(&TestObject2::Value, cbase), 228.0);
        CHECK_EQ(wstl::InvokeReturn<double>(&TestObject2::Value, &cbase), 228.0);
        CHECK_EQ(wstl::InvokeReturn<double>(&TestObject2::Value, wstl::Reference(cbase)), 228.0);

        CHECK_EQ(wstl::InvokeReturn<long double>(&TestObject2::Sum, base, 3), 70.0L);
        CHECK_EQ(wstl::InvokeReturn<long double>(&TestObject2::Sum, &base, 4), 71.0L);
        CHECK_EQ(wstl::InvokeReturn<long double>(&TestObject2::Sum, wstl::Reference(base), 5), 72.0L);
        CHECK_EQ(wstl::InvokeReturn<long double>(&TestObject2::Sum, cbase, 2), 230.0L);
        CHECK_EQ(wstl::InvokeReturn<long double>(&TestObject2::Sum, &cbase, 3), 231.0L);
        CHECK_EQ(wstl::InvokeReturn<long double>(&TestObject2::Sum, wstl::Reference(cbase), 4), 232.0L);
    }
    #endif

    TEST_CASE("ReferenceWrapper") {
        int a = 0;
        wstl::ReferenceWrapper<int> ra(a);

        ra.Get() = 67;
        CHECK_EQ(a, 67);
        CHECK_EQ(ra, 67);

        a = 69;
        CHECK_EQ(a, 69);
        CHECK_EQ(ra, 69);

        wstl::ReferenceWrapper<int> rb(ra);

        rb.Get() = 128;
        CHECK_EQ(a, 128);
        CHECK_EQ(ra, 128);
        CHECK_EQ(rb, 128);

        ra.Get() = 67;
        CHECK_EQ(a, 67);
        CHECK_EQ(ra, 67);
        CHECK_EQ(rb, 67);

        a = 69;
        CHECK_EQ(a, 69);
        CHECK_EQ(ra, 69);
        CHECK_EQ(rb, 69);

        int b = 666;
        wstl::ReferenceWrapper<int> rc(b);

        rc = ra;
        CHECK_EQ(rc, 69);

        // Container
        std::array<int, 5> arr = {1, 2, 3, 4, 5};
        std::vector<wstl::ReferenceWrapper<int> > arrRef(arr.begin(), arr.end());

        CHECK(std::equal(arr.begin(), arr.end(), arrRef.begin()));

        // Callables
        Functor f(1);
        wstl::ReferenceWrapper<Functor> rf(f);

        CHECK_EQ(rf(66), 67);

        int (*fptr)() = &FreeReturn;
        wstl::ReferenceWrapper<int (*)()> rfptr(fptr);

        CHECK_EQ(rfptr(), VALUE1);
    }

    TEST_CASE("Reference") {
        int a = 0;
        wstl::ReferenceWrapper<int> ra = wstl::Reference(a);

        ra.Get() = 67;
        CHECK_EQ(a, 67);
        CHECK_EQ(ra, 67);

        a = 69;
        CHECK_EQ(a, 69);
        CHECK_EQ(ra, 69);

        wstl::ReferenceWrapper<int> rb = wstl::Reference(ra);

        rb.Get() = 128;
        CHECK_EQ(a, 128);
        CHECK_EQ(ra, 128);
        CHECK_EQ(rb, 128);

        ra.Get() = 67;
        CHECK_EQ(a, 67);
        CHECK_EQ(ra, 67);
        CHECK_EQ(rb, 67);

        a = 69;
        CHECK_EQ(a, 69);
        CHECK_EQ(ra, 69);
        CHECK_EQ(rb, 69);
    }

    TEST_CASE("ConstReference") {
        int a = 0;
        wstl::ReferenceWrapper<const int> ra = wstl::ConstReference(a);

        a = 69;
        CHECK_EQ(a, 69);
        CHECK_EQ(ra, 69);

        wstl::ReferenceWrapper<const int> rb = wstl::ConstReference(ra);

        a = 67;
        CHECK_EQ(a, 67);
        CHECK_EQ(ra, 67);
        CHECK_EQ(rb, 67);
    }

    TEST_CASE("UnwrapReference ReferenceWrapper specialization") {
        CHECK((wstl::IsSame<wstl::UnwrapReference<wstl::ReferenceWrapper<int> >::Type, int&>::Value));
        CHECK((wstl::IsSame<wstl::UnwrapReference<wstl::ReferenceWrapper<const int> >::Type, const int&>::Value));
    }

    TEST_CASE("HasIsTransparent") {
        CHECK(wstl::HasIsTransparent<HeterogeneousFunctor>::Value);
        CHECK_FALSE(wstl::HasIsTransparent<Functor>::Value);
    }

    TEST_CASE("Plus") {
        wstl::Plus<int> function;
        CHECK_EQ(function(3, 4), 3 + 4);

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::Plus<void>();
        CHECK(wstl::HasIsTransparent<wstl::Plus<void>>::Value);

        CHECK_EQ(voidFunction((int) 67, (long) 69), 67 + 69);
        #endif

        // compile-time version
        CHECK_EQ((wstl::compile::Plus<int>::Apply<3, 4>::Value), 3 + 4);
    }

    TEST_CASE("Minus") {
        wstl::Minus<int> function;
        CHECK_EQ(function(3, 4), 3 - 4);

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::Minus<void>();
        CHECK(wstl::HasIsTransparent<wstl::Minus<void>>::Value);

        CHECK_EQ(voidFunction((int) 67, (long) 69), 67 - 69);
        #endif

        // compile-time version
        CHECK_EQ((wstl::compile::Minus<int>::Apply<3, 4>::Value), 3 - 4);
    }

    TEST_CASE("Multiplies") {
        wstl::Multiplies<int> function;
        CHECK_EQ(function(3, 4), 3 * 4);

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::Multiplies<void>();
        CHECK(wstl::HasIsTransparent<wstl::Multiplies<void>>::Value);

        CHECK_EQ(voidFunction((int) 67, (long) 69), 67 * 69);
        #endif

        // compile-time version
        CHECK_EQ((wstl::compile::Multiplies<int>::Apply<3, 4>::Value), 3 * 4);
    }

    TEST_CASE("Divides") {
        wstl::Divides<int> function;
        CHECK_EQ(function(4, 4), 4 / 4);

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::Divides<void>();
        CHECK(wstl::HasIsTransparent<wstl::Divides<void>>::Value);

        CHECK_EQ(voidFunction((int) 100, (long) 2), 100 / 2);
        #endif

        // compile-time version
        CHECK_EQ((wstl::compile::Divides<int>::Apply<12, 4>::Value), 12 / 4);
    }

    TEST_CASE("Modulus") {
        wstl::Modulus<int> function;
        CHECK_EQ(function(5, 4), 5 % 4);

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::Modulus<void>();
        CHECK(wstl::HasIsTransparent<wstl::Modulus<void>>::Value);

        CHECK_EQ(voidFunction((int) 67, (long) 69), 67 % 69);
        #endif

        // compile-time version
        CHECK_EQ((wstl::compile::Modulus<int>::Apply<4, 4>::Value), 4 % 4);
    }

    TEST_CASE("Negate") {
        wstl::Negate<int> function;
        CHECK_EQ(function(-2), 2);
        CHECK_EQ(function(2), -2);

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::Negate<void>();
        CHECK(wstl::HasIsTransparent<wstl::Negate<void>>::Value);

        CHECK_EQ(voidFunction((long) 69), -69);
        CHECK_EQ(voidFunction((long) -69), 69);
        #endif

        // compile-time version
        CHECK_EQ(wstl::compile::Negate<int>::Apply<4>::Value, -4);
        CHECK_EQ(wstl::compile::Negate<int>::Apply<-4>::Value, 4);
    }

    TEST_CASE("EqualTo") {
        wstl::EqualTo<int> function;
        CHECK(function(1, 1));
        CHECK_FALSE(function(1, 2));
        CHECK_FALSE(function(2, 1));

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::EqualTo<void>();
        CHECK(wstl::HasIsTransparent<wstl::EqualTo<void>>::Value);

        std::function<void()> f1;
        CHECK(voidFunction(f1, nullptr));
        CHECK(voidFunction(nullptr, f1));

        std::function<void()> f2(FreeVoid);
        CHECK_FALSE(voidFunction(f2, nullptr));
        CHECK_FALSE(voidFunction(nullptr, f2));

        CHECK(voidFunction((int) 67, (long) 67));
        CHECK_FALSE(voidFunction((int) 69, (long) 67));
        CHECK_FALSE(voidFunction((int) 67, (long) 69));
        #endif

        // compile-time version
        CHECK((wstl::compile::EqualTo<int>::Apply<4, 4>::Value));
        CHECK_FALSE((wstl::compile::EqualTo<int>::Apply<3, 4>::Value));
        CHECK_FALSE((wstl::compile::EqualTo<int>::Apply<4, 3>::Value));
    }

    TEST_CASE("NotEqualTo") {
        wstl::NotEqualTo<int> function;
        CHECK_FALSE(function(1, 1));
        CHECK(function(1, 2));
        CHECK(function(2, 1));

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::NotEqualTo<void>();
        CHECK(wstl::HasIsTransparent<wstl::NotEqualTo<void>>::Value);

        std::function<void()> f1;
        CHECK_FALSE(voidFunction(f1, nullptr));
        CHECK_FALSE(voidFunction(nullptr, f1));

        std::function<void()> f2(FreeVoid);
        CHECK(voidFunction(f2, nullptr));
        CHECK(voidFunction(nullptr, f2));

        CHECK_FALSE(voidFunction((int) 67, (long) 67));
        CHECK(voidFunction((int) 69, (long) 67));
        CHECK(voidFunction((int) 67, (long) 69));
        #endif

        // compile-time version
        CHECK_FALSE((wstl::compile::NotEqualTo<int>::Apply<4, 4>::Value));
        CHECK((wstl::compile::NotEqualTo<int>::Apply<3, 4>::Value));
        CHECK((wstl::compile::NotEqualTo<int>::Apply<4, 3>::Value));
    }

    TEST_CASE("Greater") {
        wstl::Greater<int> function;
        CHECK_FALSE(function(1, 1));
        CHECK_FALSE(function(1, 2));
        CHECK(function(2, 1));

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::Greater<void>();
        CHECK(wstl::HasIsTransparent<wstl::Greater<void>>::Value);

        CHECK_FALSE(voidFunction((int) 67, (long) 67));
        CHECK(voidFunction((int) 69, (long) 67));
        CHECK_FALSE(voidFunction((int) 67, (long) 69));
        #endif

        // compile-time version
        CHECK_FALSE((wstl::compile::Greater<int>::Apply<4, 4>::Value));
        CHECK_FALSE((wstl::compile::Greater<int>::Apply<3, 4>::Value));
        CHECK((wstl::compile::Greater<int>::Apply<4, 3>::Value));
    }

    TEST_CASE("Less") {
        wstl::Less<int> function;
        CHECK_FALSE(function(1, 1));
        CHECK(function(1, 2));
        CHECK_FALSE(function(2, 1));

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::Less<void>();
        CHECK(wstl::HasIsTransparent<wstl::Less<void>>::Value);

        CHECK_FALSE(voidFunction((int) 67, (long) 67));
        CHECK_FALSE(voidFunction((int) 69, (long) 67));
        CHECK(voidFunction((int) 67, (long) 69));
        #endif

        // compile-time version
        CHECK_FALSE((wstl::compile::Less<int>::Apply<4, 4>::Value));
        CHECK((wstl::compile::Less<int>::Apply<3, 4>::Value));
        CHECK_FALSE((wstl::compile::Less<int>::Apply<4, 3>::Value));
    }

    TEST_CASE("GreaterEqual") {
        wstl::GreaterEqual<int> function;
        CHECK(function(1, 1));
        CHECK_FALSE(function(1, 2));
        CHECK(function(2, 1));

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::GreaterEqual<void>();
        CHECK(wstl::HasIsTransparent<wstl::GreaterEqual<void>>::Value);

        CHECK(voidFunction((int) 67, (long) 67));
        CHECK(voidFunction((int) 69, (long) 67));
        CHECK_FALSE(voidFunction((int) 67, (long) 69));
        #endif

        // compile-time version
        CHECK((wstl::compile::GreaterEqual<int>::Apply<4, 4>::Value));
        CHECK_FALSE((wstl::compile::GreaterEqual<int>::Apply<3, 4>::Value));
        CHECK((wstl::compile::GreaterEqual<int>::Apply<4, 3>::Value));
    }

    TEST_CASE("LessEqual") {
        wstl::LessEqual<int> function;
        CHECK(function(1, 1));
        CHECK(function(1, 2));
        CHECK_FALSE(function(2, 1));

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::LessEqual<void>();
        CHECK(wstl::HasIsTransparent<wstl::LessEqual<void>>::Value);

        CHECK(voidFunction((int) 67, (long) 67));
        CHECK_FALSE(voidFunction((int) 69, (long) 67));
        CHECK(voidFunction((int) 67, (long) 69));
        #endif

        // compile-time version
        CHECK((wstl::compile::LessEqual<int>::Apply<4, 4>::Value));
        CHECK((wstl::compile::LessEqual<int>::Apply<3, 4>::Value));
        CHECK_FALSE((wstl::compile::LessEqual<int>::Apply<4, 3>::Value));
    }

    TEST_CASE("LogicalAnd") {
        wstl::LogicalAnd<bool> function;
        CHECK(function(true, true));
        CHECK_FALSE(function(false, true));
        CHECK_FALSE(function(true, false));
        CHECK_FALSE(function(false, false));

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::LogicalAnd<void>();
        CHECK(wstl::HasIsTransparent<wstl::LogicalAnd<void>>::Value);

        CHECK(voidFunction(true, true));
        CHECK_FALSE(voidFunction(false, true));
        CHECK_FALSE(voidFunction(true, false));
        CHECK_FALSE(voidFunction(false, false));
        #endif

        // compile-time version
        CHECK((wstl::compile::LogicalAnd<bool>::Apply<true, true>::Value));
        CHECK_FALSE((wstl::compile::LogicalAnd<bool>::Apply<false, true>::Value));
        CHECK_FALSE((wstl::compile::LogicalAnd<bool>::Apply<true, false>::Value));
        CHECK_FALSE((wstl::compile::LogicalAnd<bool>::Apply<false, false>::Value));
    }

    TEST_CASE("LogicalOr") {
        wstl::LogicalOr<bool> function;
        CHECK(function(true, true));
        CHECK(function(false, true));
        CHECK(function(true, false));
        CHECK_FALSE(function(false, false));

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::LogicalOr<void>();
        CHECK(wstl::HasIsTransparent<wstl::LogicalOr<void>>::Value);

        CHECK(voidFunction(true, true));
        CHECK(voidFunction(false, true));
        CHECK(voidFunction(true, false));
        CHECK_FALSE(voidFunction(false, false));
        #endif

        // compile-time version
        CHECK((wstl::compile::LogicalOr<bool>::Apply<true, true>::Value));
        CHECK((wstl::compile::LogicalOr<bool>::Apply<false, true>::Value));
        CHECK((wstl::compile::LogicalOr<bool>::Apply<true, false>::Value));
        CHECK_FALSE((wstl::compile::LogicalOr<bool>::Apply<false, false>::Value));
    }

    TEST_CASE("LogicalNot") {
        wstl::LogicalNot<bool> function;
        CHECK_FALSE(function(true));
        CHECK(function(false));

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::LogicalNot<void>();
        CHECK(wstl::HasIsTransparent<wstl::LogicalNot<void>>::Value);

        CHECK_FALSE(voidFunction(true));
        CHECK(voidFunction(false));
        #endif

        // compile-time version
        CHECK_FALSE(wstl::compile::LogicalNot<bool>::Apply<true>::Value);
        CHECK(wstl::compile::LogicalNot<bool>::Apply<false>::Value);
    }

    TEST_CASE("BitwiseAnd") {
        wstl::BitwiseAnd<int> function;
        CHECK_EQ(function(3, 5), 3 & 5);

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::BitwiseAnd<void>();
        CHECK(wstl::HasIsTransparent<wstl::BitwiseAnd<void>>::Value);

        CHECK_EQ(voidFunction((int) 67, (long) 69), 67 & 69);
        #endif

        // compile-time version
        CHECK_EQ((wstl::compile::BitwiseAnd<int>::Apply<3, 5>::Value), 3 & 5);
    }

    TEST_CASE("BitwiseOr") {
        wstl::BitwiseOr<int> function;
        CHECK_EQ(function(3, 5), 3 | 5);

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::BitwiseOr<void>();
        CHECK(wstl::HasIsTransparent<wstl::BitwiseOr<void>>::Value);

        CHECK_EQ(voidFunction((int) 67, (long) 69), 67 | 69);
        #endif

        // compile-time version
        CHECK_EQ((wstl::compile::BitwiseOr<int>::Apply<3, 5>::Value), 3 | 5);
    }

    TEST_CASE("BitwiseXor") {
        wstl::BitwiseXor<int> function;
        CHECK_EQ(function(3, 5), 3 ^ 5);

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::BitwiseXor<void>();
        CHECK(wstl::HasIsTransparent<wstl::BitwiseXor<void>>::Value);

        CHECK_EQ(voidFunction((int) 67, (long) 69), 67 ^ 69);
        #endif

        // compile-time version
        CHECK_EQ((wstl::compile::BitwiseXor<int>::Apply<3, 5>::Value), 3 ^ 5);
    }

    TEST_CASE("BitwiseNot") {
        wstl::BitwiseNot<int> function;
        CHECK_EQ(function(3), ~3);

        // void specialization
        #ifdef __WSTL_CXX11__
        auto voidFunction = wstl::BitwiseNot<void>();
        CHECK(wstl::HasIsTransparent<wstl::BitwiseNot<void>>::Value);

        CHECK_EQ(voidFunction((char) 67), (char) ~67);
        #endif

        // compile-time version
        CHECK_EQ(wstl::compile::BitwiseNot<int>::Apply<3>::Value, ~3);
    }

    TEST_CASE("Identity") {
        int i = 67;

        wstl::Identity function;
        CHECK(wstl::HasIsTransparent<wstl::Identity>::Value);
        CHECK_EQ(function(3), 3);

        int& ref = function(i);
        CHECK_EQ(ref, 67);

        ref = 69;
        CHECK_EQ(i, 69);
    }

    TEST_CASE("NotFunction") {
        #ifdef __WSTL_CXX11__
        auto f0 = wstl::NotFunction(FreeBool0);

        functionCalled = false;
        CHECK(f0());

        functionCalled = true;
        CHECK_FALSE(f0());

        auto f1 = wstl::NotFunction(FreeBool);

        CHECK(f1(228));
        CHECK_FALSE(f1(VALUE1));

        #ifdef __WSTL_CXX17__
        CHECK(noexcept(f1(228)));
        #endif

        auto f2 = wstl::NotFunction(FreeBool2);

        CHECK(f2(0, 1));
        CHECK_FALSE(f2(1, 1));

        auto mf1 = wstl::NotFunction(&TestObject::MemberBool);

        CHECK(mf1(testObject, 128));
        CHECK_FALSE(mf1(testObject, VALUE1));
        CHECK(mf1(&testObject, 128));
        CHECK_FALSE(mf1(&testObject, VALUE1));
        CHECK(mf1(wstl::Reference(testObject), 128));
        CHECK_FALSE(mf1(wstl::Reference(testObject), VALUE1));

        BoolMember mbt = {true};
        BoolMember mbf = {false};
        auto mf2 = wstl::NotFunction(&BoolMember::Value);

        CHECK(mf2(mbf));
        CHECK_FALSE(mf2(mbt));
        CHECK(mf2(&mbf));
        CHECK_FALSE(mf2(&mbt));
        CHECK(mf2(wstl::Reference(mbf)));
        CHECK_FALSE(mf2(wstl::Reference(mbt)));

        // Bound callable cvref test
        auto func5 = wstl::NotFunction(BoolCVRefCallable());
        const auto cfunc5 = wstl::NotFunction(BoolCVRefCallable());

        CHECK(func5(0));
        CHECK_FALSE(wstl::Move(func5)(0));
        CHECK(cfunc5(0));
        CHECK_FALSE(wstl::Move(cfunc5)(0));

        #else
        wstl::NotFunctionType<GetBoolFunctor> ff0 = wstl::NotFunction(GetBoolFunctor());

        BoolVariable = false;
        CHECK(ff0());

        BoolVariable = true;
        CHECK_FALSE(ff0());

        wstl::NotFunctionType<BoolFunctor1> ff1 = wstl::NotFunction(BoolFunctor1());

        FakeInteger vi = {.Value = 100};

        CHECK_FALSE(ff1(VALUE3));
        CHECK(ff1(vi));

        wstl::NotFunctionType<Functor2> ff2 = wstl::NotFunctionType<Functor2>(Functor2());

        CHECK_FALSE(ff2(VALUE1, VALUE2));
        CHECK(ff2(0, '\0'));

        wstl::NotFunctionType<decltype(&FreeBool0)> f0(&FreeBool0);

        functionCalled = false;
        CHECK(f0());

        functionCalled = true;
        CHECK_FALSE(f0());

        wstl::NotFunctionType<decltype(&FreeBool)> f1(&FreeBool);

        CHECK(f1(128));
        CHECK_FALSE(f1(VALUE1));

        wstl::NotFunctionType<decltype(&FreeBool2)> f2 = wstl::NotFunction(FreeBool2);

        CHECK(f2(10, 'c'));
        CHECK_FALSE(f2(10, '\n'));

        // Invalid function without required typedefs, 
        // the following lines should fail with a compilation error
        // wstl::NotFunctionType<InvalidFunctor> err(InvalidFunctor());
        // err(VALUE1);
        #endif
    }

    #ifdef __WSTL_CXX11__
    TEST_CASE("IsPlaceholder") {
        CHECK_EQ(wstl::IsPlaceholder<decltype(wstl::placeholders::_1)>::Value, 1);
        CHECK_EQ(wstl::IsPlaceholder<decltype(wstl::placeholders::_2)>::Value, 2);
        CHECK_EQ(wstl::IsPlaceholder<decltype(wstl::placeholders::_3)>::Value, 3);
        CHECK_EQ(wstl::IsPlaceholder<decltype(wstl::placeholders::_4)>::Value, 4);
        CHECK_EQ(wstl::IsPlaceholder<decltype(wstl::placeholders::_5)>::Value, 5);
        CHECK_EQ(wstl::IsPlaceholder<decltype(wstl::placeholders::_6)>::Value, 6);
        CHECK_EQ(wstl::IsPlaceholder<decltype(wstl::placeholders::_7)>::Value, 7);
        CHECK_EQ(wstl::IsPlaceholder<decltype(wstl::placeholders::_8)>::Value, 8);
        CHECK_EQ(wstl::IsPlaceholder<decltype(wstl::placeholders::_9)>::Value, 9);

        CHECK_EQ(wstl::IsPlaceholder<wstl::RemoveConstType<decltype(wstl::placeholders::_1)>>::Value, 1);
        CHECK_EQ(wstl::IsPlaceholder<wstl::RemoveConstType<decltype(wstl::placeholders::_2)>>::Value, 2);
        CHECK_EQ(wstl::IsPlaceholder<wstl::RemoveConstType<decltype(wstl::placeholders::_3)>>::Value, 3);
        CHECK_EQ(wstl::IsPlaceholder<wstl::RemoveConstType<decltype(wstl::placeholders::_4)>>::Value, 4);
        CHECK_EQ(wstl::IsPlaceholder<wstl::RemoveConstType<decltype(wstl::placeholders::_5)>>::Value, 5);
        CHECK_EQ(wstl::IsPlaceholder<wstl::RemoveConstType<decltype(wstl::placeholders::_6)>>::Value, 6);
        CHECK_EQ(wstl::IsPlaceholder<wstl::RemoveConstType<decltype(wstl::placeholders::_7)>>::Value, 7);
        CHECK_EQ(wstl::IsPlaceholder<wstl::RemoveConstType<decltype(wstl::placeholders::_8)>>::Value, 8);
        CHECK_EQ(wstl::IsPlaceholder<wstl::RemoveConstType<decltype(wstl::placeholders::_9)>>::Value, 9);

        CHECK_EQ(wstl::IsPlaceholder<volatile wstl::RemoveConstType<decltype(wstl::placeholders::_1)>>::Value, 1);
        CHECK_EQ(wstl::IsPlaceholder<volatile wstl::RemoveConstType<decltype(wstl::placeholders::_2)>>::Value, 2);
        CHECK_EQ(wstl::IsPlaceholder<volatile wstl::RemoveConstType<decltype(wstl::placeholders::_3)>>::Value, 3);
        CHECK_EQ(wstl::IsPlaceholder<volatile wstl::RemoveConstType<decltype(wstl::placeholders::_4)>>::Value, 4);
        CHECK_EQ(wstl::IsPlaceholder<volatile wstl::RemoveConstType<decltype(wstl::placeholders::_5)>>::Value, 5);
        CHECK_EQ(wstl::IsPlaceholder<volatile wstl::RemoveConstType<decltype(wstl::placeholders::_6)>>::Value, 6);
        CHECK_EQ(wstl::IsPlaceholder<volatile wstl::RemoveConstType<decltype(wstl::placeholders::_7)>>::Value, 7);
        CHECK_EQ(wstl::IsPlaceholder<volatile wstl::RemoveConstType<decltype(wstl::placeholders::_8)>>::Value, 8);
        CHECK_EQ(wstl::IsPlaceholder<volatile wstl::RemoveConstType<decltype(wstl::placeholders::_9)>>::Value, 9);

        CHECK_EQ(wstl::IsPlaceholder<volatile decltype(wstl::placeholders::_1)>::Value, 1);
        CHECK_EQ(wstl::IsPlaceholder<volatile decltype(wstl::placeholders::_2)>::Value, 2);
        CHECK_EQ(wstl::IsPlaceholder<volatile decltype(wstl::placeholders::_3)>::Value, 3);
        CHECK_EQ(wstl::IsPlaceholder<volatile decltype(wstl::placeholders::_4)>::Value, 4);
        CHECK_EQ(wstl::IsPlaceholder<volatile decltype(wstl::placeholders::_5)>::Value, 5);
        CHECK_EQ(wstl::IsPlaceholder<volatile decltype(wstl::placeholders::_6)>::Value, 6);
        CHECK_EQ(wstl::IsPlaceholder<volatile decltype(wstl::placeholders::_7)>::Value, 7);
        CHECK_EQ(wstl::IsPlaceholder<volatile decltype(wstl::placeholders::_8)>::Value, 8);
        CHECK_EQ(wstl::IsPlaceholder<volatile decltype(wstl::placeholders::_9)>::Value, 9);

        CHECK_EQ(wstl::IsPlaceholder<int>::Value, 0);
        CHECK_EQ(wstl::IsPlaceholder<bool>::Value, 0);
    }
    
    TEST_CASE("IsBindExpression") {
        auto func1 = wstl::Bind(Free3, VALUE1, wstl::placeholders::_1, wstl::placeholders::_2);
        auto func2 = wstl::Bind<long>(FreeAdd, VALUE1, wstl::placeholders::_1);
        auto func3 = wstl::BindFront(Free3, VALUE1);

        CHECK(wstl::IsBindExpression<decltype(func1)>::Value);
        CHECK(wstl::IsBindExpression<decltype(func2)>::Value);
        CHECK_FALSE(wstl::IsBindExpression<decltype(func3)>::Value);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Bind normal parameters") {
        auto func1 = wstl::Bind(Free3, VALUE1, VALUE2, VALUE3);
        auto func2 = wstl::Bind(FreeBool, VALUE1);
        auto func3 = wstl::Bind<long>(FreeAdd, 62, 5);
        auto func4 = wstl::Bind<void>(FreeBool, VALUE1);

        func1();
        
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
        CHECK(wstl::IsSame<decltype(func1()), void>::Value);

        CHECK(func2());
        CHECK(wstl::IsSame<decltype(func2()), bool>::Value);

        CHECK_EQ(func3(), 67L);
        CHECK(wstl::IsSame<decltype(func3()), long>::Value);

        functionCalled = false;
        func4();

        CHECK(functionCalled);
        CHECK(wstl::IsSame<decltype(func4()), void>::Value);

        #ifdef __WSTL_CXX17__
        CHECK(noexcept(func2()));
        CHECK(noexcept(func4()));
        #endif

        // Bound argument cvref test
        auto func5 = wstl::Bind(CVRefCallable(), VALUE1);
        const auto cfunc5 = wstl::Bind(CVRefCallable(), VALUE1);
        auto func6 = wstl::Bind<long>(CVRefCallable(), VALUE1);
        const auto cfunc6 = wstl::Bind<long>(CVRefCallable(), VALUE1);

        CHECK_EQ(func5(), 1);
        CHECK_EQ(wstl::Move(func5)(), 3);
        CHECK_EQ(cfunc5(), 2);
        CHECK_EQ(wstl::Move(cfunc5)(), 4);
        CHECK_EQ(func6(), 1L);
        CHECK_EQ(wstl::Move(func6)(), 3L);
        CHECK_EQ(cfunc6(), 2L);
        CHECK_EQ(wstl::Move(cfunc6)(), 4L);
    }

    TEST_CASE_FIXTURE(SetupFixture, "Bind placeholders") {
        using namespace wstl::placeholders;

        auto func1 = wstl::Bind(Free3, _1, VALUE2, _2);
        auto func2 = wstl::Bind(FreeBool, _1);
        auto func3 = wstl::Bind<long>(FreeAdd, _1, 1);
        auto func4 = wstl::Bind<void>(FreeBool, _1);

        func1(VALUE1, VALUE3);
        
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
        CHECK(wstl::IsSame<decltype(func1(VALUE1, VALUE3)), void>::Value);

        CHECK(func2(VALUE1));
        CHECK_FALSE(func2(128));
        CHECK(wstl::IsSame<decltype(func2(1)), bool>::Value);

        CHECK_EQ(func3(VALUE1 - 1), VALUE1);
        CHECK(wstl::IsSame<decltype(func3(1)), long>::Value);

        functionCalled = false;
        paramsCorrect[0] = false;

        func4(VALUE1);

        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(wstl::IsSame<decltype(func4(1)), void>::Value);

        #ifdef __WSTL_CXX17__
        CHECK(noexcept(func2(1)));
        CHECK(noexcept(func4(1)));
        #endif
    }

    TEST_CASE_FIXTURE(SetupFixture, "Bind reference wrapper") {
        auto func1 = wstl::Bind(Free3, wstl::Reference(VALUE1), VALUE2, wstl::placeholders::_1);
        auto func2 = wstl::Bind(FreeBool, wstl::Reference(VALUE1));
        auto func3 = wstl::Bind<long>(FreeAdd, wstl::Reference(VALUE1), wstl::placeholders::_1);
        auto func4 = wstl::Bind<void>(FreeBool, wstl::Reference(VALUE1));

        func1(VALUE3);
        
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
        CHECK(wstl::IsSame<decltype(func1(VALUE3)), void>::Value);

        CHECK(func2());
        CHECK(wstl::IsSame<decltype(func2()), bool>::Value);

        CHECK_EQ(func3(1), 68L);
        CHECK(wstl::IsSame<decltype(func3(1)), long>::Value);

        functionCalled = false;
        func4();

        CHECK(functionCalled);
        CHECK(wstl::IsSame<decltype(func4()), void>::Value);

        #ifdef __WSTL_CXX17__
        CHECK(noexcept(func2()));
        CHECK(noexcept(func4()));
        #endif
    }

    TEST_CASE_FIXTURE(SetupFixture, "Bind nested") {
        using namespace wstl::placeholders;

        auto func1 = wstl::Bind(Free3, wstl::Reference(VALUE1), wstl::Bind(FreeAdd, _1, 1), _2);
        auto func2 = wstl::Bind(FreeBool, wstl::Bind(FreeAdd, _1, 1));
        auto func3 = wstl::Bind<long>(Free3Add, wstl::Reference(VALUE1), wstl::Bind(FreeAdd, _1, 1), _2);
        auto func4 = wstl::Bind<void>(FreeBool, wstl::Bind(FreeAdd, _1, -1));

        func1(VALUE2 - 1, VALUE3);
        
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
        CHECK(wstl::IsSame<decltype(func1('\0', FakeInteger{0})), void>::Value);

        CHECK(func2(VALUE1 - 1));
        CHECK_FALSE(func2(1));
        CHECK(wstl::IsSame<decltype(func2(1)), bool>::Value);

        CHECK_EQ(func3(VALUE2 - 1, VALUE3), 256L);
        CHECK(wstl::IsSame<decltype(func3('\0', FakeInteger{0})), long>::Value);

        functionCalled = false;
        paramsCorrect[0] = false;

        func4(VALUE1 + 1);

        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(wstl::IsSame<decltype(func4(1)), void>::Value);

        #ifdef __WSTL_CXX17__
        CHECK_FALSE(noexcept(func2(1)));
        CHECK_FALSE(noexcept(func4(1)));
        #endif
    }

    TEST_CASE_FIXTURE(SetupFixture, "BindFront") {
        auto func1 = wstl::BindFront(Free3, VALUE1, VALUE2);
        auto func2 = wstl::BindFront(FreeBool, VALUE1);

        func1(VALUE3);
        
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
        CHECK(wstl::IsSame<decltype(func1(VALUE3)), void>::Value);

        CHECK(func2());
        CHECK(wstl::IsSame<decltype(func2()), bool>::Value);

        // Bound argument cvref test
        auto func5 = wstl::BindFront(CVRefCallable(), VALUE1);
        const auto cfunc5 = wstl::BindFront(CVRefCallable(), VALUE1);

        CHECK_EQ(func5(), 1);
        CHECK_EQ(wstl::Move(func5)(), 3);
        CHECK_EQ(cfunc5(), 2);
        CHECK_EQ(wstl::Move(cfunc5)(), 4);

        #ifdef __WSTL_CXX17__
        auto func3 = wstl::BindFront<Free3Add>(1, 'c');
        auto func4 = wstl::BindFront<FreeBool2>(1);

        CHECK_EQ(func3(FakeInteger{28}), 128);
        CHECK(wstl::IsSame<decltype(func3(FakeInteger{0})), int>::Value);

        CHECK(func4(1));
        CHECK(wstl::IsSame<decltype(func4(0)), bool>::Value);

        CHECK(noexcept(func2()));
        CHECK_FALSE(noexcept(func4(1)));

        // Bound argument cvref test

        #ifdef __WSTL_CXX20__
        auto func6 = wstl::BindFront<CVRefCallable{}>(VALUE1);
        const auto cfunc6 = wstl::BindFront<CVRefCallable{}>(VALUE1);

        CHECK_EQ(func6(), 1L);
        CHECK_EQ(wstl::Move(func6)(), 3L);
        CHECK_EQ(cfunc6(), 2L);
        CHECK_EQ(wstl::Move(cfunc6)(), 4L);
        #endif
        #endif
    }

    TEST_CASE_FIXTURE(SetupFixture, "BindBack") {
        auto func1 = wstl::BindBack(Free3, VALUE2, VALUE3);
        auto func2 = wstl::BindBack(FreeBool, VALUE1);

        func1(VALUE1);
        
        CHECK(functionCalled);
        CHECK(paramsCorrect[0]);
        CHECK(paramsCorrect[1]);
        CHECK(paramsCorrect[2]);
        CHECK(wstl::IsSame<decltype(func1(VALUE1)), void>::Value);

        CHECK(func2());
        CHECK(wstl::IsSame<decltype(func2()), bool>::Value);

        // Bound argument cvref test
        auto func5 = wstl::BindBack(CVRefCallable(), VALUE1);
        const auto cfunc5 = wstl::BindBack(CVRefCallable(), VALUE1);

        CHECK_EQ(func5(), 1);
        CHECK_EQ(wstl::Move(func5)(), 3);
        CHECK_EQ(cfunc5(), 2);
        CHECK_EQ(wstl::Move(cfunc5)(), 4);

        #ifdef __WSTL_CXX17__
        auto func3 = wstl::BindBack<Free3Add>('c', FakeInteger{28});
        auto func4 = wstl::BindBack<FreeBool2>(1);

        CHECK_EQ(func3(1), 128);
        CHECK(wstl::IsSame<decltype(func3(0)), int>::Value);

        CHECK(func4(1));
        CHECK(wstl::IsSame<decltype(func4(0)), bool>::Value);

        CHECK(noexcept(func2()));
        CHECK_FALSE(noexcept(func4(1)));

        // Bound argument cvref test

        #ifdef __WSTL_CXX20__
        auto func6 = wstl::BindBack<CVRefCallable{}>(VALUE1);
        const auto cfunc6 = wstl::BindBack<CVRefCallable{}>(VALUE1);

        CHECK_EQ(func6(), 1L);
        CHECK_EQ(wstl::Move(func6)(), 3L);
        CHECK_EQ(cfunc6(), 2L);
        CHECK_EQ(wstl::Move(cfunc6)(), 4L);
        #endif
        #endif
    }
    #endif

    TEST_CASE("BinderFirst") {
        wstl::BinderFirst<decltype(&FreeAdd), int> b1(&FreeAdd, 67);

        CHECK_EQ(b1(3), 70);

        CHECK_FALSE(wstl::BindFirst(&FreeBool2, VALUE1)(VALUE2));

        wstl::BinderFirst<Functor2, int> b2 = wstl::BindFirst(Functor2(), VALUE1);

        CHECK(b2(VALUE2));
        CHECK_FALSE(b2('\0'));
    }

    TEST_CASE("BinderSecond") {
        wstl::BinderSecond<decltype(&FreeAdd), int> b1(&FreeAdd, 67);

        CHECK_EQ(b1(3), 70);

        CHECK_FALSE(wstl::BindSecond(&FreeBool2, VALUE2)(VALUE1));

        wstl::BinderSecond<Functor2, char> b2 = wstl::BindSecond(Functor2(), VALUE2);

        CHECK(b2(VALUE1));
        CHECK_FALSE(b2(0));
    }

    TEST_CASE("MemberFunction") {
        wstl::MemberFunctionType<decltype(&TestObject::ConstMemberReturn)> f0(&TestObject::ConstMemberReturn);

        CHECK_EQ(f0(testObject), VALUE1);
        CHECK_EQ(f0(&testObject), VALUE1);
        CHECK_EQ(f0(wstl::Reference(testObject)), VALUE1);
        CHECK_EQ(f0(constTestObject), VALUE1);
        CHECK_EQ(f0(&constTestObject), VALUE1);
        CHECK_EQ(f0(wstl::Reference(constTestObject)), VALUE1);

        wstl::MemberFunctionType<decltype(&TestObject::MemberBool)> f1(&TestObject::MemberBool);

        CHECK(f1(testObject, VALUE1));
        CHECK(f1(&testObject, VALUE1));
        CHECK(f1(wstl::Reference(testObject), VALUE1));

        // The following line should fail with a compilation error
        // CHECK(f1(constTestObject, VALUE1));

        wstl::MemberFunctionType<decltype(&Functor2::operator())> f2(&Functor2::operator());
        Functor2 func;
        const Functor2 cfunc;

        CHECK(f2(func, VALUE1, VALUE2));
        CHECK(f2(&func, VALUE1, VALUE2));
        CHECK(f2(wstl::Reference(func), VALUE1, VALUE2));
        CHECK(f2(cfunc, VALUE1, VALUE2));
        CHECK(f2(&cfunc, VALUE1, VALUE2));
        CHECK(f2(wstl::Reference(cfunc), VALUE1, VALUE2));

        #ifdef __WSTL_CXX11__
        wstl::MemberFunctionType<decltype(&TestObject::MemberBool3)> f3(&TestObject::MemberBool3);

        CHECK(f3(testObject, VALUE1, VALUE2, VALUE3));
        CHECK(f3(&testObject, VALUE1, VALUE2, VALUE3));
        CHECK(f3(wstl::Reference(testObject), VALUE1, VALUE2, VALUE3));
        CHECK(f3(constTestObject, VALUE1, VALUE2, VALUE3));
        CHECK(f3(&constTestObject, VALUE1, VALUE2, VALUE3));
        CHECK(f3(wstl::Reference(constTestObject), VALUE1, VALUE2, VALUE3));
        #endif

        wstl::MemberFunctionType<decltype(&Functor::Value)> m1(&Functor::Value);
        Functor func2(10);
        const Functor cfunc2(20);

        CHECK_EQ(m1(func2), 10);
        CHECK_EQ(m1(&func2), 10);
        CHECK_EQ(m1(wstl::Reference(func2)), 10);
        CHECK_EQ(m1(cfunc2), 20);
        CHECK_EQ(m1(&cfunc2), 20);
        CHECK_EQ(m1(wstl::Reference(cfunc2)), 20);
    }
}
