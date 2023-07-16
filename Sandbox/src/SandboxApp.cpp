#include "spkpch.h"
#include <Spark.h>

class ExampleLayer : public Spark::Layer
{
public:
	ExampleLayer() :Layer("Example")
	{

	}

	void OnUpdate() override
	{
		SPK_INFO("Example::Update");
	}

	void OnEvent(Spark::Event& event) override
	{
		SPK_TRACE("{0}", event);
	}
};


/// <summary>
/// �̳�Spark�����е�Applicationʵ��
/// </summary>
class Sandbox : public Spark::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Spark::Application* Spark::CreateApplication()
{
	return new Sandbox();
}
