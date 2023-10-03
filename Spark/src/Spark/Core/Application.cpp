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
		// 创建Window
		m_Window = Window::Create();
		// 将事件处理程序绑定到WindowData的一个变量，这样通过该变量即可调用该函数
		m_Window->SetEventCallback(SPK_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		// Layer 层
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		SPK_PROFILE_FUNCTION();

		Renderer::ShutDown();
	} 

	/// <summary>
	/// 事件响应函数
	/// </summary>
	/// <param name="e"></param>
	void Application::OnEvent(Event& e)
	{
		SPK_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		// 判定对象是否为WindowCloseEvent，若是则执行OnWindowClose操作
		dispatcher.Dispatch<WindowCloseEvent>(SPK_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(SPK_BIND_EVENT_FN(Application::OnWindowResize));

		// Layer部分
		for (auto it = m_LayerStock.rbegin(); it != m_LayerStock.rend(); ++it)
		{
			if (e.m_Handled)
				break;
			(*it)->OnEvent(e);
		}
	}
	
	/// <summary>
	/// Layer部分
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

			// Platform 实现，获取DeltaTime
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_minimized)
			{
				{
					SPK_PROFILE_SCOPE("LayerStock::LayerOnUpdate");

					// 获取layer事件
					for (Layer* layer : m_LayerStock)
						layer->OnUpdate(timestep);
				}
			}

			// 最小化时UI层正常更新
			m_ImGuiLayer->Begin();
			{
				SPK_PROFILE_SCOPE("LayerStock::OnImGuiRenderer");

				for (Layer* layer : m_LayerStock)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}
			// 主程序循环类型
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