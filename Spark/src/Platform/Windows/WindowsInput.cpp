#include "spkpch.h"
#include "Spark/Core/Application.h"
#include "Platform/Windows/WindowsInput.h"

#include <GLFW/glfw3.h>

namespace Spark {

	// 初始化Instance
	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();


	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto statue = glfwGetKey(window, keycode);
		return statue == GLFW_PRESS || statue == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto statue = glfwGetMouseButton(window, button);
		return statue == GLFW_PRESS || statue == GLFW_REPEAT;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos,(float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		// C++14 实现
		// auto v = GetMousePositionImpl();
		// return std::get<0>(v);
		// C++17
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}