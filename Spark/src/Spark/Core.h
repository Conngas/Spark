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