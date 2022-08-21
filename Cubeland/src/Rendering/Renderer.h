#pragma once

#include "Rendering/Buffer.h"

namespace Cubeland
{
	class Renderer
	{
	public:
		static void DrawIndexed(const Ref<VertexArray>& vertexArray);
	};
}
