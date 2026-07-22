// Part of WardenSTL - https://github.com/WardenHD/WardenSTL
// Copyright (c) 2025 Artem Bezruchko (WardenHD)
//
// Licensed under the MIT License. See LICENSE file for details.

#include <doctest.h>
#include <wstl/Exception.hpp>
#include <cstring>


TEST_SUITE("Exception") {
    TEST_CASE("Constuctor") {
        wstl::Exception e("Sample file", 67, "Sample exception");

        CHECK_EQ(strcmp(e.Filename(), "Sample file"), 0);
        CHECK_EQ(e.Line(), 67U);
        CHECK_EQ(strcmp(e.What(), "Sample exception"), 0);
        CHECK_EQ(strcmp(e.Name(), "Exception"), 0);
    }

    TEST_CASE("Exception") {
        wstl::Exception e("Sample file", 67, "Sample exception");

        try {
            throw e;
        }
        catch(const wstl::Exception& c) {
            CHECK_EQ(strcmp(c.Filename(), "Sample file"), 0);
            CHECK_EQ(c.Line(), 67U);
            CHECK_EQ(strcmp(c.What(), "Sample exception"), 0);
            CHECK_EQ(strcmp(c.Name(), "Exception"), 0);
        }
    }
}