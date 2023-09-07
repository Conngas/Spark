#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Spark {
	class SPARK_API Log
	{
	public:
		static void Init();

		// sharedָ���й�
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};
}

// ��Core Logϵͳ���к��װ���﷨����__VA_ARGS__�滻(...)
#define SPK_CORE_TRACE(...)	::Spark::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SPK_CORE_INFO(...)	::Spark::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SPK_CORE_WARN(...)	::Spark::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SPK_CORE_ERROR(...) ::Spark::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SPK_CORE_FATAL(...) ::Spark::Log::GetCoreLogger()->critical(__VA_ARGS__)

// ��Client Logϵͳ���к��װ���﷨����__VA_ARGS__�滻(...)
#define SPK_TRACE(...)		::Spark::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SPK_INFO(...)		::Spark::Log::GetClientLogger()->info(__VA_ARGS__)
#define SPK_WARN(...)		::Spark::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SPK_ERROR(...)		::Spark::Log::GetClientLogger()->error(__VA_ARGS__)
#define SPK_FATAL(...)		::Spark::Log::GetClientLogger()->critical(__VA_ARGS__)





