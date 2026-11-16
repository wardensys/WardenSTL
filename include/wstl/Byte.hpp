// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2026 Artem Bezruchko (WardenHD)
//
// This file is based on the Embedded Template Library (ETL)'s byte.h
// from https://github.com/ETLCPP/etl, licensed under the MIT License.
//
// It has been slightly modified by Artem Bezruchko (WardenHD) for WardenSTL.
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_BYTE_HPP__
#define __WSTL_BYTE_HPP__

#include "private/Platform.hpp"
#include "TypeTraits.hpp"


namespace wstl {
    #ifdef __WSTL_CXX11__
    /// @brief A class representing a byte, which is an unsigned char type
    /// @ingroup utilities
    /// @see https://en.cppreference.com/w/cpp/types/byte
    enum class Byte : unsigned char {};

    /// @brief Type alias for STL naming support
    using byte = Byte;
    #else
    /// @brief A class representing a byte, which is an unsigned char type
    /// @warning In C++98 arithmetic and comparion operations involving integral types cannot be properly constrained
    /// @ingroup utilities
    /// @see https://en.cppreference.com/w/cpp/types/byte
    class Byte {
    public:
        Byte() __WSTL_NOEXCEPT__ : m_Value(0) {}

        template<typename T>
        explicit Byte(T value) __WSTL_NOEXCEPT__ : m_Value(static_cast<unsigned char>(value)) {}

        template<typename T>
        operator T() const __WSTL_NOEXCEPT__ {
            return static_cast<T>(m_Value);
        }

        friend inline bool operator==(Byte a, Byte b) __WSTL_NOEXCEPT__ {
            return a.m_Value == b.m_Value;
        }

        friend inline bool operator!=(Byte a, Byte b) __WSTL_NOEXCEPT__ {
            return !(a == b);
        }

        friend inline bool operator<(Byte a, Byte b) __WSTL_NOEXCEPT__ {
            return a.m_Value < b.m_Value;
        }

        friend inline bool operator<=(Byte a, Byte b) __WSTL_NOEXCEPT__ {
            return !(b < a);
        }

        friend inline bool operator>(Byte a, Byte b) __WSTL_NOEXCEPT__ {
            return b < a;
        }

        friend inline bool operator>=(Byte a, Byte b) __WSTL_NOEXCEPT__ {
            return !(a < b);
        }

    private:
        unsigned char m_Value;
    };

    /// @brief Type alias for STL naming support
    typedef Byte byte;
    #endif

    // To integer

    template<typename IntegerType>
    __WSTL_CONSTEXPR__ typename EnableIf<IsIntegral<IntegerType>::Value, IntegerType>::Type ToInteger(Byte b) __WSTL_NOEXCEPT__ {
        return IntegerType(b);
    }
   
    // Shift operators

    template<typename IntegerType>
    __WSTL_CONSTEXPR__ typename EnableIf<IsIntegral<IntegerType>::Value, Byte>::Type operator<<(Byte b, IntegerType shift) __WSTL_NOEXCEPT__ {
        return Byte(static_cast<unsigned int>(b) << shift);
    }

    template<typename IntegerType>
    __WSTL_CONSTEXPR__ typename EnableIf<IsIntegral<IntegerType>::Value, Byte>::Type operator>>(Byte b, IntegerType shift) __WSTL_NOEXCEPT__ {
        return Byte(static_cast<unsigned int>(b) >> shift);
    }

    template<typename IntegerType>
    __WSTL_CONSTEXPR__ typename EnableIf<IsIntegral<IntegerType>::Value, Byte&>::Type operator<<=(Byte& b, IntegerType shift) __WSTL_NOEXCEPT__ {
        return b = b << shift;
    }

    template<typename IntegerType>
    __WSTL_CONSTEXPR__ typename EnableIf<IsIntegral<IntegerType>::Value, Byte&>::Type operator>>=(Byte& b, IntegerType shift) __WSTL_NOEXCEPT__ {
        return b = b >> shift;
    }

    // Bitwise operators

    inline __WSTL_CONSTEXPR__ Byte operator|(Byte a, Byte b) __WSTL_NOEXCEPT__ {
        return Byte(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
    }

    inline __WSTL_CONSTEXPR__ Byte operator&(Byte a, Byte b) __WSTL_NOEXCEPT__ {
        return Byte(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
    }

    inline __WSTL_CONSTEXPR__ Byte operator^(Byte a, Byte b) __WSTL_NOEXCEPT__ {
        return Byte(static_cast<unsigned int>(a) ^ static_cast<unsigned int>(b));
    }

    inline __WSTL_CONSTEXPR__ Byte operator~(Byte b) __WSTL_NOEXCEPT__ {
        return Byte(~static_cast<unsigned int>(b));
    }

    inline __WSTL_CONSTEXPR14__ Byte& operator|=(Byte& a, Byte b) __WSTL_NOEXCEPT__ {
        return a = a | b;
    }

    inline __WSTL_CONSTEXPR14__ Byte& operator&=(Byte& a, Byte b) __WSTL_NOEXCEPT__ {
        return a = a & b;
    }

    inline __WSTL_CONSTEXPR14__ Byte& operator^=(Byte& a, Byte b) __WSTL_NOEXCEPT__ {
        return a = a ^ b;
    }
}

#endif
