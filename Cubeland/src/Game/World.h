#pragma once

#include "Core/UUID.h"

#include "Game/Components.h"

#include <entt.hpp>
#include <glm/glm.hpp>

namespace Cubeland
{
	class Entity;

	class World
	{
	public:
		~World() = default;

		static World* CreateNewWorld();
		static void DestroyActiveWorld();
		static World* GetActiveWorld() { return s_ActiveWorld; }

		void Start();

		Entity TryGetEntityWithUUID(UUID id) const;
		Entity FindEntityByName(std::string_view name);

		Entity CreateEntity(const std::string& name);
		Entity CreateEntity(UUID uuid, const std::string& name);
		Entity CreateChildEntity(Entity parent, const std::string& name);
		void DestroyEntity(Entity entity);

		void OnUpdate(float ts);

		bool IsPaused() const { return m_Paused; }
		void SetPaused(bool paused) { m_Paused = paused; }

	private:
		World() = default;
		void Stop();

		void Render();

		std::pair<Entity, CameraComponent*> GetMainCameraEntity();
		glm::mat4 GetWorldSpaceTransformMatrix(Entity entity) const;

		bool CreateScriptableEntityInstance(ScriptComponent& sc, entt::entity handle, bool callOnCreate);
		void DestroyScriptableEntityInstance(ScriptComponent& sc);

	private:
		static World* s_ActiveWorld;
		entt::registry m_Registry;
		std::unordered_map<UUID, Entity> m_EntityIdMap;
		bool m_Paused = false;

		friend class Entity;
	};
}
