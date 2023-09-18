#include "spkpch.h"
#include "Spark/Core/Window.h"

#ifdef SPK_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif // SPK_PLATFORM_WINDOWS

namespace Spark {

	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef SPK_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		SPK_CORE_ASSERT(false, "Unknown Platform!");
		return nullptr;
	#endif // SPK_PLATFORM_WINDOWS
	}
}