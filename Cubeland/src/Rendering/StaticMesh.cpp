#include "pch.h"
/*
#include "StaticMesh.h"

namespace Cubeland
{
	static float g_CubeVertices[] = {
		-1.0f,  1.0f,   1.0f, // LEFT UP FRONT
		 1.0f,  1.0f,   1.0f, // RIGHT UP FRONT
		-1.0f, -1.0f,   1.0f, // LEFT DOWN FRONT
		 1.0f, -1.0f,   1.0f, // RIGHT DOWN FRONT
		-1.0f,  1.0f,  -1.0f, // LEFT UP BACK
		 1.0f,  1.0f,  -1.0f, // RIGHT UP BACK
		-1.0f, -1.0f,  -1.0f, // LEFT DOWN BACK
		 1.0f, -1.0f,  -1.0f, // RIGHT DOWN BACK
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
		{ "a_Position", ShaderDataType::Float3 }
	};

	Ref<StaticMesh> StaticMesh::CreateCube()
	{
		return Create({ g_CubeVertices, std::size(g_CubeVertices), g_CubeVerticesLayout, g_CubeIndices, std::size(g_CubeIndices) });
	}

	Ref<StaticMesh> StaticMesh::Create(const StaticMeshSpec& spec)
	{
		return CreateRef<StaticMesh>(spec);
	}

	void StaticMesh::Bind() const
	{
		m_VertexArray->Bind();
		m_IndexBuffer->Bind();
	}

	StaticMesh::StaticMesh(const StaticMeshSpec& spec)
	{
		m_VertexBuffer = VertexBuffer::Create(spec.Vertices, spec.VerticesCount);
		m_VertexBuffer->SetLayout(spec.VerticesLayout);

		m_IndexBuffer = IndexBuffer::Create(spec.Indices, spec.IndicesCount);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->SetVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}
}
*/
