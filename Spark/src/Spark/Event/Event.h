#pragma once
#include "spkpch.h"

#include "Spark/Core/Base.h"


namespace Spark {
	// ���¼�����ʱ�����������ַ�����
	// δ���Ľ��ɽ��¼����뻺����У������¼����벿�ִ���

	/// <summary>
	/// �¼�����ö��
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
	/// �¼����ͣ�����ƫ�ƣ�BIT(0)=1,BIT(1)=2,BIT(2)=4,BIT(3)=6
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

// ��ʵ���趨�¼�����
#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() {return EventType::type; }\
								virtual EventType GetEventType() const override {return GetStaticType(); }\
								virtual const char* GetName() const override {return #type; }

// ��ʵ���趨�¼�����
#define EVENT_CLASS_CATEGORY(categoary)	virtual int GetCategoryFlags() const override {return categoary; }

	/// <summary>
	/// �¼�ϵͳ����
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
		// ������Ӧ��Ϣ��Խ��
		bool m_Handled = false;

	};

	/// <summary>
	/// �¼��ַ�ϵͳ�����뺯��func�ַ�����Ӧ�ĺ�������
	/// </summary>
	class EventDispatcher
	{
		template<typename T>
		// std::function����������T����boolֵ���ɵ������������ĳ���¼�����boolֵ
		using Eventfn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : m_Event(event) {}
		
		template<typename T>
		bool Dispatch(Eventfn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				// ����func����
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;				
			}
			return false;
		}

	private:
		Event& m_Event;		
	};

	/// <summary>
	/// ���ض�Event�����������
	/// </summary>
	/// <param name="os"></param>
	/// <param name="e"></param>
	/// <returns></returns>
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}

