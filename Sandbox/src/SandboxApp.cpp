#include "Spark.h"
#include "Spark/Core/EntryPoint.h"

#include "Sandbox2D.h"
#include "ExampleLayer.h"

/// <summary>
/// �̳�Spark�����е�Applicationʵ��
/// </summary>
class Sandbox : public Spark::Application
{
public:
	Sandbox()
	{
		// ���Բ���
		// PushLayer(new ExampleLayer());		
		// 2D ��Ⱦ��׼��
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
