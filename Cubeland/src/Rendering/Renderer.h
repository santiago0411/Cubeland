#pragma once

#include "Rendering/Camera.h"
#include "Rendering/Buffer.h"
#include "Rendering/StaticMesh.h"

namespace Cubeland
{
	class Renderer
	{
	public:
		static void DrawIndexed(const Ref<VertexArray>& vertexArray);
		static void DrawStaticMesh(const Ref<StaticMesh>& staticMesh, const Camera& camera);
	};
}
