#include "spkpch.h"
#include "Spark/Core/Application.h"
#include "Spark/Utils/PlatformUtils.h"

#include <sstream>
#include <commdlg.h>
#include <GLFW/glfw3.h>
// 获取父窗口并冻结
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace Spark {
	// Standard Windows OpenFile Option 标准Win文件操作，使用宽字符串保证兼容性
	std::optional<std::wstring> FileDialogs::OpenFile(const WCHAR* filter)
	{
		// 存储文件对话框配置信息
		OPENFILENAMEW openfileName;
		// 定义文件路径字符宽度
		WCHAR szFile[260] = { 0 };
		// 初始化成员为0
		ZeroMemory(&openfileName, sizeof(OPENFILENAMEW));
		// 设置结构大小
		openfileName.lStructSize = sizeof(OPENFILENAMEW);
		// Get glfw nativeWindow 获取glfw父窗口以便在新窗口产生时冻结原窗口
		openfileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		// 存储文件路径缓冲区
		openfileName.lpstrFile = szFile;
		// 设置缓冲区大小
		openfileName.nMaxFile = sizeof(szFile);
		// 设置文件过滤器，决定使用哪些文件与否
		openfileName.lpstrFilter = filter;
		// 文件过滤器索引
		openfileName.nFilterIndex = 1;
		// 只能输入存在路径 只能选择存在文件 对话框不改变当前工作目录
		openfileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		// 打开保存文件对话框，点击保存则返回文件路径，W版本用于处理Unicode字符，多文本语言支持
		if (GetOpenFileNameW(&openfileName) == TRUE)
		{
			return openfileName.lpstrFile;
		}
		// 取消操作返回空optional文件
		return std::nullopt;
	}

	std::optional<std::wstring> FileDialogs::SaveFile(const WCHAR* filter)
	{
		OPENFILENAMEW openFileName;
		WCHAR szFile[260] = { 0 };
		ZeroMemory(&openFileName, sizeof(OPENFILENAMEW));
		openFileName.lStructSize = sizeof(OPENFILENAMEW);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		openFileName.lpstrFile = szFile;
		openFileName.nMaxFile = sizeof(szFile);
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		// 未指定名称时，使用默认的扩展名称存储文件
		openFileName.lpstrDefExt = std::wcschr(filter, L'\0') + 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameW(&openFileName) == TRUE)
		{
			return openFileName.lpstrFile;
		}
		return std::nullopt;
	}
}
