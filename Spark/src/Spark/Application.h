#pragma once
#include"Core.h"

namespace Spark {
	class SPARK_API Application								// DLL�ർ��
	{
	public:
		Application();
		virtual ~Application();

	public:
		void Run();
	};

	/// <summary>
	/// ��Spark�ڶ���
	/// </summary>
	/// <returns></returns>
	Application* CreateApplication();
}
