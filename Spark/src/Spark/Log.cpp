#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Spark {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		// 设置语句log格式，参考 https://github.com/gabime/spdlog/wiki/1.-QuickStart
		spdlog::set_pattern("%^[%T] %n: %v%$");

		// 核心代码颜色等级设置
		s_CoreLogger = spdlog::stdout_color_mt("Spark");
		s_CoreLogger->set_level(spdlog::level::trace);
		// 客户端代码设置
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}