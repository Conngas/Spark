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

// TODO Mirco ʵ�ֲ�������
#ifdef SPK_ENABLE_ASSERTS
	// �����ӡ�����ڴ����ж�
	#define SPK_ASSERT(x,...) {if(!(x)) { SPK_ERROR("Assertion Faild: {0}",__VA_ARGS__); SPK_DEBUGBREAK(); }}
	#define SPK_CORE_ASSERT(x,...) {if(!(x)) {SPK_CORE_ERROR("Assertion Faild: {0}",__VA_ARGS__); SPK_DEBUGBREAK(); }}
#else
	#define SPK_ASSERT(x,...)
	#define SPK_CORE_ASSERT(x,...)
#endif // SPK_ENABLE_ASSERTS	

// Event---------------
// ����������൱�ڳ�2
#define BIT(x) (1 << x)
// ���������ڶ����ϣ�����Ҫ�Զ�����
// #define SPK_BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)
#define SPK_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// Profile//////////////////////////////////////////////////////////////////////////
#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

/// Ref ָ��
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