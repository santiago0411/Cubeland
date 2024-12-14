#include "pch.h"
#include "DebugLayer.h"

#include <imgui.h>

#include "Events/KeyboardEvent.h"

#include "ImGui/ImGuiUtils.h"

#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"

namespace Cubeland
{
	void DebugLayer::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::KeyPressed)
		{
			const auto& keyPressedEvent = dynamic_cast<KeyPressedEvent&>(e);
			if (keyPressedEvent.GetKeyCode() == KeyCode::F1)
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

		bool renderer = ImGui::TreeNodeEx("Renderer", 
			ImGuiTreeNodeFlags_OpenOnArrow | 
			ImGuiTreeNodeFlags_SpanAvailWidth);

		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		if (renderer)
		{
			ImGui::FontScope font(ImGui::Font::DogicaSmall);
			
			ImGui::RightCheckbox("Wireframe Mode", Renderer::GetWireFrameModeOn());
			ImGui::Dummy(ImVec2(0.0f, 3.0f));

			for (Ref<Shader> shader : ShaderLibrary::GetAllShaders())
			{
				ImGui::Columns(2, nullptr, false);
				ImGui::TextUnformatted(shader->GetName().c_str());
				ImGui::NextColumn();

				const char* reloadButtonText = "Reload";
				float buttonWidth = ImGui::CalcTextSize(reloadButtonText).x + ImGui::GetStyle().FramePadding.x * 2;
				float availableWidth = ImGui::GetContentRegionAvail().x;
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availableWidth - buttonWidth);

				if (ImGui::Button(reloadButtonText))
				{
					ShaderLibrary::ReloadShader(shader);
				}

				ImGui::Columns(1);
			}

			ImGui::TreePop();
		}

		bool rendererStats = ImGui::TreeNodeEx("Renderer Stats",
			ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_SpanAvailWidth);

		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		if (rendererStats)
		{
			ImGui::FontScope font(ImGui::Font::DogicaSmall);
			Renderer::Statistics stats = Renderer::GetStats();

			ImGui::Text("Draw Calls: %u", stats.DrawCalls);
			ImGui::Text("Cubes: %u", stats.CubeCount);
			ImGui::Text("Vertices: %u", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %u", stats.GetTotalIndexCount());

			ImGui::TreePop();
		}

		ImGui::End();

		ImGui::SetWindowFocus("Cubeland Debug");
	}
}
