#pragma once

#include "Core/Base.h"
#include "Core/LayerStack.h"
#include "Core/Window.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Utils/Assert.h"

namespace Cubeland
{
	struct CommandLineArguments
	{
		int32_t Count = 0;
		char** Args = nullptr;

		const char* operator[](int32_t index) const
		{
			CL_ASSERT(index < Count)
			return Args[index];
		}
	};

	class Application
	{
	public:
		explicit Application(const CommandLineArguments& args);
		~Application();

		void Run();
		void OnEvent(Event& e);

		const Window& GetWindow() const { return *m_Window; }

		static const Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClosed(WindowClosedEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);

	private:
		static Application* s_Instance;

		Scope<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	};
}

