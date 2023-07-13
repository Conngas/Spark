#include "spkpch.h"
#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Log.h" 
#include "GLFW/glfw3.h"

namespace Spark {
	Application::Application()
	{
		// ´´½¨Window
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_windowRunning)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}