// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/BumpAllocator.hpp>
#include <wstl/Iterator.hpp>


uint8_t memory[256];

TEST_SUITE("BumpAllocator") {
    TEST_CASE("Constructor") {
        wstl::BumpAllocator allocator(memory, wstl::Size(memory));

        CHECK_EQ(allocator.MemorySize(), wstl::Size(memory));
        CHECK_EQ(allocator.Available(), wstl::Size(memory));
        CHECK_EQ(allocator.Used(), 0U);

        // The following lines should fail with a compilation error
        // wstl::BumpAllocator er1(allocator);
        // wstl::BumpAllocator er2(wstl::Move(allocator));
    }

    TEST_CASE("Allocate") {
        wstl::BumpAllocator allocator(memory, wstl::Size(memory));

        void* addr1 = allocator.Allocate(20);

        CHECK_EQ(addr1, memory);
        CHECK_EQ(allocator.Available(), wstl::Size(memory) - 20);
        CHECK_EQ(allocator.Used(), 20U);

        void* addr2 = allocator.Allocate(110);

        CHECK_EQ(addr2, memory + 20);
        CHECK_EQ(allocator.Available(), wstl::Size(memory) - 130);
        CHECK_EQ(allocator.Used(), 130U);

        CHECK_THROWS_AS({ void* i = allocator.Allocate(1000); (void) i; }, wstl::BadAllocation);
    }

    TEST_CASE("Free") {
        wstl::BumpAllocator allocator(memory, wstl::Size(memory));

        void* addr = allocator.Allocate(20);

        CHECK_EQ(addr, memory);
        CHECK_EQ(allocator.Available(), wstl::Size(memory) - 20);
        CHECK_EQ(allocator.Used(), 20U);

        allocator.Free(addr); // should do nothing

        CHECK_EQ(allocator.Available(), wstl::Size(memory) - 20);
        CHECK_EQ(allocator.Used(), 20U);
    }

    TEST_CASE("MemorySize") {
        wstl::BumpAllocator allocator(memory, wstl::Size(memory));

        CHECK_EQ(allocator.MemorySize(), wstl::Size(memory));
    }

    TEST_CASE("Available") {
        wstl::BumpAllocator allocator(memory, wstl::Size(memory));

        CHECK_EQ(allocator.Available(), wstl::Size(memory));

        allocator.Allocate(3);

        CHECK_EQ(allocator.Available(), wstl::Size(memory) - 3);
    }

    TEST_CASE("Used") {
        wstl::BumpAllocator allocator(memory, wstl::Size(memory));

        CHECK_EQ(allocator.Used(), 0U);

        allocator.Allocate(3);

        CHECK_EQ(allocator.Used(), 3U);
    }
}