#pragma once

#include <mutex>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Spark/Core/Log.h"

namespace Spark {
	struct ProfileResult
	{
		std::string Name;
		std::chrono::duration<double, std::micro> Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			// 开始后锁住线程
			std::lock_guard lock(m_Mutex);
			// 存在线程
			if (m_CurSession)
			{
				if (Log::GetCoreLogger())
					SPK_CORE_ERROR("Instrumentor::BeginSession('{0}') When Session '{1}' Already Open.", name, m_CurSession->Name);
				InternalEndSession();
			}
			m_OutputStream.open(filepath);
			m_CurSession = new InstrumentationSession{ name };
			if (m_OutputStream.is_open())
			{
				m_CurSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else
			{
				if (Log::GetCoreLogger())
				{
					SPK_CORE_ERROR("Instrumentor Could Not Open Result File '{0}'.", filepath);
				}
			}
		}

		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			json << std::setprecision(3) << std::fixed;
			json << "{";
			json << "\"cat\":\"function\", ";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_CurSession)
			{
				// 及时更新，防止程序崩溃时数据丢失
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		Instrumentor() : m_CurSession(nullptr) {}
		~Instrumentor() { EndSession(); }

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {}, \"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		void InternalEndSession()
		{
			if (m_CurSession)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurSession;
				m_CurSession = nullptr;
			}
		}
	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = std::chrono::duration<double, std::micro>{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime =	std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()-
								std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({m_Name, highResStart, elapsedTime, std::this_thread::get_id()});

			m_Stopped = true;
		}

	private:
		bool m_Stopped;
		const char* m_Name;
		std::chrono::time_point <std::chrono::steady_clock> m_StartTimepoint;
	};
}


#define SPK_PROFILE 0
#if SPK_PROFILE
// 兼容实现__FUNCSIG__
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define SPK_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define SPK_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define SPK_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define SPK_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define SPK_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define SPK_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define SPK_FUNC_SIG __func__
#else
#define SPK_FUNC_SIG "HZ_FUNC_SIG unknown!"
#endif

#define SPK_PROFILE_BEGIN_SESSION(name,filepath) ::Spark::Instrumentor::Get().BeginSession(name, filepath)
#define SPK_PROFILE_END_SESSION() ::Spark::Instrumentor::Get().EndSession();
#define HZ_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::Hazel::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
																			::Hazel::InstrumentationTimer timer##line(fixedName##line.Data)
#define HZ_PROFILE_SCOPE_LINE(name, line) HZ_PROFILE_SCOPE_LINE2(name, line)
#define HZ_PROFILE_SCOPE(name) HZ_PROFILE_SCOPE_LINE(name, __LINE__)
#define SPK_PROFILE_FUNCTION() SPK_PROFILE_SCOPE(SPK_FUNC_SIG)
#else
#define SPK_PROFILE_BEGIN_SESSION
#define SPK_PROFILE_END_SESSION()
#define SPK_PROFILE_SCOPE(name)
#define SPK_PROFILE_FUNCTION()
#endif