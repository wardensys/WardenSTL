#include <wstl/Algorithm.hpp>

static_assert(WSTL_COUNT_ARGS(A) == 1, "");
static_assert(WSTL_COUNT_ARGS(A, B) == 2, "");
static_assert(WSTL_COUNT_ARGS(A, B, C) == 3, "");
static_assert(WSTL_COUNT_ARGS(A, B, C, D, E) == 5, "");