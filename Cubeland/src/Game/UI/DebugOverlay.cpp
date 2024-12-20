#include "pch.h"
#include "DebugOverlay.h"

#include "Core/Application.h"

#include "Game/PlayerComponent.h"

#include "imgui.h"

namespace Cubeland
{
	void DebugOverlay::OnImGuiRender()
	{
		if (m_PlayerEntity)
		{
			const auto& transform = m_PlayerEntity.GetComponent<TransformComponent>();
			ImGui::TextUnformatted("Player:");
			ImGui::Text("\tPosition: X: %.3f Y: %.3f Z: %.3f", transform.Position.x, transform.Position.y, transform.Position.z);
			ImGui::Text("\tRotation: X: %.3f Y: %.3f Z: %.3f", transform.Rotation.x, transform.Rotation.y, transform.Rotation.z);
		}
	}
}
