#include "pch.h"
#include "GameLayer.h"

#include "Game/GameTextures.h"

#include "Rendering/OpenGLContext.h"
#include "Rendering/Renderer.h"

namespace Cubeland
{
	void GameLayer::OnAttach()
	{
		CL_LOG_DEBUG("Initializing GameLayer.");
		m_Camera = Camera(45.0f, 0.1f, 100.0f);
		GameTextures::InitTextures();
	}

	void GameLayer::OnDetach()
	{

	}

	void GameLayer::OnUpdate(float ts)
	{
		m_Camera.OnUpdate(ts);
		OpenGLContext::Clear();
		Renderer::BeginScene(m_Camera);
		Renderer::DrawCube(glm::vec3(0), GameTextures::DirtSubTexture);
		Renderer::DrawCube(glm::vec3(1, 0, 0), GameTextures::StoneSubTexture);
		Renderer::EndScene();
	}

	void GameLayer::OnImGuiRender()
	{

	}

	void GameLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizedEvent>(CL_BIND_EVENT_FN(GameLayer::OnWindowResized));
		dispatcher.Dispatch<MouseMovedEvent>(CL_BIND_EVENT_FN(GameLayer::OnMouseMoved));
	}

	bool GameLayer::OnWindowResized(WindowResizedEvent& e)
	{
		if (e.GetWidth() != 0 && e.GetHeight() != 0)
			m_Camera.SetViewportSize((float)e.GetWidth(), (float)e.GetHeight());

		return false;
	}

	bool GameLayer::OnMouseMoved(MouseMovedEvent& e)
	{
		m_Camera.OnMouseMoved(e);
		return false;
	}
}
