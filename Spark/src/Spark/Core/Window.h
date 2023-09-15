#pragma once

#include "Spark/Event/Event.h"

namespace Spark {

	// ��������
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Spark Engine",
					unsigned int width = 1280,
					unsigned int height = 720) : Title(title),Width(width),Height(height)
		{}
	};


	// ����ϵͳ�Ĵ���API
	class SPARK_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}
		virtual void OnUpdate() {}
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window ������ʵ��
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		// �ṩWindowָ�����
		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}


