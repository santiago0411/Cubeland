#include "pch.h"
#include "Application.h"

#include "Core/Base.h"

#include "Game/GameLayer.h"

#include "Rendering/OpenGLContext.h"
#include "Rendering/Renderer.h"

#include <GLFW/glfw3.h>
#include <ranges>

namespace Cubeland
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const CommandLineArguments& args)
	{
		CL_LOG_DEBUG("Starting Application.");
		CL_ASSERT(!s_Instance, "APPLICATION INSTANCE ALREADY EXISTS.");
		s_Instance = this;

		m_Window = Window::Create(1920, 1080);
		m_Window->SetEventCallback(CL_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_LayerStack.PushLayer(new GameLayer());
		CL_LOG_DEBUG("Application start up successful.");
	}

	Application::~Application()
	{
		CL_LOG_DEBUG("Shutting down application.");
		Renderer::Shutdown();
		CL_LOG_DEBUG("Application shutdown successful.");
	}

	void Application::Run()
	{
		while (m_Running)
		{
			const auto time = (float)glfwGetTime();
			const float ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (m_Minimized)
				continue;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(ts);

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(CL_BIND_EVENT_FN(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizedEvent>(CL_BIND_EVENT_FN(Application::OnWindowResized));

		for (const auto& layer : std::ranges::reverse_view(m_LayerStack))
		{
			if (e.Handled)
				break;
			layer->OnEvent(e);
		}
	}

	bool Application::OnWindowClosed(WindowClosedEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizedEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		OpenGLContext::SetViewport(e.GetWidth(), e.GetHeight());
		return false;
	}
}
