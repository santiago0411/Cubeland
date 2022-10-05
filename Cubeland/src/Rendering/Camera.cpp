#include "pch.h"
#include "Camera.h"

#include "Core/Application.h"
#include "Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

constexpr float ROTATION_SPEED = 0.8f;
constexpr float MOV_SPEED = 10.0f;

namespace Cubeland
{
	Camera::Camera(float fov, float nearClip, float farClip)
		: m_Fov(fov), m_NearClip(nearClip), m_FarClip(farClip)
	{
		glm::vec2 widthHeight = Application::Get().GetWindow().GetWidthAndHeight();
		m_ViewportWidth = widthHeight.x;
		m_ViewportHeight = widthHeight.y;

		UpdateProjection();
		UpdateView();
	}

	void Camera::UpdateView()
	{
		m_Position = m_FocalPoint - GetForwardDirection() * m_Distance;
		const glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	void Camera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void Camera::Rotate(const glm::vec2& mousePosition)
	{
		glm::vec2 delta = (mousePosition - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mousePosition;

		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * ROTATION_SPEED;
		m_Pitch += delta.y * ROTATION_SPEED;

		UpdateView();
	}

	glm::quat Camera::GetOrientation() const
	{
		return { glm::vec3(-m_Pitch, -m_Yaw, 0.0f) };
	}

	glm::vec3 Camera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Camera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 Camera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	void Camera::OnUpdate(float ts)
	{
		float velocity = ts * MOV_SPEED;
		bool moved = false;

		if (Input::IsKeyPressed(KeyCode::W))
		{
			m_FocalPoint += GetForwardDirection() * velocity;
			moved = true;
		}

		if (Input::IsKeyPressed(KeyCode::S))
		{
			m_FocalPoint += -GetForwardDirection() * velocity;
			moved = true;
		}

		if (Input::IsKeyPressed(KeyCode::A))
		{
			m_FocalPoint += -GetRightDirection() * velocity;
			moved = true;
		}

		if (Input::IsKeyPressed(KeyCode::D))
		{
			m_FocalPoint += GetRightDirection() * velocity;
			moved = true;
		}

		if (Input::IsKeyPressed(KeyCode::LeftShift))
		{
			m_FocalPoint += -GetUpDirection() * velocity / 2.0f;
			moved = true;
		}

		if (Input::IsKeyPressed(KeyCode::Space))
		{
			m_FocalPoint += GetUpDirection() * velocity / 2.0f;
			moved = true;
		}

		if (moved)
			UpdateView();
	}

	void Camera::OnMouseMoved(MouseMovedEvent& e)
	{
		Rotate({ e.GetX(), e.GetY() });
	}
}
