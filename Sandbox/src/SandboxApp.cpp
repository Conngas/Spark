#include "spkpch.h"
#include <Spark.h>

/// <summary>
/// �̳�Spark�����е�Applicationʵ��
/// </summary>
class Sandbox : public Spark::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Spark::Application* Spark::CreateApplication()
{
	return new Sandbox();
}
