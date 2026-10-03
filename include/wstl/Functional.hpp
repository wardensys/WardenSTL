// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_FUNCTIONAL_HPP__
#define __WSTL_FUNCTIONAL_HPP__

#include "private/Platform.hpp"
#include "private/ns/compile.hpp"
#include "private/Error.hpp"
#include "Tuple.hpp"
#include "FunctionTraits.hpp"


/// @defgroup functional Functional
/// @brief Functional programming utilities
/// @ingroup utilities

namespace wstl {
    // Function exceptions

    /// @brief Exception class for invalid function calls
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bad_function_call
    class BadFunctionCall : public Exception {
    public:
        #ifdef __WSTL_EXCEPTION_LOCATION__
        /// @brief Constructor
        /// @param file The name of the source file where the exception occurred
        /// @param line The line number in the source file where the exception occurred
        /// @param message The message describing the exception, default is `Bad function call`
        BadFunctionCall(StringType file, NumericType line, StringType message = "Bad function call") __WSTL_NOEXCEPT__ : Exception(file, line, message) {}
        #else
        /// @brief Constructor
        /// @param message The exception message, default is `Bad function call`
        BadFunctionCall(StringType message = "Bad function call") __WSTL_NOEXCEPT__ : Exception(message) {}
        #endif

        /// @copydoc Exception::Name()
        __WSTL_CONSTEXPR20__ virtual StringType Name() const __WSTL_NOEXCEPT__ __WSTL_OVERRIDE__ {
            return "BadFunctionCall";
        }
    };

    // Nullary function
    
    /// @brief Template for nullary function traits, functions that take no arguments
    /// @tparam Return Return type of the function
    /// @deprecated Use `wstl::Function` instead
    /// @ingroup functional
    template<typename Return>
    struct NullaryFunction {
        typedef Return ResultType;
    };

    // Unary function
    
    /// @brief Template for unary function traits, functions that take one argument
    /// @tparam Arg Type of the argument
    /// @tparam Return Return type of the function
    /// @deprecated Use `wstl::Function` instead
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/unary_function
    template<typename Arg, typename Return>
    struct UnaryFunction {
        typedef Return ResultType;
        typedef Arg ArgumentType;
    };

    // Binary function

    /// @brief Template for binary function traits, functions that take two arguments
    /// @tparam Arg1 Type of the first argument
    /// @tparam Arg2 Type of the second argument
    /// @tparam Return Return type of the function
    /// @deprecated Use `wstl::Function` instead
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/binary_function
    template<typename Arg1, typename Arg2, typename Return>
    struct BinaryFunction {
        typedef Return ResultType;
        typedef Arg1 FirstArgumentType;
        typedef Arg2 SecondArgumentType;
    };

    // Function base

    /// @brief Base template for encapsulating callable objects
    /// @tparam Signature The function signature (`Return(Args...)`)
    /// @note In C++98 it supports maximum two arguments
    /// @ingroup functional
    template<typename Signature>
    class FunctionBase;

    #ifdef __WSTL_CXX11__
    template<typename Return, typename... Args>
    class FunctionBase<Return(Args...)> {
    public:
        typedef Return ResultType;

        virtual ~FunctionBase() {}

        virtual Return operator()(Args...) const = 0;
    };
    #else
    template<typename Return, typename Arg1, typename Arg2>
    class FunctionBase<Return(Arg1, Arg2)> : BinaryFunction<Arg1, Arg2, Return> {
    public:
        virtual ~FunctionBase() {}

        virtual Return operator()(Arg1, Arg2) const = 0;
    };

    template<typename Return, typename Arg>
    class FunctionBase<Return(Arg)> : UnaryFunction<Arg, Return> {
    public:
        virtual ~FunctionBase() {}

        virtual Return operator()(Arg) const = 0;
    };

    template<typename Return>
    class FunctionBase<Return()> : NullaryFunction<Return> {
    public:
        virtual ~FunctionBase() {}

        virtual Return operator()() const = 0;
    };
    #endif

    // Function 

    /// @brief Generic function wrapper that can hold and invoke callable objects
    /// @tparam Signature The signature of the function or callable object (`int(int, double)`)
    /// @tparam Object (Optional) The type of object for member functions. Default is `void` for free functions.
    /// @note In C++98 it supports maximum two arguments
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/function
    template<typename Signature, typename Object = void>
    class Function;

    #ifdef __WSTL_CXX11__
    // Function (many parameters)
    
    template<typename Return, typename... Args>
    class Function<Return(Args...)> : public FunctionBase<Return(Args...)> {
    public:
        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ Function() __WSTL_NOEXCEPT__ : m_Function(nullptr) {}

        /// @brief Creates an empty `Function` object
        __WSTL_CONSTEXPR14__ Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(nullptr) {}

        /// @brief Parameterized constructor
        /// @param function Pointer to a free function
        __WSTL_CONSTEXPR14__ Function(Return(*function)(Args...)) : m_Function(function) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        __WSTL_CONSTEXPR14__ Function(const Function& other) : m_Function(other.m_Function) {}

        /// @brief Move constructor
        /// @param other Another `Function` object to move from
        __WSTL_CONSTEXPR14__ Function(Function&& other) __WSTL_NOEXCEPT__ : m_Function(other.m_Function) {
            other.m_Function = nullptr;
        }

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        __WSTL_CONSTEXPR14__ Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            return *this;
        }

