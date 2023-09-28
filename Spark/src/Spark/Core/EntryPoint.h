#pragma once
#include "Spark/Core/Base.h"
#ifdef SPK_PLATFORM_WINDOWS
// 外部声明函数
extern Spark::Application* Spark::CreateApplication();

int main(int argc, char** argv)
{
	Spark::Log::Init();
	SPK_CORE_WARN("Initialized Log!");

	SPK_PROFILE_BEGIN_SESSION("StarUp", "SparkProfile-Starup.json");
	auto app = Spark::CreateApplication();
	SPK_PROFILE_END_SESSION();

	SPK_PROFILE_BEGIN_SESSION("Runtime", "SparkProfile-Runtime.json");
	app->Run();
	SPK_PROFILE_END_SESSION();

	SPK_PROFILE_BEGIN_SESSION("ShutDown", "SparkProfile-Shutdown.json");
	delete app;
	SPK_PROFILE_END_SESSION();
}



#endif // SPK_PLATFORM_WINDOWS

