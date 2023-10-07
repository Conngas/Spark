#pragma once
#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
		#define SPK_PLATFORM_WINDOWS
	#else
		#error "x86 Builds Are NOT Supported!"
	#endif // _WIN32	
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditions.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS Simulator Is NOT Supported!"
	#elef TARGET_OS_IPHONE == 1
		#define SPK_PLATFORM_IOS
		#error "IOS Is NOT Supported!"
	#elif TARGET_OS_MAC == 1
		#define SPK_PLATFORM_MACOS
		#error "MACOS Is NOT Supported!"
	#else
		#error "Unknown Apple Platform!"
	#endif
#elif #define(__ANDROID__)
	#define SPK_PLATFORM_ANDROID
	#error "Android Is NOT Supported!"
#elif #define(__Linux__)
	#define SPK_PLATFORM_LINUX
	#error "Linux Is NOT Supported!"
#else
	#error "Unknown Platform!"
#endif

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

// TODO Mirco 实现参数输入
#ifdef SPK_ENABLE_ASSERTS
	// 出错打印，并在错误处中断
	#define SPK_ASSERT(x,...) {if(!(x)) { SPK_ERROR("Assertion Faild: {0}",__VA_ARGS__); SPK_DEBUGBREAK(); }}
	#define SPK_CORE_ASSERT(x,...) {if(!(x)) {SPK_CORE_ERROR("Assertion Faild: {0}",__VA_ARGS__); SPK_DEBUGBREAK(); }}
#else
	#define SPK_ASSERT(x,...)
	#define SPK_CORE_ASSERT(x,...)
#endif // SPK_ENABLE_ASSERTS	

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