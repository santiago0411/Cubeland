#include "pch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Cubeland
{
	static bool s_Initialized = false;

	static void OpenGLMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				CL_LOG_CRITICAL(message);
				return;
			case GL_DEBUG_SEVERITY_MEDIUM:
				CL_LOG_ERROR(message);
				return;
			case GL_DEBUG_SEVERITY_LOW:
				CL_LOG_WARN(message);
				return;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				CL_LOG_DEBUG(message);
				return;
		}

		CL_ASSERT(false)
	}

	void OpenGLContext::Init(GLFWwindow* window)
	{
		glfwMakeContextCurrent(window);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if (s_Initialized)
			return;

		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CL_ASSERT(success, "Failed to initialize OpenGLContext!")

		CL_LOG_INFO("OpenGL Info:");
		CL_LOG_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		CL_LOG_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		CL_LOG_INFO("  Version: {0}", glGetString(GL_VERSION));

		CL_ASSERT(GLVersion.major > 4 || GLVersion.major == 4 && GLVersion.minor >= 5, "Cubeland requires at least OpenGL version 4.5!");

#if !defined(APP_DIST)
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);

		s_Initialized = true;
	}

	void OpenGLContext::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLContext::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLContext::SetViewport(uint32_t width, uint32_t height)
	{
		glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	}

	void OpenGLContext::SetLineWidth(float width)
	{
		glLineWidth(width);
	}
}
