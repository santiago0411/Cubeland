#include "pch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace Cubeland
{
	void Renderer::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		uint32_t count = vertexArray->GetIndexBuffer()->GetCount();
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, (int)count, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::DrawStaticMesh(const Ref<StaticMesh>& staticMesh, const Camera& camera)
	{
		staticMesh->Bind();
		glDrawElements(GL_TRIANGLES, (int)staticMesh->GetIndicesCount(), GL_UNSIGNED_INT, nullptr);
	}
}
