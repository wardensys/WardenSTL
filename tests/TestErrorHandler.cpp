// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/ErrorHandler.hpp>
#include <wstl/StandardExceptions.hpp>


namespace {
    static bool CallbackCalled = false;

    void TestErrorHandler(const wstl::Exception&) {
        CallbackCalled = true;
    }

    struct TestHandler : wstl::FunctionBase<void(const wstl::Exception&)> {
        virtual void operator()(const wstl::Exception&) const __WSTL_OVERRIDE__ {
            CallbackCalled = true;
        }
    };
}

TEST_SUITE("ErrorHandler") {
    TEST_CASE("SetCallback") {
        wstl::Function<void(const wstl::Exception&)> handler(&TestErrorHandler);
        wstl::ErrorHandler::SetCallback(handler);
        
        CallbackCalled = false;
        wstl::ErrorHandler::Error(WSTL_MAKE_EXCEPTION(wstl::LogicError));
        CHECK(CallbackCalled);
    }

    TEST_CASE("ClearCallback") {
        wstl::ErrorHandler::ClearCallback();
        
        CallbackCalled = false;
        wstl::ErrorHandler::Error(WSTL_MAKE_EXCEPTION(wstl::LogicError));
        CHECK_FALSE(CallbackCalled);
    }

    TEST_CASE("Error") {
        const TestHandler handler;
        wstl::ErrorHandler::SetCallback(handler);
        
        CallbackCalled = false;
        wstl::ErrorHandler::Error(WSTL_MAKE_EXCEPTION(wstl::LogicError));
        CHECK(CallbackCalled);

        wstl::ErrorHandler::ClearCallback();

        CallbackCalled = false;
        wstl::ErrorHandler::Error(WSTL_MAKE_EXCEPTION(wstl::LogicError));
        CHECK_FALSE(CallbackCalled);
    }
}
