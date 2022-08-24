#include "pch.h"
#include "DebugLayer.h"

#include "Core/Application.h"

#include "Rendering/Renderer.h"
#include "Rendering/OpenGLContext.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Cubeland
{
	struct CameraData
	{
		glm::mat4 Model;
		glm::mat4 View;
		glm::mat4 Projection;
	};

	static CameraData* g_CameraData = new CameraData;

	void DebugLayer::OnAttach()
	{
		m_Camera = Camera(45.0f, 0.1f, 100.0f);

		static float vertices[] = {
			-1.0f,  1.0f,   1.0f,
			 1.0f,  1.0f,   1.0f,
			-1.0f, -1.0f,   1.0f,
			 1.0f, -1.0f,   1.0f,
			-1.0f,  1.0f,  -1.0f,
			 1.0f,  1.0f,  -1.0f,
			-1.0f, -1.0f,  -1.0f,
			 1.0f, -1.0f,  -1.0f,
		};

		static uint32_t indices[] = {
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

		const auto vertexBuffer = CreateRef<VertexBuffer>(vertices, std::size(vertices));
		vertexBuffer->SetLayout({
			{ "a_Position", BufferDataType::Float3 }
		});

		const auto indexBuffer = CreateRef<IndexBuffer>(indices, std::size(indices));

		m_VertexArray = CreateRef<VertexArray>();
		m_VertexArray->SetVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		g_CameraData->Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		g_CameraData->Projection = m_Camera.GetProjection();

		m_CameraUniformBuffer = CreateRef<UniformBuffer>(sizeof(CameraData), 0);

		m_Shader = CreateScope<Shader>("BasicShader", std::unordered_map<ShaderType, Utils::Filepath>{
			{ ShaderType::Vertex,	"assets/shaders/BasicShaderVert.glsl"	},
			{ ShaderType::Fragment, "assets/shaders/BasicShaderFrag.glsl"	}
		});
		m_Shader->Bind();
	}

	void DebugLayer::OnUpdate(float ts)
	{
		g_CameraData->View = m_Camera.GetView();
		m_CameraUniformBuffer->SetData(g_CameraData, sizeof(CameraData));

		OpenGLContext::Clear();
		Renderer::DrawIndexed(m_VertexArray);
	}

	void DebugLayer::OnEvent(Event& e)
	{
		m_Camera.OnEvent(e);
	}
}
