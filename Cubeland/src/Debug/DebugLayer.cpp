#include "pch.h"
#include "DebugLayer.h"

#include <imgui.h>

#include "Events/KeyboardEvent.h"

namespace Cubeland
{
	void DebugLayer::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::KeyPressed)
		{
			const auto& keyPressedEvent = dynamic_cast<KeyPressedEvent&>(e);
			if (keyPressedEvent.GetKeyCode() == KeyCode::F3)
			{
				m_WindowOpened = !m_WindowOpened;
			}
		}
	}

	void DebugLayer::OnImGuiRender()
	{
		if (!m_WindowOpened)
			return;

		ImGui::SetNextWindowSize({ 400, 800 });
		ImGui::Begin("Cubeland Debug", &m_WindowOpened);

		ImGui::End();
	}
}
