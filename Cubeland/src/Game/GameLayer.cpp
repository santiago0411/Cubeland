#include "pch.h"
#include "GameLayer.h"

#include "Core/Input.h"

#include "Game/GameTextures.h"
#include "Game/PlayerComponent.h"
#include "Game/ScriptableEntity.h"
#include "Game/World.h"

#include "Rendering/Renderer.h"

namespace Cubeland
{
	void GameLayer::OnAttach()
	{
		CL_LOG_DEBUG("Initializing GameLayer.");
		GameTextures::InitTextures();

		auto* world = World::CreateNewWorld();
		m_PlayerEntity = world->CreateEntity("Player");
		m_PlayerEntity.AddComponent<ScriptComponent>().Bind<PlayerComponent>();

		auto cameraEntity = world->CreateChildEntity(m_PlayerEntity, "Camera");
		cameraEntity.AddComponent<CameraComponent>().Camera = Camera(45.0f, 0.1f, 100.0f);

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
		World::GetActiveWorld()->OnUpdate(ts);
	}

	void GameLayer::OnImGuiRender()
	{

	}

	void GameLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizedEvent>(CL_BIND_EVENT_FN(GameLayer::OnWindowResized));
		dispatcher.Dispatch<KeyPressedEvent>(CL_BIND_EVENT_FN(GameLayer::OnKeyPressed));
	}

	bool GameLayer::OnWindowResized(WindowResizedEvent& e)
	{
		if (e.GetWidth() != 0 && e.GetHeight() != 0)
		{
			auto& cc = m_PlayerEntity.GetComponent<CameraComponent>();
			cc.Camera.SetViewportSize((float)e.GetWidth(), (float)e.GetHeight());
		}

		return false;
	}

	bool GameLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == KeyCode::Escape)
		{
			World* world = World::GetActiveWorld();
			if (Input::GetCursorMode() == CursorMode::Normal)
			{
				Input::SetCursorMode(CursorMode::Hidden);
				if (world)
					world->SetPaused(false);
			}
			else
			{
				Input::SetCursorMode(CursorMode::Normal);
				if (world)
					world->SetPaused(true);
			}
		}

		return false;
	}
}
