#include "pch.h"
#include "Cube.h"

#include <glm/glm.hpp>

namespace Cubeland
{
	struct CubeVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
		uint32_t TexId;
	};
}