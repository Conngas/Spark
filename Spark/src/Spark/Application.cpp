#include "spkpch.h"
#include "Application.h"
#include "Log.h" 
#include "GLFW/glfw3.h"

namespace Spark {

#define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)

	Application::Application()
	{
		// 创建Window
		m_Window = std::unique_ptr<Window>(Window::Create());
		// 将事件处理程序绑定到WindowData的一个变量，这样通过该变量即可调用该函数
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{

	}

	/// <summary>
	/// 事件响应函数
	/// </summary>
	/// <param name="e"></param>
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		SPK_CORE_TRACE("{0}", e);

		// Layer部分
		for (auto it = m_LayerStock.end(); it != m_LayerStock.end();)
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	/// <summary>
	/// Layer部分
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

			// 获取layer事件
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