#pragma once

#include "Rendering/Buffer.h"

namespace Cubeland
{
	class StaticMesh
	{
	public:
		static Ref<StaticMesh> CreateCube();
		static Ref<StaticMesh> Create(float* vertices, uint32_t verticesCount, const BufferLayout& verticesLayout, uint32_t* indices, uint32_t indicesCount);

		StaticMesh(float* vertices, uint32_t verticesCount, const BufferLayout& verticesLayout, uint32_t* indices, uint32_t indicesCount);
		~StaticMesh() = default;

		void Bind() const;
		uint32_t GetIndicesCount() const { return m_IndexBuffer->GetCount(); }

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}
