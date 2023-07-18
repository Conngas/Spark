#pragma once
#include "Spark/Layer.h"

#include "Spark//Event/ApplicationEvent.h"
#include "Spark/Event/KeyEvent.h"
#include "Spark/Event/MouseEvent.h"

namespace Spark {
	
	class SPARK_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
		// ½»»¥ÊÂ¼þ
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrollEvent(MousScrollEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		// bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		float m_Time = 0.0f;
	};
}
