#pragma once
#include "Core.h"
#include "Window.h"
#include "Event/Event.h"
#include "LayerStack.h"
#include "Event/ApplicationEvent.h"

#include "Spark/ImGui/ImGuiLayer.h"

namespace Spark {
	class SPARK_API Application								// DLL类导出
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

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_windowRunning = true;
		LayerStack m_LayerStock;
		// 单个应用仅有一个App，因此采取静态实例
		static Application* s_Instance;
	};

	/// <summary>
	/// 在Spark内定义
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}
