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
		// ����Window
		m_Window = std::unique_ptr<Window>(Window::Create());
		// ���¼��������󶨵�WindowData��һ������������ͨ���ñ������ɵ��øú���
		m_Window->SetEventCallback(SPK_BIND_EVENT_FN(Application::OnEvent));
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
		// �ж������Ƿ�ΪWindowCloseEvent��������ִ��OnWindowClose����
		dispatcher.Dispatch<WindowCloseEvent>(SPK_BIND_EVENT_FN(Application::OnWindowClose));

		// Layer����
		for (auto it = m_LayerStock.end(); it != m_LayerStock.begin();)
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