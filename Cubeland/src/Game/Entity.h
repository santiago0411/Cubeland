#pragma once

#include "Game/Components.h"
#include "Game/World.h"

#include <entt.hpp>

namespace Cubeland
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& other) = default;
		Entity(entt::entity handle, World* world)
			: m_Handle(handle), m_World(world) {}

		template<typename T, typename ... Args>
		T& AddComponent(Args&& ... args)
		{
			CL_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_World->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
		}

		template<typename T, typename ... Args>
		T& AddOrReplaceComponent(Args&& ... args)
		{
			return m_World->m_Registry.emplace_or_replace<T>(m_Handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			CL_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_World->m_Registry.get<T>(m_Handle);
		}

		template<typename T>
		T& GetComponent() const
		{
			CL_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_World->m_Registry.get<T>(m_Handle);
		}

		template<typename T>
		T& GetScriptComponentAs()
		{
			static_assert(std::is_base_of_v<ScriptableEntity, T>, "Cannot get a ScriptComponent that doesn't inherit from ScriptableEntity");
			CL_ASSERT(HasComponent<ScriptComponent>(), "Entity does not have script component!");
			const auto& sc = m_World->m_Registry.get<ScriptComponent>(m_Handle);
			CL_ASSERT(sc.Instance);
			return *dynamic_cast<T*>(sc.Instance);
		}

		template<typename T>
		bool TryGetComponentInChildren(T& outComponent)
		{
			for (UUID child : Children())
			{
				Entity childEntity = m_World->TryGetEntityWithUUID(child);
				if (childEntity.HasComponent<T>())
				{
					outComponent = childEntity.GetComponent<T>();
					return true;
				}
			}
			return false;
		}

		template<typename T>
		std::vector<T*> GetComponentsInChildren()
		{
			std::vector<T*> components(0);
			for (UUID child : Children())
			{
				Entity childEntity = m_World->TryGetEntityWithUUID(child);
				if (childEntity.HasComponent<T>())
					components.push_back(&childEntity.GetComponent<T>());
			}
			return components;
		}

		template<typename T>
		bool HasComponent()
		{
			return m_World->m_Registry.any_of<T>(m_Handle);
		}

		template<typename T>
		bool HasComponent() const
		{
			return m_World->m_Registry.any_of<T>(m_Handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			CL_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_World->m_Registry.remove<T>(m_Handle);
		}

		Entity GetParent() const
		{
			return m_World->TryGetEntityWithUUID(GetParentUUID());
		}

		void SetParent(Entity parent)
		{
			Entity currentParent = GetParent();
			if (currentParent == parent)
				return;

			if (currentParent)
				currentParent.RemoveChild(*this);

			if (parent)
			{
				SetParentUUID(parent.GetUUID());
				auto& parentChildren = parent.Children();
				UUID uuid = GetUUID();
				if (std::ranges::find(parentChildren, uuid) == parentChildren.end())
					parentChildren.emplace_back(uuid);
			}
			else
			{
				SetParentUUID(0);
			}
		}

		bool RemoveChild(Entity child)
		{
			UUID childId = child.GetUUID();
			std::vector<UUID>& children = Children();
			const auto it = std::ranges::find(children, childId);
			if (it != children.end())
			{
				children.erase(it);
				return true;
			}

			return false;
		}

		UUID GetUUID() { return GetComponent<IdComponent>().Id; }
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

		UUID GetParentUUID() const { return GetComponent<RelationshipComponent>().ParentId; }
		void SetParentUUID(UUID parent) { GetComponent<RelationshipComponent>().ParentId = parent; }
		std::vector<UUID>& Children() { return GetComponent<RelationshipComponent>().Children; }
		const std::vector<UUID>& Children() const { return GetComponent<RelationshipComponent>().Children; }

		operator bool() const { return m_Handle != entt::null; }
		operator entt::entity() const { return m_Handle; }
		operator uint32_t() const { return (uint32_t)m_Handle; }

		bool operator==(const Entity& other) const
		{
			return m_Handle == other.m_Handle && m_World == other.m_World;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_Handle = entt::null;
		World* m_World = nullptr;
	};
}
