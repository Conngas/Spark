#pragma once
#include "Spark/Core/Layer.h"

#include "Spark/Event/ApplicationEvent.h"
#include "Spark/Event/KeyEvent.h"
#include "Spark/Event/MouseEvent.h"

namespace Spark {
	
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;
		void OnImGuiRender() override;

		void Begin();
		void End();
		void BlockEvents(bool block) { m_BlockEvent = block; }

		void SetDarkThemeColor();
	private:
		float m_BlockEvent = true;
		float m_Time = 0.0f;
	};
}
