#pragma once
#ifdef SPK_PLATFORM_WINDOWS
// �ⲿ��������
extern Spark::Application* Spark::CreateApplication();

int main(int argc, char** argv)
{
	printf("Spark Engine\n");
	auto app = Spark::CreateApplication();
	app->Run();
	delete app;
}



#endif // SPK_PLATFORM_WINDOWS

