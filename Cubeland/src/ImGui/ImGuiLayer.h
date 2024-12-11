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

		ImFont* GetFontDefault() const { return m_DefaultFont; }
		void SetDefaultFont(ImFont* font) { m_DefaultFont = font; }

		ImFont* GetFontDogicaSmall() const { return m_SmallDogicaFont; }
		ImFont* GetFontDogicaMedium() const { return m_MediumDogicaFont; }
		ImFont* GetFontDogicaLarge() const { return m_LargeDogicaFont; }

	private:
		bool m_BlockEvents = false;

		ImFont* m_DefaultFont = nullptr;

		ImFont* m_SmallDogicaFont = nullptr;
		ImFont* m_MediumDogicaFont = nullptr;
		ImFont* m_LargeDogicaFont = nullptr;
	};
}