#pragma once
#include "Spark/Core/PlatformDetection.h"

#include <memory>

#ifdef SPK_DEBUG
	#ifdef SPK_PLATFORM_WINDOWS
		#define SPK_DEBUGBREAK() __debugbreak()
	#elif defined(SPK_PLATFORM_LINUX)
		#include <signal.h>
		#define SPK_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform Does Not Support DebugBreak"
	#endif
	#define SPK_ENABLE_ASSERTS
#else
		#define SPK_DEBUGBREAK()
#endif // HZ_DEBUG

#define SPK_EXPAND_MACRO(x) x
#define SPK_STRINGIFY_MACRO(x) #x

// Event---------------
// 左移运算符相当于乘2
#define BIT(x) (1 << x)
// 将函数绑定在对象上，绑定需要自定义域
// #define SPK_BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)
#define SPK_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// Profile//////////////////////////////////////////////////////////////////////////
#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

/// Ref 指针
namespace Spark {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Spark/Core/Log.h"
#include "Spark/Core/Assert.h"
