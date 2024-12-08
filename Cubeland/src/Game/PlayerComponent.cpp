#include "pch.h"
#include "PlayerComponent.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"

#include <glm/glm.hpp>

namespace Cubeland
{
	constexpr float MOUSE_SENSITIVITY = 0.003f;
	constexpr float ROTATION_SPEED = 20.0f;
	constexpr float MOV_SPEED = 10.0f;

	void PlayerComponent::OnCreate()
	{
		m_LastMousePosition = Input::GetMousePosition();
	}

	void PlayerComponent::OnDestroy()
	{
	}

	void PlayerComponent::OnUpdate(float ts)
	{
		MoveAndRotateCamera(ts);
	}

	void PlayerComponent::MoveAndRotateCamera(float ts)
	{
		const float velocity = ts * MOV_SPEED;
		TransformComponent& transform = Transform();

		glm::mat4 rotationMatrix = transform.GetRotationMatrix();
		glm::vec3 forwardDirection = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0, 0, -1, 0)));
		glm::vec3 rightDirection = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(1, 0, 0, 0)));

		if (Input::IsKeyPressed(KeyCode::W))
			transform.Position += forwardDirection * velocity;

		if (Input::IsKeyPressed(KeyCode::S))
			transform.Position -= forwardDirection * velocity;

		if (Input::IsKeyPressed(KeyCode::A))
			transform.Position -= rightDirection * velocity;

		if (Input::IsKeyPressed(KeyCode::D))
			transform.Position += rightDirection * velocity;

		if (Input::IsKeyPressed(KeyCode::LeftShift))
			transform.Position.y -= velocity / 1.5f;

		if (Input::IsKeyPressed(KeyCode::Space))
			transform.Position.y += velocity / 1.5f;

		glm::vec2 mousePosition = Input::GetMousePosition();
		if (mousePosition == m_LastMousePosition)
			return;

		float mouseVelocity = MOUSE_SENSITIVITY * ROTATION_SPEED * ts;
		glm::vec2 delta = (mousePosition - m_LastMousePosition) * mouseVelocity;
		m_LastMousePosition = mousePosition;

		glm::vec3& rotation = transform.Rotation;
		rotation.x -= delta.y;
		rotation.y -= delta.x;

		// Clamp pitch between -90 and 90 degrees
		rotation.x = glm::clamp(rotation.x, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);
	}
}
