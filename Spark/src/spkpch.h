#pragma once
// 建议在cpp文件中引用此文件，.h文件引用其他文件
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Spark/Log.h"
#include "Spark/Core.h"

#ifdef SPK_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // SPK_PLATFORM_WINDOWS
