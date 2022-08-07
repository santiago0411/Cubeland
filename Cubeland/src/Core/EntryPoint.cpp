#include "pch.h"
#include "Core/Application.h"
#include "Core/Log.h"

#if defined(APP_DIST)
#include <windows.h>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Cubeland::Log::Init();
	auto* app = new Cubeland::Application({ __argc, __argv });
#else
int main(int argc, char** argv)
{
	Cubeland::Log::Init();
	auto* app = new Cubeland::Application({ argc, argv });
#endif
	app->Run();
	delete app;
	return 0;
}