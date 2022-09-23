#include "pch.h"
#include "DebugLayer.h"

#include "Core/Application.h"

#include "Rendering/Renderer.h"
#include "Rendering/OpenGLContext.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Cubeland
{
	void DebugLayer::OnAttach()
	{
		m_Camera = Camera(45.0f, 0.1f, 100.0f);

		m_CubeMesh = StaticMesh::CreateCube();

		g_CameraData->Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		g_CameraData->Projection = m_Camera.GetProjection();

		m_CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);

		m_Shader = CreateScope<Shader>("BasicShader", ShaderFilesMap{
			{ ShaderType::Vertex,	"assets/shaders/BasicShaderVert.glsl"	},
			{ ShaderType::Fragment, "assets/shaders/BasicShaderFrag.glsl"	}
		});
		m_Shader->Bind();
	}

	void DebugLayer::OnUpdate(float ts)
	{
		g_CameraData->View = m_Camera.GetView();
		m_CameraUniformBuffer->SetData(g_CameraData);

		OpenGLContext::Clear();
		Renderer::DrawStaticMesh(m_CubeMesh);
	}

	void DebugLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizedEvent>([this, camData = g_CameraData](WindowResizedEvent& e)
		{
			m_Camera.SetViewportSize((float)e.GetWidth(), (float)e.GetHeight());
			camData->Projection = m_Camera.GetProjection();
			return false;
		});

		dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent& e)
		{
			m_Camera.OnMouseMoved(e);
			return false;
		});
	}
}
