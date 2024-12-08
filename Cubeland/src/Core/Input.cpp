#include "pch.h"
#include "Input.h"

#include "Core/Application.h"

#include <GLFW/glfw3.h>

#define GET_NATIVE_WINDOW() static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow())

namespace Cubeland
{
	bool Input::IsKeyPressed(const KeyCode key)
	{
		GLFWwindow* window = GET_NATIVE_WINDOW();
		return glfwGetKey(window, (int)key) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(const MouseButton button)
	{
		GLFWwindow* window = GET_NATIVE_WINDOW();
		return glfwGetMouseButton(window, (int)button);
	}

	glm::vec2 Input::GetMousePosition()
	{
		GLFWwindow* window = GET_NATIVE_WINDOW();
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		GLFWwindow* window = GET_NATIVE_WINDOW();
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
	}

	CursorMode Input::GetCursorMode()
	{
		GLFWwindow* window = GET_NATIVE_WINDOW();
		return (CursorMode)(glfwGetInputMode(window, GLFW_CURSOR) - GLFW_CURSOR_NORMAL);
	}
}
