#pragma once
#ifdef SPK_PLATFORM_WINDOWS
// 外部声明函数
extern Spark::Application* Spark::CreateApplication();

int main(int argc, char** argv)
{
	Spark::Log::Init();
	SPK_CORE_WARN("Initialized Log!");
	int a = 100;
	SPK_INFO("Hello! {0}",a);

	auto app = Spark::CreateApplication();
	app->Run();
	delete app;
}



#endif // SPK_PLATFORM_WINDOWS

