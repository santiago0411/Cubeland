#pragma once

#include "Core/Layer.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"

#include "Game/Entity.h"

#include "Rendering/Framebuffer.h"

namespace Cubeland
{
	class GameLayer : public Layer
	{
	public:
		GameLayer() = default;
		~GameLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float ts) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		Entity m_PlayerEntity;
		Camera* m_GameCamera = nullptr;
		glm::vec2 m_ViewportSize{0};

		Ref<Framebuffer> m_Framebuffer;
	};
}
