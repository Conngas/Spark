#include "Spark.h"
#include "Spark/Core/EntryPoint.h"

#include "Sandbox2D.h"
#include "ExampleLayer.h"

/// <summary>
/// 继承Spark核心中的Application实现
/// </summary>
class Sandbox : public Spark::Application
{
public:
	Sandbox()
	{
		// 测试部分
		// PushLayer(new ExampleLayer());		
		// 2D 渲染器准备
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Spark::Application* Spark::CreateApplication()
{
	return new Sandbox();
}
