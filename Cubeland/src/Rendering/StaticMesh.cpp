#include "pch.h"
#include "StaticMesh.h"

namespace Cubeland
{
	static float g_CubeVertices[] = {
		-1.0f,  1.0f,   1.0f,
		 1.0f,  1.0f,   1.0f,
		-1.0f, -1.0f,   1.0f,
		 1.0f, -1.0f,   1.0f,
		-1.0f,  1.0f,  -1.0f,
		 1.0f,  1.0f,  -1.0f,
		-1.0f, -1.0f,  -1.0f,
		 1.0f, -1.0f,  -1.0f,
	};

	static uint32_t g_CubeIndices[] = {
		0, 1, 2,
		1, 2, 3,
		4, 5, 6,
		5, 6, 7,
		0, 4, 5,
		0, 1, 5,
		2, 6, 7,
		2, 3, 7,
		0, 4, 6,
		0, 2, 6,
		1, 3, 7,
		1, 5, 7
	};

	static BufferLayout g_CubeVerticesLayout = {
		{ "a_Position", BufferDataType::Float3 }
	};

	Ref<StaticMesh> StaticMesh::CreateCube()
	{
		return CreateRef<StaticMesh>(g_CubeVertices, std::size(g_CubeVertices), g_CubeVerticesLayout, g_CubeIndices, std::size(g_CubeIndices));
	}

	Ref<StaticMesh> StaticMesh::Create(float* vertices, uint32_t verticesCount, const BufferLayout& verticesLayout, uint32_t* indices, uint32_t indicesCount)
	{
		return  CreateRef<StaticMesh>(vertices, verticesCount, verticesLayout, indices, indicesCount);;
	}

	void StaticMesh::Bind() const
	{
		m_VertexArray->Bind();
		m_IndexBuffer->Bind();
	}

	StaticMesh::StaticMesh(float* vertices, uint32_t verticesCount, const BufferLayout& verticesLayout, uint32_t* indices, uint32_t indicesCount)
	{
		m_VertexBuffer = VertexBuffer::Create(vertices, verticesCount);
		m_VertexBuffer->SetLayout(verticesLayout);

		m_IndexBuffer = IndexBuffer::Create(indices, indicesCount);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->SetVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}
}
