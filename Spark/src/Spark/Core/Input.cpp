#include "spkpch.h"
#include "Spark/Core/Input.h"

#ifdef SPK_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif // SPK_PLATFORM_WINDOWS

namespace Spark {
	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
		#ifdef SPK_PLATFORM_WINDOWS
			return CreateScope<WindowsInput>();
		#else
			SPK_CORE_ASSERT(false, "Unknown Platform!");
			return nullptr;
		#endif // SPK_PLATFORM_WINDOWS
	}
}