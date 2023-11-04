#pragma once
#include <string.h>
#include <optional>

namespace Spark {

	class FileDialogs
	{
	public:
		// 使用optional现代方法返回操作结果
		static std::optional<std::wstring> OpenFile(const WCHAR* filter);
		static std::optional<std::wstring> SaveFile(const WCHAR* filter);
	};

}
