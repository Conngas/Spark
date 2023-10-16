#include "spkpch.h"
#include "Spark/Core/Application.h"
#include "Spark/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Spark {

	bool Input::IsKeyPressed(const KeyCode keycode)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto statue = glfwGetKey(window, static_cast<int32_t>(keycode));
		return statue == GLFW_PRESS || statue == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto statue = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return statue == GLFW_PRESS || statue == GLFW_REPEAT;
	}

	glm::vec2 Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos,(float)ypos };
	}

	float Input::GetMouseX()
	{
		// C++14 й╣ож
		// auto v = GetMousePosition();
		// return std::get<0>(v);
		// C++17
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}
