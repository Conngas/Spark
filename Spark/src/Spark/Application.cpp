#include "spkpch.h"
#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Log.h" 
#include "GLFW/glfw3.h"

namespace Spark {

#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)

	Application::Application()
	{
		// ����Window
		m_Window = std::unique_ptr<Window>(Window::Create());
		// ���¼��������󶨵�WindowData��һ������������ͨ���ñ������ɵ��øú���
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{

	}

	/// <summary>
	/// �¼���Ӧ����
	/// </summary>
	/// <param name="e"></param>
	void OnEvent(Event& e)
	{
		SPK_CORE_INFO("{0}", e);
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