#include "spkpch.h"
#include "WindowsWindow.h"

namespace Spark {
	
	static bool s_GLFWInitalized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}
	
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Height = props.Height;
		m_Data.Width = props.Width;
		
		SPK_CORE_INFO("Create window {0} ({1},{2})", props.Title, props.Height, props.Width);

		// �Գ�ʼ���������
		if (!s_GLFWInitalized)
		{
			int success = glfwInit();
			SPK_CORE_ASSERT(success, "Could not initalize GLFW");

			s_GLFWInitalized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enable)
	{
		if (enable)
			// ����ͬ��������ȴ���Ⱦ��ɺ������Ⱦ
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