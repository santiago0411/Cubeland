#include "pch.h"
#include "GameLayer.h"

namespace Cubeland
{
	void GameLayer::OnAttach()
	{
		m_Camera = Camera(45.0f, 0.1f, 100.0f);
	}

	void GameLayer::OnDetach()
	{

	}

	void GameLayer::OnUpdate(float ts)
	{

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
