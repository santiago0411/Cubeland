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

		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }

	private:
		void UpdateProjection();

	private:
		float m_Fov, m_AspectRatio, m_NearClip, m_FarClip;
		float m_ViewportWidth, m_ViewportHeight;

		glm::mat4 m_ProjectionMatrix;
	};
}
