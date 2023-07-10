#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Spark {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		// �������log��ʽ���ο� https://github.com/gabime/spdlog/wiki/1.-QuickStart
		spdlog::set_pattern("%^[%T] %n: %v%$");

		// ���Ĵ�����ɫ�ȼ�����
		s_CoreLogger = spdlog::stdout_color_mt("Spark");
		s_CoreLogger->set_level(spdlog::level::trace);
		// �ͻ��˴�������
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}