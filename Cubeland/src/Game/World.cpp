#include "pch.h"
#include "World.h"

#include "Core/Application.h"

#include "Game/Components.h"
#include "Game/Entity.h"
#include "Game/ScriptableEntity.h"

#include "Rendering/Renderer.h"

namespace Cubeland
{
	World* World::s_ActiveWorld = nullptr;

	World* World::CreateNewWorld()
	{
		DestroyActiveWorld();
		s_ActiveWorld = new World;
		return s_ActiveWorld;
	}

	void World::DestroyActiveWorld()
	{
		if (s_ActiveWorld)
			s_ActiveWorld->Stop();

		delete s_ActiveWorld;
	}

	void World::Start()
	{
		// Instantiate
		{
			m_Registry.view<ScriptComponent>().each([this](entt::entity handle, ScriptComponent& sc)
			{
				CreateScriptableEntityInstance(sc, handle, false);
			});
		}
		// Run OnCreate()
		{
			m_Registry.view<ScriptComponent>().each([](const ScriptComponent& sc)
			{
				if (sc.Instance)
					sc.Instance->OnCreate();
			});
		}
	}

	void World::Stop()
	{
		m_Registry.view<ScriptComponent>().each([this](ScriptComponent& sc)
		{
			DestroyScriptableEntityInstance(sc);
		});
	}

	Entity World::TryGetEntityWithUUID(UUID id) const
	{
		if (const auto it = m_EntityIdMap.find(id); it != m_EntityIdMap.end())
			return m_EntityIdMap.at(id);

		return {};
	}

	Entity World::FindEntityByName(const std::string_view name)
	{
		const auto view = m_Registry.view<TagComponent>();
		for (const auto entityId : view)
		{
			if (view.get<TagComponent>(entityId).Tag == name)
				return { entityId, this };
		}

		return {};
	}

	Entity World::CreateEntity(const std::string& name)
	{
		return CreateEntity(UUID(), name);
	}

	Entity World::CreateEntity(UUID uuid, const std::string& name)
	{
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<IdComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<RelationshipComponent>();
		entity.AddComponent<TagComponent>(name.empty() ? "Unnamed entity" : name);
		m_EntityIdMap[uuid] = entity;
		return entity;
	}

	Entity World::CreateChildEntity(Entity parent, const std::string& name)
	{
		Entity entity = CreateEntity(name);
		if (parent)
			entity.SetParent(parent);
		return entity;
	}

	void World::DestroyEntity(Entity entity)
	{
		if (entity.HasComponent<ScriptComponent>())
			DestroyScriptableEntityInstance(entity.GetComponent<ScriptComponent>());

		m_Registry.destroy(entity);
		m_EntityIdMap.erase(entity.GetUUID());
	}

	void World::OnUpdate(float ts)
	{
		if (m_Paused)
			ts = 0.0f;

		// Update scripts
		m_Registry.view<ScriptComponent>().each([this, ts](entt::entity handle, ScriptComponent& sc)
		{
			bool isValid = true;
			if (!sc.Instance)
				isValid = CreateScriptableEntityInstance(sc, handle, true);

			if (isValid)
				sc.Instance->OnUpdate(ts);
		});

		Render();
	}


	void World::Render()
	{
		auto [cameraEntity, cameraComponent] = GetMainCameraEntity();
		if (!cameraEntity || !cameraComponent)
			return;

		glm::mat4 cameraViewMatrix = glm::inverse(GetWorldSpaceTransformMatrix(cameraEntity));
		const Camera& camera = cameraComponent->Camera;

		Renderer::BeginScene(camera, cameraViewMatrix);

		// Render cubes
		{
			m_Registry.view<CubeComponent>().each([](const CubeComponent& cube)
			{
				Renderer::DrawCube(cube.Position, cube.Texture);
			});
		}

		Renderer::EndScene();
	}

	std::pair<Entity, CameraComponent*> World::GetMainCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entityId : view)
		{
			auto& cc = view.get<CameraComponent>(entityId);
			if (cc.Primary)
				return std::make_pair(Entity{ entityId, this }, &cc);
		}

		return {};
	}

	glm::mat4 World::GetWorldSpaceTransformMatrix(Entity entity) const
	{
		glm::mat4 transform(1.0f);

		while (entity)
		{
			transform *= entity.GetComponent<TransformComponent>().GetTransform();
			entity = TryGetEntityWithUUID(entity.GetParentUUID());
		}

		return transform;
	}

	bool World::CreateScriptableEntityInstance(ScriptComponent& sc, entt::entity handle, bool callOnCreate)
	{
		if (!sc.InstantiateScript)
		{
			Entity e{ handle, this };
			CL_LOG_ERROR("Entity '{}' has a ScriptComponent but its Bind() function was never called.", e.GetName());
			CL_ASSERT(false);
			return false;
		}

		sc.Instance = sc.InstantiateScript();
		sc.Instance->m_Entity = { handle, this };
		if (callOnCreate)
			sc.Instance->OnCreate();
		return true;
	}

	void World::DestroyScriptableEntityInstance(ScriptComponent& sc)
	{
		if (sc.Instance)
		{
			sc.Instance->OnDestroy();
			sc.DestroyScript(sc);
		}
	}
}
