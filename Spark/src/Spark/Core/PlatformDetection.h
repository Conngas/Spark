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
#endif // End of Detection