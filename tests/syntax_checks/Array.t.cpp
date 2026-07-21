#include <wstl/Array.hpp>

#ifdef _CPPUNWIND
#pragma message("_CPPUNWIND is defined")
#else
#pragma message("_CPPUNWIND is NOT defined")
#endif

#ifdef _EXCEPTIONS
#pragma message("_EXCEPTIONS is defined")
#else
#pragma message("_EXCEPTIONS is NOT defined")
#endif

#ifdef __cpp_exceptions
#pragma message("__cpp_exceptions is defined")
#else
#pragma message("__cpp_exceptions is NOT defined")
#endif

#ifdef __wasi__s
#pragma message("__wasi__ is defined")
#else
#pragma message("__wasi__ is NOT defined")
#endif