#include "pch.h"
#include "Camera.h"

#include "Core/Application.h"
#include "Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Cubeland
{
	Camera::Camera(float fov, float nearClip, float farClip)
		: m_Fov(fov), m_NearClip(nearClip), m_FarClip(farClip)
	{
		glm::vec2 widthHeight = Application::Get().GetWindow().GetWidthAndHeight();
		m_ViewportWidth = widthHeight.x;
		m_ViewportHeight = widthHeight.y;

		UpdateProjection();

		Input::SetCursorMode(CursorMode::Locked);
	}

	void Camera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
	}
}
