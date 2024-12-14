#pragma once

#include "Core/Application.h"

#include <imgui.h>

namespace ImGui
{
	enum class Font
	{
		Default,

		DogicaSmall,
		DogicaMedium,
		DogicaLarge,
	};

	struct FontScope
	{
		FontScope(const Font font)
		{
			const auto* imGuiLayer = Cubeland::Application::Get().GetImGuiLayer();
			switch (font) {
				case Font::Default:
					PushFont(imGuiLayer->GetFontDefault());
					break;
				case Font::DogicaSmall:
					PushFont(imGuiLayer->GetFontDogicaSmall());
					break;
				case Font::DogicaMedium:
					PushFont(imGuiLayer->GetFontDogicaMedium());
					break;
				case Font::DogicaLarge:
					PushFont(imGuiLayer->GetFontDogicaLarge());
					break;
				default:
					CL_ASSERT(false, "Invalid font enum value");
					break;
			}
		}

		~FontScope()
		{
			PopFont();
		}
	};

	static bool RightCheckbox(const char* label, bool* v)
	{
		float availableWidth = GetContentRegionAvail().x;
		float checkboxPosX = GetCursorPosX() + availableWidth - GetFontSize() - GetStyle().FramePadding.x * 2;
		TextUnformatted(label);
		SameLine(checkboxPosX);
		char id[256];
		sprintf_s(id, sizeof id, "##%s", label);
		return Checkbox(id, v);
	}
}