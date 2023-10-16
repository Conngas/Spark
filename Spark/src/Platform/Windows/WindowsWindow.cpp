#include "spkpch.h"
#include "Platform/Windows/WindowsWindow.h"

#include "Spark/Event/ApplicationEvent.h"
#include "Spark/Event/KeyEvent.h"
#include "Spark/Event/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Spark {
	
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		SPK_CORE_ERROR("GLFW Error ({0}),{1}", error, description);
	}
	
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		SPK_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		SPK_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		SPK_PROFILE_FUNCTION();

		m_Data.Title = props.Title;
		m_Data.Height = props.Height;
		m_Data.Width = props.Width;

		SPK_CORE_INFO("Create window {0} ({1},{2})", props.Title, props.Height, props.Width);
				
		// 对初始化情况断言
		if (s_GLFWWindowCount == 0)
		{
			SPK_PROFILE_SCOPE("glfwInit");

			int success = glfwInit();
			SPK_CORE_ASSERT(success, "Could not initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			SPK_PROFILE_SCOPE("glCreateWindow");

			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// 设置GLFW回调，传入临时函数
		// 设置窗口大小
		glfwSetWindowSizeCallback(	m_Window, [](GLFWwindow* window, int width, int height)
									{WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
									data.Height = height;
									data.Width = width;
									WindowResizeEvent event(width, height);
									data.EventCallback(event);});
		// 窗口关闭
		glfwSetWindowCloseCallback(	m_Window, [](GLFWwindow* window)
									{WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
									WindowCloseEvent event;
									data.EventCallback(event);});
		// 按键事件
		glfwSetKeyCallback(	m_Window,[](GLFWwindow* window,int key,int scancode,int action,int mode)
						  {WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
							switch (action)
							{
							case GLFW_PRESS:
								{
									KeyPressedEvent event(key, 0);
									data.EventCallback(event);
									break;
								}
							case GLFW_RELEASE:
								{
									KeyReleasedEvent event(key);
									data.EventCallback(event);
									break;
								}
							case GLFW_REPEAT:
								{
									KeyPressedEvent event(key, 1);
									data.EventCallback(event);
									break;
								}
							}});
		// 按键按下事件
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
							{WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
							KeyTypedEvent event(keycode);
							data.EventCallback(event);});

		// 鼠标事件
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window,int button,int action,int mods)
								{WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
								switch (action)
								{
								case GLFW_PRESS:
									{
										MouseButtonPressedEvent event(button);
										data.EventCallback(event);
										break;
									}
								case GLFW_RELEASE:
									{
										MouseButtonReleasedEvent event(button);
										data.EventCallback(event);
										break;
									}
								}});
		// 鼠标滚轮事件
		glfwSetScrollCallback(	m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
								{WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
								MouseScrollEvent event((float)xOffset, (float)yOffset);
								data.EventCallback(event); });
		// 鼠标指针事件
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
								{WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
								MouseMovedEvent event((float)xPos, (float)yPos);
								data.EventCallback(event); });

	}

	void WindowsWindow::Shutdown()
	{
		SPK_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			// GL清理资源
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		SPK_PROFILE_FUNCTION();

		// 调用事件（输入的EventCallback用于调用）
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enable)
	{
		SPK_PROFILE_FUNCTION();

		if (enable)
			// 开启同步，必须等待渲染完成后继续渲染
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enable;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}