#include "pch.h"
#include "DebugLayer.h"

#include "Rendering/Renderer.h"
#include "Rendering/OpenGLContext.h"

namespace Cubeland
{
	void DebugLayer::OnAttach()
	{
		static float vertices[] = {
			-0.5f,  0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f
		};

		static uint32_t indices[] = {
			0, 1, 2,
			1, 2, 3
		};

		const auto vertexBuffer = CreateRef<VertexBuffer>(vertices, std::size(vertices));
		vertexBuffer->SetLayout({
			{ "a_Position", BufferDataType::Float3 }
		});

		const auto indexBuffer = CreateRef<IndexBuffer>(indices, std::size(indices));

		m_VertexArray = CreateRef<VertexArray>();
		m_VertexArray->SetVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader = CreateScope<Shader>("BasicShader", std::unordered_map<ShaderType, Utils::Filepath>{
			{ ShaderType::Vertex,	"assets/shaders/BasicShaderVert.glsl"	},
			{ ShaderType::Fragment, "assets/shaders/BasicShaderFrag.glsl"	}
		});
		m_Shader->Bind();
	}

	void DebugLayer::OnUpdate(float ts)
	{
		OpenGLContext::Clear();
		Renderer::DrawIndexed(m_VertexArray);
	}
}
