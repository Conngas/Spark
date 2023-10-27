#pragma once

#include "Spark/Core/Log.h"
#include "Spark/Core/Base.h"

#include <filesystem>

#ifdef SPK_ENABLE_ASSERT

	#define SPK_INTERNAL_ASSERT_IMPL(type, check, msg, ...) {if(!(check)) {SPK##ERROR(msg,__VA_ARGS__); SPK_DEBUGBREAK();} }
	#define SPK_INTERNAL_ASSERT_WITH_MSG(type, check, ...) SPK_INTERNAL_ASSERT_IMPL(type, check , "Asserion Failed: {0}", __VA__ARGS__)
	#define SPK_INTERNAL_ASSERT_NO_MSG(type, check) SPK_INTERNAL_ASSERT_IMPL(type, check, "Assersion '{0}' failed at {1}:{2}",
			SPK_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
	#define SPK_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define SPK_INTERNAL_ASSERT_GET_MACRO(...) SPK_EXPAND_MACRO(SPK_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__, SPK_INTERNAL_ASSERT_WITH_MSG, SPK_INTERNAL_ASSERT_NO_MSG))

	// 出错打印，并在错误处中断
	#define SPK_ASSERT(x,...) {if(!(x)) { SPK_ERROR("Assertion Faild: {0}",__VA_ARGS__); SPK_DEBUGBREAK(); }}
	#define SPK_CORE_ASSERT(x,...) {if(!(x)) {SPK_CORE_ERROR("Assertion Faild: {0}",__VA_ARGS__); SPK_DEBUGBREAK(); }}
#else
	#define SPK_ASSERT(x,...)
	#define SPK_CORE_ASSERT(x,...)
#endif // SPK_ENABLE_ASSERT

