#pragma once

#include "Core/Layer.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include "Rendering/Camera.h"

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
		bool OnMouseMoved(MouseMovedEvent& e);

	private:
		Camera m_Camera;
	};
}
