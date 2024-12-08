#pragma once

#include "Core/Layer.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"

#include "Game/Entity.h"

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
		bool OnWindowResized(WindowResizedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

	private:
		Entity m_PlayerEntity;
	};
}
