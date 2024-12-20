#pragma once

#include "Core/Layer.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"

#include "Game/Entity.h"
#include "Game/UI/DebugOverlay.h"

#include "Rendering/Framebuffer.h"

namespace Cubeland
{
	class GameLayer final : public Layer
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
		bool OnKeyPressed(const KeyPressedEvent& e);
		bool OnWindowFocus(const WindowFocusEvent& e);

	private:
		Entity m_PlayerEntity{};
		Camera* m_GameCamera = nullptr;
		glm::vec2 m_ViewportSize{0};

		Ref<Framebuffer> m_Framebuffer = nullptr;

		Scope<DebugOverlay> m_DebugOverlay = nullptr;
		bool m_RenderDebugOverlay = false;
	};
}
