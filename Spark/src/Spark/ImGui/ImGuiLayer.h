#pragma once
#include "Spark/Core/Layer.h"

#include "Spark/Event/ApplicationEvent.h"
#include "Spark/Event/KeyEvent.h"
#include "Spark/Event/MouseEvent.h"

namespace Spark {
	
	class SPARK_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
		void BlockEvents(bool block) { m_BlockEvent = block; }
	private:
		float m_BlockEvent = true;
		float m_Time = 0.0f;
	};
}
