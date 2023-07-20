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
	// 出错打印，并在错误处中断
	#define SPK_ASSERT(x,...) {if(!(x)) { SPK_ERROR("Assertion Faild: {0}",__VA_ARGS__); __debugbreak(); }}
	#define SPK_CORE_ASSERT(x,...) {if(!(x)) {SPK_CORE_ERROR("Assertion Faild: {0}",__VA_ARGS__); __debugbreak(); }}
#else
	#define SPK_ASSERT(x,...)
	#define SPK_CORE_ASSERT(x,...)
#endif // SPK_ENABLE_ASSERTS	

// Event---------------
// 左移运算符相当于乘2
#define BIT(x) (1 << x)
// 将函数绑定在对象上，绑定需要自定义域
#define SPK_BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)