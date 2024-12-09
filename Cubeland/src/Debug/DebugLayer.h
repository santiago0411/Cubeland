#pragma once

#include "Core/Layer.h"

#include "Game/PlayerComponent.h"

namespace Cubeland
{
	class DebugLayer final : public Layer
	{
	public:
		DebugLayer() = default;
		~DebugLayer() override = default;

		void OnEvent(Event& e) override;
		void OnImGuiRender() override;

	private:
		bool m_WindowOpened = false;
	};
}
