#pragma once

#include <glm/glm.hpp>

namespace Cubeland
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(float fov, float nearClip, float farClip);
		~Camera() = default;

		void SetViewportSize(const float width, const float height)
		{
			m_ViewportWidth = width;
			m_ViewportHeight = height;
			UpdateProjection();
		}

		const glm::mat4& GetView() const { return m_ViewMatrix; }
		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }

	private:
		void UpdateProjection();

	private:
		float m_Fov, m_AspectRatio, m_NearClip, m_FarClip;
		float m_ViewportWidth, m_ViewportHeight;
		float m_Distance = 10.0f, m_Pitch = 0.0f, m_Yaw = 0.0f;

		glm::mat4 m_ProjectionMatrix;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_FocalPoint = glm::vec3(0.0f);

		glm::vec2 m_InitialMousePosition = glm::vec2(0.0f);
	};
}
