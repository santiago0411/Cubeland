#pragma once

#include "Game/Entity.h"

namespace Cubeland
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		template<typename T>
		T& GetComponent() const
		{
			return m_Entity.GetComponent<T>();
		}

		template<typename T>
		bool HasComponent() const
		{
			return m_Entity.HasComponent<T>();
		}

		TransformComponent& Transform()
		{
			return m_Entity.GetComponent<TransformComponent>();
		}

		TransformComponent& Transform() const
		{
			return m_Entity.GetComponent<TransformComponent>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(float ts) {}

	private:
		Entity m_Entity;
		friend class World;
	};
}
