#pragma once

#include "Rendering/Buffer.h"

namespace Cubeland
{
	struct StaticMeshSpec
	{
		float* Vertices;
		uint32_t VerticesCount;
		BufferLayout VerticesLayout;
		uint32_t* Indices;
		uint32_t IndicesCount;
	};

	class StaticMesh
	{
	public:
		static Ref<StaticMesh> CreateCube();
		static Ref<StaticMesh> Create(const StaticMeshSpec& spec);

		StaticMesh(const StaticMeshSpec& spec);
		~StaticMesh() = default;

		void Bind() const;
		uint32_t GetIndicesCount() const { return m_IndexBuffer->GetCount(); }

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}
