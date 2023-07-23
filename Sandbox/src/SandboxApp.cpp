#include "spkpch.h"
#include <Spark.h>
#include "imgui/imgui.h"

class ExampleLayer : public Spark::Layer
{
public:
	ExampleLayer() :Layer("Example")
	{

	}

	void OnUpdate() override
	{
		if (Spark::Input::IsKeyPress(SPK_KEY_TAB))
			SPK_TRACE("Key TABLE Pressed (Poll)");
	}

	//virtual void OnImGuiRender() override
	//{
	//	ImGui::Begin("Test");
	//	ImGui::Text("Hello World");
	//	ImGui::End();
	//}

	void OnEvent(Spark::Event& event) override
	{
		if ((event.GetEventType() == Spark::EventType::KeyPressed))
		{
			Spark::KeyPressedEvent& e = (Spark::KeyPressedEvent&)event;
			if (e.GetKeyCode() == SPK_KEY_TAB)
				SPK_TRACE("Key TABLE Pressed (Event)");
			SPK_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};


/// <summary>
/// 继承Spark核心中的Application实现
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
