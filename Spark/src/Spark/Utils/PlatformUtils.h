#pragma once
#include <string.h>

namespace Spark {

	class FileDialogs
	{
	public:
		static std::wstring OpenFile(const WCHAR* filter);
		static std::wstring SaveFile(const WCHAR* filter);
	};

}
