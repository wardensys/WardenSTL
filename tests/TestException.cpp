// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Exception.hpp>


TEST_SUITE("Exception") {
    TEST_CASE("Constuctor") {
        wstl::Exception e("Sample file", 67, "Sample exception");

        CHECK_EQ(e.Filename(), "Sample file");
        CHECK_EQ(e.Line(), 67U);
        CHECK_EQ(e.What(), "Sample exception");
        CHECK_EQ(e.Name(), "Exception");
    }

    TEST_CASE("Exception") {
        wstl::Exception e("Sample file", 67, "Sample exception");

        try {
            throw e;
        }
        catch(const wstl::Exception& c) {
            CHECK_EQ(c.Filename(), "Sample file");
            CHECK_EQ(c.Line(), 67U);
            CHECK_EQ(c.What(), "Sample exception");
            CHECK_EQ(c.Name(), "Exception");
        }
    }
}