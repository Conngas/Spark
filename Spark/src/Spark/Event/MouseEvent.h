#pragma once

#include "Spark/Event/Event.h"
#include "Spark/Core/MouseCodes.h"

namespace Spark {
	/// <summary>
	/// 鼠标移动事件
	/// </summary>
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x,const float y):m_MouseX(x),m_MouseY(y){}
		
		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouse)
	private:
		float m_MouseX, m_MouseY;
	};

	/// <summary>
	/// 鼠标滚轮事件
	/// </summary>
	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(const float xOffset,const float yOffset):m_XOffset(xOffset),m_YOffset(yOffset){}

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
	/// 鼠标按钮事件基类
	/// </summary>
	class MouseButtonEvent : public Event
	{
	public:
		MouseCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	
	protected:
		MouseButtonEvent(MouseCode button) : m_Button(button){}
		MouseCode m_Button;
	};

	/// <summary>
	/// 鼠标按钮按下事件
	/// </summary>
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode button): MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	/// <summary>
	/// 鼠标按钮释放事件
	/// </summary>
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}
