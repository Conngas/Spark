#pragma once
#include "Spark/Core/Base.h"
#include "Spark/Event/Event.h"

#include <sstream>

namespace Spark {

	// 窗口属性
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Spark Engine",
					uint32_t width = 1600,
					uint32_t height = 900) : Title(title),Width(width),Height(height)
		{}
	};


	// 桌面系统的窗口API
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}
		virtual void OnUpdate() {}
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window 抽象函数实现
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;

		// 提供Window指针调用
		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}


