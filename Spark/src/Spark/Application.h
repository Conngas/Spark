#pragma once
#include"Core.h"

namespace Spark {
	class SPARK_API Application								// DLL类导出
	{
	public:
		Application();
		virtual ~Application();

	public:
		void Run();
	};

	/// <summary>
	/// 在Spark内定义
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}
