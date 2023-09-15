#pragma once

#include "Spark/Event/Event.h"

namespace Spark {

	/// <summary>
	/// KeyEvent����
	/// </summary>
	class SPARK_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode) : m_KeyCode(keycode){}

		int m_KeyCode;
	};

	/// <summary>
	/// ���̰����¼�
	/// </summary>
	class SPARK_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode,int repeatCount): KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent:" << m_KeyCode << "(" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	/// <summary>
	/// �����ͷ��¼�
	/// </summary>
	class SPARK_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode){}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	/// <summary>
	/// ���̰����¼�
	/// </summary>
	class SPARK_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent:" << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}

