#include "spkpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Spark/Core/Log.h" 
#include "Spark/Core/Input.h"
#include "Spark/Core/Application.h"
#include "Spark/Renderer/Renderer.h"

namespace Spark {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SPK_PROFILE_FUNCTION();

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
		SPK_PROFILE_FUNCTION();

		Renderer::ShutDown();
	} 

	/// <summary>
	/// �¼���Ӧ����
	/// </summary>
	/// <param name="e"></param>
	void Application::OnEvent(Event& e)
	{
		SPK_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		// �ж������Ƿ�ΪWindowCloseEvent��������ִ��OnWindowClose����
		dispatcher.Dispatch<WindowCloseEvent>(SPK_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SPK_BIND_EVENT_FN(Application::OnWindowResize));

		// Layer����
		for (auto it = m_LayerStock.rbegin(); it != m_LayerStock.rend(); ++it)
		{
			if (e.m_Handled)
				break;
			(*it)->OnEvent(e);
		}
	}
	
	/// <summary>
	/// Layer����
	/// </summary>
	void Application::PushLayer(Layer* layer)
	{
		SPK_PROFILE_FUNCTION();

		m_LayerStock.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		SPK_PROFILE_FUNCTION();

		m_LayerStock.PushOverLay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_windowRunning = false;
	}

	void Application::Run()
	{
		SPK_PROFILE_FUNCTION();

		while (m_windowRunning)
		{
			SPK_PROFILE_SCOPE("RunLoop");

			// Platform ʵ�֣���ȡDeltaTime
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_minimized)
			{
				{
					SPK_PROFILE_SCOPE("LayerStock::LayerOnUpdate");

					// ��ȡlayer�¼�
					for (Layer* layer : m_LayerStock)
						layer->OnUpdate(timestep);
				}
			}

			// ��С��ʱUI����������
			m_ImGuiLayer->Begin();
			{
				SPK_PROFILE_SCOPE("LayerStock::OnImGuiRenderer");

				for (Layer* layer : m_LayerStock)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}
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
		SPK_PROFILE_FUNCTION();

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