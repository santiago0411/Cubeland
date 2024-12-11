#include "pch.h"
#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include "Rendering/OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Cubeland
{
	static bool s_glfwInitialized = false;

	static void glfwErrorCallback(int32_t error, const char* description)
	{
		CL_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Scope<Window> Window::Create(uint32_t width, uint32_t height)
	{
		return CreateScope<Window>(width, height);
	}

	Window::Window(uint32_t width, uint32_t height)
	{
		m_Data.Width = width;
		m_Data.Height = height;

		if (!s_glfwInitialized)
		{
			int success = glfwInit();
			CL_ASSERT(success, "Failed to initialize GLFW!");
			glfwSetErrorCallback(glfwErrorCallback);
			s_glfwInitialized = true;
		}

#if defined(APP_DEBUG)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
		CreateGlfwWindow();
		OpenGLContext::Init(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
		SetEvents();
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		s_glfwInitialized = false;
	}

	void Window::OnUpdate() const
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::SetEventCallback(const EventCallbackFn& callback)
	{
		m_Data.EventCallback = callback;
	}

	void Window::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled);
		m_Data.VSync = enabled;
	}

	bool Window::IsVSync() const
	{
		return m_Data.VSync;
	}

	bool Window::IsFocused() const
	{
		return glfwGetWindowAttrib(m_Window, GLFW_FOCUSED);
	}

	void Window::CreateGlfwWindow()
	{
		int32_t count;
		int32_t monitorX, monitorY;

		// Get the monitor viewport and the video mode spec
		GLFWmonitor** monitors = glfwGetMonitors(&count);
		glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);
		const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);

		// Set the visibility hint to false for the window creation
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		// Create the glfw window
		m_Window = glfwCreateWindow((int32_t)m_Data.Width, (int32_t)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

		// Set its position to be centered based on the width and height of the monitor
		glfwSetWindowPos(m_Window,
			monitorX + (int32_t)((videoMode->width - m_Data.Width) / 2),
			monitorY + (int32_t)((videoMode->height - m_Data.Height) / 2));

		// Finally show the window
		glfwShowWindow(m_Window);
	}

	void Window::SetEvents() const
	{
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int32_t width, int32_t height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = (uint32_t)width;
			data.Height = (uint32_t)height;

			WindowResizedEvent event(data.Width, data.Height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowClosedEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int32_t key, int32_t scanCode, int32_t action, int32_t modes)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event((KeyCode)key, false);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event((KeyCode)key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event((KeyCode)key, true);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event((KeyCode)keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int32_t button, int32_t action, int32_t modes)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event((MouseButton)button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event((MouseButton)button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffSet)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffSet);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}
}
