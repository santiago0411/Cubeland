#pragma once

#include "Core/UUID.h"

#include "Game/ScriptableEntityManager.h"

#include "Rendering/Camera.h"
#include "Rendering/Texture2D.h"

#include <entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>

namespace Cubeland
{
	struct IdComponent
	{
		UUID Id;

		IdComponent() = default;
		IdComponent(const IdComponent&) = default;
		IdComponent(UUID uuid) : Id(uuid) {}
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct RelationshipComponent
	{
		UUID ParentId = 0;
		std::vector<UUID> Children;

		RelationshipComponent() = default;
		RelationshipComponent(const RelationshipComponent&) = default;
		RelationshipComponent(UUID parent)
			: ParentId(parent) {}
	};

	struct TransformComponent
	{
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position)
			: Position(position) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::mat4 GetRotationMatrix() const
		{
			return glm::yawPitchRoll(Rotation.y, Rotation.x, Rotation.z);
		}
	};

	struct CameraComponent
	{
		Camera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct CubeComponent
	{
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		Ref<SubTexture2D> Texture;

		CubeComponent() = default;
		CubeComponent(const CubeComponent&) = default;
 	};

	class ScriptableEntity;

	struct ScriptComponent
	{
		Ref<ScriptableEntity> Instance = nullptr;

		Ref<ScriptableEntity> (*InstantiateScript)();
		void (*DestroyScript)(ScriptComponent&);

		template <typename T>
		void Bind()
		{
			static_assert(std::is_base_of_v<ScriptableEntity, T>, "Cannot Bind() a ScriptComponent to a type that doesn't inherit from ScriptableEntity");
			InstantiateScript = ScriptableEntityManager::CreateInstance<T>;
			DestroyScript = [](ScriptComponent& se) { se.Instance = nullptr; };
		}
	};
}
