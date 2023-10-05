#pragma once
#include "spkpch.h"

#include "Spark/Core/Base.h"


namespace Spark {
	// 当事件发生时，将会立即分发处理
	// 未来改进可将事件放入缓冲池中，并在事件代码部分处理

	/// <summary>
	/// 事件类型枚举
	/// </summary>
	enum class EventType
	{
		None = 0,
		// Window------------
		WindowClose,WindowResize,WindowFocus,WindowLostFocus,WindowMoved,
		// App---------------
		AppTick,AppUpdate,AppRender,
		// Key---------------
		KeyPressed,KeyReleased,KeyTyped,
		// Mouse-------------
		MouseButtonPressed,MouseButtonReleased,MouseMoved,MouseScrolled
	};

	/// <summary>
	/// 事件类型，左移偏移，BIT(0)=1,BIT(1)=2,BIT(2)=4,BIT(3)=6
	/// </summary>
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),		
		EventCategoryInput			= BIT(1),		
		EventCategoryKeyboard		= BIT(2),		
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

// 宏实现设定事件动作
#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() {return EventType::type; }\
								virtual EventType GetEventType() const override {return GetStaticType(); }\
								virtual const char* GetName() const override {return #type; }

// 宏实现设定事件类型
#define EVENT_CLASS_CATEGORY(categoary)	virtual int GetCategoryFlags() const override {return categoary; }

	/// <summary>
	/// 事件系统基类
	/// </summary>
	class SPARK_API Event
	{
		friend class EventDispatcher;
	public:
		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
		// 控制相应信息不越界
		bool m_Handled = false;

	};

	/// <summary>
	/// 事件分发系统，传入函数func分发给对应的函数处理
	/// </summary>
	class EventDispatcher
	{
		template<typename T>
		// std::function别名，接收T返回bool值，可调用任意对象处理某个事件返回bool值
		using Eventfn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : m_Event(event) {}
		
		template<typename T>
		bool Dispatch(Eventfn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				// 调用func函数
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;				
			}
			return false;
		}

	private:
		Event& m_Event;		
	};

	/// <summary>
	/// 重载对Event的输出操作符
	/// </summary>
	/// <param name="os"></param>
	/// <param name="e"></param>
	/// <returns></returns>
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}

