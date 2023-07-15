#pragma once
#include "Core.h"
#include "Window.h"
#include "Event/Event.h"

namespace Spark {
	class SPARK_API Application								// DLL�ർ��
	{
	public:
		Application();
		virtual ~Application();

	public:
		void Run();

		void OnEvent(Event& e);
	private:
		std::unique_ptr<Window> m_Window;
		bool m_windowRunning = true;
	};

	/// <summary>
	/// ��Spark�ڶ���
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}
