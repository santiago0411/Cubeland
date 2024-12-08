#pragma once

#include "Core/Base.h"
#include <filesystem>

#if defined(CL_ENABLE_ASSERTS)

// Alternatively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define CL_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { CL_LOG##type##ERROR(msg, __VA_ARGS__); __debugbreak(); } }
#define CL_INTERNAL_ASSERT_WITH_MSG(type, check, ...) CL_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define CL_INTERNAL_ASSERT_NO_MSG(type, check) CL_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", CL_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define CL_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define CL_INTERNAL_ASSERT_GET_MACRO(...) CL_EXPAND_MACRO( CL_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, CL_INTERNAL_ASSERT_WITH_MSG, CL_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define CL_ASSERT(...) do { CL_EXPAND_MACRO( CL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) ) } while (0)
#else
#define CL_ASSERT(...)
#endif