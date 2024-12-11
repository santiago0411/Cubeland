#include "pch.h"
#include "GameLayer.h"

#include "Core/Application.h"
#include "Core/Input.h"

#include "Game/GameTextures.h"
#include "Game/PlayerComponent.h"
#include "Game/World.h"

#include "Rendering/Framebuffer.h"
#include "Rendering/Renderer.h"

#include <imgui.h>

namespace Cubeland
{
	void GameLayer::OnAttach()
	{
		CL_LOG_DEBUG("Initializing GameLayer.");
		GameTextures::InitTextures();

		CL_LOG_DEBUG("Creating GameLayer framebuffer");
		FramebufferSpecification framebufferSpec;
		framebufferSpec.Attachments = {
			FramebufferTextureFormat::RGBA8,
			FramebufferTextureFormat::RED_INTEGER,
			FramebufferTextureFormat::Depth,
		};
		framebufferSpec.Width = 1;
		framebufferSpec.Height = 1;
		m_Framebuffer = CreateRef<Framebuffer>(framebufferSpec);

		auto* world = World::CreateNewWorld();
		m_PlayerEntity = world->CreateEntity("Player");
		m_PlayerEntity.AddComponent<ScriptComponent>().Bind<PlayerComponent>();

		auto cameraEntity = world->CreateChildEntity(m_PlayerEntity, "Camera");
		auto& cc  = cameraEntity.AddComponent<CameraComponent>();
		cc.Camera = Camera(45.0f, 0.1f, 100.0f);
		m_GameCamera = &cc.Camera;

		m_DebugOverlay = CreateScope<DebugOverlay>(m_PlayerEntity);

		for (int y = 0; y < 2; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				for (int z = 0; z < 20; z++)
				{
					Entity cube = world->CreateEntity("Cube");
					auto& cc = cube.AddComponent<CubeComponent>();
					cc.Position = { x, y, z };
					cc.Texture = y == 0 ? GameTextures::DirtSubTexture : GameTextures::StoneSubTexture;
				}
			}
		}

		world->Start();
	}

	void GameLayer::OnDetach()
	{
		World::DestroyActiveWorld();
	}

	void GameLayer::OnUpdate(float ts)
	{
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
		{
			const auto& spec = m_Framebuffer->GetSpecification();
			if (glm::vec2(spec.Width, spec.Height) != m_ViewportSize)
			{
				m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_GameCamera->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			}
		}

		Renderer::ResetStats();
		m_Framebuffer->Bind();
			World::GetActiveWorld()->OnUpdate(ts);
		m_Framebuffer->Unbind();
	}

	void GameLayer::OnImGuiRender()
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar| 
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		static bool open = true;
		ImGui::Begin("Cubeland", &open, windowFlags);
		ImGui::PopStyleVar(3); // Pop rounding, size, padding

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportSize.x, viewportSize.y };

		const ImVec2 windowPos = ImGui::GetWindowPos();
		const ImVec2 windowSize = ImGui::GetContentRegionAvail();
		const ImVec2 windowPadding = ImGui::GetStyle().WindowPadding;

		const ImVec2 imageStartPos = ImVec2(windowPos.x + windowPadding.x, windowPos.y + windowPadding.y);
		const ImVec2 imageEndPos = ImVec2(imageStartPos.x + windowSize.x, imageStartPos.y + windowSize.y);

		// Render the scene framebuffer as the background
		ImGui::GetWindowDrawList()->AddImage(
			// Double cast to avoid warning
			(ImTextureID)(uint64_t)m_Framebuffer->GetColorAttachmentRendererId(),
			imageStartPos,
			imageEndPos,
			{ 0, 1 },   // UV top-left
			{ 1, 0 }    // UV bottom-right
		);

		if (m_RenderDebugOverlay)
			m_DebugOverlay->OnImGuiRender();

		ImGui::End();
	}

	void GameLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(CL_BIND_EVENT_FN(GameLayer::OnKeyPressed));
	}

	bool GameLayer::OnKeyPressed(const KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == KeyCode::Escape)
		{
			if (World* world = World::GetActiveWorld())
			{
				if (world->IsPaused())
				{
					Input::SetCursorMode(CursorMode::Locked);
					world->SetPaused(false);
				}
				else
				{
					Input::SetCursorMode(CursorMode::Normal);
					if (world)
						world->SetPaused(true);
				}
			}
		}
		else if (e.GetKeyCode() == KeyCode::F3)
		{
			m_RenderDebugOverlay = !m_RenderDebugOverlay;
		}

		return false;
	}
}
