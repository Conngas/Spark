#pragma once

#ifdef SPK_PLATFORM_WINDOWS
	#ifdef SPK_BUILD_DLL
		#define SPARK_API __declspec(dllexport)
	#else
		#define SPARK_API __declspec(dllimport)
	#endif // SPK_BUILD_DLL
#else
	#error Spark only support windows!
#endif // SPK_PLATFROM_WINDOWS

#ifdef SPK_DEBUG
	#define SPK_ENABLE_ASSERTS
#endif // HZ_DEBUG

#ifdef SPK_ENABLE_ASSERTS
	// �����ӡ�����ڴ����ж�
	#define SPK_ASSERT(x,...) {if(!(x)) { SPK_ERROR("Assertion Faild: {0}",__VA_ARGS__); __debugbreak(); }}
	#define SPK_CORE_ASSERT(x,...) {if(!(x)) {SPK_CORE_ERROR("Assertion Faild: {0}",__VA_ARGS__); __debugbreak(); }}
#else
	#define SPK_ASSERT(x,...)
	#define SPK_CORE_ASSERT(x,...)
#endif // SPK_ENABLE_ASSERTS	

// Event---------------
// ����������൱�ڳ�2
#define BIT(x) (1 << x)
// ���������ڶ����ϣ�����Ҫ�Զ�����
#define SPK_BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)