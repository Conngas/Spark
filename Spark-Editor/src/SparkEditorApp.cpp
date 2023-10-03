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
			// ���Բ���
			// PushLayer(new ExampleLayer());		
			// 2D ��Ⱦ��׼��
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
