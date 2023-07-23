#include "spkpch.h"
#include "Application.h"
#include "Log.h" 

#include <glad/glad.h>
#include "Input.h"

namespace Spark {

	// Temp
	// #define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SPK_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		// 创建Window
		m_Window = std::unique_ptr<Window>(Window::Create());
		// 将事件处理程序绑定到WindowData的一个变量，这样通过该变量即可调用该函数
		m_Window->SetEventCallback(SPK_BIND_EVENT_FN(Application::OnEvent));
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
		// 判定对象是否为WindowCloseEvent，若是则执行OnWindowClose操作
		dispatcher.Dispatch<WindowCloseEvent>(SPK_BIND_EVENT_FN(Application::OnWindowClose));

		// Layer部分
		for (auto it = m_LayerStock.end(); it != m_LayerStock.begin();)
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
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStock.PushOverLay(layer);
		layer->OnAttach();
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