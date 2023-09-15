#include "spkpch.h"

#include <glad/glad.h>
#include "glfw/glfw3.h"

#include "Spark/Core/Log.h" 
#include "Spark/Core/Input.h"
#include "Spark/Core/Application.h"
#include "Spark/Renderer/Renderer.h"

namespace Spark {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SPK_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		// ����Window
		m_Window = Window::Create();
		// ���¼��������󶨵�WindowData��һ������������ͨ���ñ������ɵ��øú���
		m_Window->SetEventCallback(SPK_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		// Layer ��
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		Renderer::ShutDown();
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
		dispatcher.Dispatch<WindowResizeEvent>(SPK_BIND_EVENT_FN(Application::OnWindowResize));

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
			// Platform ʵ�֣���ȡDeltaTime
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_minimized)
			{
				// ��ȡlayer�¼�
				for (Layer* layer : m_LayerStock)
					layer->OnUpdate(timestep);
			}
			// UI�����
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStock)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			// ������ѭ������
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_windowRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_minimized = true;
			return false;
		}
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		m_minimized = false;
		return false;
	}
}