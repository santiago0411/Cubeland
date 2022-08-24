#pragma once

#include "Core/Base.h"
#include "Events/Event.h"

#include <glm/vec2.hpp>

struct GLFWwindow;

namespace Cubeland
{
	using EventCallbackFn = std::function<void(Event&)>;

	class Window
	{
	public:
		Window(uint32_t width, uint32_t height);
		~Window();

		static Scope<Window> Create(uint32_t width, uint32_t height);

		void OnUpdate() const;

		void SetEventCallback(const EventCallbackFn& callback);
		void SetVSync(bool enabled);
		bool IsVSync() const;

		glm::vec2 GetWidthAndHeight() const { return { m_Data.Width, m_Data.Height }; }

		void* GetNativeWindow() const { return m_Window; }

	private:
		void CreateGlfwWindow();
		void SetEvents() const;

	private:
		GLFWwindow* m_Window = nullptr;

		struct WindowData
		{
			std::string Title = "Cubeland";
			uint32_t Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
