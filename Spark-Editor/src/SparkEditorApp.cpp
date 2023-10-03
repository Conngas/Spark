#include "Spark.h"
#include "Spark/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace Spark {
	class SparkEditor : public Spark::Application
	{
	public:
		SparkEditor()
			: Application("Spark Editor")
		{
			// 测试部分
			// PushLayer(new ExampleLayer());		
			// 2D 渲染器准备
			PushLayer(new EditorLayer());
		}

		~SparkEditor()
		{

		}
	};

	Spark::Application* Spark::CreateApplication()
	{
		return new SparkEditor();
	}
}
