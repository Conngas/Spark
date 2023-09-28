#pragma once
#include <memory>

#ifdef SPK_PLATFORM_WINDOWS
#if SPARK_DYNAMIC_LINK
	#ifdef SPK_BUILD_DLL
		#define SPARK_API __declspec(dllexport)
	#else
		#define SPARK_API __declspec(dllimport)
	#endif // SPK_BUILD_DLL
#else
	#define SPARK_API
#endif
#else
	#error Spark only support windows!
#endif // SPK_PLATFROM_WINDOWS

#ifdef SPK_DEBUG
	#if defined(SPK_PLATFORM_WINDOWS)
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
#define SPK_BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)

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