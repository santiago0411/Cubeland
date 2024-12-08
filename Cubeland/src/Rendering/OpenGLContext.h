#pragma once

#include <glm/vec4.hpp>

#include "Rendering/Buffer.h"

struct GLFWwindow;

namespace Cubeland
{
	class OpenGLContext
	{
	public:
		static void Init(GLFWwindow* window);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void SetViewport(uint32_t width, uint32_t height);
		static void SetLineWidth(float width);

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0);
	};
}
