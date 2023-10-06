#pragma once

#include "Spark/Event/Event.h"
#include "Spark/Core/Input.h"

namespace Spark {
	/// <summary>
	/// ����ƶ��¼�
	/// </summary>
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x,float y):m_MouseX(x),m_MouseY(y){}
		
		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	/// <summary>
	/// �������¼�
	/// </summary>
	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xOffset,float yOffset):m_XOffset(xOffset),m_YOffset(yOffset){}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

	private:
		float m_XOffset, m_YOffset;
	};

	/// <summary>
	/// ��갴ť�¼�����
	/// </summary>
	class MouseButtonEvent : public Event
	{
	public:
		inline MouseCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	
	protected:
		MouseButtonEvent(MouseCode button) : m_Button(button){}
		MouseCode m_Button;
	};

	/// <summary>
	/// ��갴ť�����¼�
	/// </summary>
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseCode button): MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	/// <summary>
	/// ��갴ť�ͷ��¼�
	/// </summary>
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}