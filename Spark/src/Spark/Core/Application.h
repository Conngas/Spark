#pragma once
#include "Spark/Core/Core.h"
#include "Spark/Core/Window.h"
#include "Spark/Core/Timestep.h"
#include "Spark/Core/LayerStack.h"

#include "Spark/Event/Event.h"
#include "Spark/Event/ApplicationEvent.h"

#include "Spark/ImGui/ImGuiLayer.h"

namespace Spark {
	class Application
	{
	public:
		Application();
		virtual ~Application();

	public:
		void Run();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);


		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_windowRunning = true;
		bool m_minimized = false;
		LayerStack m_LayerStock;
		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;
	private:
		// 单个应用仅有一个App，因此采取静态实例
		static Application* s_Instance;
	};

	/// <summary>
	/// 在Spark内定义
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}
