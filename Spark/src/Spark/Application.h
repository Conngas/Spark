#pragma once
#include "Core.h"
#include "Window.h"
#include "Event/Event.h"

namespace Spark {
	class SPARK_API Application								// DLL类导出
	{
	public:
		Application();
		virtual ~Application();

	public:
		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_windowRunning = true;
	};

	/// <summary>
	/// 在Spark内定义
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}
