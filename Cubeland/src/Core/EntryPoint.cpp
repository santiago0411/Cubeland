#include "pch.h"
#include "Core/Application.h"
#include "Core/Log.h"

namespace Cubeland
{
	int Main(int argc, char** argv)
	{
		Log::Init();
		auto* app = new Application({ argc, argv });
		app->Run();
		delete app;
		return 0;
	}
}

#if defined(APP_DIST)

#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return Cubeland::Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return Cubeland::Main(argc, argv);
}

#endif