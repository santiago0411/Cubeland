#pragma once

#include "Core/Layer.h"

struct ImFont;

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

		ImFont* GetDefault() const { return m_DefaultDogicaFont; }
		ImFont* GetDefaultLarge() const { return m_LargeDogicaFont; }

	private:
		bool m_BlockEvents = false;

		ImFont* m_DefaultDogicaFont = nullptr;
		ImFont* m_LargeDogicaFont = nullptr;
	};
}