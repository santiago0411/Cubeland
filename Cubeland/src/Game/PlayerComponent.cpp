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
		glm::vec3 forwardDirection = glm::vec3(rotationMatrix * glm::vec4(0, 0, -1, 0));
		glm::vec3 rightDirection = glm::vec3(rotationMatrix * glm::vec4(1, 0, 0, 0));

		// Project forward/right directions onto the XZ plane removing the vertical component
		forwardDirection.y = 0.0f;
		forwardDirection = glm::normalize(forwardDirection);

		rightDirection.y = 0.0f;
		rightDirection = glm::normalize(rightDirection);

		const glm::vec3 forwardMovement = forwardDirection * velocity;
		const glm::vec3 rightMovement = rightDirection * velocity;

		if (Input::IsKeyPressed(KeyCode::W))
			transform.Position += forwardMovement;

		if (Input::IsKeyPressed(KeyCode::S))
			transform.Position -= forwardMovement;

		if (Input::IsKeyPressed(KeyCode::A))
			transform.Position -= rightMovement;

		if (Input::IsKeyPressed(KeyCode::D))
			transform.Position += rightMovement;

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
