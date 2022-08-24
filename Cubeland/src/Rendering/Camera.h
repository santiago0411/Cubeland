#pragma once

#include <glm/glm.hpp>

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace Cubeland
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(float fov, float nearClip, float farClip);
		~Camera() = default;

		void OnEvent(Event& e);

		void SetViewportSize(const float width, const float height)
		{
			m_ViewportWidth = width;
			m_ViewportHeight = height;
			UpdateProjection();
		}

		const glm::mat4& GetView() const { return m_ViewMatrix; }
		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }

	private:
		void UpdateView();
		void UpdateProjection();

		void Rotate(const glm::vec2& mousePosition);

		glm::quat GetOrientation() const;
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		bool OnWindowResized(WindowResizedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);

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
