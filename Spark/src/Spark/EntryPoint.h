#pragma once
#ifdef SPK_PLATFORM_WINDOWS
// 外部声明函数
extern Spark::Application* Spark::CreateApplication();

int main(int argc, char** argv)
{
	printf("Spark Engine\n");
	auto app = Spark::CreateApplication();
	app->Run();
	delete app;
}



#endif // SPK_PLATFORM_WINDOWS

