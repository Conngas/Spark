#include "spkpch.h"
#include "Application.h"
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
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		SPK_CORE_TRACE("{0}", e);

		// Layer����
		for (auto it = m_LayerStock.end(); it != m_LayerStock.end();)
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	/// <summary>
	/// Layer����
	/// </summary>
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStock.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStock.PushOverLay(layer);
	}

	void Application::Run()
	{
		while (m_windowRunning)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// ��ȡlayer�¼�
			for (Layer* layer : m_LayerStock)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_windowRunning = false;
		return true;
	}
}