#include "spkpch.h"
#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Log.h" 

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
			m_Window->OnUpdate();
		}
	}
}