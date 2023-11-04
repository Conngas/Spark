#pragma once
#include <string.h>
#include <optional>

namespace Spark {

	class FileDialogs
	{
	public:
		// ʹ��optional�ִ��������ز������
		static std::optional<std::wstring> OpenFile(const WCHAR* filter);
		static std::optional<std::wstring> SaveFile(const WCHAR* filter);
	};

}
