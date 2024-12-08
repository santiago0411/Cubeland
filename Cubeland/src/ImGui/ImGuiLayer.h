#pragma once

#include "Core/Layer.h"

namespace Cubeland
{
	class ImGuiLayer final : public Layer
	{
	public:
		ImGuiLayer() = default;
		~ImGuiLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetBlockEvents(const bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
	};
}