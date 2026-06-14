// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#ifndef __WSTL_BUMPALLOCATOR_HPP__
#define __WSTL_BUMPALLOCATOR_HPP__

#include "Allocator.hpp"
#include "NullPointer.hpp"
#include "private/Error.hpp" 
#include <stdint.h>


namespace wstl {
    /// @brief A simple bump allocator that allocates memory in a contiguous block
    /// @note Does not support free operation
    /// @ingroup allocator
    class BumpAllocator __WSTL_FINAL__ : public Allocator {
    public:
        /// @brief Constructor
        /// @param base The base address of the memory block
        /// @param limit The limit of the memory block
        BumpAllocator(void* base, size_t limit) : m_Base(reinterpret_cast<uint8_t*>(base)),
            m_Allocated(0), m_Limit(limit) {}

        /// @copydoc Allocator::Allocate(size_t)
        virtual void* Allocate(size_t size) __WSTL_OVERRIDE__ {
            __WSTL_ASSERT_RETURNVALUE__(m_Allocated + size <= m_Limit, WSTL_MAKE_EXCEPTION(BadAllocation, "BumpAllocator: Allocation exceeds limit"), NullPointer);

            void* result = m_Base + m_Allocated;
            m_Allocated += size;
            return result;
        }

        /// @brief Frees a block of memory at the specified address - does not do anything
        virtual void Free(void*) __WSTL_OVERRIDE__ {
            // Does not do anything, not operational
        }

        /// @brief Gets the total size of the memory block
        size_t MemorySize() const __WSTL_NOEXCEPT__ {
            return m_Limit;
        }

        /// @brief Gets the remaining available memory
        size_t Available() const __WSTL_NOEXCEPT__ {
            return m_Limit - m_Allocated;
        }

        /// @brief Gets the total allocated memory
        size_t Used() const __WSTL_NOEXCEPT__ {
            return m_Allocated;
        }
    
    private:
        uint8_t* m_Base;
        size_t m_Allocated;
        size_t m_Limit;
    };
}

#endif