        /// @brief Move assignment operator
        /// @param other Another `Function` object to move from
        __WSTL_CONSTEXPR14__ Function& operator=(Function&& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                other.m_Function = nullptr;
            }

            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        __WSTL_CONSTEXPR14__ Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = nullptr;
            return *this;
        }

        /// @brief Assignment operator for a free function pointer
        /// @param function Pointer to a free function to assign
        __WSTL_CONSTEXPR14__ Function& operator=(Return(*function)(Args...)) {
            m_Function = function;
            return *this;
        }

        /// @brief Resets the `Function` to hold a new free function pointer
        /// @param function Pointer to a free function to assign
        __WSTL_CONSTEXPR14__ void Reset(Return(*function)(Args...)) {
            m_Function = function;
        }

        /// @brief Swap the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        __WSTL_CONSTEXPR14__ void Swap(Function& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Function, other.m_Function);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        __WSTL_CONSTEXPR14__ operator bool() const __WSTL_NOEXCEPT__ {
            return m_Function != nullptr;
        }

        /// @brief Function call operator to invoke the stored callable
        /// @param ...args Arguments to pass to the callable
        /// @return The result of invoking the callable
        virtual Return operator()(Args... args) const override {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (*m_Function)(Forward<Args>(args)...);
        }

        /// @brief Gets the target function pointer
        __WSTL_CONSTEXPR14__ Return(*Target() const __WSTL_NOEXCEPT__)(Args...) {
            return m_Function;
        }

    private:
        Return (*m_Function)(Args...);
    };

    // Member

    template<typename Object, typename Return, typename... Args>
    class Function<Return(Args...), Object> : public FunctionBase<Return(Args...)> {
    public:
        typedef Object ObjectType;
        
        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ Function() __WSTL_NOEXCEPT__ : m_Function(nullptr), m_Object(nullptr) {}

        /// @brief Creates an empty `Function` object
        __WSTL_CONSTEXPR14__ Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(nullptr), m_Object(nullptr) {}

        /// @brief Parameterized constructor
        /// @param object Pointer to the object instance
        /// @param function Pointer to the member function of the object
        __WSTL_CONSTEXPR14__ Function(Object* object, Return(Object::*function)(Args...)) 
            : m_Function(function), m_Object(object) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        __WSTL_CONSTEXPR14__ Function(const Function& other) 
            : m_Function(other.m_Function), m_Object(other.m_Object) {}

        /// @brief Move constructor
        /// @param other Another `Function` object to move from
        __WSTL_CONSTEXPR14__ Function(Function&& other) __WSTL_NOEXCEPT__ : m_Function(other.m_Function), m_Object(other.m_Object) {
            other.m_Object = nullptr;
            other.m_Function = nullptr;
        }

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        __WSTL_CONSTEXPR14__ Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            m_Object = other.m_Object;
            return *this;
        }

        /// @brief Move assignment operator
        /// @param other Another `Function` object to move from
        __WSTL_CONSTEXPR14__ Function& operator=(Function&& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
                other.m_Object = nullptr;
                other.m_Function = nullptr;
            }

            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        __WSTL_CONSTEXPR14__ Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = nullptr;
            m_Object = nullptr;
            return *this;
        }

        /// @brief Resets the `Function` to hold a new member function pointer and an object
        /// @param object Pointer to the object instance
        /// @param function Pointer to the member function of the object
        __WSTL_CONSTEXPR14__ void Reset(Object* object, Return (Object::*function)(Args...)) {
            m_Object = object;
            m_Function = function;
        }

        /// @brief Swaps the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        __WSTL_CONSTEXPR14__ void Swap(Function& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        __WSTL_CONSTEXPR14__ operator bool() const __WSTL_NOEXCEPT__ {
            return (m_Function != nullptr) && (m_Object != nullptr);
        }

        /// @brief Function call operator to invoke the stored member function on the object
        /// @param ...args Arguments to pass to the member function
        /// @return The result of invoking the member function
        virtual Return operator()(Args... args) const override {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (m_Object->*m_Function)(Forward<Args>(args)...);
        }

        /// @brief Gets the target member function pointer
        __WSTL_CONSTEXPR14__ Return(Object::*Target() const __WSTL_NOEXCEPT__)(Args...) {
            return m_Function;
        }

        /// @brief Gets the target const object pointer
        __WSTL_CONSTEXPR14__ const Object* TargetObject() const __WSTL_NOEXCEPT__ {
            return m_Object;
        }

    private:
        Return (Object::*m_Function)(Args...);
        Object* m_Object;
    };

    // Const member

    template<typename Object, typename Return, typename... Args>
    class Function<Return(Args...), const Object> : public FunctionBase<Return(Args...)> {
    public:
        typedef const Object ObjectType;

        /// @brief Default constructor
        __WSTL_CONSTEXPR14__ Function() __WSTL_NOEXCEPT__ : m_Function(nullptr), m_Object(nullptr) {}

        /// @brief Creates an empty `Function` object
        __WSTL_CONSTEXPR14__ Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(nullptr), m_Object(nullptr) {}

        /// @brief Parameterized constructor
        /// @param object Pointer to the const object instance
        /// @param function Pointer to the const member function of the object
        __WSTL_CONSTEXPR14__ Function(const Object* object, Return(Object::*function)(Args...) const) 
            : m_Function(function), m_Object(object) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        __WSTL_CONSTEXPR14__ Function(const Function& other) 
            : m_Function(other.m_Function), m_Object(other.m_Object) {}

        /// @brief Move constructor
        /// @param other Another `Function` object to move from
        __WSTL_CONSTEXPR14__ Function(Function&& other) __WSTL_NOEXCEPT__ : m_Function(other.m_Function), m_Object(other.m_Object) {
            other.m_Object = nullptr;
            other.m_Function = nullptr;
        }

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        __WSTL_CONSTEXPR14__ Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            m_Object = other.m_Object;

            return *this;
        }

        /// @brief Move assignment operator
        /// @param other Another `Function` object to move from
        __WSTL_CONSTEXPR14__ Function& operator=(Function&& other) {
            if(this != &other) {
                m_Function = other.m_Function;
                m_Object = other.m_Object;
                other.m_Object = nullptr;
                other.m_Function = nullptr;
            }

            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        __WSTL_CONSTEXPR14__ Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = nullptr;
            m_Object = nullptr;
            return *this;
        }
        
        /// @brief Resets the `Function` to hold a new const member function pointer and an object
        /// @param object Pointer to the const object instance
        /// @param function Pointer to the const member function of the object
        __WSTL_CONSTEXPR14__ void Reset(const Object* object, Return (Object::*function)(Args...) const) {
            m_Object = object;
            m_Function = function;
        }

        /// @brief Swaps the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        __WSTL_CONSTEXPR14__ void Swap(Function& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        __WSTL_CONSTEXPR14__ operator bool() const __WSTL_NOEXCEPT__ {
            return (m_Function != nullptr) && (m_Object != nullptr);
        }

        /// @brief Function call operator to invoke the stored const member function on the object
        /// @param ...args Arguments to pass to the const member function
        /// @return The result of invoking the const member function
        virtual Return operator()(Args... args) const override {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (m_Object->*m_Function)(Forward<Args>(args)...);
        }

        /// @brief Gets the target const member function pointer
        __WSTL_CONSTEXPR14__ Return(Object::*Target() const __WSTL_NOEXCEPT__)(Args...) const {
            return m_Function;
        }

        /// @brief Gets the target const object pointer
        __WSTL_CONSTEXPR14__ const Object* TargetObject() const __WSTL_NOEXCEPT__ {
            return m_Object;
        }

    private:
        Return (Object::*m_Function)(Args...) const;
        const Object* m_Object;
    };

    // Comparison operators with NullPointerType (nullptr_t)

    template<typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator==(const Function<Return(Args...)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator==(NullPointerType, const Function<Return(Args...)>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator==(const Function<Return(Args...), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator==(NullPointerType, const Function<Return(Args...), Object>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator!=(const Function<Return(Args...)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == nullptr);
    }

    template<typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator!=(NullPointerType, const Function<Return(Args...)>& function) __WSTL_NOEXCEPT__ {
        return !(nullptr == function);
    }

    template<typename Object, typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator!=(const Function<Return(Args...), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == nullptr);
    }

    template<typename Object, typename Return, typename... Args>
    __WSTL_CONSTEXPR14__ 
    inline bool operator!=(NullPointerType, const Function<Return(Args...), Object>& function) __WSTL_NOEXCEPT__ {
        return !(nullptr == function);
    }

    #else
    template<typename Return, typename Arg1, typename Arg2>
    class Function<Return(Arg1, Arg2)> : public FunctionBase<Return(Arg1, Arg2)> {
    public:
        /// @brief Default constructor
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer) {}

        /// @brief Creates an empty `Function` object
        Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(NullPointer) {}

        /// @brief Parameterized constructor
        /// @param function Pointer to a free function
        Function(Return(*function)(Arg1, Arg2)) : m_Function(function) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        Function(const Function& other) : m_Function(other.m_Function) {}

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = NullPointer;
            return *this;
        }

        /// @brief Assignment operator for a free function pointer
        /// @param function Pointer to a free function to assign
        Function& operator=(Return(*function)(Arg1, Arg2)) {
            m_Function = function;
            return *this;
        }

        /// @brief Resets the `Function` to hold a new free function pointer
        /// @param function Pointer to a free function to assign
        void Reset(Return(*function)(Arg1, Arg2)) {
            m_Function = function;
        }

        /// @brief Swaps the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        void Swap(Function& other) {
            wstl::Swap(m_Function, other.m_Function);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        operator bool() const __WSTL_NOEXCEPT__ {
            return m_Function != NullPointer;
        }

        /// @brief Function call operator to invoke the stored callable
        /// @param arg1 First argument to pass to the callable
        /// @param arg2 Second argument to pass to the callable
        /// @return The result of invoking the callable
        virtual Return operator()(Arg1 arg1, Arg2 arg2) const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (*m_Function)(arg1, arg2);
        }

        /// @brief Gets the target function pointer
        Return(*Target() const __WSTL_NOEXCEPT__)(Arg1, Arg2) {
            return m_Function;
        }

    private:
        Return (*m_Function)(Arg1, Arg2);
    };

    // Member 

    template<typename Object, typename Return, typename Arg1, typename Arg2>
    class Function<Return(Arg1, Arg2), Object> : public FunctionBase<Return(Arg1, Arg2)> {
    public:
        typedef Object ObjectType;

        /// @brief Default constructor
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Creates an empty `Function` object
        Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Parameterized constructor
        /// @param object Pointer to the object instance
        /// @param function Pointer to the member function of the object
        Function(Object* object, Return(Object::*function)(Arg1, Arg2)) 
            : m_Function(function), m_Object(object) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            m_Object = other.m_Object;
            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = NullPointer;
            m_Object = NullPointer;
            return *this;
        }

        /// @brief Resets the `Function` to hold a new member function pointer and an object
        /// @param object Pointer to the object instance
        /// @param function Pointer to the member function of the object
        void Reset(Object* object, Return (Object::*function)(Arg1, Arg2)) {
            m_Object = object;
            m_Function = function;
        }

        /// @brief Swaps the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        void Swap(Function& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        operator bool() const __WSTL_NOEXCEPT__ {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        /// @brief Function call operator to invoke the stored member function on the object
        /// @param arg1 First argument to pass to the member function
        /// @param arg2 Second argument to pass to the member function
        /// @return The result of invoking the member function
        virtual Return operator()(Arg1 arg1, Arg2 arg2) const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (m_Object->*m_Function)(arg1, arg2);
        }

        /// @brief Gets the target member function pointer
        Return(Object::*Target() const __WSTL_NOEXCEPT__)(Arg1, Arg2) {
            return m_Function;
        }

        /// @brief Gets the target const object pointer
        const Object* TargetObject() const __WSTL_NOEXCEPT__ {
            return m_Object;
        }

    private:
        Return (Object::*m_Function)(Arg1, Arg2);
        Object* m_Object;
    };

    // Const member

    template<typename Object, typename Return, typename Arg1, typename Arg2>
    class Function<Return(Arg1, Arg2), const Object> : public FunctionBase<Return(Arg1, Arg2)> {
    public:
        typedef const Object ObjectType;

        /// @brief Default constructor
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Creates an empty `Function` object
        Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Parameterized constructor
        /// @param object Pointer to the const object instance
        /// @param function Pointer to the const member function of the object
        Function(const Object* object, Return(Object::*function)(Arg1, Arg2) const) 
            : m_Function(function), m_Object(object) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            m_Object = other.m_Object;
            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = NullPointer;
            m_Object = NullPointer;
            return *this;
        }

        /// @brief Resets the `Function` to hold a new const member function pointer and an object
        /// @param object Pointer to the const object instance
        /// @param function Pointer to the const member function of the object
        void Reset(const Object* object, Return (Object::*function)(Arg1, Arg2) const) {
            m_Object = object;
            m_Function = function;
        }

        /// @brief Swaps the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        void Swap(Function& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        operator bool() const __WSTL_NOEXCEPT__ {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        /// @brief Function call operator to invoke the stored const member function on the object
        /// @param arg1 First argument to pass to the const member function
        /// @param arg2 Second argument to pass to the const member function
        /// @return The result of invoking the const member function
        virtual Return operator()(Arg1 arg1, Arg2 arg2) const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (m_Object->*m_Function)(arg1, arg2);
        }

        /// @brief Gets the target const member function pointer
        Return(Object::*Target() const __WSTL_NOEXCEPT__)(Arg1, Arg2) const {
            return m_Function;
        }

        /// @brief Gets the target const object pointer
        const Object* TargetObject() const __WSTL_NOEXCEPT__ {
            return m_Object;
        }

    private:
        Return (Object::*m_Function)(Arg1, Arg2) const;
        const Object* m_Object;
    };

    // Comparison operators with NullPointerType (nullptr_t)

    template<typename Return, typename Arg1, typename Arg2> 
    inline bool operator==(const Function<Return(Arg1, Arg2)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename Arg1, typename Arg2> 
    inline bool operator==(NullPointerType, const Function<Return(Arg1, Arg2)>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename Arg1, typename Arg2> 
    inline bool operator==(const Function<Return(Arg1, Arg2), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename Arg1, typename Arg2> 
    inline bool operator==(NullPointerType, const Function<Return(Arg1, Arg2), Object>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename Arg1, typename Arg2> 
    inline bool operator!=(const Function<Return(Arg1, Arg2)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Return, typename Arg1, typename Arg2> 
    inline bool operator!=(NullPointerType, const Function<Return(Arg1, Arg2)>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }

    template<typename Object, typename Return, typename Arg1, typename Arg2> 
    inline bool operator!=(const Function<Return(Arg1, Arg2), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Object, typename Return, typename Arg1, typename Arg2> 
    inline bool operator!=(NullPointerType, const Function<Return(Arg1, Arg2), Object>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }


    // Function (1 argument)

    template<typename Return, typename Arg>
    class Function<Return(Arg)> : public FunctionBase<Return(Arg)> {
    public:
        /// @brief Default constructor
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer) {}

        /// @brief Creates an empty `Function` object
        Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(NullPointer) {}

        /// @brief Parameterized constructor
        /// @param function Pointer to a free function
        Function(Return(*function)(Arg)) : m_Function(function) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        Function(const Function& other) : m_Function(other.m_Function) {}

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = NullPointer;
            return *this;
        }

        /// @brief Assignment operator for a free function pointer
        /// @param function Pointer to a free function to assign
        Function& operator=(Return(*function)(Arg)) {
            m_Function = function;
            return *this;
        }

        /// @brief Resets the `Function` to hold a new free function pointer
        /// @param function Pointer to a free function to assign
        void Reset(Return(*function)(Arg)) {
            m_Function = function;
        }

        /// @brief Swaps the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        void Swap(Function& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Function, other.m_Function);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        operator bool() const __WSTL_NOEXCEPT__ {
            return m_Function != NullPointer;
        }

        /// @brief Function call operator to invoke the stored callable
        /// @param arg Argument to pass to the callable
        /// @return The result of invoking the callable
        virtual Return operator()(Arg arg) const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (*m_Function)(arg);
        }

        /// @brief Gets the target function pointer
        Return(*Target() const __WSTL_NOEXCEPT__)(Arg) {
            return m_Function;
        }

    private:
        Return (*m_Function)(Arg);
    };

    // Member

    template<typename Object, typename Return, typename Arg>
    class Function<Return(Arg), Object> : public FunctionBase<Return(Arg)> {
    public:
        typedef Object ObjectType;

        /// @brief Default constructor
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Creates an empty `Function` object
        Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Parameterized constructor
        /// @param object Pointer to the object instance
        /// @param function Pointer to the member function of the object
        Function(Object* object, Return(Object::*function)(Arg)) : m_Function(function), m_Object(object) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            m_Object = other.m_Object;
            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = NullPointer;
            m_Object = NullPointer;
            return *this;
        }

        /// @brief Resets the `Function` to hold a new member function pointer and an object
        /// @param object Pointer to the object instance
        /// @param function Pointer to the member function of the object
        void Reset(Object* object, Return (Object::*function)(Arg)) {
            m_Object = object;
            m_Function = function;
        }

        /// @brief Swaps the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        void Swap(Function& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        operator bool() const __WSTL_NOEXCEPT__ {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        /// @brief Function call operator to invoke the stored member function on the object
        /// @param arg Argument to pass to the member function
        /// @return The result of invoking the member function
        virtual Return operator()(Arg arg) const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (m_Object->*m_Function)(arg);
        }

        /// @brief Gets the target member function pointer
        Return(Object::*Target() const __WSTL_NOEXCEPT__)(Arg) {
            return m_Function;
        }

        /// @brief Gets the target const object pointer
        const Object* TargetObject() const __WSTL_NOEXCEPT__ {
            return m_Object;
        }

    private:
        Return (Object::*m_Function)(Arg);
        Object* m_Object;
    };

    // Const member

    template<typename Object, typename Return, typename Arg>
    class Function<Return(Arg), const Object> : public FunctionBase<Return(Arg)> {
    public:
        typedef const Object ObjectType;

        /// @brief Default constructor
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Creates an empty `Function` object
        Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Parameterized constructor
        /// @param object Pointer to the const object instance
        /// @param function Pointer to the const member function of the object
        Function(const Object* object, Return(Object::*function)(Arg) const) 
            : m_Function(function), m_Object(object) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            m_Object = other.m_Object;
            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = NullPointer;
            m_Object = NullPointer;
            return *this;
        }

        /// @brief Resets the `Function` to hold a new const member function pointer and an object
        /// @param object Pointer to the const object instance
        /// @param function Pointer to the const member function of the object
        void Reset(const Object* object, Return (Object::*function)(Arg) const) {
            m_Object = object;
            m_Function = function;
        }

        /// @brief Swaps the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        void Swap(Function& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        operator bool() const __WSTL_NOEXCEPT__ {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        /// @brief Function call operator to invoke the stored const member function on the object
        /// @param arg Argument to pass to the const member function
        /// @return The result of invoking the const member function
        virtual Return operator()(Arg arg) const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (m_Object->*m_Function)(arg);
        }

        /// @brief Gets the target const member function pointer
        Return(Object::*Target() const __WSTL_NOEXCEPT__)(Arg) const {
            return m_Function;
        }

        /// @brief Gets the target const object pointer
        const Object* TargetObject() const __WSTL_NOEXCEPT__ {
            return m_Object;
        }

    private:
        Return (Object::*m_Function)(Arg) const;
        const Object* m_Object;
    };

    // Comparison operators with NullPointerType (nullptr_t)

    template<typename Return, typename Arg> 
    inline bool operator==(const Function<Return(Arg)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename Arg> 
    inline bool operator==(NullPointerType, const Function<Return(Arg)>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename Arg> 
    inline bool operator==(const Function<Return(Arg), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return, typename Arg> 
    inline bool operator==(NullPointerType, const Function<Return(Arg), Object>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return, typename Arg> 
    inline bool operator!=(const Function<Return(Arg)>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Return, typename Arg> 
    inline bool operator!=(NullPointerType, const Function<Return(Arg)>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }

    template<typename Object, typename Return, typename Arg> 
    inline bool operator!=(const Function<Return(Arg), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Object, typename Return, typename Arg> 
    inline bool operator!=(NullPointerType, const Function<Return(Arg), Object>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }


    // Function (no arguments)

    template<typename Return>
    class Function<Return()> : public FunctionBase<Return()> {
    public:
        /// @brief Default constructor
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer) {}

        /// @brief Creates an empty `Function` object
        Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(NullPointer) {}

        /// @brief Parameterized constructor
        /// @param function Pointer to a free function
        Function(Return(*function)()) : m_Function(function) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        Function(const Function& other) : m_Function(other.m_Function) {}

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = NullPointer;
            return *this;
        }

        /// @brief Assignment operator for a free function pointer
        /// @param function Pointer to a free function to assign
        Function& operator=(Return(*function)()) {
            m_Function = function;
            return *this;
        }

        /// @brief Resets the `Function` to hold a new free function pointer
        /// @param function Pointer to a free function to assign
        void Reset(Return(*function)()) {
            m_Function = function;
        }

        /// @brief Swaps the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        void Swap(Function& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Function, other.m_Function);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        operator bool() const __WSTL_NOEXCEPT__ {
            return m_Function != NullPointer;
        }

        /// @brief Function call operator to invoke the stored callable
        /// @return The result of invoking the callable
        virtual Return operator()() const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (*m_Function)();
        }

        /// @brief Gets the target function pointer
        Return(*Target() const __WSTL_NOEXCEPT__)() {
            return m_Function;
        }

    private:
        Return (*m_Function)();
    };

    // Member

    template<typename Object, typename Return>
    class Function<Return(), Object> : public FunctionBase<Return()> {
    public:
        typedef Object ObjectType;

        /// @brief Default constructor
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Creates an empty `Function` object
        Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Parameterized constructor
        /// @param object Pointer to the object instance
        /// @param function Pointer to the member function of the object
        Function(Object* object, Return(Object::*function)()) : m_Function(function), m_Object(object) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            m_Object = other.m_Object;
            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = NullPointer;
            m_Object = NullPointer;
            return *this;
        }

        /// @brief Resets the `Function` to hold a new member function pointer and an object
        /// @param object Pointer to the object instance
        /// @param function Pointer to the member function of the object
        void Reset(Object* object, Return (Object::*function)()) {
            m_Object = object;
            m_Function = function;
        }

        /// @brief Swaps the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        void Swap(Function& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        operator bool() const __WSTL_NOEXCEPT__ {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        /// @brief Function call operator to invoke the stored member function on the object
        /// @return The result of invoking the member function
        virtual Return operator()() const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (m_Object->*m_Function)();
        }

        /// @brief Gets the target member function pointer
        Return(Object::*Target() const __WSTL_NOEXCEPT__)() {
            return m_Function;
        }

        /// @brief Gets the target const object pointer
        const Object* TargetObject() const __WSTL_NOEXCEPT__ {
            return m_Object;
        }

    private:
        Return (Object::*m_Function)();
        Object* m_Object;
    };

    // Const member

    template<typename Object, typename Return>
    class Function<Return(), const Object> : public FunctionBase<Return()> {
    public:
        typedef const Object ObjectType;

        /// @brief Default constructor
        Function() __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Creates an empty `Function` object
        Function(NullPointerType) __WSTL_NOEXCEPT__ : m_Function(NullPointer), m_Object(NullPointer) {}

        /// @brief Parameterized constructor
        /// @param object Pointer to the const object instance
        /// @param function Pointer to the const member function of the object
        Function(const Object* object, Return(Object::*function)() const) 
            : m_Function(function), m_Object(object) {}

        /// @brief Copy constructor
        /// @param other Another `Function` object to copy from
        Function(const Function& other) : m_Function(other.m_Function), m_Object(other.m_Object) {}

        /// @brief Copy assignment operator
        /// @param other Another `Function` object to copy from
        Function& operator=(const Function& other) {
            m_Function = other.m_Function;
            m_Object = other.m_Object;
            return *this;
        }

        /// @brief Assignment operator for a null pointer, resets to an empty state
        Function& operator=(NullPointerType) __WSTL_NOEXCEPT__ {
            m_Function = NullPointer;
            m_Object = NullPointer;
            return *this;
        }

        /// @brief Resets the `Function` to hold a new const member function pointer and an object
        /// @param object Pointer to the const object instance
        /// @param function Pointer to the const member function of the object
        void Reset(const Object* object, Return (Object::*function)() const) {
            m_Object = object;
            m_Function = function;
        }

        /// @brief Swaps the contents of this `Function` with another
        /// @param other Another `Function` object to swap with
        void Swap(Function& other) __WSTL_NOEXCEPT__ {
            wstl::Swap(m_Function, other.m_Function);
            wstl::Swap(m_Object, other.m_Object);
        }

        /// @brief Conversion operator to check if the `Function` is valid (i.e. has a callable)
        operator bool() const __WSTL_NOEXCEPT__ {
            return (m_Function != NullPointer) && (m_Object != NullPointer);
        }

        /// @brief Function call operator to invoke the stored const member function on the object
        /// @return The result of invoking the const member function
        virtual Return operator()() const {
            __WSTL_ASSERT_RETURNVALUE__(this->operator bool(), WSTL_MAKE_EXCEPTION(BadFunctionCall), static_cast<Return>(-1));
            return (m_Object->*m_Function)();
        }

        /// @brief Gets the target const member function pointer
        Return(Object::*Target() const __WSTL_NOEXCEPT__)() const {
            return m_Function;
        }

        /// @brief Gets the target const object pointer
        const Object* TargetObject() const __WSTL_NOEXCEPT__ {
            return m_Object;
        }

    private:
        Return (Object::*m_Function)() const;
        const Object* m_Object;
    };

    // Comparison operators with NullPointerType (nullptr_t)

    template<typename Return> 
    inline bool operator==(const Function<Return()>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return> 
    inline bool operator==(NullPointerType, const Function<Return()>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return> 
    inline bool operator==(const Function<Return(), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Object, typename Return> 
    inline bool operator==(NullPointerType, const Function<Return(), Object>& function) __WSTL_NOEXCEPT__ {
        return !function;
    }

    template<typename Return> 
    inline bool operator!=(const Function<Return()>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Return> 
    inline bool operator!=(NullPointerType, const Function<Return()>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }  

    template<typename Object, typename Return> 
    inline bool operator!=(const Function<Return(), Object>& function, NullPointerType) __WSTL_NOEXCEPT__ {
        return !(function == NullPointer);
    }

    template<typename Object, typename Return> 
    inline bool operator!=(NullPointerType, const Function<Return(), Object>& function) __WSTL_NOEXCEPT__ {
        return !(NullPointer == function);
    }
    #endif

    #ifdef __WSTL_CXX11__
    // Invoke

    /// @brief Invokes a callable object with the provided arguments by forwarding them
    /// @param function The callable object to invoke
    /// @param ...args The arguments to be forwarded
    /// @return The result of invoking the callable object
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/functional/invoke
    template<typename Function, typename... Args>
    constexpr auto Invoke(Function&& function, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
        noexcept(__private::__Invoke(Forward<Function>(function), Forward<Args>(args)...))
    ) -> decltype(__private::__Invoke(Forward<Function>(function), Forward<Args>(args)...)) {
        return __private::__Invoke(Forward<Function>(function), Forward<Args>(args)...);
    }

    // Invoke return

    /// @brief Invokes a callable object with the provided arguments by forwarding them, 
    /// and returns the result cast to the specified type, void oveload
    /// @tparam Result The type to cast the result to
    /// @param function The callable object to invoke
    /// @param ...args The arguments to be forwarded
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/functional/invoke
    template<typename Result, typename Function, typename... Args>
    constexpr EnableIfType<IsVoid<Result>::Value && IsInvocableReturn<Result, Function, Args...>::Value, void> 
    InvokeReturn(Function&& function, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
        noexcept(Invoke(Forward<Function>(function), Forward<Args>(args)...))
    ) {
        return (void) Invoke(Forward<Function>(function), Forward<Args>(args)...);
    }

    /// @brief Invokes a callable object with the provided arguments by forwarding them, 
    /// and returns the result cast to the specified type, non-void overload
    /// @tparam Result The type to cast the result to
    /// @param function The callable object to invoke
    /// @param ...args The arguments to be forwarded
    /// @return The result of invoking the callable object, cast to the specified type
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/functional/invoke
    template<typename Result, typename Function, typename... Args>
    constexpr EnableIfType<!IsVoid<Result>::Value && IsInvocableReturn<Result, Function, Args...>::Value, Result> 
    InvokeReturn(Function&& function, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
        noexcept(static_cast<Result>(Invoke(Forward<Function>(function), Forward<Args>(args)...)))
    ) {
        return static_cast<Result>(Invoke(Forward<Function>(function), Forward<Args>(args)...));
    }
    #endif

    // Reference wrapper

    #ifndef __WSTL_CXX11__
    namespace __private {
        template<typename T, bool = sizeof(__private::__TestResultOf<T>(0)) == sizeof(long)>
        class __ReferenceWrapper;

        template<typename T>
        class __ReferenceWrapper<T, true> {
        public:
            /// @brief Calls the referenced object with two arguments
            /// @param arg1 Value of the first argument
            /// @param arg2 Value of the second argument
            /// @return The result of calling the referenced object
            template<typename Arg1, typename Arg2>
            typename ResultOf<T>::Type operator()(Arg1 arg1, Arg2 arg2) const {
                return (*m_Pointer)(arg1, arg2);
            }

            /// @brief Calls the referenced object with one argument
            /// @param arg Value of the argument
            /// @return The result of calling the referenced object
            template<typename Arg>
            typename ResultOf<T>::Type operator()(Arg arg) const {
                return (*m_Pointer)(arg);
            }

            /// @brief Calls the referenced object with no arguments
            /// @return The result of calling the referenced object
            typename ResultOf<T>::Type operator()() const {
                return (*m_Pointer)();
            }
        
        protected:
            T* m_Pointer;

            explicit __ReferenceWrapper(T* value) __WSTL_NOEXCEPT__ : m_Pointer(value) {}
        };

        template<typename T>
        class __ReferenceWrapper<T, false> {
        protected:
            T* m_Pointer;

            explicit __ReferenceWrapper(T* value) __WSTL_NOEXCEPT__ : m_Pointer(value) {}
        };
    }

    /// @brief Wrapper class to store references
    /// @tparam T Type of the object to wrap
    /// @ingroup functional
    /// @note In C++98 it supports maximum two arguments for callable objects
    /// @see https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
    template<typename T>
    class ReferenceWrapper : public __private::__ReferenceWrapper<T> {
    public:
        /// @brief Alias for the wrapped type
        typedef T Type;

        /// @brief Constructor
        /// @param value The reference to the object to wrap
        explicit ReferenceWrapper(T& value) __WSTL_NOEXCEPT__ : __private::__ReferenceWrapper<T>(&value) {}

        /// @brief Copy constructor
        /// @param other Reference wrapper to copy from
        ReferenceWrapper(const ReferenceWrapper& other) __WSTL_NOEXCEPT__ 
            : __private::__ReferenceWrapper<T>(other.m_Pointer) {}      
        
        /// @brief Copy assignment operator
        /// @param other Reference wrapper to copy from
        ReferenceWrapper& operator=(const ReferenceWrapper& other) __WSTL_NOEXCEPT__ {
            this->m_Pointer = other.m_Pointer;
            return *this;
        }

        /// @brief Gets the wrapped reference to the object
        T& Get() const __WSTL_NOEXCEPT__ {
            return *this->m_Pointer;
        }

        /// @brief Implicit conversion operator to the wrapped reference
        operator T&() const __WSTL_NOEXCEPT__ {
            return *this->m_Pointer;
        }
    };

    #else
    /// @brief Wrapper class to store references
    /// @tparam T Type of the object to wrap
    /// @ingroup functional
    /// @note In C++98 it supports maximum two arguments for callable objects
    /// @see https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
    template<typename T>
    class ReferenceWrapper {
    public:
        /// @brief Alias for the wrapped type
        typedef T Type;

        /// @brief Constructor
        /// @param value The reference to the object to wrap
        constexpr explicit ReferenceWrapper(T& value) __WSTL_NOEXCEPT__ : m_Pointer(&value)  {}
        
        /// @brief Copy constructor
        /// @param other Reference wrapper to copy from
        constexpr ReferenceWrapper(const ReferenceWrapper& other) __WSTL_NOEXCEPT__ 
            : m_Pointer(other.m_Pointer) {}      
        
        /// @brief Copy assignment operator
        /// @param other Reference wrapper to copy from
        __WSTL_CONSTEXPR14__ ReferenceWrapper& operator=(const ReferenceWrapper& other) __WSTL_NOEXCEPT__ {
            m_Pointer = other.m_Pointer;
            return *this;
        }

        /// @brief Gets the wrapped reference to the object
        constexpr T& Get() const __WSTL_NOEXCEPT__ {
            return *m_Pointer;
        }

        /// @brief Implicit conversion operator to the wrapped reference
        constexpr operator T&() const __WSTL_NOEXCEPT__ {
            return *m_Pointer;
        }
        
        /// @brief Calls the referenced object with the provided arguments, forwarding them
        /// @param args The arguments to be forwarded
        /// @return The result of calling the referenced object
        template<typename... Args>
        constexpr InvokeResultType<T&, Args...> operator()(Args&&... args) const __WSTL_NOEXCEPT_EXPR__(
            noexcept(Invoke(Get(), Forward<Args>(args)...))
        ) {
            return Invoke(Get(), Forward<Args>(args)...);
        }

    private:
        T* m_Pointer;
    };
    #endif

    // Reference

    /// @brief Creates a `ReferenceWrapper` for non-const reference
    /// @param t Object to wrap
    /// @return A `ReferenceWrapper` that holds a reference to given object
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/ref
    template<typename T>
    __WSTL_CONSTEXPR__ inline ReferenceWrapper<T> Reference(T& t) {
        return ReferenceWrapper<T>(t);
    }

    /// @brief Creates a `ReferenceWrapper` from another `ReferenceWrapper` for non-const reference
    /// @param t The `ReferenceWrapper` object to convert
    /// @return A `ReferenceWrapper` that holds a reference to object in given `ReferenceWrapper`
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/ref
    template<typename T>
    __WSTL_CONSTEXPR__ inline ReferenceWrapper<T> Reference(ReferenceWrapper<T> t) {
        return ReferenceWrapper<T>(t.Get());
    }

    // Const reference

    /// @brief Creates a `ReferenceWrapper` for const reference
    /// @param t Object to wrap
    /// @return A `ReferenceWrapper` that holds a reference to given object
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/ref
    template<typename T>
    __WSTL_CONSTEXPR__ inline ReferenceWrapper<const T> ConstReference(const T& t) {
        return ReferenceWrapper<const T>(t);
    }

    /// @brief Creates a `ReferenceWrapper` from another `ReferenceWrapper` for const reference
    /// @param t The `ReferenceWrapper` object to convert
    /// @return A `ReferenceWrapper` that holds a reference to object in given `ReferenceWrapper`
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/ref
    template<typename T>
    __WSTL_CONSTEXPR__ inline ReferenceWrapper<const T> ConstReference(ReferenceWrapper<T> t) {
        return ReferenceWrapper<const T>(t.Get());
    }

    // UnwrapReference specialization

    template<typename T>
    struct UnwrapReference<ReferenceWrapper<T> > { typedef T& Type; };

    // HasIsTransparent

    namespace __private {
        WSTL_DECLARE_TYPEDEF_TEST(__TestIsTransparent, IsTransparent)
    }

    /// @brief Checks if a functor has a nested type `IsTransparent`, 
    /// indicating it is heterogeneous and can accept different types of arguments
    /// @tparam T Type of the functor to check
    /// @ingroup functional
    template<typename T>
    struct HasIsTransparent : BoolConstant<WSTL_TYPEDEF_TEST_RESULT(__private::__TestIsTransparent, T, 1)> {};

    // Plus

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Plus;
    #else
    template<typename T>
    struct Plus;
    #endif

    /// @brief Functor that represents addition for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/plus
    template<typename T>
    struct Plus : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The sum of two arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a + b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents addition for two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/plus_void
    template<>
    struct Plus<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The sum of two arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) + Forward<U>(b)) {
            return Forward<T>(a) + Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Adds two integral constant values at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct Plus {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : IntegralConstant<T, (A + B)> {};
        };
    }

    // Minus

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Minus;
    #else
    template<typename T>
    struct Minus;
    #endif

    /// @brief Functor that represents subtraction for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/minus
    template<typename T>
    struct Minus : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The difference of two arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a - b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents subtraction for two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/minus_void
    template<>
    struct Minus<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The difference of two arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) - Forward<U>(b)) {
            return Forward<T>(a) - Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Subtracts two integral constant values at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct Minus {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : IntegralConstant<T, (A - B)> {};
        };
    }

    // Multiplies

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Multiplies;
    #else
    template<typename T>
    struct Multiplies;
    #endif

    /// @brief Functor that represents multiplication for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/multiplies
    template<typename T>
    struct Multiplies : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The product of two arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a * b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents multiplication for two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/multiplies_void
    template<>
    struct Multiplies<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The product of two arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) * Forward<U>(b)) {
            return Forward<T>(a) * Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Multiplies two integral constant values at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct Multiplies {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : IntegralConstant<T, (A * B)> {};
        };
    }

    // Divides

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Divides;
    #else
    template<typename T>
    struct Divides;
    #endif

    /// @brief Functor that represents division for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/divides
    template<typename T>
    struct Divides : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The quotient of two arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a / b;
        }
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents division for two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/divides_void
    template<>
    struct Divides<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The quotient of two arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) / Forward<U>(b)) {
            return Forward<T>(a) / Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Divides two integral constant values at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct Divides {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : IntegralConstant<T, (A / B)> {};
        };
    }

    // Modulus

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Modulus;
    #else
    template<typename T>
    struct Modulus;
    #endif

    /// @brief Functor that represents the modulus operation for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/modulus
    template<typename T>
    struct Modulus : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The remainder of the division of the first argument by the second argument
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a % b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents the modulus operation for two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/modulus_void
    template<>
    struct Modulus<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return The remainder of the division of the first argument by the second argument
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) % Forward<U>(b)) {
            return Forward<T>(a) % Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Computes the modulus of two integral constant values at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct Modulus {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : IntegralConstant<T, (A % B)> {};
        };
    }

    // Negate

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Negate;
    #else
    template<typename T>
    struct Negate;
    #endif

    /// @brief Functor that represents the negation operation for an object
    /// @tparam T Type of the object
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/negate
    template<typename T>
    struct Negate : UnaryFunction<T, T> {
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return The negation of the argument
        __WSTL_CONSTEXPR__ T operator()(const T& x) const {
            return -x;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents the negation operation for an object 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/negate_void
    template<>
    struct Negate<void> : UnaryFunction<void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return The negation of the argument
        template<typename T>
        constexpr auto operator()(T&& x) const -> decltype(-Forward<T>(x)) {
            return -Forward<T>(x);
        } 
    };
    #endif

    namespace compile {
        /// @brief Computes the negation of an integral constant value at compile time
        /// @tparam T Type of the integral constant value
        /// @ingroup functional
        template<typename T>
        struct Negate {
            /// @brief Applies the operation to an integral constant value
            /// @tparam X Value of the integral constant
            template<T X>
            struct Apply : IntegralConstant<T, (-X)> {};
        };
    }

    // Equal to

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct EqualTo;
    #else
    template<typename T>
    struct EqualTo;
    #endif
    
    /// @brief Functor that represents equality comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/equal_to
    template<typename T>
    struct EqualTo : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the arguments are equal, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a == b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents equality comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/equal_to_void
    template<>
    struct EqualTo<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the arguments are equal, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) == Forward<U>(b)) {
            return Forward<T>(a) == Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Compares two integral constant values for equality at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct EqualTo {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : BoolConstant<(A == B)> {};
        };
    }

    // Not equal to

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct NotEqualTo;
    #else
    template<typename T>
    struct NotEqualTo;
    #endif

    /// @brief Functor that represents inequality comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/not_equal_to
    template<typename T>
    struct NotEqualTo : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the arguments are not equal, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a != b;
        }
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents inequality comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/not_equal_to_void
    template<>
    struct NotEqualTo<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;
    
        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the arguments are not equal, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) != Forward<U>(b)) {
            return Forward<T>(a) != Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Compares two integral constant values for inequality at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct NotEqualTo {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : BoolConstant<(A != B)> {};
        };
    }

    // Greater

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Greater;
    #else
    template<typename T>
    struct Greater;
    #endif

    /// @brief Functor that represents greater-than comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/greater
    template<typename T>
    struct Greater : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is greater than the second, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a > b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents greater-than comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/greater_void
    template<>
    struct Greater<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is greater than the second, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) > Forward<U>(b)) {
            return Forward<T>(a) > Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Compares two integral constant values for greater-than relation at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct Greater {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : BoolConstant<(A > B)> {};
        };
    }

    // Less

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct Less;
    #else
    template<typename T>
    struct Less;
    #endif
    
    /// @brief Functor that represents less-than comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/less
    template<typename T>
    struct Less : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is less than the second, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a < b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents less-than comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/less_void
    template<>
    struct Less<void> : public BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is less than the second, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) < Forward<U>(b)) {
            return Forward<T>(a) < Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Compares two integral constant values for less-than relation at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct Less {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : BoolConstant<(A < B)> {};
        };
    }

    // Greater equal

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct GreaterEqual;
    #else
    template<typename T>
    struct GreaterEqual;
    #endif

    /// @brief Functor that represents greater-than-or-equal-to comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/greater_equal
    template<typename T>
    struct GreaterEqual : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is greater than or equal to the second, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a >= b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents greater-than-or-equal-to comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/greater_equal_void
    template<>
    struct GreaterEqual<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is greater than or equal to the second, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) >= Forward<U>(b)) {
            return Forward<T>(a) >= Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Compares two integral constant values for greater-than-or-equal-to relation at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct GreaterEqual {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : BoolConstant<(A >= B)> {};
        };
    }

    // Less equal

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct LessEqual;
    #else
    template<typename T>
    struct LessEqual;
    #endif

    /// @brief Functor that represents less-than-or-equal-to comparison between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/less_equal
    template<typename T>
    struct LessEqual : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is less than or equal to the second, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a <= b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents less-than-or-equal-to comparison between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/less_equal_void
    template<>
    struct LessEqual<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if the first argument is less than or equal to the second, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) <= Forward<U>(b)) {
            return Forward<T>(a) <= Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Compares two integral constant values for less-than-or-equal-to relation at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct LessEqual {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : BoolConstant<(A <= B)> {};
        };
    }

    // Logical and

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct LogicalAnd;
    #else
    template<typename T>
    struct LogicalAnd;
    #endif

    /// @brief Functor that represents logical AND operation between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_and
    template<typename T>
    struct LogicalAnd : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if both arguments are true, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a && b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents logical AND operation between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_and_void
    template<>
    struct LogicalAnd<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if both arguments are true, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) && Forward<U>(b)) {
            return Forward<T>(a) && Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Computes the logical AND of two integral constant values at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct LogicalAnd {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : BoolConstant<(A && B)> {};
        };
    }

    // Logical or

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct LogicalOr;
    #else
    template<typename T>
    struct LogicalOr;
    #endif

    /// @brief Functor that represents logical OR operation between two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_or
    template<typename T>
    struct LogicalOr : BinaryFunction<T, T, bool> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if at least one of the arguments is true, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& a, const T& b) const {
            return a || b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents logical OR operation between two objects 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_or_void
    template<>
    struct LogicalOr<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return True if at least one of the arguments is true, false otherwise
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) || Forward<U>(b)) {
            return Forward<T>(a) || Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Computes the logical OR of two integral constant values at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct LogicalOr {
            /// @brief Applies the operation to two integral constant values
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : BoolConstant<(A || B)> {};
        };
    }

    // Logical not

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct LogicalNot;
    #else
    template<typename T>
    struct LogicalNot;
    #endif

    /// @brief Functor that represents logical NOT operation for an object
    /// @tparam T Type of the object
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_not
    template<typename T>
    struct LogicalNot : UnaryFunction<T, bool> {
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return True if the argument is false, false otherwise
        __WSTL_CONSTEXPR__ bool operator()(const T& x) const {
            return !x;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents logical NOT operation for an object 
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/logical_not_void
    template<>
    struct LogicalNot<void> : UnaryFunction<void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return True if the argument is false, false otherwise
        template<typename T>
        constexpr auto operator()(T&& x) const -> decltype(!Forward<T>(x)) {
            return !Forward<T>(x);
        } 
    };
    #endif

    namespace compile {
        /// @brief Computes the logical NOT of an integral constant value at compile time
        /// @tparam T Type of the integral constant value
        /// @ingroup functional
        template<typename T>
        struct LogicalNot {
            /// @brief Applies the operation to an integral constant value
            /// @tparam X Value of the integral constant
            template<T X>
            struct Apply : BoolConstant<!X> {};
        };
    }

    // Bitwise and

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct BitwiseAnd;
    #else
    template<typename T>
    struct BitwiseAnd;
    #endif

    /// @brief Functor that represents bitwise AND operation for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_and
    template<typename T>
    struct BitwiseAnd : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise AND of the arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a & b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents bitwise AND operation for an object
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_and_void
    template<>
    struct BitwiseAnd<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise AND of the arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) & Forward<U>(b)) {
            return Forward<T>(a) & Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Computes the bitwise AND of two integral constant values at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct BitwiseAnd {
            /// @brief Applies the operation to an integral constant value
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : IntegralConstant<T, (A & B)> {};
        };
    }

    // Bitwise or

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct BitwiseOr;
    #else
    template<typename T>
    struct BitwiseOr;
    #endif

    /// @brief Functor that represents bitwise OR operation for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_or
    template<typename T>
    struct BitwiseOr : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise OR of the arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a | b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents bitwise OR operation for an object
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_or_void
    template<>
    struct BitwiseOr<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise OR of the arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) | Forward<U>(b)) {
            return Forward<T>(a) | Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Computes the bitwise OR of two integral constant values at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct BitwiseOr {
            /// @brief Applies the operation to an integral constant value
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : IntegralConstant<T, (A | B)> {};
        };
    }

    // Bitwise xor

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct BitwiseXor;
    #else
    template<typename T>
    struct BitwiseXor;
    #endif

    /// @brief Functor that represents bitwise XOR operation for two objects
    /// @tparam T Type of the objects
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_xor
    template<typename T>
    struct BitwiseXor : BinaryFunction<T, T, T> {
        /// @brief Applies the functor to two arguments
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise XOR of the arguments
        __WSTL_CONSTEXPR__ T operator()(const T& a, const T& b) const {
            return a ^ b;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents bitwise XOR operation for an object
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_xor_void
    template<>
    struct BitwiseXor<void> : BinaryFunction<void, void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        /// @brief Applies the functor to two arguments of potentially different types
        /// @param a Value of the first argument
        /// @param b Value of the second argument
        /// @return Bitwise XOR of the arguments
        template<typename T, typename U>
        constexpr auto operator()(T&& a, U&& b) const -> decltype(Forward<T>(a) ^ Forward<U>(b)) {
            return Forward<T>(a) ^ Forward<U>(b);
        } 
    };
    #endif

    namespace compile {
        /// @brief Computes the bitwise XOR of two integral constant values at compile time
        /// @tparam T Type of the integral constant values
        /// @ingroup functional
        template<typename T>
        struct BitwiseXor {
            /// @brief Applies the operation to an integral constant value
            /// @tparam A Value of the first integral constant
            /// @tparam B Value of the second integral constant
            template<T A, T B>
            struct Apply : IntegralConstant<T, (A ^ B)> {};
        };
    }

    // Bitwise not

    #ifdef __WSTL_CXX11__
    template<typename T = void>
    struct BitwiseNot;
    #else
    template<typename T>
    struct BitwiseNot;
    #endif

    /// @brief Functor that represents bitwise NOT operation for an object
    /// @tparam T Type of the object
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_not
    template<typename T>
    struct BitwiseNot : UnaryFunction<T, T> {
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return Bitwise NOT of the argument
        __WSTL_CONSTEXPR__ T operator()(const T& x) const {
            return ~x;
        } 
    };

    #ifdef __WSTL_CXX11__
    /// @brief Functor that represents bitwise NOT operation for an object
    /// with transparent forwarding for any types
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bitwise_not_void
    template<>
    struct BitwiseNot<void> : UnaryFunction<void, void> {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;
        
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return Bitwise NOT of the argument
        template<typename T>
        constexpr auto operator()(T&& x) const -> decltype(~Forward<T>(x)) {
            return ~Forward<T>(x);
        } 
    };
    #endif

    namespace compile {
        /// @brief Computes the bitwise NOT of an integral constant value at compile time
        /// @tparam T Type of the integral constant value
        /// @ingroup functional
        template<typename T>
        struct BitwiseNot {
            /// @brief Applies the operation to an integral constant value
            /// @tparam X Value of the integral constant
            template<T X>
            struct Apply : IntegralConstant<T, ~X> {};
        };
    }

    // Identity

    /// @brief Functor that returns its input unchanged for non-void types
    /// @tparam T Type of the input and output
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/identity
    struct Identity {
        /// @brief Type definition indicating the functor is transparent
        typedef void IsTransparent;

        #ifdef __WSTL_CXX11__
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return The argument itself
        template<typename T>
        constexpr T&& operator()(T&& x) const __WSTL_NOEXCEPT__ {
            return Forward<T>(x);
        }

        #else
        /// @brief Applies the functor to an argument
        /// @param x Value of the argument
        /// @return The argument itself
        template<typename T>
        __WSTL_CONSTEXPR__ T& operator()(T& x) const __WSTL_NOEXCEPT__ {
            return x;
        }

        /// @brief Applies the functor to an argument
        /// @param x Const value of the argument
        /// @return The argument itself
        template<typename T>
        __WSTL_CONSTEXPR__ const T& operator()(const T& x) const __WSTL_NOEXCEPT__ {
            return x;
        }
        #endif
    };

    // Not function

    #ifdef __WSTL_CXX11__
    /// @brief Functor that negates the result of a given callable
    /// @tparam Function Type of the callable object
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/not_fn
    template<typename Function>
    class NotFunctionType {
    public:
        /// @brief Constructor that initializes the functor with a callable object
        /// @param function The callable object to wrap and negate
        explicit constexpr NotFunctionType(Function&& function) : m_Function(Forward<Function>(function)) {}

        template<typename... Args>
        __WSTL_CONSTEXPR14__ auto operator()(Args&&... args) & __WSTL_NOEXCEPT_EXPR__(
            noexcept(!Invoke(DeclareValue<Function&>(), Forward<Args>(args)...))
        ) -> decltype(!Invoke(DeclareValue<Function&>(), Forward<Args>(args)...)) {
            return !Invoke(m_Function, Forward<Args>(args)...);
        }

        template<typename... Args>
        constexpr auto operator()(Args&&... args) const& __WSTL_NOEXCEPT_EXPR__(
            noexcept(!Invoke(DeclareValue<const Function&>(), Forward<Args>(args)...))
        ) -> decltype(!Invoke(DeclareValue<const Function&>(), Forward<Args>(args)...)) {
            return !Invoke(m_Function, Forward<Args>(args)...);
        }

        template<typename... Args>
        __WSTL_CONSTEXPR14__ auto operator()(Args&&... args) && __WSTL_NOEXCEPT_EXPR__(
            noexcept(!Invoke(DeclareValue<Function&&>(), Forward<Args>(args)...))
        ) -> decltype(!Invoke(DeclareValue<Function&&>(), Forward<Args>(args)...)) {
            return !Invoke(wstl::Move(m_Function), Forward<Args>(args)...);
        }

        template<typename... Args>
        constexpr auto operator()(Args&&... args) const&& __WSTL_NOEXCEPT_EXPR__(
            noexcept(!Invoke(DeclareValue<const Function&&>(), Forward<Args>(args)...))
        ) -> decltype(!Invoke(DeclareValue<const Function&&>(), Forward<Args>(args)...)) {
            return !Invoke(wstl::Move(m_Function), Forward<Args>(args)...);
        }

    private:
        Function m_Function;
    };

    /// @brief Creates a functor that negates the result of a given callable
    /// @param function The callable object to wrap and negate
    /// @return A functor that negates the result of the given callable
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/not_fn
    template<typename Function>
    constexpr NotFunctionType<DecayType<Function>> NotFunction(Function&& function) {
        return NotFunctionType<DecayType<Function>>(Forward<Function>(function));
    }
    #else
    namespace __private {
        template<typename Function, size_t = FunctionTraits<Function>::Arity>
        struct __NotFunctionType;

        template<typename Function>
        struct __NotFunctionType<Function, 0> {
            typedef bool ResultType;

            bool operator()() const {
                return !(m_Function());
            }

        private:
            Function m_Function;

        protected:
            explicit __NotFunctionType(const Function& function) : m_Function(function) {}
        };
        
        template<typename Function>
        struct __NotFunctionType<Function, 1> {
            typedef typename TupleElement<0, typename FunctionTraits<Function>::ArgumentTypes>::Type ArgumentType;
            typedef bool ResultType;

            bool operator()(ArgumentType arg) const {
                return !(m_Function(arg));
            }

        private:
            Function m_Function;

        protected:
            explicit __NotFunctionType(const Function& function) : m_Function(function) {}
        };

        template<typename Function>
        struct __NotFunctionType<Function, 2> {
            typedef typename TupleElement<0, typename FunctionTraits<Function>::ArgumentTypes>::Type FirstArgumentType;
            typedef typename TupleElement<1, typename FunctionTraits<Function>::ArgumentTypes>::Type SecondArgumentType;
            typedef bool ResultType;

            bool operator()(FirstArgumentType arg1, SecondArgumentType arg2) const {
                return !(m_Function(arg1, arg2));
            }

        private:
            Function m_Function;

        protected:
            explicit __NotFunctionType(const Function& function) : m_Function(function) {}
        };
    }

    /// @brief Functor that negates the result of a given callable
    /// @tparam Function Type of the callable object (max 2 arguments)
    /// @ingroup functional
    template<typename Function>
    struct NotFunctionType : __private::__NotFunctionType<Function> {
        /// @brief Constructor that initializes the functor with a callable object
        /// @param function The callable object to wrap and negate
        explicit NotFunctionType(const Function& function) : __private::__NotFunctionType<Function>(function) {}
    };

    /// @brief Creates a functor that negates the result of a given callable
    /// @param function The callable object to wrap and negate (max 2 arguments)
    /// @return A functor that negates the result of the given callable
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/not_fn
    template<typename Function>
    inline NotFunctionType<typename Decay<Function>::Type> NotFunction(const Function& function) {
        return NotFunctionType<typename Decay<Function>::Type>(function);
    }
    #endif

    #ifdef __WSTL_CXX11__
    // Placeholders

    namespace __private {
        template<int N>
        struct __Placeholder {};

        template<typename T>
        struct __IsPlaceholder : IntegralConstant<int, 0> {};

        template<int N>
        struct __IsPlaceholder<__Placeholder<N>> : IntegralConstant<int, N> {};
    }

    /// @brief Namespace that contain placeholders (unbound arguments) for `wstl::Bind` function
    namespace placeholders {
        #ifdef __WSTL_CXX11__
            #define __WSTL_PLACEHOLDER__ __WSTL_INLINE_VARIABLE__ constexpr
        #else
            #define __WSTL_PLACEHOLDER__ static const
        #endif
        
        __WSTL_PLACEHOLDER__ __private::__Placeholder<1> _1;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<2> _2;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<3> _3;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<4> _4;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<5> _5;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<6> _6;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<7> _7;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<8> _8;
        __WSTL_PLACEHOLDER__ __private::__Placeholder<9> _9;
    }

    // Is placeholder

    /// @brief Checks whether type is placeholder, stores int `IntegralConstant` 
    /// with the value of placeholder number
    /// @tparam T Type to check
    /// @ingroup functional
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/functional/is_placeholder
    template<typename T>
    struct IsPlaceholder : __private::__IsPlaceholder<typename RemoveCVReference<T>::Type> {};

    #ifdef __WSTL_CXX17__
    /// @copydoc IsPlaceholder
    /// @since C++17
    template<typename T>
    inline constexpr int IsPlaceholderValue = IsPlaceholder<T>::Value;
    #endif

    // Is bind expression

    namespace __private {
        template<typename Function, typename... BoundArgs>
        class __Bind;

        template<typename Result, typename Function, typename... BoundArgs>
        class __BindReturn;

        template<typename T>
        struct __IsBindExpression : FalseType {};

        template<typename Function, typename... BoundArgs>
        struct __IsBindExpression<__Bind<Function, BoundArgs...>> : TrueType {};

        template<typename Result, typename Function, typename... BoundArgs>
        struct __IsBindExpression<__BindReturn<Result, Function, BoundArgs...>> : TrueType {};
    }

    /// @brief Checks whether type is bind expression
    /// @tparam T Type to check
    /// @since C++11
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/is_bind_expression
    template<typename T>
    struct IsBindExpression : __private::__IsBindExpression<typename RemoveCVReference<T>::Type> {};

    // Bind

    namespace __private {
        template<typename T, typename Tuple>
        constexpr T& __EvaluateBindArg(ReferenceWrapper<T> ref, Tuple&&) __WSTL_NOEXCEPT__ {
            return ref.Get();
        }

        template<typename T, typename... Args, size_t... Indices>
        constexpr auto __ExpandBindExpression(T&& bind, Tuple<Args...>&& args, IndexSequence<Indices...>) __WSTL_NOEXCEPT_EXPR__(
            noexcept(Forward<T>(bind)(Forward<Args>(Get<Indices>(args))...))
        ) -> decltype(Forward<T>(bind)(Forward<Args>(Get<Indices>(args))...)) {
            return Forward<T>(bind)(Forward<Args>(Get<Indices>(args))...);
        }

        template<typename T, typename... Args, EnableIfType<IsBindExpression<T>::Value, int> = 0>
        constexpr auto __EvaluateBindArg(T&& bind, Tuple<Args...>&& args) __WSTL_NOEXCEPT_EXPR__(
            noexcept(__ExpandBindExpression(Forward<T>(bind), Forward<Tuple<Args...>>(args), IndexSequenceFor<Args...>{}))
        ) -> decltype(__ExpandBindExpression(Forward<T>(bind), Forward<Tuple<Args...>>(args), IndexSequenceFor<Args...>{})) {
            return __ExpandBindExpression(Forward<T>(bind), Forward<Tuple<Args...>>(args), IndexSequenceFor<Args...>{});
        }

        template<typename T, typename Tuple, EnableIfType<(IsPlaceholder<T>::Value > 0), int> = 0>
        constexpr TupleElementType<IsPlaceholder<T>::Value - 1, Tuple> __EvaluateBindArg(T&&, Tuple&& args) __WSTL_NOEXCEPT__ {
            return Forward<TupleElementType<IsPlaceholder<T>::Value - 1, Tuple>>(Get<IsPlaceholder<T>::Value - 1>(args));
        }

        template<typename T, typename Tuple, EnableIfType<(!IsBindExpression<T>::Value && IsPlaceholder<T>::Value == 0), int> = 0>
        constexpr T&& __EvaluateBindArg(T&& arg, Tuple&&) __WSTL_NOEXCEPT__ {
            return Forward<T>(arg);
        }

        template<typename Function, typename... BoundArgs>
        class __Bind {
            DecayType<Function> m_Function;
            Tuple<DecayType<BoundArgs>...> m_BoundArgs;

            template<typename Self, typename... Args, size_t... Indices>
            static constexpr auto __Call(Self&& self, IndexSequence<Indices...>, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
                noexcept(Invoke(self.m_Function, __EvaluateBindArg(Get<Indices>(ForwardLike<Self>(self.m_BoundArgs)), ForwardAsTuple(args...))...))
            ) -> decltype(Invoke(self.m_Function, __EvaluateBindArg(Get<Indices>(ForwardLike<Self>(self.m_BoundArgs)), ForwardAsTuple(args...))...)) {
                return Invoke(self.m_Function, __EvaluateBindArg(Get<Indices>(ForwardLike<Self>(self.m_BoundArgs)), ForwardAsTuple(args...))...);
            }

        public:
            constexpr __Bind(Function&& function, BoundArgs&&... boundArgs) 
                : m_Function(Forward<Function>(function)), m_BoundArgs(Forward<BoundArgs>(boundArgs)...) {}

            template<typename... Args>
            __WSTL_CONSTEXPR14__ auto operator()(Args&&... args) & __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }

            template<typename... Args>
            constexpr auto operator()(Args&&... args) const& __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }

            template<typename... Args>
            __WSTL_CONSTEXPR14__ auto operator()(Args&&... args) && __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }
            
            template<typename... Args>
            constexpr auto operator()(Args&&... args) const&& __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }
        };

        template<typename Result, typename Function, typename... BoundArgs>
        class __BindReturn {
        private:
            DecayType<Function> m_Function;
            Tuple<DecayType<BoundArgs>...> m_BoundArgs;

            template<typename Self, typename... Args, size_t... Indices>
            static constexpr Result __Call(Self&& self, IndexSequence<Indices...>, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
                noexcept(InvokeReturn<Result>(self.m_Function, __EvaluateBindArg(Get<Indices>(ForwardLike<Self>(self.m_BoundArgs)), ForwardAsTuple(args...))...))
            ) {
                return InvokeReturn<Result>(self.m_Function, __EvaluateBindArg(Get<Indices>(ForwardLike<Self>(self.m_BoundArgs)), ForwardAsTuple(args...))...);
            }

        public:
            constexpr __BindReturn(Function&& function, BoundArgs&&... boundArgs) 
                : m_Function(Forward<Function>(function)), m_BoundArgs(Forward<BoundArgs>(boundArgs)...) {} 

            template<typename... Args>
            __WSTL_CONSTEXPR14__ Result operator()(Args&&... args) & __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) {
                return __Call(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }

            template<typename... Args>
            constexpr Result operator()(Args&&... args) const& __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) {
                return __Call(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }

            template<typename... Args>
            __WSTL_CONSTEXPR14__ Result operator()(Args&&... args) && __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) {
                return __Call(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }
            
            template<typename... Args>
            constexpr Result operator()(Args&&... args) const&& __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) {
                return __Call(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }
        };
    }

    #ifdef __WSTL_CXX17__
    /// @copydoc IsBindExpression
    /// @since C++17
    template<typename T>
    inline constexpr bool IsBindExpressionValue = IsBindExpression<T>::Value;
    #endif

    /// @brief Binds a function with specified arguments, including placeholders if needed
    /// @param function The function to bind
    /// @param ...boundArgs The arguments to bind, which may include placeholders
    /// @return A bound function object with the provided arguments
    /// @ingroup functional
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bind
    template<typename Function, typename... BoundArgs>
    constexpr __private::__Bind<Function, BoundArgs...> Bind(Function&& function, BoundArgs&&... boundArgs) {
        return {Forward<Function>(function), Forward<BoundArgs>(boundArgs)...};
    }

    /// @brief Binds a function with specified arguments, including placeholders if needed, and specifies the return type
    /// @tparam Result The return type of the bound function
    /// @param function The function to bind
    /// @param ...boundArgs The arguments to bind, which may include placeholders
    /// @return A bound function object with the provided arguments and specified return type
    /// @ingroup functional
    /// @since C++11
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bind
    template<typename Result, typename Function, typename... BoundArgs>
    constexpr __private::__BindReturn<Result, Function, BoundArgs...> Bind(Function&& function, BoundArgs&&... boundArgs) {
        return {Forward<Function>(function), Forward<BoundArgs>(boundArgs)...};
    }

    // Bind front & bind back

    namespace __private {        
        template<typename Function, bool Back, typename... BoundArgs>
        class __BindDirection {
            DecayType<Function> m_Function;
            Tuple<DecayType<BoundArgs>...> m_BoundArgs;

            template<bool B, typename Self, typename... Args, size_t... Indices, EnableIfType<!B, int> = 0>
            static constexpr auto __Call(Self&& self, IndexSequence<Indices...>, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
                noexcept(Invoke(self.m_Function, Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))..., Forward<Args>(args)...))
            ) -> decltype(Invoke(self.m_Function, Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))..., Forward<Args>(args)...)) {
                return Invoke(self.m_Function, Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))..., Forward<Args>(args)...);
            }

            template<bool B, typename Self, typename... Args, size_t... Indices, EnableIfType<B, int> = 0>
            static constexpr auto __Call(Self&& self, IndexSequence<Indices...>, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
                noexcept(Invoke(self.m_Function, Forward<Args>(args)..., Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))...))
            ) -> decltype(Invoke(self.m_Function, Forward<Args>(args)..., Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))...)) {
                return Invoke(self.m_Function, Forward<Args>(args)..., Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))...);
            }

        public:
            constexpr __BindDirection(Function&& function, BoundArgs&&... boundArgs) 
                : m_Function(Forward<Function>(function)), m_BoundArgs(Forward<BoundArgs>(boundArgs)...) {}

            template<typename... Args>
            __WSTL_CONSTEXPR14__ auto operator()(Args&&... args) & __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }

            template<typename... Args>
            constexpr auto operator()(Args&&... args) const& __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }

            template<typename... Args>
            __WSTL_CONSTEXPR14__ auto operator()(Args&&... args) && __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }
            
            template<typename... Args>
            constexpr auto operator()(Args&&... args) const&& __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }
        };

        #ifdef __WSTL_CXX17__
        template<auto ConstFunction, bool Back, typename... BoundArgs>
        class __BindDirectionConst {
            Tuple<DecayType<BoundArgs>...> m_BoundArgs;

            template<bool B, typename Self, typename... Args, size_t... Indices, EnableIfType<!B, int> = 0>
            static constexpr auto __Call(Self&& self, IndexSequence<Indices...>, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
                noexcept(Invoke(ConstFunction, Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))..., Forward<Args>(args)...))
            ) -> decltype(Invoke(ConstFunction, Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))..., Forward<Args>(args)...)) {
                return Invoke(ConstFunction, Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))..., Forward<Args>(args)...);
            }

            template<bool B, typename Self, typename... Args, size_t... Indices, EnableIfType<B, int> = 0>
            static constexpr auto __Call(Self&& self, IndexSequence<Indices...>, Args&&... args) __WSTL_NOEXCEPT_EXPR__(
                noexcept(Invoke(ConstFunction, Forward<Args>(args)..., Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))...))
            ) -> decltype(Invoke(ConstFunction, Forward<Args>(args)..., Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))...)) {
                return Invoke(ConstFunction, Forward<Args>(args)..., Get<Indices>(ForwardLike<Self>(self.m_BoundArgs))...);
            }

        public:
            constexpr __BindDirectionConst(BoundArgs&&... boundArgs) : m_BoundArgs(Forward<BoundArgs>(boundArgs)...) {}

            template<typename... Args>
            __WSTL_CONSTEXPR14__ auto operator()(Args&&... args) & __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }

            template<typename... Args>
            constexpr auto operator()(Args&&... args) const& __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call<Back>(*this, IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }

            template<typename... Args>
            __WSTL_CONSTEXPR14__ auto operator()(Args&&... args) && __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }
            
            template<typename... Args>
            constexpr auto operator()(Args&&... args) const&& __WSTL_NOEXCEPT_EXPR__(
                noexcept(__Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...))
            ) -> decltype(__Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...)) {
                return __Call<Back>(Move(*this), IndexSequenceFor<BoundArgs...>{}, Forward<Args>(args)...);
            }
        };
        #endif
    }

    /// @brief Binds a function with specified arguments to the front
    /// @details Arguments are not processed and are passed as-is to the function
    /// @param function The function to bind
    /// @param ...boundArgs The arguments to bind
    /// @return A bound function object with the provided arguments bound to the front
    /// @ingroup functional
    /// @since C++11
    /// @see https://cppreference.com/cpp/utility/functional/bind_front
    template<typename Function, typename... BoundArgs>
    constexpr __private::__BindDirection<Function, false, BoundArgs...> BindFront(Function&& function, BoundArgs&&... boundArgs) {
        return {Forward<Function>(function), Forward<BoundArgs>(boundArgs)...};
    }

    /// @brief Binds a function with specified arguments to the back
    /// @details Arguments are not processed and are passed as-is to the function
    /// @param function The function to bind
    /// @param ...boundArgs The arguments to bind
    /// @return A bound function object with the provided arguments bound to the back
    /// @ingroup functional
    /// @since C++11
    /// @see https://cppreference.com/cpp/utility/functional/bind_front
    template<typename Function, typename... BoundArgs>
    constexpr __private::__BindDirection<Function, true, BoundArgs...> BindBack(Function&& function, BoundArgs&&... boundArgs) {
        return {Forward<Function>(function), Forward<BoundArgs>(boundArgs)...};
    }

    #ifdef __WSTL_CXX17__
    /// @brief Binds a constant function with specified arguments to the front
    /// @details Arguments are not processed and are passed as-is to the function.
    /// Only from C++20 can be used with `constexpr` function objects.
    /// @tparam ConstFunction The constant function to bind
    /// @param ...boundArgs The arguments to bind
    /// @return A bound function object with the provided arguments bound to the front
    /// @ingroup functional
    /// @since C++17
    /// @see https://cppreference.com/cpp/utility/functional/bind_front
    template<auto ConstFunction, typename... BoundArgs>
    constexpr __private::__BindDirectionConst<ConstFunction, false, BoundArgs...> BindFront(BoundArgs&&... boundArgs) {
        return {Forward<BoundArgs>(boundArgs)...};
    }

    /// @brief Binds a constant function with specified arguments to the back
    /// @details Arguments are not processed and are passed as-is to the function.
    /// Only from C++20 can be used with `constexpr` function objects.
    /// @tparam ConstFunction The constant function to bind
    /// @param ...boundArgs The arguments to bind
    /// @return A bound function object with the provided arguments bound to the back
    /// @ingroup functional
    /// @since C++17
    /// @see https://cppreference.com/cpp/utility/functional/bind_front
    template<auto ConstFunction, typename... BoundArgs>
    constexpr __private::__BindDirectionConst<ConstFunction, true, BoundArgs...> BindBack(BoundArgs&&... boundArgs) {
        return {Forward<BoundArgs>(boundArgs)...};
    }
    #endif

    #endif

    // Binder first

    /// @brief Functor that binds the first argument of a callable object
    /// @tparam Function Type of the callable object
    /// @tparam BoundArgument Type of the argument to bind
    /// @ingroup functional
    /// @deprecated Recommended below C++11 or for older API compatibility. Otherwise better use `wstl::Bind`
    /// @see https://en.cppreference.com/w/cpp/utility/functional/binder12
    template<typename Function, typename BoundArgument>
    class BinderFirst {
    public:
        typedef typename FunctionTraits<Function>::ResultType ResultType;
        typedef typename TupleElement<1, typename FunctionTraits<Function>::ArgumentTypes>::Type ArgumentType;

        /// @brief Constructor
        /// @param function Callable object to bind
        /// @param arg Value of the first argument to bind
        __WSTL_CONSTEXPR__ BinderFirst(const Function& function, const BoundArgument& arg) :
            m_Function(function), m_Arg(arg) {}

        /// @brief Invokes the callable object
        /// @param arg Value of the second argument
        /// @return Result of the callable object
        __WSTL_CONSTEXPR__ ResultType operator()(ArgumentType arg) const {
            return m_Function(m_Arg, arg);
        }

    protected:
        Function m_Function;
        BoundArgument m_Arg;
    };

    /// @brief Creates a `BinderFirst` object that binds the first argument of a callable object
    /// @param function The callable object to bind 
    /// @param arg Value of the first argument to bind
    /// @return A `BinderFirst` object that wraps the callable object with the first argument bound
    /// @ingroup functional
    /// @deprecated Recommended below C++11 or for older API compatibility. Otherwise better use `wstl::Bind`
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bind12
    template<typename Function, typename T>
    __WSTL_CONSTEXPR__ BinderFirst<Function, T> BindFirst(const Function& function, const T& arg) {
        return BinderFirst<Function, T>(function, arg);
    }

    // Binder second

    /// @brief Functor that binds the second argument of a callable object
    /// @tparam Function Type of the callable object
    /// @ingroup functional
    /// @deprecated Recommended below C++11. Otherwise better use `wstl::Bind`
    /// @see https://en.cppreference.com/w/cpp/utility/functional/binder12
    template<typename Function, typename BoundArgument>
    class BinderSecond {
    public:
        typedef typename FunctionTraits<Function>::ResultType ResultType;
        typedef typename TupleElement<0, typename FunctionTraits<Function>::ArgumentTypes>::Type ArgumentType;

        /// @brief Constructor
        /// @param function Callable object to bind
        /// @param arg Value of the second argument to bind
        __WSTL_CONSTEXPR__ BinderSecond(const Function& function, const BoundArgument arg) :
            m_Function(function), m_Arg(arg) {}
        
        /// @brief Invokes the callable object
        /// @param arg Value of the first argument
        /// @return Result of the callable object
        __WSTL_CONSTEXPR__ ResultType operator()(ArgumentType arg) const {
            return m_Function(arg, m_Arg);
        }

    protected:
        Function m_Function;
        BoundArgument m_Arg;
    };

    /// @brief Creates a `BinderSecond` object that binds the second argument of a callable object
    /// @param function The callable object to bind 
    /// @param arg Value of the second argument to bind
    /// @return A `BinderSecond` object that wraps the callable object with the second argument bound
    /// @ingroup functional
    /// @deprecated Recommended below C++11. Otherwise better use `wstl::Bind`
    /// @see https://en.cppreference.com/w/cpp/utility/functional/bind12
    template<typename Function, typename T>
    __WSTL_CONSTEXPR__ BinderSecond<Function, T> BindSecond(const Function& function, const T& arg) {
        return BinderSecond<Function, T>(function, arg);
    }

    // Member function

    namespace __private {
        #ifdef __WSTL_CXX11__
        template<typename T, typename = typename EnableIf<IsMemberPointer<T>::Value>::Type>
        class __MemberFunctionType;
        
        template<typename T>
        class __MemberFunctionType<T, void> {
        public:
            /// @brief Invokes the member function on the given object with the provided arguments
            /// @param args Arguments to pass to the member function (first argument must be the object on which to invoke the member function)
            /// @return Result of the member function invocation
            template<typename... Args>
            constexpr InvokeResultType<T, Args...> operator()(Args&&... args) const __WSTL_NOEXCEPT_EXPR__(
                noexcept(Invoke(m_Function, Forward<Args>(args)...))
            ) {
                return Invoke(m_Function, Forward<Args>(args)...);
            }
            
        protected:
            T m_Function;

            explicit constexpr __MemberFunctionType(T function) : m_Function(function) {}
        };
        #else
        template<typename T, bool = IsMemberFunctionPointer<T>::Value>
        struct __MemberFunctionArity;

        template<typename T>
        struct __MemberFunctionArity<T, true> {
            static const size_t Value = FunctionTraits<T>::Arity;
        };

        template<typename T>
        const size_t __MemberFunctionArity<T, true>::Value;

        template<typename T>
        struct __MemberFunctionArity<T, false> {
            static const size_t Value = size_t(-1);
        };

        template<typename T>
        const size_t __MemberFunctionArity<T, false>::Value;

        template<typename T>
        struct __MemberObjectTypes;

        template<typename T, typename M>
        struct __MemberObjectTypes<T M::*> { 
            typedef T ValueType;
            typedef M MemberType;
        };

        template<typename T, bool = IsMemberFunctionPointer<T>::Value, 
        bool = IsMemberObjectPointer<T>::Value, size_t = __MemberFunctionArity<T>::Value>
        class __MemberFunctionType;

        template<typename T>
        class __MemberFunctionType<T, true, false, 2> {
        private:
            typedef typename FunctionTraits<T>::ObjectType ObjectType;

        public:
            typedef typename FunctionTraits<T>::ResultType ResultType;
            typedef typename TupleElement<0, typename FunctionTraits<T>::ArgumentTypes>::Type FirstArgumentType;
            typedef typename TupleElement<1, typename FunctionTraits<T>::ArgumentTypes>::Type SecondArgumentType;
            
            /// @brief Invokes the member function on the given object with the provided arguments
            /// @param object Pointer to the object on which to invoke the member function
            /// @param arg1 First argument to pass to the member function
            /// @param arg2 Second argument to pass to the member function
            /// @return Result of the member function invocation
            ResultType operator()(ObjectType* object, FirstArgumentType arg1, SecondArgumentType arg2) const {
                return (object->*m_Function)(arg1, arg2);
            }

            /// @brief Invokes the member function on the given const object with the provided arguments
            /// @param object Pointer to the const object on which to invoke the member function
            /// @param arg1 First argument to pass to the member function
            /// @param arg2 Second argument to pass to the member function
            /// @return Result of the member function invocation
            ResultType operator()(const ObjectType* object, FirstArgumentType arg1, SecondArgumentType arg2) const {
                return (object->*m_Function)(arg1, arg2);
            }

            /// @brief Invokes the member function on the given object reference with the provided arguments
            /// @param object Reference to the object on which to invoke the member function
            /// @param arg1 First argument to pass to the member function
            /// @param arg2 Second argument to pass to the member function
            /// @return Result of the member function invocation
            ResultType operator()(ObjectType& object, FirstArgumentType arg1, SecondArgumentType arg2) const {
                return (object.*m_Function)(arg1, arg2);
            }

            /// @brief Invokes the member function on the given const object reference with the provided arguments
            /// @param object Reference to the const object on which to invoke the member function
            /// @param arg1 First argument to pass to the member function
            /// @param arg2 Second argument to pass to the member function
            /// @return Result of the member function invocation
            ResultType operator()(const ObjectType& object, FirstArgumentType arg1, SecondArgumentType arg2) const {
                return (object.*m_Function)(arg1, arg2);
            }

        protected:
            T m_Function;

            explicit __MemberFunctionType(T function) : m_Function(function) {}
        };

        template<typename T>
        class __MemberFunctionType<T, true, false, 1> {
        private:
            typedef typename FunctionTraits<T>::ObjectType ObjectType;

        public:
            typedef typename FunctionTraits<T>::ResultType ResultType;
            typedef typename TupleElement<0, typename FunctionTraits<T>::ArgumentTypes>::Type ArgumentType;
            
            /// @brief Invokes the member function on the given object with the provided argument
            /// @param object Pointer to the object on which to invoke the member function
            /// @param arg Argument to pass to the member function
            /// @return Result of the member function invocation
            ResultType operator()(ObjectType* object, ArgumentType arg) const {
                return (object->*m_Function)(arg);
            }

            /// @brief Invokes the member function on the given const object with the provided argument
            /// @param object Pointer to the const object on which to invoke the member function
            /// @param arg Argument to pass to the member function
            /// @return Result of the member function invocation
            ResultType operator()(const ObjectType* object, ArgumentType arg) const {
                return (object->*m_Function)(arg);
            }

            /// @brief Invokes the member function on the given object reference with the provided argument
            /// @param object Reference to the object on which to invoke the member function
            /// @param arg Argument to pass to the member function
            /// @return Result of the member function invocation
            ResultType operator()(ObjectType& object, ArgumentType arg) const {
                return (object.*m_Function)(arg);
            }

            /// @brief Invokes the member function on the given const object reference with the provided argument
            /// @param object Reference to the const object on which to invoke the member function
            /// @param arg Argument to pass to the member function
            /// @return Result of the member function invocation
            ResultType operator()(const ObjectType& object, ArgumentType arg) const {
                return (object.*m_Function)(arg);
            }

        protected:
            T m_Function;

            explicit __MemberFunctionType(T function) : m_Function(function) {}
        };

        template<typename T>
        class __MemberFunctionType<T, true, false, 0> {
        private:
            typedef typename FunctionTraits<T>::ObjectType ObjectType;

        public:
            typedef typename FunctionTraits<T>::ResultType ResultType;

            /// @brief Invokes the member function on the given object
            /// @param object Pointer to the object on which to invoke the member function
            /// @return Result of the member function invocation
            ResultType operator()(ObjectType* object) const {
                return (object->*m_Function)();
            }

            /// @brief Invokes the member function on the given const object
            /// @param object Pointer to the const object on which to invoke the member function
            /// @return Result of the member function invocation
            ResultType operator()(const ObjectType* object) const {
                return (object->*m_Function)();
            }

            /// @brief Invokes the member function on the given object reference
            /// @param object Reference to the object on which to invoke the member function
            /// @return Result of the member function invocation
            ResultType operator()(ObjectType& object) const {
                return (object.*m_Function)();
            }

            /// @brief Invokes the member function on the given const object reference
            /// @param object Reference to the const object on which to invoke the member function
            /// @return Result of the member function invocation
            ResultType operator()(const ObjectType& object) const {
                return (object.*m_Function)();
            }

        protected:
            T m_Function;

            explicit __MemberFunctionType(T function) : m_Function(function) {}
        };

        template<typename T>
        class __MemberFunctionType<T, false, true, size_t(-1)> {
        private:
            typedef typename __MemberObjectTypes<T>::MemberType ObjectType;

        public:
            typedef typename __MemberObjectTypes<T>::ValueType ResultType;
            
            /// @brief Accesses the member object of the given object
            /// @param object Pointer to the object on which to access the member object
            /// @return Reference to the member object
            ResultType& operator()(ObjectType* object) const {
                return object->*m_Pointer;
            }

            /// @brief Accesses the member object of the given const object
            /// @param object Pointer to the const object on which to access the member object
            /// @return Const reference to the member object
            const ResultType& operator()(const ObjectType* object) const {
                return object->*m_Pointer;
            }

            /// @brief Accesses the member object of the given object reference
            /// @param object Reference to the object on which to access the member object
            /// @return Reference to the member object
            ResultType& operator()(ObjectType& object) const {
                return object.*m_Pointer;
            }

            /// @brief Accesses the member object of the given const object reference
            /// @param object Reference to the const object on which to access the member object
            /// @return Const reference to the member object
            const ResultType& operator()(const ObjectType& object) const {
                return object.*m_Pointer;
            }

        protected:
            T m_Pointer;

            explicit __MemberFunctionType(T pointer) : m_Pointer(pointer) {}
        };
        #endif
    }

    /// @brief Wraps a member pointer into a functor
    /// @tparam T Type of the member pointer (function or object)
    /// @note In C++98 maximum 2 arguments are supported
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fn
    template<typename T>
    class MemberFunctionType : public __private::__MemberFunctionType<T> {
    public:
        /// @brief Constructor
        /// @param function Pointer to the member function or member object to be wrapped
        explicit MemberFunctionType(T function) : __private::__MemberFunctionType<T>(function) {}
    };

    /// @brief Creates a functor from a member pointer
    /// @param pointer Pointer to the member function or member object
    /// @return A `MemberFunctionType` object wrapping the member pointer
    /// @ingroup functional
    /// @see https://en.cppreference.com/w/cpp/utility/functional/mem_fn
    template<typename M, typename T>
    __WSTL_CONSTEXPR__ MemberFunctionType<M T::*> MemberFunction(M T::* pointer) {
        return MemberFunctionType<M T::*>(pointer);
    }
}

#endif
