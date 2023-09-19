#include "spkpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <typeinfo>

namespace Spark {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		SPK_CORE_ASSERT(windowHandle, "Window Handle is null !")
	}

	void OpenGLContext::Init()
	{
		SPK_PROFILE_FUNCTION();

		char* str = "This is a test";

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SPK_CORE_ASSERT(status, "Failed to initialize Glad!");

		SPK_CORE_INFO("OpenGL Renderer Info :");
		SPK_CORE_INFO("  Vender:\t{0}", (char*)glGetString(GL_VENDOR));
		SPK_CORE_INFO("  Renderer:\t{0}", (char*)glGetString(GL_RENDERER));
		SPK_CORE_INFO("  Version:\t{0}", (char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		SPK_PROFILE_FUNCTION();

		glfwSwapBuffers(m_windowHandle);
	}

}