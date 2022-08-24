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

	void GameLayer::OnEvent(Event& e)
	{
		m_Camera.OnEvent(e);
	}
}
